#include "directive.h"
#include "input.h"
#include "macro.h"
#include "preprocess.h"
#include "strtab.h"
#include "tokenize.h"
#include <jayl/context.h>
#include <jayl/deque.h>

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 * Buffer of preprocessed tokens, ready to be consumed by the parser.
 * Filled lazily on calls to peek(0), peekn(1) and next(0).
 */
static deque_of(jayl_token_t) lookahead;

/* Toggle for producing preprocessed output (-E). */
static int output_preprocessed;

/* Line currently being tokenized. */
static char *line_buffer;

static void cleanup(void)
{
    deque_destroy(&lookahead);
}

static void ensure_initialized(void)
{
    static int done;

    if (!done) {
        atexit(cleanup);
        done = 1;
    }
}

static jayl_token_t get_token(void)
{
    jayl_token_t r;
    char *endptr;

    if (!line_buffer && (line_buffer = getprepline()) == NULL) {
        r = basic_token[END];
    } else {
        r = jayl_tokenize(line_buffer, &endptr);
        line_buffer = endptr;
        if (r.token == END) {
            /*
             * Newlines are removed by getprepline, and never present in
             * the input data. Instead intercept end of string, which
             * represents end of line.
             */
            line_buffer = NULL;
            r = basic_token[NEWLINE];
        }
    }

    return r;
}

/*
 * Keep track of the nesting depth of macro arguments. For example;
 * MAX( MAX(10, 12), 20 ) should complete on the last parenthesis, which
 * makes the expression balanced. Read lines until full macro invocation
 * is included.
 */
static void read_macro_invocation(TokenArray *line, const struct macro *macro)
{
    int nesting;
    jayl_token_t t;
    assert(macro->type == FUNCTION_LIKE);

    t = get_token();
    array_push_back(line, t);
    if (t.token != '(')
        /*
         * Only expand function-like macros if they appear as function
         * invocations, beginning with an open paranthesis.
         */
        return;

    nesting = 1;
    while (nesting) {
        t = get_token();
        if (t.token == '(') {
            nesting++;
        }
        if (t.token == ')') {
            assert(nesting);
            nesting--;
        }
        if (t.token == NEWLINE) {
            /*
             * This is the only scenario where reading a line is not
             * enough. Macro invocations can span lines, and we want to
             * have everything in the same token list.
             */
            continue;
        }
        assert(t.token != END);
        array_push_back(line, t);
    }
    if (nesting) {
        error("Unbalanced invocation of macro '%s'.", str_raw(macro->name));
        exit(1);
    }
}

/* Replace 'defined name' and 'defined (name)' with 0 or 1 constants. */
static void read_defined_operator(TokenArray *line)
{
    int is_parens = 0;
    char *endptr;
    jayl_token_t t = get_token();

    if (t.token == '(') {
        t = get_token();
        is_parens = 1;
    }

    if (!t.is_expandable) {
        error("Expected identifier in 'defined' clause, but got '%s'",
            str_raw(t.d.string));
        exit(1);
    }

    if (definition(t.d.string))
        t = jayl_tokenize("1", &endptr);
    else
        t = jayl_tokenize("0", &endptr);

    array_push_back(line, t);
    if (is_parens) {
        t = get_token();
        if (t.token != ')') {
            error("Expected ')' to close 'defined' clause.");
            exit(1);
        }
    }
}

/*
 * Get token at position i of existing line, or add new token from input
 * stream to line at posistion. Overwrite the trailing newline.
 */
static jayl_token_t skip_or_get_token(TokenArray *line, int i)
{
    jayl_token_t t;

    if (i == array_len(line) - 1) {
        t = array_get(line, i);
        if (t.token == NEWLINE) {
            (void) array_pop_back(line);
        }
    }

    if (i == array_len(line)) {
        do {
            t = get_token();
        } while (t.token == NEWLINE);
        assert(t.token != END);
        array_push_back(line, t);
    } else {
        assert(i >= 0);
        assert(i < array_len(line));
        t = array_get(line, i);
    }

    return t;
}

