/* Файл: interface_func.c
 * Описание: раздел проекта, связанный с оформлением ввода-вывода. */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ************/

#include "interface_func.h"


/******************* ПЕРЕМЕННЫЕ *******************/

const char *months_str[NUMBER_OF_MONTHS] = {
    NULL, "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};


/******************** ФУНКЦИИ ********************/

// Пауза длиной в заданное количество тактов работы программы.
void wait(uint32_t period) {
    clock_t current_t = clock();
    clock_t previous_t = current_t;
    while(current_t - previous_t < period) {
        current_t = clock();
    }
    return;
}

/* Вывод в консоль "вращающейся" стрелки.
 * rotation_period - длительность каждого "кадра" в тактах работы программы.
 * rotation_cycles - количество циклов "вращения". */
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
