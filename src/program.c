/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#include "program.h"

#include "entity.h"

void il_program_init(il_program_t *self) {
  *self = (il_program_t) {{0}};
}

void il_program_dtor(il_program_t *self) {
  il_entity_t entity;

  adt_vector_foreach(self->entities, entity) {
    il_entity_dtor(&entity);
  }
  adt_vector_dtor(self->entities);
}