/*
 * Make sure expanded token list contains enough tokens to do additional
 * expansions. Read more input if the provided function-like macro at
 * posistion i does not have all parameters on the current line.
 */
static int skip_or_read_expansion(
    const struct macro *def,
    TokenArray *line,
    int i)
{
    int start = i, nest;
    jayl_token_t t;

    assert(def->type == FUNCTION_LIKE);
    t = skip_or_get_token(line, i++);
    if (t.token != '(') {
        return i - start;
    }

    nest = 1;
    while (nest) {
        t = skip_or_get_token(line, i++);
        if (t.token == '(') nest++;
        if (t.token == ')') nest--;
    }

    return i - start;
}

/*
 * Read tokens until reaching end of line. If initial token is '#', stop
 * on first newline. Otherwise make sure macro invocations spanning
 * multiple lines are joined. Replace 'defined' with 0 or 1.
 *
 * Returns a buffer containing all necessary tokens to preprocess a
 * line. Always ends with a newline (\n) token, but never contains any
 * newlines in the array itself.
 */
static int read_complete_line(TokenArray *line, jayl_token_t t, int directive)
{
    int expandable = 1, macros = 0;
    const struct macro *def;

    if (directive) {
        array_push_back(line, t);
        expandable = (t.token == IF) || !tok_cmp(t, ident__elif);
        t = get_token();
    }

    while (t.token != NEWLINE) {
        assert(t.token != END);
        if (expandable && t.is_expandable) {
            if (directive && !tok_cmp(t, ident__defined)) {
                read_defined_operator(line);
            } else {
                def = definition(t.d.string);
                if (def) {
                    macros += 1;
                    if (def->type == FUNCTION_LIKE) {
                        array_push_back(line, t);
                        read_macro_invocation(line, def);
                    } else {
                        array_push_back(line, t);
                    }
                } else {
                    array_push_back(line, t);
                }
            }
        } else {
            array_push_back(line, t);
        }
        t = get_token();
    }

    assert(t.token == NEWLINE);
    array_push_back(line, t);
    return macros;
}

/*
 * After expansion, it might be that we need to read a bit more input to
 * get argument of new expansion. Look through the array and see whether
 * there is a partial macro invocation that needs more input.
 *
 * Return non-zero if there are more function-like macros that needs to
 * be expanded.
 */
static int refill_expanding_line(TokenArray *line)
{
    int i, n, len;
    jayl_token_t t;
    const struct macro *def;

    n = 0;
    len = array_len(line);
    if (len) {
        for (i = 0; i < len; ++i) {
            t = array_get(line, i);
            if (t.is_expandable && !t.disable_expand) {
                def = definition(t.d.string);
                if (def && def->type == FUNCTION_LIKE) {
                    i += skip_or_read_expansion(def, line, i + 1);
                    n += 1;
                }
            }
        }

        /* Make sure a complete line is read, to not mix directives. */
        if (t.token != NEWLINE) {
            t = get_token();
            n += read_complete_line(line, t, 0);
        }
    }

    return n;
}

/*
 * Add preprocessed token to lookahead buffer, ready to be consumed by
 * the parser.
 *
 * This is the last step of preprocessing, where we also do join of
 * adjacent string literals, and conversion from preprocessing number to
 * proper numeric values.
 */
static void add_to_lookahead(jayl_token_t t)
{
    String s;
    jayl_token_t prev;

    if (!output_preprocessed) {
        if (t.token == STRING && deque_len(&lookahead)) {
            prev = deque_back(&lookahead);
            if (prev.token == STRING) {
                t.d.string = str_cat(prev.d.string, t.d.string);
                deque_back(&lookahead) = t;
                goto added;
            }
        } else if (t.token == PREP_NUMBER) {
            t = jayl_token_convert(t);
        }
    }

    deque_push_back(&lookahead, t);

added:
    if (context.verbose) {
        s = jayl_token_str(t);
        verbose("   token( %s )", str_raw(s));
    }
}

