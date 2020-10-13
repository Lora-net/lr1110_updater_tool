/*!
 * \file      system_i2c.c
 *
 * \brief     MCU I2C-related functions
 *
 * \copyright Revised BSD License, see file LICENSE.txt
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2019 Semtech
 *
 * \endcode
 *
 * \authors   Semtech WSP Applications Team
 */

#include "system_i2c.h"

void system_i2c_init( void )
{
    LL_I2C_InitTypeDef I2C_InitStruct = { 0 };

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOB );
    /**I2C1 GPIO Configuration
    PB8   ------> I2C1_SCL
    PB9   ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate  = LL_GPIO_AF_4;
    LL_GPIO_Init( GPIOB, &GPIO_InitStruct );

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_I2C1 );

    /**I2C Initialization
     */
    LL_I2C_EnableAutoEndMode( I2C1 );
    LL_I2C_DisableOwnAddress2( I2C1 );
    LL_I2C_DisableGeneralCall( I2C1 );
    LL_I2C_EnableClockStretching( I2C1 );
    I2C_InitStruct.PeripheralMode  = LL_I2C_MODE_I2C;
    I2C_InitStruct.Timing          = 0x10909CEC;
    I2C_InitStruct.AnalogFilter    = LL_I2C_ANALOGFILTER_ENABLE;
    I2C_InitStruct.DigitalFilter   = 0;
    I2C_InitStruct.OwnAddress1     = 0;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;
    LL_I2C_Init( I2C1, &I2C_InitStruct );
    LL_I2C_SetOwnAddress2( I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK );
}

void system_i2c_write( const uint8_t address, const uint8_t* buffer_in,
                       const uint8_t length, const bool repeated )
{
    uint8_t i = 0;

    LL_I2C_HandleTransfer( I2C1, 0x70, LL_I2C_ADDRSLAVE_7BIT, length,
                           LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE );

    /* Loop until STOP flag is raised  */
    while( !LL_I2C_IsActiveFlag_STOP( I2C1 ) )
    {
        if( LL_I2C_IsActiveFlag_TXIS( I2C1 ) )
        {
            LL_I2C_TransmitData8( I2C1, buffer_in[i++] );
        }
    }

    LL_I2C_ClearFlag_STOP( I2C1 );
}

void system_i2c_read( const uint8_t address, uint8_t* buffer_out,
                      const uint8_t length, const bool repeated )
{
    uint8_t i = 0;

    LL_I2C_HandleTransfer( I2C1, 0x70, LL_I2C_ADDRSLAVE_7BIT, length,
                           LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ );

    /* Loop until STOP flag is raised  */
    while( !LL_I2C_IsActiveFlag_STOP( I2C1 ) )
    {
        if( LL_I2C_IsActiveFlag_RXNE( I2C1 ) )
        {
            buffer_out[i++] = LL_I2C_ReceiveData8( I2C1 );
        }
    }

    LL_I2C_ClearFlag_STOP( I2C1 );
}
