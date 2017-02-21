#ifndef _JAYL_JAY_PARSER_H_
#define _JAYL_JAY_PARSER_H_

#include "frontend/parser.h"

#define JAY_SRC_EXT ".jay"

void jay_parse_str(parser_t *this, const char *buffer, fir_tus_t *translation_units);

void jay_parse(parser_t *this, fir_tus_t *translation_units);

#endif /* _JAYL_JAY_PARSER_H_ */
