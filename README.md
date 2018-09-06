blinky tasks to try out openocd's support for rtos's

# Directories

## fr-blink (works for some versions)
FreeRTOS + libopencm3.  See the README and Makefile there.

  * Freertos 8.2.1 [pass]
  * Freertos 8.2.3 [pass]
  * Freertos 9.0.0 [fail]
  * Freertos 10.0.1 [fail]
  * Freertos 10.1.0 [fail]

## nuttx-blink (sort of maybe works)
NuttX, see the README there.

  * NuttX 7.25 ~sort of works, if you're generous.

## chibi (works)
Just a note on how to build their demo. NO attempt at skipping chibiHAL
