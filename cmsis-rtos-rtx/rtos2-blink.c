/*
 * libopencm3+cmsis-rtos2
 * Consider to be BSD2 Clause, Apache 2.0, MIT, or ISC licensed, at your
 * pleasure.
 * karl Palsson <karlp@tweak.net.au>
 */

#include <assert.h>
#include <string.h>

#include "cmsis_os2.h"

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
static osTimerId_t timerBlue;

static void timerBlueCB(void *args)
{
	(void) args;
	gpio_toggle(LED_BLUE_PORT, LED_BLUE_PIN);
}

/**
 * separate task, even to use a timer, as osTimerStart can't be
 * called before osKernelStart()
 * so we just start our timers in this task and it exits
 */
static void taskBlueBlink(void *args)
{
	(void)args;
	osTimerAttr_t attrBlue = {
		.name = "blue.timer"
	};
	timerBlue = osTimerNew(timerBlueCB, osTimerPeriodic, NULL, &attrBlue);
	if (timerBlue) {
		if (osTimerStart(timerBlue, 200) != osOK) {
			/* FIXME - trace here please */
		}
	} else {
		/* FIXME - trace here please */
	}
}

#if defined (LED_GREEN_PORT)
static void taskGreenBlink(void *args)
{
	(void) args;
	while (1) {
		osDelay(1000);
		gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
	}
}
#endif


int main(void)
{
	rcc_clock_setup_pll(&rcc_clock_config[RCC_CLOCK_VRANGE1_HSI_PLL_32MHZ]);
	/* Leaving this at zero is also "no sub groups"
	 * Explicitly setting it to 3 just matches the number of
	 * priorities available on cm3 but serves no other purpose than
	 * to confuse the reader and be less portable to cm0.
	scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);
	 */
	/* CMSIS-RTOS2 just asks that systick, pendsv and svc are all equal priority.
	 * _ALL_ handlers are at priority 0 at reset, so there's nothing to do.
	 * We just need to make sure that any _application_ irqs we install are
	 * at a lower priority (higher number) than 0.
	 */

	gpio_setup();
	osKernelInitialize();
#if defined (LED_GREEN_PORT)
	osThreadAttr_t attrGreen = {
		.name = "green.task"
	};
	osThreadNew(taskGreenBlink, NULL, &attrGreen);
#endif
	osThreadAttr_t attrBlue = {
		.name = "blue.task"
	};
	osThreadNew(taskBlueBlink, NULL, &attrBlue);

	osKernelStart();   

	return 0;
}