/*
 * Determine whether we need to read more input in anticipation of a new
 * string literal needing to be joined with the current lookahead. This
 * is the case if buffer is non-empty, and last element is STRING, which
 * can be followed by any number of NEWLINE.
 */
static int is_lookahead_ready(int n)
{
    unsigned len;
    jayl_token_t last;

    len = deque_len(&lookahead);
    if (len < n) {
        return 0;
    }

    if (len > 0 && !output_preprocessed) {
        last = deque_back(&lookahead);
        if (last.token == STRING) {
            return 0;
        }
    }

    return 1;
}

/*
 * Consume at least one line, up until the final newline or end of file.
 * Fill up lookahead buffer to hold at least n tokens. In case of end of
 * input, put END tokens in remaining slots.
 */
static void preprocess_line(int n)
{
    static TokenArray line;

    int i;
    jayl_token_t t;

    ensure_initialized();
    do {
        t = get_token();
        if (t.token == END) {
            array_clear(&line);
            break;
        }

        line.length = 0;
        if (t.token == '#') {
            t = get_token();
            if (in_active_block() || (
                t.token == IF ||
                !tok_cmp(t, ident__ifdef) ||
                !tok_cmp(t, ident__ifndef) ||
                !tok_cmp(t, ident__elif) ||
                !tok_cmp(t, ident__endif) ||
                t.token == ELSE))
            {
                read_complete_line(&line, t, 1);
                preprocess_directive(&line);
            } else {
                line_buffer = NULL;
            }
        } else {
            assert(in_active_block());
            i = read_complete_line(&line, t, 0);
            while (i && expand(&line)) {
                i = refill_expanding_line(&line);
            }
            for (i = 0; i < array_len(&line); ++i) {
                t = array_get(&line, i);
                if (t.token != NEWLINE || output_preprocessed) {
                    add_to_lookahead(t);
                }
            }
        }
    } while (!is_lookahead_ready(n));

    while (deque_len(&lookahead) < n) {
        add_to_lookahead(basic_token[END]);
    }
}

void inject_line(char *line)
{
    assert(!line_buffer);
    line_buffer = line;
    preprocess_line(0);
    while (deque_len(&lookahead) && deque_back(&lookahead).token == END) {
        (void) deque_pop_back(&lookahead);
    }

    line_buffer = NULL;
}

jayl_token_t next(void)
{
    if (deque_len(&lookahead) < 1) {
        preprocess_line(1);
    }

    return deque_pop_front(&lookahead);
}

jayl_token_t peek(void)
{
    return peekn(1);
}

jayl_token_t peekn(int n)
{
    assert(n > 0);
    if (deque_len(&lookahead) < n) {
        preprocess_line(n);
    }

    return deque_get(&lookahead, n - 1);
}

jayl_token_t consume(jayl_token_type_t token)
{
    String s;
    jayl_token_t t = next();

    if (t.token != token) {
        s = jayl_token_str(t);
        switch (token) {
        case IDENTIFIER:
        case NUMBER:
        case STRING:
            error("Unexpected token '%s', expected %s.",
                str_raw(s),
                (token == IDENTIFIER) ? "identifier" :
                (token == NUMBER) ? "number" : "string");
            break;
        default:
            error("Unexpected token '%s', expected '%s'.",
                str_raw(s), str_raw(basic_token[token].d.string));
            break;
        }
        exit(1);
    }

    return t;
}

void preprocess(FILE *output)
{
    jayl_token_t t;
    String s;

    output_preprocessed = 1;
    while ((t = next()).token != END) {
        if (t.leading_whitespace) {
            fprintf(output, "%*s", t.leading_whitespace, " ");
        }
        if (t.token == STRING) {
            fprintstr(output, t.d.string); 
        } else {
            s = jayl_token_str(t);
            fprintf(output, "%s", str_raw(s));
        }
    }
}
