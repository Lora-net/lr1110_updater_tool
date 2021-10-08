/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lr1110_hal.h"
#include "configuration.h"
#include "lr1110_system.h"
#include "lr1110_radio.h"
#include "rtc-board.h"
#include "lr1110_bootloader.h"
#include "lr1110_wifi.h"
#include "lr1110_gnss.h"
#include "lr1110_regmem.h"
#include "lr1110_crypto_engine_types.h"
#include "lr1110_crypto_engine.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
LPTIM_HandleTypeDef hlptim1;

QSPI_HandleTypeDef hqspi;

//RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */
extern radio_t lr1110;
lr1110_radio_pa_config_t pa_cfg;
lr1110_radio_modulation_param_lora_t mod_params;
lr1110_radio_packet_param_lora_t pkt_param;
lr1110_system_rfswitch_config_t rf_switch_cfg;
int8_t pwr_in_dbm;
lr1110_radio_ramp_time_t ramp_time;
uint8_t buff_len;
uint8_t* get_buff;
uint8_t results_nb1;
lr1110_wifi_basic_mac_type_channel_result_t all_results[LR1110_WIFI_MAX_RESULTS] = {0};
uint8_t nb_results = 0;
bool sleep_flag = false;

//lr1110_system_irq_mask_t irq;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_QUADSPI_Init(void);
/* USER CODE BEGIN PFP */
void lr1110_modem_set_transmit();
void lr1110_modem_scan_gnss(void);
void lr1110_modem_scan_wifi(void);
void lora_modem_transmit_data(uint8_t* rbufferd, uint8_t rlength);
void lora_modem_receive_data(uint32_t timeout);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_Device_Init();
  //MX_RTC_Init();
  MX_SPI2_Init();
  MX_LPTIM1_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  RtcInit();

  /***********************************LR1110***********************************/
  //LR1110 Reset
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
   HAL_Delay(1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
   HAL_Delay(500);

  // Turn on Power LED (red)
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

  //Clear all interrupt flags
    lr1110_system_clear_irq( &lr1110, 0x1FFFFFF );
    // 32.768 kHz crystal oscillator is enabled with wait for Xtal 32k ready.
        lr1110_system_config_lfclk( &lr1110, LR1110_SYSTEM_LFCLK_EXT, 0);
	  lr1110_modem_set_transmit();
	  uint8_t rlength = 2;
	  	  	uint8_t rbu = 0x5222;
	  lora_modem_transmit_data(&rbu, rlength);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.PLLSAI1.PLLN = 6;
  PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLR = RCC_PLLR_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_USBCLK;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 15;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
/*static void MX_RTC_Init(void)
{

   USER CODE BEGIN RTC_Init 0

   USER CODE END RTC_Init 0

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

   USER CODE BEGIN RTC_Init 1

   USER CODE END RTC_Init 1
  * Initialize RTC Only

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

   USER CODE BEGIN Check_RTC_BKUP

   USER CODE END Check_RTC_BKUP

  * Initialize RTC and set the Time and Date

  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.SubSeconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  * Enable the Alarm A

  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  * Enable the TimeStamp

  if (HAL_RTCEx_SetTimeStamp_IT(&hrtc, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_DEFAULT) != HAL_OK)
  {
    Error_Handler();
  }
   USER CODE BEGIN RTC_Init 2

   USER CODE END RTC_Init 2

}*/

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_12|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA8
                           PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB0 PB12 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  * @brief LR1110 Setting LoRa Modem for GNSS scanning
  * @retval None
  */
void lr1110_modem_scan_gnss(void)
{
	//Turn on GNSS status LED
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	//Display GNSS Status on COM Port
	 char data12[]="LR1110 GNSS Scanning Started\n\r";
	 /*while(CDC_Transmit_FS(&data12, strlen(data12)) == USBD_BUSY)
		 	HAL_Delay(5);*/

	 lr1110_gnss_solver_assistance_position_t assistance_position;
	 	 assistance_position.latitude = 8.50933;//8.475537;//8.808535;
	 	 assistance_position.longitude = 76.96156;//76.972554;//76.668477;
	 	 lr1110_gnss_set_assistance_position(&lr1110, &assistance_position );
	 	 //HAL_Delay(1000);


	//Set constellation as GPS
	 const lr1110_gnss_constellation_mask_t constellation_to_use=0x01;
	 lr1110_gnss_set_constellations_to_use( &lr1110, constellation_to_use );

	//Set GNSS mode as single scanning
	 uint8_t inter_capture_delay_second = 0x1E; //30sec
	 lr1110_gnss_set_scan_mode( &lr1110, LR1110_GNSS_SINGLE_SCAN_MODE, &inter_capture_delay_second );
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

	//Clear all interrupt flags
	 //lr1110_system_clear_irq_status( &lr1110, 0x1FFFFFF );
	//Autonomous scan
	 const lr1110_gnss_date_t date = 1315130500;
	 const lr1110_gnss_search_mode_t effort_mode=LR1110_GNSS_OPTION_DEFAULT;
	 const uint8_t gnss_input_parameters = 1;//0x6E;//( 1 << 0 );// 0x03;//0x6E;
	 const uint8_t nb_sat = 0x00;

	 lr1110_gnss_scan_autonomous( &lr1110, date, gnss_input_parameters, nb_sat );

	//Assisted scan
   //lr1110_gnss_scan_assisted( &lr1110, date, effort_mode, gnss_input_parameters, nb_sat );
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

	 char data13[]="LR1110 GNSS Scanning Completed\n\r";
	/* while(CDC_Transmit_FS(&data13, strlen(data13)) == USBD_BUSY)
		HAL_Delay(5);*/

	//Clear all interrupt flags
	 lr1110_system_clear_irq( &lr1110, 0x1FFFFFF );

	//Read number of satellites scanned
	 uint8_t nb_detected_satellites;
	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

	 lr1110_gnss_get_nb_detected_satellites( &lr1110, &nb_detected_satellites );
	 /*uint32_t irqRegs;
	 	 	 	 	 	 	 lr1110_system_stat1_t stat1;
	 	 	 	 	 	 	 lr1110_system_stat2_t stat2;
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irqRegs );*///}
	 uint8_t no_detected_satellites=nb_detected_satellites + 48;
	 /*char data14[]="Number of Satellites scanned during GNSS Scanning:";
	 char data15[]="\n\r";
	 while(CDC_Transmit_FS(&data14, strlen(data14)) == USBD_BUSY)
	 	HAL_Delay(5);
	 while(CDC_Transmit_FS(&no_detected_satellites, 1) == USBD_BUSY)
	 	 	   HAL_Delay(5);
	 while(CDC_Transmit_FS(&data15, strlen(data15)) == USBD_BUSY)
	 	HAL_Delay(5);*/

	//Read GNSS result size
	 uint16_t result_size = 0;
	 lr1110_gnss_get_result_size( &lr1110, &result_size );
	 uint16_t gnss_size = result_size + 48;
	 /*char data16[]="Size of GNSS Scanning Result:";
	 while(CDC_Transmit_FS(&data16, strlen(data16)) == USBD_BUSY)
	   	HAL_Delay(5);
	 while(CDC_Transmit_FS(&gnss_size, 2) == USBD_BUSY)
	   	HAL_Delay(5);
	 while(CDC_Transmit_FS(&data15, strlen(data15)) == USBD_BUSY)
	 	HAL_Delay(5);*/

	//Read GNSS results
	 uint8_t result_buffer[gnss_size];
	 lr1110_gnss_read_results( &lr1110, result_buffer, gnss_size );
	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
/*
	 uint32_t irqRegs;
	 	 	 	 	 	      lr1110_system_stat1_t stat1;
	 	 	 	 	 	      lr1110_system_stat2_t stat2;*/
	 	 	 	 	 	      //lr1110_system_get_status( &lr1110, &stat1, &stat2, &irqRegs );
  	//Turn off GNSS status LED
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
}

/**
  * @brief LR1110 Setting LoRa Modem for WiFi scanning
  * @retval None
  */
void lr1110_modem_scan_wifi(void)
{
  //Turn on Wi-Fi status LED
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);

  //Display Wi-Fi Status on COM Port
   /*char data12[]="LR1110 Wi-Fi Scanning Started\n\r";
   while(CDC_Transmit_FS(&data12, strlen(data12)) == USBD_BUSY)
   	   HAL_Delay(5);*/

  //Scan Wi-Fi signals
   const lr1110_wifi_signal_type_scan_t signal_type = LR1110_WIFI_TYPE_SCAN_B_G_N;
   const lr1110_wifi_channel_mask_t channels=0x3FFF;//16383;
   const lr1110_wifi_mode_t scan_mode = LR1110_WIFI_SCAN_MODE_BEACON_AND_PACKET;
   const uint8_t max_results = 32;
   const uint8_t nb_scan_per_channel = 2;
   const uint16_t timeout_in_ms = 105;
   const bool abort_on_timeout = true;
   lr1110_wifi_scan(&lr1110, signal_type, channels, scan_mode, max_results,nb_scan_per_channel, timeout_in_ms, abort_on_timeout );
   char data13[]="LR1110 Wi-Fi Scanning Completed\n\r";
   /*while(CDC_Transmit_FS(&data13, strlen(data13)) == USBD_BUSY)
      HAL_Delay(5);*/

  //Clear all interrupt flags
   lr1110_system_clear_irq( &lr1110, 0x1FFFFFF );

  //Get WiFi Result size
   lr1110_wifi_get_nb_results( &lr1110, &nb_results );
   uint8_t results_nb = nb_results+48;
   results_nb1 = results_nb;
  //Display Wi-Fi status on COM port
   char data14[]="Number of Wi-Fi Scanning Results obtained:";
   char data15[]="\n\r";
   /*while(CDC_Transmit_FS(&data14, strlen(data14)) == USBD_BUSY)
      HAL_Delay(5);
   while(CDC_Transmit_FS(&results_nb, 1) == USBD_BUSY)
      HAL_Delay(5);
   while(CDC_Transmit_FS(&data15, strlen(data15)) == USBD_BUSY)
      HAL_Delay(5);*/

  //Clear all interrupt flags
   lr1110_system_clear_irq( &lr1110, 0x1FFFFFF );

  //Read Wi-Fi Basic MAC Type Channel Results
   lr1110_wifi_read_basic_mac_type_channel_results(&lr1110, 0, nb_results, all_results);

  //Turn off Wi-Fi status LED
   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
}

