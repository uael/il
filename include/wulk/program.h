/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef   WULK_PROGRAM_H__
# define  WULK_PROGRAM_H__

#include <adt/hash.h>

#include "symbol.h"

typedef struct wulk_program_t wulk_program_t;

struct wulk_program_t {
  wulk_entity_r entities;
  wulk_symtab_t symtab;
};

void wulk_program_init(wulk_program_t *self);
void wulk_program_dtor(wulk_program_t *self);

#endif /* WULK_PROGRAM_H__ */
