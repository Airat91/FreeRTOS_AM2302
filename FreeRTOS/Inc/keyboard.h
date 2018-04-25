// Library for matrix type keyboard 4x4
// Ver_1.0

/*========== LIBRARY DESCRIPTION ==========
- Library use STM32F3xx_HAL_Driver
*/

// Buttons map

//      STB1|STB2|STB3|STB4
// LIN1  1  | 2  | 3  | 10
// ------------------------
// LIN2  4  | 5  | 6  | 11
// ------------------------
// LIN3  7  | 8  | 9  | 12
// ------------------------
// LIN4  14 | 0  | 15 | 13

#ifndef keyboard_H_
#define keyboard_H_
#include "stm32f3xx_hal.h"

//========== FUNCTIONS PROTOTYPES ==========

void keyboard_init(void);
char keyboard_get_key();
#endif // keyboard_H_