/**
  * @brief LR1110 Setting LoRa Modem for RF transmission
  * @retval None
  */
void lr1110_modem_set_transmit()
{
	//Clear errors
	 lr1110_system_clear_errors( &lr1110);

	//Calibrate
	 const uint8_t freq1=0xD7; const uint8_t freq2=0xDB;
	 lr1110_system_calibrate_image( &lr1110, freq1, freq2 );
	 lr1110_system_stat1_t stat1;
	         lr1110_system_stat2_t stat2;
	         uint32_t              irq = 0;
	         lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );
	//PLL Calibration
	 const uint8_t calib_param = 0x24;
	 lr1110_system_calibrate( &lr1110, calib_param );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

		//Modem setting - Choose LoRa Modem
		 lr1110_radio_set_packet_type( &lr1110, LR1110_RADIO_PACKET_LORA );
         lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

		 //Radio setting - set RF frequency
	 lr1110_radio_set_rf_frequency(  &lr1110, 865062500);//866550000);
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );



	//Set LoRa Modulation parameters
	 mod_params.spreading_factor = LR1110_RADIO_LORA_SF10;//10;//5;
	 mod_params.bandwidth = LR1110_RADIO_LORA_BW125;
	 mod_params.coding_rate = LR1110_RADIO_LORA_CR45;
	 mod_params.ppm_offset = 0;
	 lr1110_radio_set_modulation_param_lora( &lr1110, &mod_params );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	//Set LoRa Packet Parameters
	 pkt_param.preamble_length_in_symb = 8;
	 pkt_param.header_type = LR1110_RADIO_LORA_HEADER_EXPLICIT;
	 pkt_param.payload_length_in_byte = 40;//buff_len;//52;//40;
	 pkt_param.crc = LR1110_RADIO_LORA_CRC_ON;
	 pkt_param.iq = LR1110_RADIO_LORA_IQ_STANDARD;
	 lr1110_radio_set_packet_param_lora( &lr1110, &pkt_param );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	//Set to Public Network
	 const lr1110_radio_lora_network_type_t network_type = LR1110_RADIO_LORA_NETWORK_PUBLIC;
	 lr1110_radio_set_lora_sync_word( &lr1110, network_type );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	//PA settings
	 pa_cfg.pa_sel = LR1110_RADIO_PA_SEL_LP;//HP;
	 pa_cfg.pa_reg_supply = LR1110_RADIO_PA_REG_SUPPLY_DCDC;//LR1110_RADIO_PA_REG_SUPPLY_VBAT;
	 pa_cfg.pa_dutycycle = 0x04;
	 pa_cfg.pa_hp_sel = 0x07;
	 lr1110_radio_set_pa_config( &lr1110, &pa_cfg );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	//RF Switch setting       Bit 0 DIO5 V2
	 rf_switch_cfg.enable = 3; 		//00000011
	 rf_switch_cfg.standby = 0;		//00000000
	 rf_switch_cfg.rx = 2;			//00000010
	 rf_switch_cfg.tx = 3;			//00000011
	 rf_switch_cfg.tx_hp = 1;			//00000001
	 rf_switch_cfg.tx_hf = 0;
	 rf_switch_cfg.gnss = 0;
	 rf_switch_cfg.wifi = 0;
	 lr1110_system_set_dio_as_rf_switch( &lr1110, &rf_switch_cfg );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

    //Set transmission parameters
	 pwr_in_dbm = 0x0E;//0x16
	 ramp_time = LR1110_RADIO_RAMP_TIME_40U;//LR1110_RADIO_RAMP_48_US;
	 lr1110_radio_set_tx_params( &lr1110, pwr_in_dbm, ramp_time );
	 lr1110_radio_stop_timeout_on_preamble( &lr1110, 1 );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	//Set to Auto Tx-Rx mode
	 //lr1110_radio_auto_tx_rx( &lr1110, 0,1,6000 );
	// lr1110_system_set_dio_irq_params( &lr1110, LR1110_SYSTEM_IRQ_TX_DONE | LR1110_SYSTEM_IRQ_TIMEOUT,
	  //                                     LR1110_SYSTEM_IRQ_NONE );
}

