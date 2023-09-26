/*************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * @brief    A project template for M480 MCU.
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define PLL_CLOCK           192000000
#define EPWM_Prescaler 48
#define EPWM_Period 1999
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

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
    CLK_EnableModuleClock(EPWM0_MODULE);
    /* Select UART clock source from HXT */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(EPWM0_MODULE,CLK_CLKSEL2_EPWM0SEL_PLL,(uint32_t)NULL);
    SYS_ResetModule(EPWM0_RST);
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();



    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    

    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA5MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA5MFP_EPWM0_CH0);

    /* Lock protected registers */
    SYS_LockReg();
}

void PWM_init(){
    EPWM_SET_PRESCALER(EPWM0,0,EPWM_Prescaler - 1);
    EPWM_SET_CNR(EPWM0,0,EPWM_Period);
    EPWM_SET_CMR(EPWM0,0,10);
    EPWM_SET_ALIGNED_TYPE(EPWM0,BIT0,EPWM_UP_COUNTER);
    EPWM_SET_OUTPUT_LEVEL(EPWM0,BIT0,EPWM_OUTPUT_LOW,EPWM_OUTPUT_HIGH,EPWM_OUTPUT_NOTHING,EPWM_OUTPUT_NOTHING);
    EPWM_EnableOutput(EPWM0,BIT0);
    EPWM_Start(EPWM0,BIT0);
}
/*
 * This is a template project for M480 series MCU. Users could based on this project to create their
 * own application without worry about the IAR/Keil project settings.
 *
 * This template application uses external crystal as HCLK source and configures UART0 to print out
 * "Hello World", users may need to do extra system configuration based on their system design.
 */

int main()
{

    SYS_Init();
    PWM_init();
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 38400);
    /* Connect UART to PC, and open a terminal tool to receive following message */
    printf("Hello World! Hello Nuvoton!\n");
    printf("Here is Nuvoton M487KMCAN.\n\n");
    printf("Mother Fucker\n\n");

    /* Got no where to go, just loop forever */
    while(1);

}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
