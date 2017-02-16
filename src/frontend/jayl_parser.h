#ifndef _JAYL_JAYL_PARSER_H_
#define _JAYL_JAYL_PARSER_H_

#include "fir_parser.h"

#define JAYL_SRC_EXT ".jay"

void jayl_parse_str(fir_parser_t *this, const char *buffer, fir_tus_t *translation_units);

void jayl_parse(fir_parser_t *this, fir_tus_t *translation_units);

#endif /* _JAYL_JAYL_PARSER_H_ */
