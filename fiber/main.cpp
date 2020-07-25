#include "fiber.hpp"
#include <iostream>
#include <time.h>
#include <sys/time.h>

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

struct ffunc_blink_light
{
  int i;
  FFUNC()
  {
    FFUNC_BEGIN
    for(i=0; i<3; ++i)
    {
      //   light_on();
      std::cout <<"Light is on!" << std::endl;
      FFUNC_CALL(ffunc_sleep, (1.0f)); // delay for 1 second (yields)
      std::cout <<"Light is off!" << std::endl;
      FFUNC_CALL(ffunc_sleep, (1.0f)); // delay for 1 second (yields)
    }
    FFUNC_END
  }
};

int main() {
    ffunc_callstack cs; // allocate stack to hold the function state
    FFUNC_START(cs, ffunc_blink_light, ()); // start blink_light fiber function in the stack
    getchar();
    float cur_time=current_timestamp(), prev_time=cur_time;
    while(cs.tick(cur_time-prev_time)) // tick function in the stack until it completes
    {
        prev_time=cur_time;
        cur_time=current_timestamp();
    }
}