/*!
 * \file      spi-board.c
 *
 * \brief     Target board SPI driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include "stm32wbxx.h"
#include "utilities.h"
#include "board.h"
#include "gpio.h"
#include "spi-board.h"

//static
SPI_HandleTypeDef SpiHandle[2];
extern Gpio_t LED1;
extern Gpio_t LED3;

void SpiInit( Spi_t *obj, SpiId_t spiId, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
    CRITICAL_SECTION_BEGIN( );

    obj->SpiId = spiId;
    /*SpiHandle[spiId].Instance = SPI2;
    SpiHandle[spiId].Init.Mode = SPI_MODE_MASTER;
    SpiHandle[spiId].Init.Direction = SPI_DIRECTION_2LINES;
    SpiHandle[spiId].Init.DataSize = SPI_DATASIZE_8BIT;
    SpiHandle[spiId].Init.CLKPolarity = SPI_POLARITY_LOW;
    SpiHandle[spiId].Init.CLKPhase = SPI_PHASE_1EDGE;
    SpiHandle[spiId].Init.NSS = SPI_NSS_SOFT;
    SpiHandle[spiId].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    SpiHandle[spiId].Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle[spiId].Init.TIMode = SPI_TIMODE_DISABLE;
    SpiHandle[spiId].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle[spiId].Init.CRCPolynomial = 7;
    SpiHandle[spiId].Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    SpiHandle[spiId].Init.NSSPMode = SPI_NSS_PULSE_DISABLE;*/
    if( spiId == SPI_1 )
    {
        __HAL_RCC_SPI1_FORCE_RESET( );
        __HAL_RCC_SPI1_RELEASE_RESET( );
        __HAL_RCC_SPI1_CLK_ENABLE( );

        SpiHandle[obj->SpiId].Instance = ( SPI_TypeDef* )SPI1_BASE;

        /*GpioInit( &obj->Mosi, mosi, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI1 );
        GpioInit( &obj->Miso, miso, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI1 );
        GpioInit( &obj->Sclk, sclk, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI1 );
        GpioInit( &obj->Nss, nss, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF5_SPI1 );*/
    }
    else
    {
        __HAL_RCC_SPI2_FORCE_RESET( );
        __HAL_RCC_SPI2_RELEASE_RESET( );
        __HAL_RCC_SPI2_CLK_ENABLE( );

        SpiHandle[obj->SpiId].Instance = ( SPI_TypeDef* )SPI2_BASE;

        /*GpioInit( &obj->Mosi, mosi, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI2 );
        GpioInit( &obj->Miso, miso, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI2 );
        GpioInit( &obj->Sclk, sclk, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, GPIO_AF5_SPI2 );
        GpioInit( &obj->Nss, nss, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF5_SPI2 );*/
    }

    /*if( nss == NC )
    {
        SpiHandle[obj->SpiId].Init.NSS = SPI_NSS_SOFT;
        SpiFormat( obj, SPI_DATASIZE_8BIT, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 0 );
    }
    else
    {
        SpiFormat( obj, SPI_DATASIZE_8BIT, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 1 );
    }*/
    //SpiFrequency( obj, 10000000 );
    //SpiFrequency( obj, 8000000 );

    HAL_StatusTypeDef sta = HAL_SPI_Init( &SpiHandle[obj->SpiId] );
/*if (sta == HAL_OK){
	GpioWrite( &LED1,1 );
}
else
	GpioWrite( &LED3,1 );*/
    CRITICAL_SECTION_END( );
}

void SpiDeInit( Spi_t *obj )
{
    HAL_SPI_DeInit( &SpiHandle[obj->SpiId] );

    GpioInit( &obj->Mosi, obj->Mosi.pin, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioInit( &obj->Miso, obj->Miso.pin, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
    GpioInit( &obj->Sclk, obj->Sclk.pin, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioInit( &obj->Nss, obj->Nss.pin, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
}

void SpiFormat( Spi_t *obj, int32_t bits, int32_t cpol, int32_t cpha, int8_t slave )
{
    SpiHandle[obj->SpiId].Init.Direction = SPI_DIRECTION_2LINES;
    if( bits == SPI_DATASIZE_8BIT )
    {
        SpiHandle[obj->SpiId].Init.DataSize = SPI_DATASIZE_8BIT;
    }
    else
    {
        SpiHandle[obj->SpiId].Init.DataSize = SPI_DATASIZE_16BIT;
    }
    SpiHandle[obj->SpiId].Init.CLKPolarity = cpol;
    SpiHandle[obj->SpiId].Init.CLKPhase = cpha;
    SpiHandle[obj->SpiId].Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle[obj->SpiId].Init.TIMode = SPI_TIMODE_DISABLE;
    SpiHandle[obj->SpiId].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle[obj->SpiId].Init.CRCPolynomial = 7;
    //Added
    SpiHandle[obj->SpiId].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    SpiHandle[obj->SpiId].Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    SpiHandle[obj->SpiId].Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if( slave == 0 )
    {
        SpiHandle[obj->SpiId].Init.Mode = SPI_MODE_MASTER;
    }
    else
    {
        SpiHandle[obj->SpiId].Init.Mode = SPI_MODE_SLAVE;
    }
}

void SpiFrequency( Spi_t *obj, uint32_t hz )
{
    uint32_t divisor = 0;
    uint32_t sysClkTmp = SystemCoreClock;
    uint32_t baudRate;

    while( sysClkTmp > hz )
    {
        divisor++;
        sysClkTmp = ( sysClkTmp >> 1 );

        if( divisor >= 7 )
        {
            break;
        }
    }

    baudRate =( ( ( divisor & 0x4 ) == 0 ) ? 0x0 : SPI_CR1_BR_2 ) |
              ( ( ( divisor & 0x2 ) == 0 ) ? 0x0 : SPI_CR1_BR_1 ) |
              ( ( ( divisor & 0x1 ) == 0 ) ? 0x0 : SPI_CR1_BR_0 );

    SpiHandle[obj->SpiId].Init.BaudRatePrescaler = baudRate;
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
    uint8_t rxData = 0;//&( ( lr1110_t* ) &LR1110 )
    /*HAL_SPI_Transmit(&SpiHandle[obj->SpiId], &outData, 1,3000);
      HAL_SPI_Receive(&SpiHandle[obj->SpiId], &rxData, 1, 3000);*/
    if( ( obj == NULL ) || ( SpiHandle[obj->SpiId].Instance ) == NULL )
    {
        assert_param( LMN_STATUS_ERROR );
    }

    __HAL_SPI_ENABLE( &SpiHandle[obj->SpiId] );

    //CRITICAL_SECTION_BEGIN( );
    __HAL_LOCK(&SpiHandle[obj->SpiId]);

    while( __HAL_SPI_GET_FLAG( &SpiHandle[obj->SpiId], SPI_FLAG_TXE ) == RESET );
    SpiHandle[obj->SpiId].Instance->DR = ( uint16_t ) ( outData & 0xFF );

    while( __HAL_SPI_GET_FLAG( &SpiHandle[obj->SpiId], SPI_FLAG_RXNE ) == RESET );
    rxData = ( uint16_t ) SpiHandle[obj->SpiId].Instance->DR;

    //CRITICAL_SECTION_END( );
    __HAL_UNLOCK(&SpiHandle[obj->SpiId]);

    return( rxData );
}

