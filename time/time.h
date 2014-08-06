/*
 * time/time.h
 * 
 *  Global definitions for some useful functions related to time.
 *
 * version:   1.0.0
 * date:      15 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
*/
#ifndef __TIME
#define __TIME

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>


/*
 * It gets the current time from the local machine.
 * return: the current time in the tm structure.
*/
struct tm *current_time();

/*
 * It prints the time given as a parameter into the specified output stream.
*/
void fprint_time(FILE *out, struct tm *t);

/*
 * It prints the time given as a parameter into the standard output stream.
*/
__inline void print_time(struct tm *t);

#ifdef __cplusplus
}
#endif

#endif
