#ifndef STATEMENT_H
#define STATEMENT_H

#include <jayl/ir.h>

struct block *statement(struct definition *def, struct block *parent);

struct block *block(struct definition *def, struct block *parent);

#endif
