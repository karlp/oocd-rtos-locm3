/*
 * libopencm3+freertos
 * Consider to be BSD2 Clause, Apache 2.0, MIT, or ISC licensed, at your
 * pleasure.
 * karl Palsson <karlp@tweak.net.au>
 */

#include <assert.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define LED_BLUE_PORT           GPIOB
#define LED_BLUE_PIN            GPIO6
#define LED_GREEN_PORT          GPIOB
#define LED_GREEN_PIN           GPIO7

static void gpio_setup(void)
{
	/* blinken lights */
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);
#if defined(LED_GREEN_PORT)
	gpio_mode_setup(LED_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GREEN_PIN);
#endif
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
	while (1) {
		vTaskDelay(portTICK_PERIOD_MS * 1000);
		gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
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

int main(void)
{
	rcc_clock_setup_pll(&rcc_clock_config[RCC_CLOCK_VRANGE1_HSI_PLL_32MHZ]);
	gpio_setup();
	scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);

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

	vTaskStartScheduler();

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

