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

#ifndef   WULK_ATTR_H__
# define  WULK_ATTR_H__

/**
 * @def WULK_NORETURN
 * Attribute to mark a function which never returns.
 */
#if defined(__GNUC__) && __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 70)
# define WULK_NORETURN __attribute__((__noreturn__)) void
#elif defined(__STDC__) && (__STDC_VERSION__ >= 201112L)
# define WULK_NORETURN _Noreturn void
#elif defined(_MSC_VER)
# define WULK_NORETURN void __declspec(noreturn)
#else
# define WULK_NORETURN void
#endif

#endif /* WULK_ATTR_H__ */
