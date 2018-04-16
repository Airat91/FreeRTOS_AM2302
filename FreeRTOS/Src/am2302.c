#include "am2302.h"

#define AM2302_PIN  GPIO_PIN_13
#define AM2302_PORT GPIOC
#define AM2302_TIMER TIM16

#define AM2302_TIMER_CLK = 72000000;

void am2302_init (void) {
  if (AM2302_PORT == GPIOA) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  } if (AM2302_PORT == GPIOB) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  } if (AM2302_PORT == GPIOC) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  } if (AM2302_PORT == GPIOD) {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  } if (AM2302_PORT == GPIOE) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  } if (AM2302_PORT == GPIOF) {
    __HAL_RCC_GPIOF_CLK_ENABLE();
  }
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pin = AM2302_PIN;
  HAL_GPIO_Init (AM2302_PORT, &GPIO_InitStruct);
  
  htim16.Instance = AM2302_TIMER;
  htim16.Init.Prescaler = (AM2302_TIMER_CLK / 2000000 - 1); //  2 MHz
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 2000;                                // timeout = 1 ms
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}
  
am2302_data am2302_get (void) {
  int T = 0;
  am2302_data result;
  char i=0;
  char am2302_timeout = 0;

  HAL_GPIO_WritePin (AM2302_PORT, AM2302_PIN, GPIO_PIN_RESET);  // SDA = 0
  HAL_Delay (1);                                                // 1ms

  __HAL_TIM_CLEAR_FLAG(&htim16, TIM_FLAG_UPDATE);                // Очистка флага прерывания
  
  HAL_TIM_Base_Start(&htim16);                                   // Запуск таймера
  while(__HAL_TIM_GET_FLAG(&htim16, TIM_FLAG_UPDATE)==0)         // Ждем пока установится флаг прерывания
  {                                                              // SDA=0 1 мс
  }
  HAL_TIM_Base_Stop(&htim16);                                    // Останавливаем таймер
*/   
  HAL_GPIO_WritePin (AM2302_PORT, AM2302_PIN, GPIO_PIN_SET);    // SDA = 1
  
  HAL_TIM_Base_Start_IT (&htim16);   // Run timeout 1us
  
  while ((HAL_GPIO_ReadPin(AM2302_SDA_GPIO_Port, AM2302_SDA_Pin) == 1) ||
         (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timout
  }
  if (am2302_timeout) {
    result.hum = 0;
    result.tmpr = 0;
    result.paritet = 0;
    return result;
  } else {
    __HAL_TIM_CLEAR_FLAG (&htim16, TIM_FLAG_UPDATE); // Run timeout 1us
  }

  while ((HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 0) ||
        (am2302_timeout == 0)) {     // Wait SDA->1 or am2302_timout
  }
  if (AM2302_TimeOut) {
    result.hum = 0;
    result.tmpr = 0;
    result.paritet = 0;
    return result;
  } else {
    __HAL_TIM_CLEAR_FLAG(&htim16, TIM_FLAG_UPDATE);
  }
  while (HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 1) {     // Ждем когда AM2302 установит 0
  }
  
  while(i<32)
  {
    htim16.Instance->CNT = 0;                                             // Обнуляем счетчик 
    HAL_TIM_Base_Start(&htim16);                                          // Запуск таймера
    while (HAL_GPIO_ReadPin(AM2302_SDA_GPIO_Port, AM2302_SDA_Pin)==0)     // Ждем когда AM2302 установит 1
    {
    }
    HAL_TIM_Base_Stop(&htim16);                                           // Останавливаем таймер
    T = htim16.Instance->CNT;                                             // Запоминаем длительность лог 0 AM2302
    htim16.Instance->CNT = 0;                                             // Обнуляем счетчик 
    HAL_TIM_Base_Start(&htim16);                                          // Запуск таймера
    while (HAL_GPIO_ReadPin(AM2302_SDA_GPIO_Port, AM2302_SDA_Pin)==1)     // Ждем когда AM2302 установит 0
    {
    }
    HAL_TIM_Base_Stop(&htim16);                                           // Останавливаем таймер
    
    RESULT=(RESULT<<1);
    i++;
    if (htim16.Instance->CNT>T)
      RESULT++;
  }
  
  
  i=0;
  return RESULT;
}