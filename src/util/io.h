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

#ifndef   WULK_IO_H__
# define  WULK_IO_H__

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

#include <stdint.h>
#include <stddef.h>
#include "adt/bool.h"

int64_t wulk_fsize(const char *path);
const char *wulk_fread(const char *path, size_t *len);
bool wulk_fexists(const char *path);
bool wulk_fwrite(const char *path, const char *buffer, size_t len);

#if defined(_WIN32)
# include <windows.h>
# define DIRREF HANDLE
#else
# include <dirent.h>
# define DIRREF DIR*
#endif

bool wulk_is_dir(const char *path);
DIRREF wulk_dir_init(const char *path);
const char *wulk_dread(DIRREF ref);

#endif /* WULK_IO_H__ */
