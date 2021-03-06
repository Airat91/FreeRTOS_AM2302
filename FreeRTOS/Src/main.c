
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */

#include "hd44780.h"
#include "am2302.h"
#include "stdint.h"
#include "keyboard.h"
//#include "arm_math.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

PCD_HandleTypeDef hpcd_USB_FS;

osThreadId Task_am2302Handle;
osThreadId Task_RTC_printHandle;
osThreadId Task_RTC_setHandle;
osSemaphoreId hd44780_SemHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

state_type clock_state = RUN;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM6_Init(void);
void StartTask_am2302(void const * argument);
void StartTask_RTC_print(void const * argument);
void StartTask_RTC_set(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  const uint8_t symbol[8]={0x06,0x09,0x09,0x06,0x00,0x00,0x00,0x00}; 

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  
  hd44780_init ();
  hd44780_user_symbol (0x00, symbol);
  hd44780_clr ();
  hd44780_string (" ����-� �������", 5);
  hd44780_xy (1, 2);
  hd44780_string (" ������ �������", 5);
  HAL_Delay (500);
  
  am2302_init ();
  keyboard_init ();
  hd44780_clr ();
 
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of hd44780_Sem */
  osSemaphoreDef(hd44780_Sem);
  hd44780_SemHandle = osSemaphoreCreate(osSemaphore(hd44780_Sem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of Task_am2302 */
  osThreadDef(Task_am2302, StartTask_am2302, osPriorityNormal, 0, 128);
  Task_am2302Handle = osThreadCreate(osThread(Task_am2302), NULL);

  /* definition and creation of Task_RTC_print */
  osThreadDef(Task_RTC_print, StartTask_RTC_print, osPriorityIdle, 0, 128);
  Task_RTC_printHandle = osThreadCreate(osThread(Task_RTC_print), NULL);

  /* definition and creation of Task_RTC_set */
  osThreadDef(Task_RTC_set, StartTask_RTC_set, osPriorityIdle, 0, 128);
  Task_RTC_setHandle = osThreadCreate(osThread(Task_RTC_set), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure LSE Drive Capability 
    */
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USB init function */
static void MX_USB_PCD_Init(void)
{

  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_64;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin 
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin 
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : AM2302_PIN_Pin */
  GPIO_InitStruct.Pin = AM2302_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AM2302_PIN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartTask_am2302 function */
void StartTask_am2302(void const * argument)
{

  /* USER CODE BEGIN 5 */

    am2302_data hum_tmpr;
    hum_tmpr.hum = 0;
    hum_tmpr.tmpr = 0;
    hum_tmpr.paritet = 0;

    /* Infinite loop */
    for(;;) {    
        hum_tmpr = am2302_get ();
        if (hd44780_SemHandle != NULL) {
            if (osSemaphoreWait (hd44780_SemHandle , 5) == osOK) {
                if ((hum_tmpr.hum == 0) && (hum_tmpr.tmpr == 0)) {
                  hd44780_xy (11,1);
                  hd44780_string ("AM2302", 0);
                  hd44780_xy (12, 2);
                  hd44780_string ("Error", 0);
                } else {
                  hd44780_xy (13,1);
                  hd44780_send (0x2C, DAT);
                  hd44780_send ((hum_tmpr.tmpr%10)|0x30, DAT);
                  hd44780_send (0x00, DAT);
                  hd44780_send (0x43, DAT);
                  hd44780_xy (12,1);
                  hd44780_num (hum_tmpr.tmpr/10);

                  hd44780_xy (14,2);
                  hd44780_send (0x2C, DAT);
                  hd44780_send ((hum_tmpr.hum%10)|0x30, DAT);
                  hd44780_send (0x25, DAT);
                  hd44780_xy (13,2);
                  hd44780_num (hum_tmpr.hum/10);
                }
            }
            osSemaphoreRelease (hd44780_SemHandle);
        }
        HAL_GPIO_TogglePin (GPIOE, GPIO_PIN_8);
        osDelay (1000);
    }
  /* USER CODE END 5 */ 
}

/* StartTask_RTC_print function */
void StartTask_RTC_print(void const * argument)
{
  /* USER CODE BEGIN StartTask_RTC_print */

    uint8_t DATE [11];
    uint8_t TIME [9];

    /* Infinite loop */
    for(;;) {
        RTC_HandleTypeDef hrtc;
        RTC_TimeTypeDef sTime;
        RTC_DateTypeDef sDate;
        hrtc.Instance = RTC;

        HAL_RTC_GetTime (&hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);

        DATE [0] = (sDate.Date/10)|0x30;
        DATE [1] = (sDate.Date%10)|0x30;
        DATE [2] = 0x2F;                       // Symbol "/"
        DATE [3] = (sDate.Month/10)|0x30;
        DATE [4] = (sDate.Month%10)|0x30;
        DATE [5] = 0x2F;                       // Symbol "/"
        DATE [6] = 0x32;                       // Symbol "2"
        DATE [7] = 0x30;                       // Symbol "0"
        DATE [8] = (sDate.Year/10)|0x30;
        DATE [9] = (sDate.Year%10)|0x30;
        DATE [10] = 0;

        TIME [0] = (sTime.Hours/10)|0x30;
        TIME [1] = (sTime.Hours%10)|0x30;
        TIME [2] = 0x3A;                       // Symbol ":"
        TIME [3] = (sTime.Minutes/10)|0x30;
        TIME [4] = (sTime.Minutes%10)|0x30;
        TIME [5] = 0x3A;                       // Symbol ":"
        TIME [6] = (sTime.Seconds/10)|0x30;
        TIME [7] = (sTime.Seconds%10)|0x30;
        TIME [8] = 0;

        if (hd44780_SemHandle != NULL) {
            if(osSemaphoreWait(hd44780_SemHandle , 5) == osOK) {
                hd44780_xy(1, 1);
                hd44780_string (TIME, 0);
                hd44780_xy(1, 2);
                hd44780_string (DATE, 0);
            }
            osSemaphoreRelease(hd44780_SemHandle);
        }

        HAL_GPIO_TogglePin (GPIOE, GPIO_PIN_9);
        osDelay (50);
    }
  /* USER CODE END StartTask_RTC_print */
}

/* StartTask_RTC_set function */
void StartTask_RTC_set(void const * argument)
{
  /* USER CODE BEGIN StartTask_RTC_set */
  
    uint8_t DATE [11];
    uint8_t TIME [9];
    uint8_t i = 1;
    uint8_t key;
  
    /* Infinite loop */
    for(;;) {
        switch (clock_state) {
        case (RUN):
            key = keyboard_get_key ();
            if (key == 13) {     // Pressed "D" Enter
                clock_state = GET_TIME;
            }
            break;
        case (GET_TIME):
            osThreadSuspend (Task_RTC_printHandle);
            osThreadSuspend (Task_am2302Handle);
            
            RTC_HandleTypeDef hrtc;
            RTC_TimeTypeDef sTime;
            RTC_DateTypeDef sDate;
            hrtc.Instance = RTC;

            HAL_RTC_GetTime (&hrtc, &sTime, RTC_FORMAT_BIN);
            HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
            
            DATE [0] = (sDate.Date/10)|0x30;
            DATE [1] = (sDate.Date%10)|0x30;
            DATE [2] = 0x2F;                       // Symbol "/"
            DATE [3] = (sDate.Month/10)|0x30;
            DATE [4] = (sDate.Month%10)|0x30;
            DATE [5] = 0x2F;                       // Symbol "/"
            DATE [6] = 0x32;                       // Symbol "2"
            DATE [7] = 0x30;                       // Symbol "0"
            DATE [8] = (sDate.Year/10)|0x30;
            DATE [9] = (sDate.Year%10)|0x30;
            DATE [10] = 0;

            TIME [0] = (sTime.Hours/10)|0x30;
            TIME [1] = (sTime.Hours%10)|0x30;
            TIME [2] = 0x3A;                       // Symbol ":"
            TIME [3] = (sTime.Minutes/10)|0x30;
            TIME [4] = (sTime.Minutes%10)|0x30;
            TIME [5] = 0x3A;                       // Symbol ":"
            TIME [6] = (sTime.Seconds/10)|0x30;
            TIME [7] = (sTime.Seconds%10)|0x30;
            TIME [8] = 0;
            
            hd44780_xy(1, 1);
            hd44780_string (TIME, 0);
            hd44780_xy(1, 2);
            hd44780_string (DATE, 0);
            hd44780_xy (1,1);
            hd44780_conf (DISPLAY_ON, CURSOR_OFF, BLINK_ON);
            
            clock_state = SET_TIME;
            break;
        case (SET_TIME):
            key = keyboard_get_key ();
            if (key < 10) {         // Pressed 0...9
                TIME [i - 1] = key|0x30;
                hd44780_send (key|0x30, DAT);
                i++;
            } else if (key == 14) { // Pressed "*" <--
                i--;
                hd44780_xy (i, 1);
                if ((i == 3) || (i == 6)) {
                    i--;
                    hd44780_xy (i, 1);
                } else if (i == 0) {
                    i = 18;
                    hd44780_xy (i - 8, 2);
                    clock_state = SET_DATE;
                }
            } else if (key == 15) { // Pressed "#" -->
                i++;
                hd44780_xy (i, 1);
            } else if (key == 13) { // Pressed "D" Enter
                hd44780_conf (DISPLAY_ON, CURSOR_OFF, BLINK_OFF);
                clock_state = WRITE_RTC;
            }
            
            if ((i == 3) || (i == 6)) {
                i++;
                hd44780_xy (i, 1);
            }
            if (i == 9) {
                hd44780_xy (i - 8, 2);
                clock_state = SET_DATE;
            }
            break;
        case (SET_DATE):
            key = keyboard_get_key ();
            if (key < 10) {
                DATE [i - 9] = key|0x30;
                hd44780_send (key|0x30, DAT);
                i++;
            } else if (key == 14) { // Pressed "*" <--
                i--;
                hd44780_xy (i - 8, 2);
                if ((i == 11) || (i == 14)) {
                    i--;
                    hd44780_xy (i - 8, 2);
                } else if (i == 8) {
                    hd44780_xy (i, 1);
                    clock_state = SET_TIME;
                }
            } else if (key == 15) { // Pressed "#" -->
                i++;
                hd44780_xy (i - 8, 2);
            } else if (key == 13) { // Pressed "D" Enter
                hd44780_conf (DISPLAY_ON, CURSOR_OFF, BLINK_OFF);
                clock_state = WRITE_RTC;
            }
            if ((i == 11) || (i == 14)) {
                i++;
                hd44780_xy (i - 8, 2);
            }
            if (i == 19) {
                i = 1;
                hd44780_xy (i, 1);
                clock_state = SET_TIME;
            }
            break;
        case (WRITE_RTC):
            sDate.Date = ((DATE[0]&0x0F)*10+(DATE[1]&0x0F));
            sDate.Month = ((DATE[3]&0x0F)*10+(DATE[4]&0x0F));
            sDate.Year = ((DATE[8]&0x0F)*10+(DATE[9]&0x0F));

            sTime.Hours = ((TIME[0]&0x0F)*10+(TIME[1]&0x0F));
            sTime.Minutes = ((TIME[3]&0x0F)*10+(TIME[4]&0x0F));
            sTime.Seconds = ((TIME[6]&0x0F)*10+(TIME[7]&0x0F));

            HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
            HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
            
            clock_state = RUN;
            hd44780_clr ();
            osThreadResume (Task_RTC_printHandle);
            osThreadResume (Task_am2302Handle);
            break;
        }
      osDelay (100);
    }
  /* USER CODE END StartTask_RTC_set */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
