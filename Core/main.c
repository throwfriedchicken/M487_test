/**************************************************************************//**
 * @file     main.c
 * @brief
 *           Use ADC to get Voltage data and send it out by PWM
 * @note
 * Copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
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

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1);

    /* Enable UART clock */
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
    
    /* Select UART clock source from HXT */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1,MODULE_NoMsk);
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();



    /* Set GPB multi-function pins for UART0 RXD and TXD */
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


    /* Lock protected registers */
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
