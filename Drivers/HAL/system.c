/**
 * @file system.c
 * @author cy023
 * @date 2023.03.20
 * @brief
 */

#include "NuMicro.h"
#include "system.h"

/*******************************************************************************
 * Peripheral Driver - private function
 ******************************************************************************/
/**
 * @brief Clock peripheral initialization.
 */
static void system_clock_init(void)
{
    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
    
    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Enable UART clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART clock source from HXT */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
}

/**
 * @brief Clock peripheral initialization.
 */
static void system_clock_deinit(void)
{
    // TODO:
}

/**
 * @brief GPIO peripheral initialization.
 *
 *  - PB.5 for prog/run dectect pin
 *  - PH.4 BOOT pin as output
 *  - PH.5 LED0 pin as output
 */
static void system_gpio_init(void)
{
    // set PB5 as prog/run detect pin
    GPIO_SetMode(PB, BIT5, GPIO_MODE_INPUT);

    // set BOOT pin (PH.4) as output
    GPIO_SetMode(PH, BIT4, GPIO_MODE_OUTPUT);

    // set LED0 pin (PH.5) as output
    GPIO_SetMode(PH, BIT5, GPIO_MODE_OUTPUT);
}

/**
 * @brief GPIO peripheral deinitialization.
 */
static void system_gpio_deinit(void)
{
    // TODO:
}

/**
 * @brief UART peripheral initialization.
 */
static void system_uart0_init(void)
{
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Init UART to 38400-8n1 for print message */
    UART_Open(UART0, 38400);
}

/**
 * @brief UART peripheral deinitialization.
 */
static void system_uart0_deinit(void)
{
    UART_Close(UART0);
}

/**
 * @brief For External Flash - w25q128jv
 *
 *  FLASH_MOSI  : PA.8
 *  FLASH_SCK   : PA.10
 *  FLASH_MISO  : PG.4
 *  FLASH_CS    : PA.11
 */
static void system_spi_init(void)
{
    // TODO:
}

static void system_spi_deinit(void)
{
    // TODO:
}

/*******************************************************************************
 * Public Function
 ******************************************************************************/

void system_init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    system_gpio_init();
    system_clock_init();
    system_uart0_init();
    system_spi_init();

    /* Lock protected registers */
    SYS_LockReg();
}

void system_deinit(void)
{
    system_spi_init();
    system_uart0_init();
    system_clock_init();
    system_gpio_init();
}

void system_delay_ms(uint32_t ms)
{
    uint16_t delay;
    volatile uint32_t i;
    for (delay = ms; delay > 0; delay--) {
        // 1 ms loop with -O0 optimization.
        for (i = 19200; i > 0; i--) {;}
    }
}
