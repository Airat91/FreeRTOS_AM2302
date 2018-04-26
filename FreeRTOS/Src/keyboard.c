#include "keyboard.h"

#define LIN1            GPIO_PIN_10
#define PORT_LIN1       GPIOD
#define LIN2            GPIO_PIN_11
#define PORT_LIN2       GPIOD
#define LIN3            GPIO_PIN_8
#define PORT_LIN3       GPIOD
#define LIN4            GPIO_PIN_9
#define PORT_LIN4       GPIOD
#define STB1            GPIO_PIN_14
#define PORT_STB1       GPIOD
#define STB2            GPIO_PIN_15
#define PORT_STB2       GPIOD
#define STB3            GPIO_PIN_12
#define PORT_STB3       GPIOD
#define STB4            GPIO_PIN_13
#define PORT_STB4       GPIOD

void keyboard_init () {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    GPIO_InitStruct.Pin = LIN1;
    HAL_GPIO_Init (PORT_LIN1, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LIN2;
    HAL_GPIO_Init (PORT_LIN2, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LIN3;
    HAL_GPIO_Init (PORT_LIN3, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LIN4;
    HAL_GPIO_Init (PORT_LIN4, &GPIO_InitStruct);
    
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    
    GPIO_InitStruct.Pin = STB1;
    HAL_GPIO_Init (PORT_STB1, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STB2;
    HAL_GPIO_Init (PORT_STB2, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STB3;
    HAL_GPIO_Init (PORT_STB3, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STB4;
    HAL_GPIO_Init (PORT_STB4, &GPIO_InitStruct);
}

char keyboard_get_key () {
    char key = 0xFF;

    HAL_GPIO_WritePin (PORT_STB1, STB1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin (PORT_STB2, STB2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin (PORT_STB3, STB3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin (PORT_STB4, STB4, GPIO_PIN_RESET);
  
    while(1) {
        HAL_GPIO_WritePin (PORT_STB1, STB1, GPIO_PIN_SET);  // STB1 = 1
    
        if (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {           // If pressed "1"
            while (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {    // Wait unpress
            }
            key = 1;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // If pressed "4"
            while (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // Wait unpress
            }
            key = 4;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // If pressed "7"
            while (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // Wait unpress
            }
            key=7;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // If pressed "*"
            while (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // Wait unpress
            }
            key = 14;
            break;
        }
    
        HAL_GPIO_WritePin (PORT_STB1, STB1, GPIO_PIN_RESET);// STB1 = 0    
        HAL_GPIO_WritePin (PORT_STB2, STB2, GPIO_PIN_SET);  // STB2 = 1
        
        if (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {           // If pressed "2"
            while (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {    // Wait unpress
            }
            key = 2;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // If pressed "5"
            while (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // Wait unpress
            }
            key = 5;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // If pressed "8"
            while (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // Wait unpress
            }
            key = 8;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // If pressed "0"
            while (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // Wait unpress
            }
            key = 0;
            break;
        }
        
        HAL_GPIO_WritePin (PORT_STB2, STB2, GPIO_PIN_RESET);// STB2 = 0    
        HAL_GPIO_WritePin (PORT_STB3, STB3, GPIO_PIN_SET);  // STB3 = 1
        
        if (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {           // If pressed "3"
            while (HAL_GPIO_ReadPin( PORT_LIN1, LIN1)) {    // Wait unpress
            }
            key = 3;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // If pressed "6"
            while (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // Wait unpress
            }
            key = 6;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // If pressed "9"
            while (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // Wait unpress
            }
            key = 9;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // If pressed "#"
            while (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // Wait unpress
            }
            key = 15;
            break;
        }
        
        HAL_GPIO_WritePin (PORT_STB3, STB3, GPIO_PIN_RESET);// STB3 = 0    
        HAL_GPIO_WritePin (PORT_STB4, STB4, GPIO_PIN_SET);  // STB4 = 1
        
        if (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {           // If pressed "A"
            while (HAL_GPIO_ReadPin (PORT_LIN1, LIN1)) {    // Wait unpress
            }
            key = 10;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // If pressed "B"
            while (HAL_GPIO_ReadPin (PORT_LIN2, LIN2)) {    // Wait unpress
            }
            key = 11;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // If pressed "C"
            while (HAL_GPIO_ReadPin (PORT_LIN3, LIN3)) {    // Wait unpress
            }
            key = 12;
            break;
        } else if (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // If pressed "D"
            while (HAL_GPIO_ReadPin (PORT_LIN4, LIN4)) {    // Wait unpress
            }
            key = 13;
            break;
        }
    
        HAL_GPIO_WritePin (PORT_STB4, STB4, GPIO_PIN_RESET);// STB4 = 0
    }    
    return key;
}

