/*
 * time/timer.h
 *
 *  Global definitions for a timer implementation.
 *
 * version:   1.0.0
 * date:      15 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
*/
#ifndef __TIMER
#define __TIMER

#ifdef __cplusplus
extern "C" {
#endif

#include "../datatypes/primitives.h"

#include <stdio.h>

#ifdef __WIN32

#include <windows.h>

__inline UINT timer(time_t sec, time_t msec, TIMERPROC timer_handler );
__inline void close_timer(UINT tid);
__inline void wait_timer_signal( VOID (*timer_signal)(MSG) );
__inline bool_t peek_timer_signal( VOID (*timer_signal)(MSG) );

#else

#include <signal.h>
#include <sys/time.h>

struct itimerval timer_val;

void timer(time_t sec, time_t msec, void (*signal_handler)(int) );

void close_timer();

#endif

#ifdef __cplusplus
}
#endif

#endif
/*
LINUX EXAMPLE:
int scounter;

void foo(int signal_code)
{
	printf("SIGALRM signal: %d seconds\n", scounter);
        if((scounter++)==10) exit(0);
}

int main()
{
   scounter = 1;

   timer(1, 0, foo);

   while(1) {}

   return 0;
}

WINDOWS EXAMPLE:
VOID CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) {
printf("Timer: %d\n",dwTime);
}

VOID TimerSignal(MSG msg)
{
   printf("%d, %d\n", msg.pt.x, msg.pt.y);
}

int main()
{
   UINT timerid = timer(1,0,TimerProc);
   while(1) {
	  peek_timer_signal(TimerSignal);
   }
   close_timer(timerid);
   return 0;
}
*/
