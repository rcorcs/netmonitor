#ifndef _CFILE
#define _CFILE

#include "cstr.h"

#if defined __unix__

#include <fcntl.h>
#include <sys/stat.h>

typedef struct _cfile {
   int desc;
   cstr *name;
} cfile;

cfile *cfile_create(const char *filename)
{
    cfile *file = (cfile*)malloc(sizeof(cfile));
    file->desc = 0;
    file->name = cstr_create(filename);
    return file;
}

void cfile_destroy(cfile **file)
{
    if(file){
        cstr_destroy(&(*file)->name);
        free(*file);
        *file = 0;
    }
}

#elif defined _WIN32
#error  PLEASE USE LINUX, IT IS A BETTER OPERATING SYSTEM
#else
#error THE SYSTEM IS NOT UNIX-LIKE
#endif

#endif
