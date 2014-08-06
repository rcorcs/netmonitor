#include "timer.h"

#ifdef __WIN32

__inline UINT timer(time_t sec, time_t msec, TIMERPROC timer_handler )
{
   return SetTimer(NULL, 0, ((sec*1000)+msec), timer_handler);
}

__inline void close_timer(UINT tid)
{
    KillTimer(NULL, tid);
}

__inline void wait_timer_signal( VOID (*timer_signal)(MSG) )
{
   MSG msg;
   if(GetMessage(&msg, NULL, 0, 0)){
      DispatchMessage(&msg);
      if(timer_signal)
         timer_signal(msg);
   }
}

__inline bool_t peek_timer_signal( VOID (*timer_signal)(MSG) )
{
   MSG msg;
   if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
      DispatchMessage(&msg);
      if(timer_signal)
         timer_signal(msg);
      return BOOL_TRUE;
   }else return BOOL_FALSE;
}

#else

void timer(time_t sec, time_t msec, void (*signal_handler)(int) )
{

   struct timeval start_after = { sec, msec };
   struct timeval interval = { sec, msec };

   timer_val.it_interval = interval;
   timer_val.it_value    = start_after;

   setitimer(ITIMER_REAL, &timer_val, 0);
   signal(SIGALRM, signal_handler);
}

void close_timer()
{
   struct timeval start_after = { 0L, 0L };
   struct timeval interval = { 0L, 0L };

   timer_val.it_interval = interval;
   timer_val.it_value    = start_after;

   setitimer(ITIMER_REAL, &timer_val, 0);
}

#endif
