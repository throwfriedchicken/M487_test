/**
 * @file test_fpu.c
 * @author cy023
 * @date 2023.05.19
 * @brief FPU performance test.
 *        Usage: Change the `FPU` and `FLOAT-ABI` setting in Makefile.
 *
 */

#include <stdio.h>
#include "NuMicro.h"
#include "system.h"

static volatile uint32_t msec = 0;
static volatile uint32_t sec = 0;

void timer0_init(void)
{
    /* Enable TMR0 clock */
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Select TMR0 clock source from HXT */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);

    // Set timer frequency to 1000HZ
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);
}

void timer0_deinit(void)
{
    // Stop Timer 0
    TIMER_Stop(TIMER0);

    // Disable timer interrupt
    TIMER_DisableInt(TIMER0);
    NVIC_DisableIRQ(TMR0_IRQn);

    // Close TIMER0
    TIMER_Close(TIMER0);

    // CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);

    /* Disable TMR0 clock */
    CLK_DisableModuleClock(TMR0_MODULE);
}

void TMR0_IRQHandler(void)
{
    msec++;
    if (msec == 1000) {
        sec++;
        msec = 0;
    }
    TIMER_ClearIntFlag(TIMER0);
}

void test_delay(char *test_name)
{
    timer0_init();
    system_delay_ms(1500);
    timer0_deinit();

    printf("[INFO]: %s\n", test_name);
    printf("Time Consuming: %ld.%03ld sec.\n\n", sec, msec);
    msec = 0;
    sec = 0;
}

// float division
void test1(char *test_name)
{
    float a = 1.12345, b = 1.23456;

    timer0_init();
    for (int i = 0; i < 100000; i++)
        b = a / b;
    timer0_deinit();

    printf("[INFO]: %s\n", test_name);
    printf("Time Consuming: %ld.%03ld sec.\n\n", sec, msec);
    msec = 0;
    sec = 0;
}

// double division
void test2(char *test_name)
{
    double a = 1.12345, b = 1.23456;

    timer0_init();
    for (int i = 0; i < 100000; i++)
        b = a / b;
    timer0_deinit();

    printf("[INFO]: %s\n", test_name);
    printf("Time Consuming: %ld.%03ld sec.\n\n", sec, msec);
    msec = 0;
    sec = 0;
}


int main()
{
    system_init();

    printf("System Boot.\n");
    printf("[test]: FPU test ...\n\n");

    test_delay("system_delay_ms() function test");
    test1("Single-precision floating-point division test");
    test2("Double-precision floating-point division test");

    return 0;
}
