### freertos blinking via a task and a timer, using libopencm3

## Status 

 * Freertos 8.2.x: working both blinks and thread info
 * Freertos 9.0.0: working both
 * Freertos 10.0.1: working both
 * Freertos 10.1.0: blink works, thread support broken because of freertos bug. (see below)
 * Freertos 10.2.1: working both
 * Freertos 10.3.0: working both

You _must_ be using the FreeRTOS-openocd.c file from after v0.10.0 or later.
(My earlier test code here had been imported from various places and used
an old buggy helper. See http://openocd.zylin.com/#/c/5577

## Older notes.

Originally this repo was to work out what really did and didn't work after seeing
various inconsistencies and problems with the rtos support.  For reference, with
working rtos support, you should see things like this:

(gdb) info threads
  Id   Target Id         Frame 
* 1    Thread 536873160 "" (Name: IDLE, State: Running) prvCheckTasksWaitingTermination ()
    at /home/karlp/src/FreeRTOSV8.2.3/FreeRTOS/Source/tasks.c:3038
  2    Thread 536872312 "" (Name: green.blink) vPortYield ()
    at /home/karlp/src/FreeRTOSV8.2.3/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c:376
  3    Thread 536873768 "" (Name: Tmr Svc) vPortYield ()
    at /home/karlp/src/FreeRTOSV8.2.3/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c:376
(gdb)

and things like "run" should work as expected.

When it's not working, you get things like this: (and you'll notice bad things happening on the oocd -d3 console)

(gdb) c
Continuing.
^C
Program received signal SIGINT, Interrupt.
[Switching to Thread 536873160]
(gdb) info threads
No threads.
(gdb) run 
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Can't kill process
(gdb) 


Download and extract freertos zips from: https://sourceforge.net/projects/freertos/files/FreeRTOS/
