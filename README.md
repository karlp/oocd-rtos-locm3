blinky tasks to try out openocd's support for rtos's

# Directories

## fr-blink (works)
FreeRTOS + libopencm3.  See the README and Makefile there.

  * Freertos 8.2.1 [pass]
  * Freertos 8.2.3 [pass]
  * Freertos 9.0.0 [pass]
  * Freertos 10.0.1 [pass]
  * Freertos 10.1.0 [fail because of freertos bug]
  * Freertos 10.2.1 [pass]
  * Freertos 10.3.0 [pass]
  * Freertos 10.4.0 [pass]

## nuttx-blink (sort of maybe works)
NuttX, see the README there.

  * NuttX 7.25 ~sort of works, if you're generous.

## chibi (works)
Just a note on how to build their demo. NO attempt at skipping chibiHAL
