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

#ifndef  IL_ERR_H__
# define IL_ERR_H__

typedef enum il_errlvl il_errlvl_t;
typedef struct il_err il_err_t;

enum il_errlvl {
  IL_ERR_LVL_NOTICE,
  IL_ERR_LVL_WARNING,
  IL_ERR_LVL_ERROR,
  IL_ERR_LVL_FATAL
};

struct il_err {
  il_errlvl_t lvl;
  const char *msg;
  struct il_err *prev;
};

#endif /* IL_ERR_H__ */
