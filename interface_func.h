/* ����: interface_func.h
 * ��������: ������������ ���� � ������������ � �����������, ���������� � ����������� �����-������. */


/************ ��������� ������������� ************/

#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#ifndef INTERFACE_FUNC
#define INTERFACE_FUNC

#define NUMBER_OF_FRAMES 8     // ���������� ������ �������� "�����������" �������.
#define NUMBER_OF_MONTHS 13    /* ���������� ����� � ������� �������� �������. ���� ������ ��������� ���
                                * ��������������� "��������" ������. */

// ���������� �� ������������� ���������.
#define HELP_PAGE                                                                                                                 \
    puts("o*********************************************** HELP PAGE ********************************************************o"); \
    puts("|Welcome to the temperature sensor output data processing utility.                                                 |"); \
    puts("|This is a coursework by Dmitry Efimov, GeekBrains IoT group 2544.                                                 |"); \
    puts("|Version 1.0                                                                                                       |"); \
    puts("|                                                                                                                  |"); \
    puts("|This program processes .csv of .txt file with temperature data in given format and outputs maximal, minimal and   |"); \
    puts("|average (mean) temperature for either year (whole file) or specified month.                                       |"); \
    puts("|                                                                                                                  |"); \
    puts("|Supported console keys (parameters) are as follows:                                                               |"); \
    puts("|  -h (optional) - opens this help page.                                                                           |"); \
    puts("|  -f <file_name> (required) - specifies a file with .csv or .txt extension containing the data to be processed.   |"); \
    puts("|Valid data format is yyyy;mm;dd;hh;mm;cc with cc being temperature in centigrades.                                |"); \
    puts("|If no valid data was read, all output values will be displayed as 101 (sort of error signal).                     |"); \
    puts("|  -m <month_number> (optional) - specifies a month regarding to which the program should process the data.        |"); \
    puts("|Valid input is a number from 1 to 12.                                                                             |"); \
    puts("|If this key wasn't inserted, the program will provide annual statistics.                                          |"); \
    puts("o******************************************************************************************************************o"); \
    puts("");


/****************** ���������� ******************/

extern const char *months_str[NUMBER_OF_MONTHS];  // ������ � ���������� �������.


/******************** ������� ********************/

void wait(uint32_t period);  // ����� ������ � �������� ���������� ������ ������ ���������.

/* ����� � ������� �������� "�����������" �������.
 * rotation_period - ������������ ������� "�����" � ������ ������ ���������.
 * rotation_cycles - ���������� ������ "��������". */
void loading_indicator(uint32_t rotation_period, uint32_t rotation_cycles);


#endif
