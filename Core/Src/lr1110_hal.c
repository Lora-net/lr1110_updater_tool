/**
 * @file      lr1110_hal.c
 *
 * @brief     HAL implementation for LR1110 radio chip
 *
 * Revised BSD License
 * Copyright Semtech Corporation 2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lr1110_hal.h"
#include "configuration.h"
//#include "system.h"
//#include "board-config.h"
#include "stm32wbxx_hal.h"
#include "timer.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;
extern bool sleep_flag;

lr1110_hal_status_t lr1110_hal_reset( const void* radio )
{
    //radio_t* radio_local = ( radio_t* ) radio;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    //system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_LOW );
    //HAL_Delay(1);
    //system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_wakeup( const void* radio )
{
    //radio_t* radio_local = ( radio_t* ) radio;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    //HAL_Delay(1);
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_read( const void* radio, const uint8_t* cbuffer, const uint16_t cbuffer_length,
                                     uint8_t* rbuffer, const uint16_t rbuffer_length )
{
    //radio_t* radio_local = ( radio_t* ) radio;
    uint8_t  dummy_byte  = { 0 };
    uint8_t rrec[500] = { 0 };
    uint8_t rbuf[500];
    /*if(sleep_flag == true){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(1);
    sleep_flag = false;
    }*/
    //HAL_Delay(500);
    //system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );
    TimerTime_t past = TimerGetCurrentTime();
    	TimerTime_t curr = 0;
    	    while( (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) || curr<10)
    	    	{
    	    	curr = TimerGetElapsedTime( past );
    	    	}
    /* 1st SPI transaction */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, cbuffer, rbuf, cbuffer_length,3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    //system_spi_write( radio_local->spi, cbuffer, cbuffer_length );
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    //system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );
    TimerTime_t past1 = TimerGetCurrentTime();
    	TimerTime_t curr1 = 0;
    	    while( (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) || curr1<10)
    	    	{
    	    	curr1 = TimerGetElapsedTime( past1 );
    	    	}
   // while( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);
    /* 2nd SPI transaction */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, dummy_byte, 1,3000);
    HAL_SPI_Receive(&hspi2, rrec, (rbuffer_length+1), 3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    for (int i=0; i<rbuffer_length; i++)
    {
    	rbuffer[i] = rrec[i+1];
    }
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

   /*uint8_t c=rbuffer[0] / 100;
    rbuffer[0] = rbuffer[0] % 100;
    uint8_t a= rbuffer[0] / 10;
    uint8_t b= rbuffer[0] % 10;
    c=c+48;a=a+48;b=b+48;
    uint8_t c1=rbuffer[1] / 100;
    rbuffer[1] = rbuffer[1] % 100;
    uint8_t a1= rbuffer[1] / 10;
    uint8_t b1= rbuffer[1] % 10;
    c1=c1+48;a1=a1+48;b1=b1+48;
    uint8_t c2=rbuffer[2] / 100;
    rbuffer[2] = rbuffer[2] % 100;
    uint8_t a2= rbuffer[2] / 10;
    uint8_t b2= rbuffer[2] % 10;
    c2=c2+48;a2=a2+48;b2=b2+48;
    uint8_t c3=rbuffer[3] / 100;
        rbuffer[3] = rbuffer[3] % 100;
        uint8_t a3= rbuffer[3] / 10;
        uint8_t b3= rbuffer[3] % 10;
        c3=c3+48;a3=a3+48;b3=b3+48;
        uint8_t c4=rbuffer[4] / 100;
        rbuffer[4] = rbuffer[4] % 100;
        uint8_t a4= rbuffer[4] / 10;
        uint8_t b4= rbuffer[4] % 10;
        c4=c4+48;a4=a4+48;b4=b4+48;
        uint8_t c5=rbuffer[5] / 100;
            rbuffer[5] = rbuffer[5] % 100;
            uint8_t a5= rbuffer[5] / 10;
            uint8_t b5= rbuffer[5] % 10;
            c5=c5+48;a5=a5+48;b5=b5+48;
            uint8_t c6=rbuffer[6] / 100;
            rbuffer[6] = rbuffer[6] % 100;
            uint8_t a6= rbuffer[6] / 10;
            uint8_t b6= rbuffer[6] % 10;
            c6=c6+48;a6=a6+48;b6=b6+48;
            uint8_t c7=rbuffer[7] / 100;
            rbuffer[7] = rbuffer[7] % 100;
            uint8_t a7= rbuffer[7] / 10;
            uint8_t b7= rbuffer[7] % 10;
            c7=c7+48;a7=a7+48;b7=b7+48;
    char da[]="\n\rBuffer0:";
    char da1[]="\n\rBuffer1:";
    char da2[]="\n\rBuffer2:";
    char da3[]="\n\rBuffer3:";
        char da4[]="\n\rBuffer4:";
        char da5[]="\n\rBuffer5:";
            char da6[]="\n\rBuffer6:";
            char da7[]="\n\rBuffer7:";
    //while(1){
    CDC_Transmit_FS(&da,10);
    HAL_Delay(500);
   	CDC_Transmit_FS(&c, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&a, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&b,1);
    HAL_Delay(500);
    CDC_Transmit_FS(&da1,10);
    HAL_Delay(500);
    CDC_Transmit_FS(&c1, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&a1, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&b1,1);
    HAL_Delay(500);
    CDC_Transmit_FS(&da2,10);
    HAL_Delay(500);
    CDC_Transmit_FS(&c2, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&a2, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&b2,1);
    HAL_Delay(500);
    CDC_Transmit_FS(&da3,10);
        HAL_Delay(500);
       	CDC_Transmit_FS(&c3, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&a3, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&b3,1);
        HAL_Delay(500);
        CDC_Transmit_FS(&da4,10);
        HAL_Delay(500);
        CDC_Transmit_FS(&c4, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&a4, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&b4,1);
        HAL_Delay(500);
                CDC_Transmit_FS(&da5,10);
                HAL_Delay(500);
                CDC_Transmit_FS(&c5, 1);
                HAL_Delay( 500 );
                CDC_Transmit_FS(&a5, 1);
                HAL_Delay( 500 );
                CDC_Transmit_FS(&b5,1);
                HAL_Delay(500);
                CDC_Transmit_FS(&da6,10);
                                HAL_Delay(500);
                               	CDC_Transmit_FS(&c6, 1);
                                HAL_Delay( 500 );
                                CDC_Transmit_FS(&a6, 1);
                                HAL_Delay( 500 );
                                CDC_Transmit_FS(&b6,1);
                                HAL_Delay(500);
                                CDC_Transmit_FS(&da7,10);
                                                HAL_Delay(500);
                                               	CDC_Transmit_FS(&c7, 1);
                                                HAL_Delay( 500 );
                                                CDC_Transmit_FS(&a7, 1);
                                                HAL_Delay( 500 );
                                                CDC_Transmit_FS(&b7,1);
                                                HAL_Delay(500);*/
    //}
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    //system_spi_write( radio_local->spi, &dummy_byte, 1 );
    //system_spi_read( radio_local->spi, rbuffer, rbuffer_length );
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_write( const void* radio, const uint8_t* cbuffer, const uint16_t cbuffer_length,
                                      const uint8_t* cdata, const uint16_t cdata_length )
{
    //radio_t* radio_local = ( radio_t* ) radio;
	uint8_t rbuffer[500];

	/*if(sleep_flag == true){
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	    HAL_Delay(1);
	    sleep_flag = false;
	    }*/
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	  //  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	TimerTime_t past = TimerGetCurrentTime();
		TimerTime_t curr = 0;
		    while( (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) || curr<10)
		    	{
		    	curr = TimerGetElapsedTime( past );
		    	}    //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, cbuffer, rbuffer, cbuffer_length,3000);
    HAL_SPI_TransmitReceive(&hspi2, cdata, rbuffer, cdata_length,3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    //system_spi_write( radio_local->spi, cbuffer, cbuffer_length );
    //system_spi_write( radio_local->spi, cdata, cdata_length );
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_write_read( const void* radio, const uint8_t* cbuffer, uint8_t* rbuffer,
                                           const uint16_t length )
{
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	/*if(sleep_flag == true){
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	    HAL_Delay(1);
	    sleep_flag = false;
	    }*/
	TimerTime_t past = TimerGetCurrentTime();
	TimerTime_t curr = 0;
	//HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);

	    while( (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) || curr<10)
	    	{
	    	curr = TimerGetElapsedTime( past );
	    	}
		//HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	    HAL_SPI_TransmitReceive(&hspi2, cbuffer, rbuffer, length,3000);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	/*while( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	    HAL_SPI_TransmitReceive(&hspi2, cbuffer, rbuffer, length,3000);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);*/
	    //system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

	    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
	    //system_spi_write_read( radio_local->spi, cbuffer, rbuffer, length );
	    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
	    //if(sleep_flag == true){
	   /* char data[]= "\n\rStat1:";
	    char data1[]= "\n\rStat2:";
	    char data2[]= "\n\rIntr3:";
	    char data3[]= "\n\rIntr2:";
	    char data4[]= "\n\rIntr1:";
	    char data5[]= "\n\rIntr0:";
	    uint8_t c=rbuffer[0] / 100;
	    rbuffer[0] = rbuffer[0] % 100;
	    uint8_t a= rbuffer[0] / 10;
	    uint8_t b= rbuffer[0] % 10;
	    c=c+48;a=a+48;b=b+48;
	    uint8_t c1=rbuffer[1] / 100;
	    rbuffer[1] = rbuffer[1] % 100;
	    uint8_t a1= rbuffer[1] / 10;
	    uint8_t b1= rbuffer[1] % 10;
	    c1=c1+48;a1=a1+48;b1=b1+48;
	    uint8_t c2=rbuffer[2] / 100;
	    rbuffer[2] = rbuffer[2] % 100;
	    uint8_t a2= rbuffer[2] / 10;
	    uint8_t b2= rbuffer[2] % 10;
	    c2=c2+48;a2=a2+48;b2=b2+48;
	    uint8_t c3=rbuffer[3] / 100;
	    rbuffer[3] = rbuffer[3] % 100;
	    uint8_t a3= rbuffer[3] / 10;
	    uint8_t b3= rbuffer[3] % 10;
	    c3=c3+48;a3=a3+48;b3=b3+48;
	    uint8_t c4=rbuffer[4] / 100;
	    rbuffer[4] = rbuffer[4] % 100;
	    uint8_t a4= rbuffer[4] / 10;
	    uint8_t b4= rbuffer[4] % 10;
	    c4=c4+48;a4=a4+48;b4=b4+48;
	        uint8_t c5=rbuffer[5] / 100;
	        rbuffer[5] = rbuffer[5] % 100;
	        uint8_t a5= rbuffer[5] / 10;
	        uint8_t b5= rbuffer[5] % 10;
	        c5=c5+48;a5=a5+48;b5=b5+48;
	    while(1){
	    CDC_Transmit_FS(&data, 8);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&c, 1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&a, 1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&b,1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&data1, 8);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&c1, 1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&a1, 1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&b1,1);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&data2, 8);
	    HAL_Delay( 500 );
	    CDC_Transmit_FS(&c2, 1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&a2, 1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&b2,1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&data3, 8);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&c3, 1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&a3, 1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&b3,1);
	        HAL_Delay( 500 );
	        CDC_Transmit_FS(&data4, 8);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&c4, 1);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&a4, 1);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&b4,1);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&data5, 8);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&c5, 1);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&a5, 1);
	            HAL_Delay( 500 );
	            CDC_Transmit_FS(&b5,1);
	            HAL_Delay( 500 );}*/
	   // }
	    return LR1110_HAL_STATUS_OK;
   /* //radio_t* radio_local = ( radio_t* ) radio;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    while( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, cbuffer, rbuffer, length,3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    //system_spi_write_read( radio_local->spi, cbuffer, rbuffer, length );
    //system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
    char data[]= "\n\rStat1:";
    char data1[]= "\n\rStat2:";
    char data2[]= "\n\rIntr3:";
    char data3[]= "\n\rIntr2:";
    char data4[]= "\n\rIntr1:";
    char data5[]= "\n\rIntr0:";
    uint8_t c=rbuffer[0] / 100;
    rbuffer[0] = rbuffer[0] % 100;
    uint8_t a= rbuffer[0] / 10;
    uint8_t b= rbuffer[0] % 10;
    c=c+48;a=a+48;b=b+48;
    uint8_t c1=rbuffer[1] / 100;
    rbuffer[1] = rbuffer[1] % 100;
    uint8_t a1= rbuffer[1] / 10;
    uint8_t b1= rbuffer[1] % 10;
    c1=c1+48;a1=a1+48;b1=b1+48;
    uint8_t c2=rbuffer[2] / 100;
    rbuffer[2] = rbuffer[2] % 100;
    uint8_t a2= rbuffer[2] / 10;
    uint8_t b2= rbuffer[2] % 10;
    c2=c2+48;a2=a2+48;b2=b2+48;
    uint8_t c3=rbuffer[3] / 100;
    rbuffer[3] = rbuffer[3] % 100;
    uint8_t a3= rbuffer[3] / 10;
    uint8_t b3= rbuffer[3] % 10;
    c3=c3+48;a3=a3+48;b3=b3+48;
    uint8_t c4=rbuffer[4] / 100;
    rbuffer[4] = rbuffer[4] % 100;
    uint8_t a4= rbuffer[4] / 10;
    uint8_t b4= rbuffer[4] % 10;
    c4=c4+48;a4=a4+48;b4=b4+48;
        uint8_t c5=rbuffer[5] / 100;
        rbuffer[5] = rbuffer[5] % 100;
        uint8_t a5= rbuffer[5] / 10;
        uint8_t b5= rbuffer[5] % 10;
        c5=c5+48;a5=a5+48;b5=b5+48;
    //while(1){
    CDC_Transmit_FS(&data, 8);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&c, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&a, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&b,1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&data1, 8);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&c1, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&a1, 1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&b1,1);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&data2, 8);
    HAL_Delay( 500 );
    CDC_Transmit_FS(&c2, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&a2, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&b2,1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&data3, 8);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&c3, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&a3, 1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&b3,1);
        HAL_Delay( 500 );
        CDC_Transmit_FS(&data4, 8);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&c4, 1);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&a4, 1);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&b4,1);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&data5, 8);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&c5, 1);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&a5, 1);
            HAL_Delay( 500 );
            CDC_Transmit_FS(&b5,1);
            HAL_Delay( 500 );
    //}
    return LR1110_HAL_STATUS_OK;*/
}

/*void lr1110_init_io_context( const void* radio )
{
	radio_t* radio_local = ( radio_t* ) radio;

	radio_local->reset.pins.pin      = RADIO_RESET;
	radio_local->nss.pins.pin        = RADIO_NSS;
    radio_local->event.pin           = RADIO_EVENT;
    radio_local->busy.pins.pin       = RADIO_BUSY;
    radio_local->spi.pins.miso  	 = RADIO_MISO;
    radio_local->spi.pins.mosi  	 = RADIO_MOSI;
    radio_local->spi.pins.sclk  	 = RADIO_SCLK;
    radio_local->event.callback = radio_event_callback;
    radio_local->event.context  = radio_local;
    //( ( lr1110_t* ) context )->spi_id         = HAL_RADIO_SPI_ID;
}

void lr1110_modem_board_init_io( const void* radio )
{
	radio_t* radio_local = ( radio_t* ) radio;
	GPIO_InitTypeDef GPIO_InitStruct1 = {0};

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    Configure GPIO pins : PC4 PC5 PC10
      GPIO_InitStruct1.Pin = GPIO_PIN_1;
      GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct1.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct1);
      Configure GPIO pins : PC4 PC5 PC10
      GPIO_InitStruct1.Pin = GPIO_PIN_5;
      GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct1.Pull = GPIO_NOPULL;
      //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct1);
    //hal_gpio_init_out( radio_local->reset.pins.pin, 1 );
    //hal_gpio_init_out( radio_local->nss.pins.pin, 1 );
    //hal_gpio_init_in( radio_local->busy.pins.pin, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    //hal_gpio_init_in( radio_local->event.pin, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_RISING,
                     // &radio_local->event );
}

void radio_event_callback( void* obj ) {}*/
