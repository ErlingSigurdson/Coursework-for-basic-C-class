/* Файл: temp_func.c
 * Описание: раздел проекта, связанный с обработкой данных о температуре. */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ************/

#include "temp_func.h"
#include "interface_func.h"


/******************** ФУНКЦИИ ********************/

static uint32_t scan_report(report *rep, FILE *f) {
    return fscanf(f, "%u; %u ;%u ;%u ;%u ;%d ", &(rep->y), &(rep->m), &(rep->d), &(rep->h), &(rep->mi), &(rep->t));
}

static void print_report(report *rep) {  // В финальной версии не используется, но на всякий случай оставил.
    printf("%u;", rep->y);
    printf("%u;", rep->m);
    printf("%u;", rep->d);
    printf("%u;", rep->h);
    printf("%u;", rep->mi);
    printf("%d\n", rep->t);
}

static bool check_values(report *rep) {
    bool result = 1;

    if (rep->y < MIN_YEAR || rep->y > MAX_YEAR) {
        result = 0;
    }

    if (rep->m < MIN_MONTH || rep->m > MAX_MONTH) {
        result = 0;
    }

    if (rep->d < MIN_DAY || rep->d > MAX_DAY) {
        result = 0;
    }

    if (rep->h < MIN_HOUR || rep->h > MAX_HOUR) {
        result = 0;
    }

    if (rep->mi < MIN_MINUTE || rep->mi > MAX_MINUTE) {
        result = 0;
    }

    if (rep->t < MIN_TEMP || rep->t > MAX_TEMP) {
        result = 0;
    }

    return result;
}

static void find_max_min_mean(int32_t *max_temp, int32_t *min_temp, float *mean_temp, FILE *f, bool m_key_OK, uint32_t month_num) {

    /* Служебные переменные, участвующие в сборе и обработке данных из файла. */
    report buffer_report;                       // Буферная структура, в которую по очереди записываются корректные отчёты (данные из подходящих по формату строк файла).
    char buffer_str[BUFFER_STR_MAX_LEN] = {0};  // Буферная строка, в которую по очереди записываются некорректные отчёты (неформатные строки файла).
    bool initial_read = 0;                      // Программа записывает данные о температуре в *max_temp и *min_temp из первого же корректного отчёта и поднимает флаг, чтобы это произошло лишь единожды.
    int64_t sum_temp = 0;                       // Сумма считанных температур нарастающим итогом.
    uint32_t items_read = 0;                    // Количество аргументов, считанных из строки файла (у подходящей по формату строки оно должно быть равно 6).
    uint32_t correct_reports_read = 0;          // Количество считанных корректных отчётов.
    uint32_t line_num = 0;                      // Количество считанных строк файла.
    bool match = 0;                             // Флаг соответствия строки файла введённому месяцу. Будет всегда поднят, если искомый месяц не определён пользователем.
    uint64_t cursor_pos = ftell(f);             // Определение положения каретки. Используется, чтобы выводить некорректные отчёты (строки) целиком.

    /* Программа на несколько секунд выводит в консоль анимацию "вращающейся" полоски
    * и затем выводит уведомление об успешном прогрессе. */
    printf("Initializing data processing...\n");
    loading_indicator(80, 3);
    printf("\n");

    while((items_read = Temp.scan_report(&buffer_report, f)) != EOF) {
        ++line_num;

        match = 0;
        if (!m_key_OK || buffer_report.m == month_num) {  // Проверка на соответствие заданному месяцу.
            match = 1;
        }

        if (items_read == CORRECT_REPORT_ARG_NUM && match && Temp.check_values(&buffer_report)) {
            ++correct_reports_read;
            sum_temp += buffer_report.t;

                if (!initial_read) {
                    *max_temp = buffer_report.t;
                    *min_temp = buffer_report.t;
                    initial_read = 1;
                }

                if (*max_temp < buffer_report.t) {
                    *max_temp = buffer_report.t;
                }

                if (*min_temp > buffer_report.t) {
                    *min_temp = buffer_report.t;
                }

                cursor_pos = ftell(f);
            } else if (items_read != CORRECT_REPORT_ARG_NUM || !Temp.check_values(&buffer_report)) {
                fseek(f, cursor_pos, SEEK_SET);
                fgets(buffer_str, BUFFER_STR_MAX_LEN, f);
                printf("Warning! Incorrect report. Line: %u. Contents: %s", line_num, buffer_str);
                memset(buffer_str, 0, BUFFER_STR_MAX_LEN);
                cursor_pos = ftell(f);
            } else if (items_read == CORRECT_REPORT_ARG_NUM && !match && Temp.check_values(&buffer_report)) {
                cursor_pos = ftell(f);
            }
        }
        fclose(f);

        if (correct_reports_read) {
            *mean_temp = sum_temp / (float)correct_reports_read;
        }
}


/****************** ПЕРЕМЕННЫЕ ******************/

// Определение сборника функций.
temp_func Temp = {
    .scan_report = &scan_report,
    .print_report = &print_report,
    .check_values = &check_values,
    .find_max_min_mean = &find_max_min_mean
};
