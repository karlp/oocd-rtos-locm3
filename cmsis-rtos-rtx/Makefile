
PROJECT	    = rtos2-blink

RTOS2_DIR = CMSIS_5/CMSIS/RTOS2
RTOS2_INCS = $(RTOS2_DIR)/RTX/Include $(RTOS2_DIR)/Include
# We can't use cmsis's systick with libopencm3, as it expects all the cmsis headers
#RTOS2_SRC = $(RTOS2_DIR)/Source/os_systick.c
RTOS2_SRCS = os_systick_locm3.c
RTOS2_SRCS += rtx_config.c
RTOS2_SRCS += $(RTOS2_DIR)/RTX/Source/GCC/irq_cm3.S
RTOS2_SRCS += rtx_delay.c rtx_evflags.c rtx_evr.c rtx_kernel.c
RTOS2_SRCS += rtx_lib.c rtx_memory.c rtx_mempool.c rtx_msgqueue.c
RTOS2_SRCS += rtx_mutex.c rtx_semaphore.c rtx_system.c
RTOS2_SRCS += rtx_thread.c rtx_timer.c

VPATH += $(RTOS2_DIR)/RTX/Source $(RTOS2_DIR)/RTX/Source/GCC

CMSIS_INCS = CMSIS_5/CMSIS/Core/Include CMSIS_5/Device/ARM/ARMCM3/Include
# Inclusion of header files
INCLUDES += $(patsubst %,-I%, . $(RTOS2_INCS) $(CMSIS_INCS))

CFILES = $(PROJECT).c
CFILES += $(RTOS2_SRCS)

OPENCM3_DIR=../libopencm3
DEVICE=stm32l151xb
OOCD_FILE = openocd.stm32l1-generic.cfg
include ../rules.mk

