#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <jayl/ir.h>

struct block *expression(struct definition *def, struct block *block);

struct var constant_expression(void);

struct block *assignment_expression(
	struct definition *def,
	struct block *block
);

#endif
