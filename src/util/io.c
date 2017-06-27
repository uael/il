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

#include "io.h"

#include <fcntl.h>
#include <stdio.h>

#include "u/types.h"

#include <il/adt/xmalloc.h>

int64_t il_fsize(const char *path) {
#ifdef WIN32
  WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
  if (GetFileAttributesExA(path, GetFileExInfoStandard, (void*)&fileInfo) == 0) return -1;
  return (uint64_t)(((__int64)fileInfo.nFileSizeHigh) << 32 ) + fileInfo.nFileSizeLow;
#else
  struct stat sb;
  if (stat(path, &sb) > 0) return -1;
  return (int64_t) sb.st_size;
#endif
}

const char *il_fread(const char *path, size_t *len) {
  int fd = 0;
  off_t fsize = 0;
  ssize_t fsize2 = 0;
  char *buffer = NULL;

  fsize = (size_t) il_fsize(path);
  if (fsize < 0) goto abort_read;

  fd = open(path, O_RDONLY);
  if (fd < 0) goto abort_read;

  buffer = (char *) xmalloc((size_t) fsize + 1);
  if (buffer == NULL) goto abort_read;
  buffer[fsize] = 0;

  fsize2 = read(fd, buffer, (size_t) fsize);
  if (fsize2 == -1) goto abort_read;

  if (len) *len = (size_t) fsize2;
  close(fd);
  return (const char *) buffer;

  abort_read:
  if (buffer) xfree((void *) buffer);
  if (fd >= 0) close(fd);
  return NULL;
}

bool il_fexists(const char *path) {
#ifdef WIN32
  BOOL isDirectory;
  DWORD attributes = GetFileAttributesA(path);

  // special directory case to drive the network path check
  if (attributes == INVALID_FILE_ATTRIBUTES)
    isDirectory = (GetLastError() == ERROR_BAD_NETPATH);
  else
    isDirectory = (FILE_ATTRIBUTE_DIRECTORY & attributes);

  if (isDirectory) {
    if (PathIsNetworkPathA(path)) return true;
    if (PathIsUNCA(path)) return true;
  }

  if (PathFileExistsA(path) == 1) return true;
#else
  if (access(path, F_OK) == 0) return true;
#endif

  return false;
}

bool il_fwrite(const char *path, const char *buffer, size_t len) {
  // RW for owner, R for group, R for others
#ifdef _WIN32
  unsigned mode = _S_IWRITE;
#else
  unsigned mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
#endif

  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if (fd < 0) return false;

  ssize_t nwrite = write(fd, buffer, len);
  close(fd);

  return ((size_t) nwrite == len);
}


bool il_is_dir(const char *path) {
#ifdef WIN32
  DWORD dwAttrs;

  dwAttrs = GetFileAttributesA(path);
  if (dwAttrs == INVALID_FILE_ATTRIBUTES) return false;
  if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY) return true;
#else
  struct stat buf;

  stat(path, &buf);
  if (S_ISDIR(buf.st_mode)) return true;
#endif

  return false;
}

DIRREF il_dir_init(const char *dirpath) {
#ifdef WIN32
  WIN32_FIND_DATA findData;
  WCHAR			path[MAX_PATH];
  WCHAR			dirpathW[MAX_PATH];
  HANDLE			hFind;
  (void)hFind;

  // convert dirpath to dirpathW
  MultiByteToWideChar(CP_UTF8, 0, dirpath, -1, dirpathW, MAX_PATH);

  // in this way I can be sure that the first file returned (and lost) is .
  PathCombineW(path, dirpathW, _T("*"));

  // if the path points to a symbolic link, the WIN32_FIND_DATA buffer contains
  // information about the symbolic link, not the target
  return FindFirstFileW(path, &findData);
#else
  return opendir(dirpath);
#endif
}

const char *il_dread(DIRREF ref) {
  if (ref == NULL) return NULL;

  while (1) {
#ifdef WIN32
    WIN32_FIND_DATA findData;
    char 			*file_name;

    if (FindNextFile(ref, &findData) == 0) {
      FindClose(ref);
      return NULL;
    }
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
    if (findData.cFileName == NULL) continue;
    if (findData.cFileName[0] == '.') continue;
    return (const char*)findData.cFileName;
#else
    struct dirent *d;
    if ((d = readdir(ref)) == NULL) {
      closedir(ref);
      return NULL;
    }
    if (d->d_name[0] == 0) continue;
    if (d->d_name[0] == '.') continue;
    return (const char *) d->d_name;
#endif
  }
  return NULL;
}
