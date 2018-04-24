#include "hd44780.h"

#define RS      GPIO_PIN_10
#define PORT_RS GPIOC
#define RW      GPIO_PIN_11
#define PORT_RW GPIOC
#define E       GPIO_PIN_12
#define PORT_E  GPIOC
#define D4      GPIO_PIN_4
#define PORT_D4 GPIOD
#define D5      GPIO_PIN_5
#define PORT_D5 GPIOD
#define D6      GPIO_PIN_6
#define PORT_D6 GPIOD
#define D7      GPIO_PIN_7
#define PORT_D7 GPIOD

#define WAIT_TIKS(x)    for (char i = x; i > 0; i--) {};
#define BIT(x)          (1 << x)

void hd44780_data_bits_output (void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  if ((PORT_D4 == GPIOA) || (PORT_D5 == GPIOA) || (PORT_D6 == GPIOA) || 
  (PORT_D7 == GPIOA) || (PORT_E == GPIOA) || (PORT_RW == GPIOA) || 
  (PORT_RS == GPIOA)) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  } if ((PORT_D4 == GPIOB) || (PORT_D5 == GPIOB) || (PORT_D6 == GPIOB) || 
  (PORT_D7 == GPIOB) || (PORT_E == GPIOB) || (PORT_RW == GPIOB) || 
  (PORT_RS == GPIOB)) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  } if ((PORT_D4 == GPIOC) || (PORT_D5 == GPIOC) || (PORT_D6 == GPIOC) || 
  (PORT_D7 == GPIOC) || (PORT_E == GPIOC) || (PORT_RW == GPIOC) || 
  (PORT_RS == GPIOC)) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  } if ((PORT_D4 == GPIOD) || (PORT_D5 == GPIOD) || (PORT_D6 == GPIOD) || 
  (PORT_D7 == GPIOD) || (PORT_E == GPIOD) || (PORT_RW == GPIOD) || 
  (PORT_RS == GPIOD)) {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  } if ((PORT_D4 == GPIOE) || (PORT_D5 == GPIOE) || (PORT_D6 == GPIOE) || 
  (PORT_D7 == GPIOE) || (PORT_E == GPIOE) || (PORT_RW == GPIOE) || 
  (PORT_RS == GPIOE)) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  } if ((PORT_D4 == GPIOF) || (PORT_D5 == GPIOF) || (PORT_D6 == GPIOF) || 
  (PORT_D7 == GPIOF) || (PORT_E == GPIOF) || (PORT_RW == GPIOF) || 
  (PORT_RS == GPIOF)) {
    __HAL_RCC_GPIOF_CLK_ENABLE();
  }
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pin = D4;
  HAL_GPIO_Init (PORT_D4, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = D5;
  HAL_GPIO_Init (PORT_D5, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = D6;
  HAL_GPIO_Init (PORT_D6, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = D7;
  HAL_GPIO_Init (PORT_D7, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = E;
  HAL_GPIO_Init (PORT_E, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = RW;
  HAL_GPIO_Init (PORT_RW, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = RS;
  HAL_GPIO_Init (PORT_RS, &GPIO_InitStruct);
}

void hd44780_send (char data, char cmd_dat) {
  
  HAL_GPIO_WritePin (PORT_E, E, GPIO_PIN_SET);          // E = 1
  HAL_GPIO_WritePin (PORT_RW, RW, GPIO_PIN_RESET);      // RW = 0
  
  if (cmd_dat == CMD) {                                 // if command
    HAL_GPIO_WritePin (PORT_RS, RS, GPIO_PIN_RESET);    // RS = 0
  } else {                                              // if data
    HAL_GPIO_WritePin (PORT_RS, RS, GPIO_PIN_SET);      // RS = 1
  }
  
  if (data & BIT(4)) {
    HAL_GPIO_WritePin (PORT_D4, D4, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D4, D4, GPIO_PIN_RESET);
  }
  
  if (data & BIT(5)) {
    HAL_GPIO_WritePin (PORT_D5, D5, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D5, D5, GPIO_PIN_RESET);
  }
  
  if (data & BIT(6)) {
    HAL_GPIO_WritePin (PORT_D6, D6, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D6, D6, GPIO_PIN_RESET);
  }
  
  if (data & BIT(7)) {
    HAL_GPIO_WritePin (PORT_D7, D7, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D7, D7, GPIO_PIN_RESET);
  }
  
  WAIT_TIKS(5)
  HAL_GPIO_WritePin (PORT_E, E, GPIO_PIN_RESET);         // E = 0
  HAL_GPIO_WritePin (PORT_E, E, GPIO_PIN_SET);           // E = 1

  if (data & BIT(0)) {
    HAL_GPIO_WritePin(PORT_D4, D4, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(PORT_D4, D4, GPIO_PIN_RESET);
  }
  
  if (data & BIT(1)) {
    HAL_GPIO_WritePin (PORT_D5, D5, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D5, D5, GPIO_PIN_RESET);
  }
  
  if (data & BIT(2)) {
    HAL_GPIO_WritePin (PORT_D6, D6, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D6, D6, GPIO_PIN_RESET);
  }
  
  if (data & BIT(3)) {
    HAL_GPIO_WritePin (PORT_D7, D7, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin (PORT_D7, D7, GPIO_PIN_RESET);
  }
  
  WAIT_TIKS(5)
  HAL_GPIO_WritePin (PORT_E, E, GPIO_PIN_RESET);         // E = 0  
  HAL_Delay (1);
}

void hd44780_clr (void) {
  hd44780_send (0x01, CMD);
  hd44780_send (0x02, CMD);
}

void hd44780_conf (display_type display, char cursor, char blink) {
  char data = (0x08 + (display << 2) + (cursor << 1) + blink);  // data structure: 0b00001DCB
                                                                // D = 1 Display is ON  / D = 0 Display is OFF
                                                                // C = 1 Cursor is ON   / C = 0 Cursor is OFF
                                                                // B = 1 Blinked is ON  / B = 0 Blinked is OFF
  hd44780_send (data, CMD);
  
/*  if ((ON_OFF == ON) & (CURSOR_NOCURSOR == CURSOR) & (BLINK_NOBLINK == BLINK)) {
    hd44780_send (0x0F, CMD);
  } else if ((ON_OFF == ON) & (CURSOR_NOCURSOR == CURSOR) & (BLINK_NOBLINK == NOBLINK)) {
    hd44780_send (0x0E, CMD);
  } else if ((ON_OFF == ON) & (CURSOR_NOCURSOR == NOCURSOR) & (BLINK_NOBLINK == BLINK)) {
    hd44780_send (0x0D, CMD);
  } else if ((ON_OFF == ON) & (CURSOR_NOCURSOR == NOCURSOR) & (BLINK_NOBLINK == NOBLINK)) {
    hd44780_send (0x0C, CMD);
  } else if (ON_OFF == OFF) {
    hd44780_send (0x0B, CMD);
  }*/
}

void hd44780_xy (char x, char y) {
#if ((x > 16) || (y > 2))
#error x > 16 or y > 2
#endif
  char adr = (x-1);
  if (y == 2) {
    adr += 0x40;
  }
  adr |= 0x80;
  hd44780_send (adr, CMD);
}

void hd44780_string (char string[], int ms) {
  char i = 0;
  const char SHRIFT[] = {
		0x41,	// 0xC0	А 
		0xA0,	// Б
		0x42,	// В
		0xA1,	// Г
		0xE0,	// Д
		0x45,	// Е
		0xA3,	// 0хС6 Ж
		0xA4,	// З
		0xA5,	// И
		0xA6,	// Й
		0x4B,	// К
		0xA7,	// Л
		0x4D,	// М
		0x48,	// Н
		0x4F,	// О
		0xA8,	// П
		0x50,	// Р			
		0x43,	// С			
		0x54,	// Т			
		0xA9,	// У			
		0xAA,	// ф	
		0x58,	// Х			
		0xE1,	// Ц
		0xAB,	// Ч			
		0xAC,	// Ш								
		0xE2,	// Щ			
		0xAD,	// Ъ
		0xAE,	// Ы
		0x62,	// Ь			
		0xAF,	// Э			
		0xB0,	// Ю			
		0xB1,	// Я			

		0x61,	// а	
		0xB2,	// б
		0xB3,	// в
		0xB4,	// г
		0xE3,	// д
		0x65,	// е
		0xB6,	// ж
		0xB7,	// з
		0xB8,	// и
		0xB9,	// й
		0xBA,	// к
		0xBB,	// л
		0xBC,	// м
		0xBD,	// н
		0x6F,	// o
		0xBE,	// п
		0x70,	// р
		0x63,	// с
		0xBF,	// т
		0x79,	// у
		0xE4,	// Ф
		0x78,	// х
		0xE5,	// ц
		0xC0,	// ч
		0xC1,	// ш
		0xE6,	// щ
		0xC2,	// ъ
		0xC3,	// ы
		0xC4,	// ь
		0xC5,	// э
		0xC6,	// ю
		0xC7,	// я
  };
  while (string[i] != 0) {
    if (string[i] > 0xBF) {
      hd44780_send (SHRIFT[(string[i] - 0xC0)], DAT);
      if(ms>0) {
        HAL_Delay (ms);
      }
    } else {
      hd44780_send (string[i], DAT);
      if (ms > 0) {
        HAL_Delay (ms);
      }
    }
    i++;
  }
}

void hd44780_user_symbol (char adr, const char symbol[8]) {
  adr = (adr << 3) | 0x40;
  for (char i=0; i<8; i++) {
    hd44780_send ((adr + i), CMD);
    hd44780_send (symbol[i], DAT);
  }
  hd44780_send (0x80, CMD);
}

void Lhd44780_shift (char LEFT_RIGHT, char num, int ms) {
  if (LEFT_RIGHT == LEFT) {
    while ( num > 0) {
      hd44780_send (0x18, CMD);
      num--;
      if (ms > 0) {
        HAL_Delay (ms);
      }
    }
  }
  if (LEFT_RIGHT == RIGHT) {
    while (num > 0) {
      hd44780_send (0x1C, CMD);
      num--;
      if (ms > 0) {
        HAL_Delay(ms);
      }
    }
  }
}

void hd44780_num (signed int num) {
  char sign = 1;    // 1 - положительное число, 0 -о трицательное число
  char digit = 0;   // переменная, которая будет содержать в себе цифру 0...9
  if (num < 0) {    // если число отрицательное
    sign = 0;       // устанавливаем соответствующий параметр sign
    num = num * -1; // делаем число положительным
  }
  do {              // цикл с постустловием
    digit = num % 10;               // Остаток от деления на 10 это младший разряд числа
    hd44780_send (digit | 0x30, DAT);   // Пишем полученное число на экран
    hd44780_send (0x10, CMD);         // Сдвигаем курсор влево 2 раза
    hd44780_send (0x10, CMD);
    num = (num - digit) / 10;         // Убираем из числа выведенный разряд
  } while (num > 0);                // Повторяем до тех пор, пока числа не закончатся
  if (sign == 0) {           // Если число было отрицаельным
    hd44780_send (0x2D, DAT); // Вывести на экран знак "-"
  }
}

void hd44780_init (void) {
  hd44780_data_bits_output ();
  HAL_Delay (30); 
  hd44780_send (0x22, CMD);
  hd44780_send (0x28, CMD);
  hd44780_conf (DISPLAY_ON, CURSOR_OFF, BLINK_OFF);
  HAL_Delay (1);
  hd44780_clr ();
  HAL_Delay (2);
  hd44780_send (0x06, CMD);

}