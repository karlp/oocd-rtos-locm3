/*
 * libopencm3+freertos
 * Consider to be BSD2 Clause, Apache 2.0, MIT, or ISC licensed, at your
 * pleasure.
 * karl Palsson <karlp@tweak.net.au>
 */

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#define LED_BLUE_PORT           GPIOB
#define LED_BLUE_PIN            GPIO7
#define LED_GREEN_PORT          GPIOB
#define LED_GREEN_PIN           GPIO14

#define USART_CONSOLE		USART3

int _write(int file, char *ptr, int len);

static void gpio_setup(void)
{
	/* blinken lights */
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);
#if defined(LED_GREEN_PORT)
	gpio_mode_setup(LED_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GREEN_PIN);
#endif
	/* USART */ 
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_USART3);
}

static void usart_setup(void)
{
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
	gpio_set_af(GPIOD, GPIO_AF7, GPIO8);

	usart_set_baudrate(USART_CONSOLE, 115200);
	usart_set_databits(USART_CONSOLE, 8);
	usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
	usart_set_mode(USART_CONSOLE, USART_MODE_TX);
	usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
	usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART_CONSOLE);
}

/**
 * Use USART_CONSOLE as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART_CONSOLE, '\r');
			}
			usart_send_blocking(USART_CONSOLE, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

static void prvTimerBlue(TimerHandle_t xTimer)
{
	(void) xTimer;
	gpio_toggle(LED_BLUE_PORT, LED_BLUE_PIN);
}

#if defined (LED_GREEN_PORT)
static void prvTaskGreenBlink1(void *pvParameters)
{
	(void) pvParameters;
	int i = 0;
	while (1) {
		vTaskDelay(portTICK_PERIOD_MS * 1000);
		gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
		i++;
		printf("lol: %d\n", i);
	}

	/* Tasks must not attempt to return from their implementing
	function or otherwise exit.  In newer FreeRTOS port
	attempting to do so will result in an configASSERT() being
	called if it is defined.  If it is necessary for a task to
	exit then have the task call vTaskDelete( NULL ) to ensure
	its exit is clean. */
	vTaskDelete(NULL);
}
#endif

static TimerHandle_t xBlueTimer;


void usb_hc_low_level_init(void)
{
	printf("init hc low\n");
	// Goal is to setup usb lines for af, and enable the "correct" irq for whatever damn port we're on!
	// nucleo f429zi (indeed, all nucleo144) says it's the FS one...
	// so, PA11 and 12 need to go to AF usb..
	rcc_periph_clock_enable(RCC_OTGFS);
	// MORE! I haven't setup the usb pll here yet I don't think!
        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11|GPIO12);
        gpio_set_af(GPIOD, GPIO_AF10, GPIO11|GPIO12);

	nvic_set_priority(NVIC_OTG_FS_IRQ, 6<<4);
        nvic_enable_irq(NVIC_OTG_FS_IRQ);
	
}

void usb_hc_low_level_deinit(void)
{
	printf("deinit hc low\n");
	// this is probably enough?
	nvic_disable_irq(NVIC_OTG_FS_IRQ);
}

int SystemCoreClock;

int main(void)
{
	//rcc_clock_setup_pll(&rcc_clock_config[RCC_CLOCK_VRANGE1_HSI_PLL_32MHZ]);
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
	SystemCoreClock = 168000000; // lol, cmsis wat
	gpio_setup();
	usart_setup();
	scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);
	printf("pre rtos...\n");

#if defined (LED_GREEN_PORT)
	xTaskCreate(prvTaskGreenBlink1, "green.blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif
	xBlueTimer = xTimerCreate("blue.blink", 200 * portTICK_PERIOD_MS, true, 0, prvTimerBlue);
	if (xBlueTimer) {
		if (xTimerStart(xBlueTimer, 0) != pdTRUE) {

		}
	} else {
		/* FIXME - trace here please */
	}

	usbh_initialize();
	usbh_class_test(); // cherry demo code

	vTaskStartScheduler();
	printf("should not reach here");
	while(1) {;}

	return 0;
}

void vAssertCalled(const char * const pcFileName, unsigned long ulLine)
{
	volatile unsigned long ulSetToNonZeroInDebuggerToContinue = 0;

	/* Parameters are not used. */
	(void) ulLine;
	(void) pcFileName;

	taskENTER_CRITICAL();
	{
		while (ulSetToNonZeroInDebuggerToContinue == 0) {
			/* Use the debugger to set ulSetToNonZeroInDebuggerToContinue to a
			non zero value to step out of this function to the point that raised
			this assert(). */
			__asm volatile( "NOP");
			__asm volatile( "NOP");
		}
	}
	taskEXIT_CRITICAL();
}

