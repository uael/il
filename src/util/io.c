/*
 * MIT License
 *
 * Copyright (c) 2014 CreoLabs
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "io.h"

#include <fcntl.h>
#include <stdio.h>

#if defined(_WIN32)
# include <stdlib.h>
# include <io.h>
# include <process.h> /* for getpid() and the exec..() family */
# include <direct.h> /* for _getcwd() and _chdir() */
# include <sys\types.h>
# include <sys\stat.h>
# ifndef __HAVE_UNISTD
# define __HAVE_UNISTD

# define srandom srand
# define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
# define R_OK    4       /* Test for read permission.  */
# define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
# define F_OK    0       /* Test for existence.  */

# define access _access
# define dup2 _dup2
# define execve _execve
# define ftruncate _chsize
# define unlink _unlink
# define fileno _fileno
# define getcwd _getcwd
# define chdir _chdir
# define isatty _isatty
# define lseek _lseek
/* read, write, and close are NOT being #defined here, because while there are file handle specific versions for Windows, they probably don't work for sockets. You need to look at your app and consider whether to call e.g. closesocket(). */

# define ssize_t int

# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2
/* should be in some equivalent to <sys/types.h> */
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
# endif
#else
# include <unistd.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
# include <sys/time.h>
# include <sys/stat.h>
#elif defined(_WIN32)
# include <windows.h>
# include <Shlwapi.h>
# include <tchar.h>
#elif defined(__MACH__)
# include <mach/mach_time.h>
#endif

#include <adt/xmalloc.h>

int64_t jl_fsize(const char *path) {
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

const char *jl_fread(const char *path, size_t *len) {
  int fd = 0;
  off_t fsize = 0;
  ssize_t fsize2 = 0;
  char *buffer = NULL;

  fsize = (size_t) jl_fsize(path);
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
  if (buffer) free((void *) buffer);
  if (fd >= 0) close(fd);
  return NULL;
}

bool jl_fexists(const char *path) {
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

const char *jl_fbuildpath(const char *filename, const char *dirpath) {
//	#ifdef WIN32
//	PathCombineA(result, filename, dirpath);
//	#else
  size_t len1 = strlen(filename);
  size_t len2 = strlen(dirpath);
  size_t len = len1 + len2 + 2;

  char *full_path = (char *) xmalloc(len);
  if (!full_path) return NULL;

  if ((len2) && (dirpath[len2 - 1] != '/'))
    snprintf(full_path, len, "%s/%s", dirpath, filename);
  else
    snprintf(full_path, len, "%s%s", dirpath, filename);
//	#endif

  return (const char *) full_path;
}

bool jl_fwrite(const char *path, const char *buffer, size_t len) {
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


bool jl_is_dir(const char *path) {
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

DIRREF jl_dir_init(const char *dirpath) {
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

const char *jl_dread(DIRREF ref) {
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
