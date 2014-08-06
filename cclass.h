#ifndef _CCLASS
#define _CCLASS

#include <stdlib.h>
#include <assert.h>

#define NEW(_CLASS_) ((_CLASS_*)malloc(sizeof(_CLASS_)))
#define DELETE(_OBJ_) free(_OBJ_)
#define FUNC(_TYPE_, _NAME_, _PARAM_) _TYPE_ (*_NAME_)_PARAM_
#define CLASS( _NAME_, _BODY_ ) typedef struct _NAME_ { _BODY_ } _NAME_

#endif
