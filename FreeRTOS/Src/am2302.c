#include "am2302.h"

#define AM2302_PIN  GPIO_PIN_13
#define AM2302_PORT GPIOC
#define AM2302_TIMER TIM6

const uint32_t AM2302_TIMER_CLK = 72000000;
volatile uint8_t am2302_timeout = 0;

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
  
    TIM_HandleTypeDef htim;
    htim.Instance = AM2302_TIMER;
    htim.Init.Prescaler = (AM2302_TIMER_CLK / 20000000 - 1); //  2 MHz
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = 20000;                                // timeout = 1 ms
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
}
  
am2302_data am2302_get (void) {
    
    am2302_data result;
    result.hum = 0;
    result.tmpr = 0;
    result.paritet = 0;
    uint16_t T = 0;
    uint8_t i = 0;
    am2302_timeout = 0;
    
    TIM_HandleTypeDef htim;
    htim.Instance = AM2302_TIMER;
//    htim.Init.Prescaler = (AM2302_TIMER_CLK / 20000000 - 1); //  2 MHz
//    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
//    htim.Init.Period = 20000;                                // timeout = 1 ms
//    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//    htim.Init.RepetitionCounter = 0;
//    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//    if (HAL_TIM_Base_Init(&htim) != HAL_OK) {
//        _Error_Handler(__FILE__, __LINE__);
//    }
    
    GPIO_InitTypeDef GPIO_InitStruct;                   // AM2032_PIN to OUT
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = AM2302_PIN;
    HAL_GPIO_Init (AM2302_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin (AM2302_PORT, AM2302_PIN, GPIO_PIN_RESET);  // SDA = 0
    
    __HAL_TIM_CLEAR_FLAG (&htim, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT (&htim);                      // Run timeout 1ms
    while (am2302_timeout == 0) {
    }
    htim.Instance->CNT = 0;
    am2302_timeout = 0;
    
    HAL_GPIO_WritePin (AM2302_PORT, AM2302_PIN, GPIO_PIN_SET);    // SDA = 1
    
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;             // AM2032_PIN to INPUT
    HAL_GPIO_Init (AM2302_PORT, &GPIO_InitStruct);
    
    __HAL_TIM_CLEAR_FLAG (&htim, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT (&htim);  // Run timeout 1ms
  
    while ((HAL_GPIO_ReadPin(AM2302_PORT, AM2302_PIN) == 1) &&
        (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timeout     
    }
    if (am2302_timeout) {
        result.hum = 0;
        result.tmpr = 0;
        result.paritet = 0;
        return result;
    } else {
        htim.Instance->CNT = 0;     // Reset timeout
    }

    while ((HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 0) &&
        (am2302_timeout == 0)) {    // Wait SDA->1 or am2302_timeout
    }
    if (am2302_timeout) {
        result.hum = 0;
        result.tmpr = 0;
        result.paritet = 0;
        return result;
    } else {
        htim.Instance->CNT = 0;     // Reset timeout
    }
  
    while ((HAL_GPIO_ReadPin(AM2302_PORT, AM2302_PIN) == 1) &&
        (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timeout
    }
    if (am2302_timeout) {
        result.hum = 0;
        result.tmpr = 0;
        result.paritet = 0;
        return result;
    } else {
        htim.Instance->CNT = 0;     // Reset timeout
    }
  
    while(i<16) {   //Read hummidity
        htim.Instance->CNT = 0;
        HAL_TIM_Base_Start(&htim);
        
        while ((HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 0) &&
            (am2302_timeout == 0)) {    // Wait SDA->1 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop (&htim);
            T = htim.Instance -> CNT;
            htim.Instance -> CNT = 0;
            HAL_TIM_Base_Start (&htim);
        }
    
        while ((HAL_GPIO_ReadPin(AM2302_PORT, AM2302_PIN) == 1) &&
            (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop(&htim);
            result.hum = (result.hum << 1);
            i++;
            if (htim.Instance -> CNT > T) {
                result.hum++;
            }
        }
    }
    i = 0;
    
    while(i<16) {   //Read temperature
        htim.Instance->CNT = 0;
        HAL_TIM_Base_Start(&htim);
        
        while ((HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 0) &&
            (am2302_timeout == 0)) {    // Wait SDA->1 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop (&htim);
            T = htim.Instance -> CNT;
            htim.Instance -> CNT = 0;
            HAL_TIM_Base_Start (&htim);
        }
    
        while ((HAL_GPIO_ReadPin(AM2302_PORT, AM2302_PIN) == 1) &&
            (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop(&htim);
            result.tmpr = (result.tmpr << 1);
            i++;
            if (htim.Instance -> CNT > T) {
                result.tmpr++;
            }
        }
    }
    i = 0;
    while(i<8) {   //Read paritet
        htim.Instance->CNT = 0;
        HAL_TIM_Base_Start(&htim);
        
        while ((HAL_GPIO_ReadPin (AM2302_PORT, AM2302_PIN) == 0) &&
            (am2302_timeout == 0)) {    // Wait SDA->1 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop (&htim);
            T = htim.Instance -> CNT;
            htim.Instance -> CNT = 0;
            HAL_TIM_Base_Start (&htim);
        }
    
        while ((HAL_GPIO_ReadPin(AM2302_PORT, AM2302_PIN) == 1) &&
            (am2302_timeout == 0)) {    // Wait SDA->0 or am2302_timeout
        }
        if (am2302_timeout) {
            result.hum = 0;
            result.tmpr = 0;
            result.paritet = 0;
            return result;
        } else {
            HAL_TIM_Base_Stop(&htim);
            result.paritet = (result.paritet << 1);
            i++;
            if (htim.Instance -> CNT > T) {
                result.paritet++;
            }
        }
    }
    
    result.paritet -= (result.hum >> 8);    // paritet = hum_H + hum_L +
    result.paritet -= result.hum;           // tmpr_H + tmpr_L
    result.paritet -= (result.tmpr >> 8);
    result.paritet -= result.tmpr;
    if (result.paritet) {
        result.hum = 0;
        result.tmpr = 0;
        result.paritet = 0;
    }
    return result;
}