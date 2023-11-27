
PROJECT	    = fr-cherry1

#FREERTOS_DIR = /home/karlp/src/FreeRTOSV8.2.1/FreeRTOS
#FREERTOS_DIR = /home/karlp/src/FreeRTOSV8.2.3/FreeRTOS
#FREERTOS_DIR = /home/karlp/src/FreeRTOSv9.0.0/FreeRTOS
#FREERTOS_DIR = /home/karlp/src/FreeRTOSv10.0.1/FreeRTOS
#FREERTOS_DIR = /home/karlp/src/FreeRTOSv10.1.0/FreeRTOS
#FREERTOS_DIR = /home/karlp/src/FreeRTOSv10.2.1/FreeRTOS
#FREERTOS_PORT = $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3
#FREERTOS_INC = $(FREERTOS_DIR)/Source/include
#FREERTOS_SRC = $(FREERTOS_DIR)/Source
#FREERTOS_MMG = $(FREERTOS_DIR)/Source/portable/MemMang
FREERTOS_DIR = /home/karlp/src/FreeRTOS-KernelV10.6.1
FREERTOS_PORT = $(FREERTOS_DIR)/portable/GCC/ARM_CM3
FREERTOS_INC = $(FREERTOS_DIR)/include
FREERTOS_SRC = $(FREERTOS_DIR)/
FREERTOS_MMG = $(FREERTOS_DIR)/portable/MemMang
#FREERTOS_SRCS = list.c queue.c tasks.c timers.c port.c heap_1.c
FREERTOS_SRCS = list.c queue.c tasks.c timers.c port.c heap_4.c
FREERTOS_SRCS += FreeRTOS-openocd.c

VPATH += $(FREERTOS_SRC) $(FREERTOS_PORT) $(FREERTOS_MMG)


CHERRY = ../extern/cherryusb
CHERRY_SRCS = usb_osal_freertos.c
CHERRY_SRCS += usb_dc_dwc2.c usb_hc_dwc2.c usb_glue_st.c
CHERRY_SRCS += usbd_core.c usbh_core.c
# Class stuff for host
CHERRY_SRCS += usbh_hub.c usbh_cdc_acm.c usbh_hid.c usbh_msc.c
CHERRY_SRCS += usb_host.c # demo file!

CHERRY_INCS += $(CHERRY)/class/cdc
CHERRY_INCS += $(CHERRY)/class/hid
CHERRY_INCS += $(CHERRY)/class/hub
CHERRY_INCS += $(CHERRY)/class/msc
CHERRY_INCS += $(CHERRY)/class/msc
CHERRY_INCS += $(CHERRY)/class/audio
CHERRY_INCS += $(CHERRY)/class/video
CHERRY_INCS += $(CHERRY)/core
CHERRY_INCS += $(CHERRY)/common
CHERRY_INCS += $(CHERRY)/demo
CHERRY_INCS += $(CHERRY)/osal
CHERRY_INCS += $(CHERRY)/port/dwc2
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-missing-prototypes

VPATH += $(CHERRY_INCS)
INCLUDES += $(patsubst %,-I%, $(CHERRY_INCS))


# Inclusion of header files
INCLUDES += $(patsubst %,-I%, . $(FREERTOS_INC) $(FREERTOS_PORT))

LDFLAGS += -Wl,--undefined=uxTopUsedPriority
#LDFLAGS += -Wl,--print-gc-sections

CFILES = $(PROJECT).c
CFILES += $(FREERTOS_SRCS)
CFILES += $(CHERRY_SRCS)

OPENCM3_DIR=../libopencm3
DEVICE=stm32f429zi
#LDSCRIPT=special.ld
OOCD_FILE = openocd.stm32l1-generic.cfg
include ../rules.mk

