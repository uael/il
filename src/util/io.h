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

#ifndef   IL_IO_H__
# define  IL_IO_H__

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
#include <u/stdbool.h>

int64_t il_fsize(__const char *path);
__const char *il_fread(__const char *path, size_t *len);
bool il_fexists(__const char *path);
bool il_fwrite(__const char *path, __const char *buffer, size_t len);

#if defined(_WIN32)
# include <windows.h>
# define DIRREF HANDLE
#else
# include <dirent.h>
# define DIRREF DIR*
#endif

bool il_is_dir(__const char *path);
DIRREF il_dir_init(__const char *path);
__const char *il_dread(DIRREF ref);

#endif /* IL_IO_H__ */
