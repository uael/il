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

#ifndef   WULK_API_H__
# define  WULK_API_H__

#define wulk_defined(e) ((e).kind != 0)
#define wulk_pdefined(e) ((e)->kind != 0)

#define wulk_is(e, a) ((e).kind == (a))
#define wulk_pis(e, a) ((e)->kind == (a))

#endif /* WULK_API_H__ */
