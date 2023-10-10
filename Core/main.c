/*
#include <stdio.h>
#include <stdlib.h>
#include "M480.h"
#include "i2c.h"
#include "spi.h"
#define PLL_CLOCK           192000000
//#define use_I2C
//#define i2c_port I2C2
//#define use_I2C2

void SYS_Init(void)
{
    SYS_UnlockReg();

    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    CLK_SetCoreClock(PLL_CLOCK);

    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1);

    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(SPI0_MODULE);
    #ifdef use_I2C
        #ifdef use_I2C0
            CLK_EnableModuleClock(I2C0_MODULE);
        #endif
        #ifdef use_I2C1
            CLK_EnableModuleClock(I2C1_MODULE);
        #endif
        #ifdef use_I2C2
            CLK_EnableModuleClock(I2C2_MODULE);
        #endif
    #endif    
    
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1,MODULE_NoMsk);
    
    SystemCoreClockUpdate();



    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    #ifdef use_I2C
        #ifdef use_I2C0
            SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA4MFP_Msk|SYS_GPA_MFPL_PA5MFP_Msk);
	        SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA4MFP_I2C0_SDA|SYS_GPA_MFPL_PA5MFP_I2C0_SCL);
        #endif
        #ifdef use_I2C1
            SYS->GPG_MFPL &= ~(SYS_GPG_MFPL_PG2MFP_Msk|SYS_GPG_MFPL_PG3MFP_Msk);
            SYS->GPG_MFPL |= (SYS_GPG_MFPL_PG2MFP_I2C1_SCL|SYS_GPG_MFPL_PG3MFP_I2C1_SDA);
        #endif
        #ifdef use_I2C2
            SYS->GPA_MFPH &= (SYS_GPA_MFPH_PA10MFP_Msk|SYS_GPA_MFPH_PA11MFP_Msk);
            SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA10MFP_I2C2_SDA|SYS_GPA_MFPH_PA11MFP_I2C2_SCL);
        #endif
    #endif

    SYS->GPF_MFPH &= ~(SYS_GPF_MFPH_PF9MFP_Msk|SYS_GPF_MFPH_PF8MFP_Msk);
    SYS->GPF_MFPH |= (SYS_GPF_MFPH_PF9MFP_SPI0_SS|SYS_GPF_MFPH_PF8MFP_SPI0_CLK);

    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF7MFP_Msk|SYS_GPF_MFPL_PF6MFP_Msk);
    SYS->GPF_MFPL |= (SYS_GPF_MFPL_PF7MFP_SPI0_MISO|SYS_GPF_MFPL_PF6MFP_SPI0_MOSI);


    SYS_LockReg();    
}


int main(void)
{
    SYS_Init();
	UART_Open(UART0,115200);
	//I2C_Open(i2c_port,100000);
    SPI_Open(SPI0,SPI_MASTER,SPI_MODE_0,8,24000000);
    SPI_DisableAutoSS(SPI0);
    uint8_t tmp,data=0;
	while(1){
		//I2C_WriteByte(i2c_port,0x24,0xAA);
        SPI_SET_SS_LOW(SPI0);
        SPI_WRITE_TX(SPI0,data);
        while(SPI_IS_BUSY(SPI0));
        tmp = SPI_READ_RX(SPI0);
        SPI_SET_SS_HIGH(SPI0);
        printf("%x\n",tmp);
        for(int i=0;i<19200;i++){

        }
        if(data==255)data=0;
        data++;
	}
	
}
*/
/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Demonstrate how to use EPWM counter output waveform.
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define PLL_CLOCK       192000000


