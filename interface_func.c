/* ����: interface_func.c
 * ��������: ������ �������, ��������� � ����������� �����-������. */


/************ ��������� ������������� ************/

#include "interface_func.h"


/******************* ���������� *******************/

const char *months_str[NUMBER_OF_MONTHS] = {
    NULL, "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};


/******************** ������� ********************/

// ����� ������ � �������� ���������� ������ ������ ���������.
void wait(uint32_t period) {
    clock_t current_t = clock();
    clock_t previous_t = current_t;
    while(current_t - previous_t < period) {
        current_t = clock();
    }
    return;
}

/* ����� � ������� "�����������" �������.
 * rotation_period - ������������ ������� "�����" � ������ ������ ���������.
 * rotation_cycles - ���������� ������ "��������". */
void loading_indicator(uint32_t rotation_period, uint32_t rotation_cycles) {

    const char *frames[NUMBER_OF_FRAMES] = {
        "|", "/", "-", "\\", "|", "/", "-", "\\"
    };

    for (uint32_t i = 0; i < rotation_cycles; ++i) {
        for(uint32_t j = 0; j < NUMBER_OF_FRAMES; ++j) {
            printf("\r%s", frames[j]);
            wait(rotation_period);
        }
    }
    printf("\r...done.\n");
}
