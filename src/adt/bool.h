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

#ifndef   WULK_BOOL_H__
# define  WULK_BOOL_H__

#if defined(_MSC_VER) && _MSC_VER < 1900
# define bool	unsigned char
# define true	1
# define false	0
# define __bool_true_false_are_defined	1
#else
# include <stdbool.h>
#endif

#endif /* WULK_BOOL_H__ */