void SYS_Init(void)
{

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Set PCLK0 = PCLK1 = HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV16 | CLK_PCLKDIV_APB1DIV_DIV16);

    /* Enable IP module clock */
    CLK_EnableModuleClock(EPWM0_MODULE);
    CLK_EnableModuleClock(EPWM1_MODULE);

    /* EPWM clock frequency is set double to PCLK: select EPWM module clock source as PLL */
    CLK_SetModuleClock(EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, (uint32_t)NULL);
    CLK_SetModuleClock(EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, (uint32_t)NULL);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Set PA.0~5 multi-function pin for EPWM0 channel 0~5 */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA0MFP_Msk) | SYS_GPA_MFPL_PA0MFP_EPWM0_CH5;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA1MFP_Msk) | SYS_GPA_MFPL_PA1MFP_EPWM0_CH4;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA2MFP_Msk) | SYS_GPA_MFPL_PA2MFP_EPWM0_CH3;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA3MFP_Msk) | SYS_GPA_MFPL_PA3MFP_EPWM0_CH2;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA4MFP_Msk) | SYS_GPA_MFPL_PA4MFP_EPWM0_CH1;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA5MFP_Msk) | SYS_GPA_MFPL_PA5MFP_EPWM0_CH0;

    SYS->GPE_MFPH = (SYS->GPE_MFPH & ~SYS_GPE_MFPH_PE13MFP_Msk) | SYS_GPE_MFPH_PE13MFP_EPWM1_CH0;
    SYS->GPC_MFPH = (SYS->GPC_MFPH & ~SYS_GPC_MFPH_PC8MFP_Msk) | SYS_GPC_MFPH_PC8MFP_EPWM1_CH1;
    SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPC_MFPL_PC7MFP_Msk) | SYS_GPC_MFPL_PC7MFP_EPWM1_CH2;
    SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPC_MFPL_PC6MFP_Msk) | SYS_GPC_MFPL_PC6MFP_EPWM1_CH3;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA7MFP_Msk) | SYS_GPA_MFPL_PA7MFP_EPWM1_CH4;
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~SYS_GPA_MFPL_PA6MFP_Msk) | SYS_GPA_MFPL_PA6MFP_EPWM1_CH5;
}

void UART0_Init()
{
    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

int main(void)
{
    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART to 115200-8n1 for print message */
    UART0_Init();

    printf("\n\nCPU @ %ldHz(PLL@ %ldHz)\n", SystemCoreClock, PllClock);
    printf("EPWM0 clock is from %s\n", (CLK->CLKSEL2 & CLK_CLKSEL2_EPWM0SEL_Msk) ? "CPU" : "PLL");
    printf("+------------------------------------------------------------------------+\n");
    printf("|                          EPWM Driver Sample Code                        |\n");
    printf("|                                                                        |\n");
    printf("+------------------------------------------------------------------------+\n");
    printf("  This sample code will output waveform with EPWM0 channel 0~5.\n");
    printf("  I/O configuration:\n");
    printf("  EPWM0 channel 0: 360000 Hz, duty 90%%.\n");
    printf("  EPWM0 channel 1: 320000 Hz, duty 80%%.\n");
    printf("  EPWM0 channel 2: 250000 Hz, duty 75%%.\n");
    printf("  EPWM0 channel 3: 180000 Hz, duty 70%%.\n");
    printf("  EPWM0 channel 4: 160000 Hz, duty 60%%.\n");
    printf("  EPWM0 channel 5: 150000 Hz, duty 50%%.\n");
    printf("    waveform output pin: EPWM0_CH0(PA.5), EPWM0_CH1(PA.4), EPWM0_CH2(PA.3), EPWM0_CH3(PA.2), EPWM0_CH4(PA.1), EPWM0_CH5(PA.0)\n");

    /* EPWM0 channel 0~5 frequency and duty configuration are as follows */
    EPWM_ConfigOutputChannel(EPWM0, 0, 50, 90);
    EPWM_ConfigOutputChannel(EPWM0, 1, 50, 80);
    EPWM_ConfigOutputChannel(EPWM0, 2, 50, 75);
    EPWM_ConfigOutputChannel(EPWM0, 3, 50, 70);
    EPWM_ConfigOutputChannel(EPWM0, 4, 50, 60);
    EPWM_ConfigOutputChannel(EPWM0, 5, 50, 50);
    EPWM_ConfigOutputChannel(EPWM1, 0, 50, 90);
    EPWM_ConfigOutputChannel(EPWM1, 1, 50, 80);
    EPWM_ConfigOutputChannel(EPWM1, 2, 50, 75);
    EPWM_ConfigOutputChannel(EPWM1, 3, 50, 70);
    EPWM_ConfigOutputChannel(EPWM1, 4, 50, 60);
    EPWM_ConfigOutputChannel(EPWM1, 5, 50, 50);
    /* Enable output of EPWM0 channel 0~5 */
    EPWM_EnableOutput(EPWM0, 0x3F);
    EPWM_EnableOutput(EPWM1, 0x3F);
    /* Start EPWM0 counter */
    EPWM_Start(EPWM0, 0x3F);
    EPWM_Start(EPWM1, 0x3F);
    printf("Press any key to stop.\n");
    //getchar();

    /* Start EPWM0 counter */
    //EPWM_ForceStop(EPWM0, 0x3F);

    printf("Done.");
    uint16_t duty = 0;
    while(1){
        EPWM_SET_CMR(EPWM0, 0, duty);
        EPWM_SET_CMR(EPWM1, 5, duty);
        duty++;
        for(int i=0;i<192;i++);
        if(duty==65535)duty = 0;
    }

}