/**
  * @brief LR1110 Setting LoRa Modem for RF transmitting data
  * @retval None
  */
void lora_modem_transmit_data(uint8_t* rbufferd, uint8_t rlength)
{
	//Transmit Data
	//uint8_t rlength = 2;
	//uint8_t rbu = 0x5222;
	//while(1){
	//Set LoRa Packet Parameters

		 pkt_param.preamble_length_in_symb = rlength;//52;//40;

		 lr1110_radio_set_packet_param_lora( &lr1110, &pkt_param );
		 lr1110_system_stat1_t stat1;
		            lr1110_system_stat2_t stat2;
		            uint32_t              irq = 0;
         lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );

	 lr1110_regmem_write_buffer8( &lr1110, rbufferd, rlength );
     lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );
	   //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);RTC_Alarm_IRQHandler

	//Set Transmission with timeout
	 lr1110_radio_set_tx( &lr1110, 3000 ); // timeout in ms
	 /*uint32_t irqRegs;
	      lr1110_system_stat1_t stat1;
	      lr1110_system_stat2_t stat2;*/
	 sleep_flag=true;
	 while((irq & LR1110_SYSTEM_IRQ_TXDONE_MASK)!= LR1110_SYSTEM_IRQ_TXDONE_MASK){
	      lr1110_system_get_status( &lr1110, &stat1, &stat2, &irq );}
	 sleep_flag=false;
	 //HAL_Delay(1000);
	 //lr1110_radio_set_rx( &lr1110, 6000 );
	 //HAL_Delay(5000);
	//}
}
/**
  * @brief LR1110 Setting LoRa Modem for RF receiving data
  * @retval None
  */
void lora_modem_receive_data(uint32_t timeout)
{
	//Transmit Data
	//uint8_t rlength = 2;
	//uint8_t rbu = 0x5222; devnonce
	//while(1){
	//Set LoRa Packet Parameters

		 pkt_param.preamble_length_in_symb =52;//40;

		 lr1110_radio_set_packet_param_lora( &lr1110, &pkt_param );
	 //lr1110_regmem_write_buffer8( &lr1110, rbufferd, rlength );
	//Set Transmission with timeout
	 lr1110_radio_set_rx( &lr1110, timeout ); // timeout in ms
	 uint32_t irqRegs;
	      lr1110_system_stat1_t stat1;
	      lr1110_system_stat2_t stat2;
	 while((irqRegs & LR1110_SYSTEM_IRQ_RXDONE_MASK)!= LR1110_SYSTEM_IRQ_RXDONE_MASK){
	      lr1110_system_get_status( &lr1110, &stat1, &stat2, &irqRegs );}
	 //HAL_Delay(1000);
	 //lr1110_radio_set_rx( &lr1110, 6000 );
	 //HAL_Delay(5000);
	//}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
