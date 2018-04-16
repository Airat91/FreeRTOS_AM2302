// Library for LCD Display 1602 based on chip HD44780
// Ver_2.1

/*========== LIBRARY DESCRIPTION ==========
- Library use STM32F3xx_HAL_Driver 
- Using LCD Display in 4-wire mode
*/

#ifndef hd44780_H_
#define hd44780_H_
#include "stm32f3xx_hal.h"

typedef enum {CMD, DAT} cmd_dat_type;
typedef enum {DISPLAY_OFF = 0, DISPLAY_ON} display_type;
typedef enum {CURSOR_OFF = 0, CURSOR_ON} cursor_type;
typedef enum {BLINK_OFF = 0, BLINK_ON} blink_type;
typedef enum {LEFT = 0, RIGHT} left_right_type;

/*========== FUNCTIONS PROTOTYPES ==========*/

void hd44780_init(void);     // Функция инициализации дисплея:
void hd44780_clr (void);               // Функция очистки экрана и установки курсора в начальное положение
//void hd44780_data_bits_input (void); // Функция настройки выводов D4..D7 на чтение
void hd44780_data_bits_output (void);  // Функция настройки выводов D4..D7 на запись
//void hd44780_BF (void);                      // Функия ожидания бита занятости
void hd44780_send (char data, cmd_dat_type cmd_dat); // Функция отправки данных в дисплей: CMD - команда, DAT - данные
void hd44780_xy (char x, char y);            // Функция установки курсора в позицию х(счет от 1до 16) строки у(счет от 1 до 2)
void hd44780_string (char string[], int ms); // Функция вывода на экран строки STRING, после вывода каждого символа происходит задержка MS миллисекунд
void hd44780_conf (display_type display, cursor_type cursor, blink_type blink);   // Функция настройки курсора дисплея: ON/OFF - включение/выключение дисплея,
                                                                        // CURSOR/NO_CURSOR - курсор в виде нижнего подчеркивания, BLINK/NO_BLINK - мигание/не мигание знакоместа
void hd44780_set_symbol (char adr, char SYMBOL[8]);  // Функция записи пользовательского символа 5х8: ADR - адрес ячейки CGRAM, SYMBOL - массив из 8 строк вида char SYMBOL[8]={
                                                // 0b000XXXXX,          
                                                // 0b000XXXXX,
                                                // 0b000XXXXX,
   //        ВАЖНО!!!                           // 0b000XXXXX,
   // После вызова данной функции               // 0b000XXXXX,
   // курсор устанавливается в                  // 0b000XXXXX,
   // начало экрана (1, 1)                      // 0b000XXXXX,
                                                // 0b000XXXXX}          // здесь Х - значащий бит: 1-точка закрашена, 0-точка не закрашена

void hd44780_shift (left_right_type left_righ, char num, int us);      // Функция сдвига экрана: LEFT_RIGHT=LEFT - влево, LEFT_RIGHT=RIGHT - вправо, NUM - количество позиций сдвига, MS - задержка после сдвига каждого символа
//void hd44780_delay(int MS);                         // Функция задержки в миллисекундах
void hd44780_num (int num);                          // Функция вывода на экран числа с учетом знака. Число пишется влево от текущей позиции курсора


#endif /* hd44780_H_ */