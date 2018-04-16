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

void hd44780_init(void);     // ������� ������������� �������:
void hd44780_clr (void);               // ������� ������� ������ � ��������� ������� � ��������� ���������
//void hd44780_data_bits_input (void); // ������� ��������� ������� D4..D7 �� ������
void hd44780_data_bits_output (void);  // ������� ��������� ������� D4..D7 �� ������
//void hd44780_BF (void);                      // ������ �������� ���� ���������
void hd44780_send (char data, cmd_dat_type cmd_dat); // ������� �������� ������ � �������: CMD - �������, DAT - ������
void hd44780_xy (char x, char y);            // ������� ��������� ������� � ������� �(���� �� 1�� 16) ������ �(���� �� 1 �� 2)
void hd44780_string (char string[], int ms); // ������� ������ �� ����� ������ STRING, ����� ������ ������� ������� ���������� �������� MS �����������
void hd44780_conf (display_type display, cursor_type cursor, blink_type blink);   // ������� ��������� ������� �������: ON/OFF - ���������/���������� �������,
                                                                        // CURSOR/NO_CURSOR - ������ � ���� ������� �������������, BLINK/NO_BLINK - �������/�� ������� ����������
void hd44780_set_symbol (char adr, char SYMBOL[8]);  // ������� ������ ����������������� ������� 5�8: ADR - ����� ������ CGRAM, SYMBOL - ������ �� 8 ����� ���� char SYMBOL[8]={
                                                // 0b000XXXXX,          
                                                // 0b000XXXXX,
                                                // 0b000XXXXX,
   //        �����!!!                           // 0b000XXXXX,
   // ����� ������ ������ �������               // 0b000XXXXX,
   // ������ ��������������� �                  // 0b000XXXXX,
   // ������ ������ (1, 1)                      // 0b000XXXXX,
                                                // 0b000XXXXX}          // ����� � - �������� ���: 1-����� ���������, 0-����� �� ���������

void hd44780_shift (left_right_type left_righ, char num, int us);      // ������� ������ ������: LEFT_RIGHT=LEFT - �����, LEFT_RIGHT=RIGHT - ������, NUM - ���������� ������� ������, MS - �������� ����� ������ ������� �������
//void hd44780_delay(int MS);                         // ������� �������� � �������������
void hd44780_num (int num);                          // ������� ������ �� ����� ����� � ������ �����. ����� ������� ����� �� ������� ������� �������


#endif /* hd44780_H_ */