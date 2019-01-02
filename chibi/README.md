##Building
Just use chibios's tree.  it's too much work to just copy it here.

Tested version: 18.2.1
```
cd ChibiOS_18.2.1/demos/STM32/RT-STM32L152-DISCOVERY
make
```


working info: 
```
(gdb) info threads
  Id   Target Id         Frame 
* 1    Thread 536873560 "" (Name: idle, State: CURRENT) _idle_thread (p=0x0) at ../../../os/rt/src/chsys.c:72
  2    Thread 536873276 "" (Name: main, State: SLEEPING) chSchGoSleepTimeoutS (newstate=<optimized out>, 
    timeout=<optimized out>) at ../../../os/rt/src/chschd.c:384
  3    Thread 536875944 "" (Name: blinker, State: SLEEPING) chSchGoSleepTimeoutS (newstate=<optimized out>, 
    timeout=<optimized out>) at ../../../os/rt/src/chschd.c:384
  4    Thread 536876272 "" (Name: blinker, State: SLEEPING) chSchGoSleepTimeoutS (newstate=<optimized out>, 
    timeout=<optimized out>) at ../../../os/rt/src/chschd.c:384
```
