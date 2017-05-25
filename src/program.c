/*
 * Wulk - Wu uniform language kit
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

void wulk_program_init(wulk_program_t *self) {
  *self = (wulk_program_t) {{0}};
}

void wulk_program_dtor(wulk_program_t *self) {
  wulk_entity_t entity;

  adt_vector_foreach(self->entities, entity) {
    wulk_entity_dtor(&entity);
  }
  adt_vector_dtor(self->entities);
}
