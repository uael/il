#ifndef _JAYL_JAY_PARSER_H_
#define _JAYL_JAY_PARSER_H_

#include "fir_parser.h"

#define JAY_SRC_EXT ".jay"

void jay_parse_str(fir_parser_t *this, const char *buffer, fir_tus_t *translation_units);

void jay_parse(fir_parser_t *this, fir_tus_t *translation_units);

#endif /* _JAYL_JAY_PARSER_H_ */
