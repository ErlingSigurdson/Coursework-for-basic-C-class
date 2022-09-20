/* Проект: курсовой проект.
 * Описание: консольная утилита, обрабатывающая сохранённые в текстовом файле выходные данные датчика температуры.
 * Автор: Дмитрий Ефимов, GeekBrains, факультет IoT, группа 2544. */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ************/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>             // Для getopt().
#include <stdlib.h>             // Для atoi().

#include "temp_func.h"          // Объявления и директивы, связанные с обработкой данных о температуре.
#include "interface_func.h"     // Объявления и директивы, связанные с оформлением ввода-вывода.

#define FILE_NAME_MAX_LEN 200   // Максимальная длина вводимого имени (адреса) обрабатываемого файла.


/* Включение/выключение тестового режима.
 * Если макрос теста раскомментирован, то:
 *  - основной функционал программы будет отключён (введённые из консоли ключи не будут обрабатываться).
 *  - программа создаст (или перезапишет) тестовый файл test_input.txt с фиксированным содержанием
 * (см. макрос из temp_func.h) и обработает содержащиеся в нём данные согласно инструкциям теста. */
//#define TEST


/******************** ФУНКЦИИ ********************/

int main(int argc, char *argv[]) {

    /* Программа на несколько секунд выводит в консоль анимацию "вращающейся" полоски
     * и затем выводит уведомление об успешном прогрессе. */
    printf("Initializing program...\n");
    loading_indicator(80, 3);
    printf("\n");

    /* Основной функционал программы */
    #ifndef TEST

        // Переменные, заполняемые в соответствии с введёнными ключами и их значениями.
        bool h_key_OK = 0;
        char file_name[FILE_NAME_MAX_LEN] = {0};
        FILE *f = 0;
        bool f_key_OK = 0;
        uint32_t month_num = 0;
        bool m_key_OK = 0;

        /* Обработка введённых ключей. Доступны следующие ключи:
         *   -h (опциональный) - вывод страницы с инструкцией по использованию программы (согласно макросу из interface_func.h).
         *   -f <имя_файла> (обязательный) - ввод строки с именем (адресом) обрабатываемого файла.
         *   -m <номер_месяца> (опциональный) - ввод номера месяца (от 1 до 12), за который требуется обработать данные.
         * Если месяц не указан, программа обработает данные и выведет статистику за год (обработает файл целиком). */
        int32_t opt;
        while((opt = getopt(argc, argv, "hf:m:")) != -1) {
            switch (opt) {
                case 'h':
                    h_key_OK = 1;
                    HELP_PAGE;
                    break;

                case 'f':
                    strcpy(file_name, optarg);
                    printf("File name inserted: ");
                    printf("%s\n", file_name);

                    f = fopen(file_name, "r");   // Программа попытается открыть файл с указанным именем (по указанному адресу).

                    if (f) {                     // Если указатель на файл ненулевой (если указанный пользователем файл существует).
                        f_key_OK = 1;
                        printf("File name is valid.\n\n");
                    }

                    while(!f) {                                   // Пока указатель на файл нулевой (пока пользователь не введёт имя (адрес) существующего файла).
                        memset(file_name, 0, FILE_NAME_MAX_LEN);  // Заполняем строку нулями, чтобы гарантированно удалить остатки старого ввода.
                        printf("Invalid file name. Please insert existing file name.\n");
                        scanf("%s", file_name);                   // Повторный ввод, если предыдущий ввод был некорректным.
                        printf("File name inserted: ");
                        printf("%s\n", file_name);
                        f = fopen(file_name, "r");                // Программа попытается открыть файл с вновь указанным именем (адресом).
                        if (f) {
                            f_key_OK = 1;
                            printf("File name is valid.\n\n");
                            break;
                        }
                    }
                    break;

                case 'm':
                    month_num = atoi(optarg);

                    if (month_num > 0 && month_num <= 12) {
                        m_key_OK = 1;
                        printf("Number of month inserted: %u\n", month_num);
                        printf("Month selected: %s\n\n", months_str[month_num]);  // Имя месяца берётся из массива в interface_func.c.
                    }

                    while (!m_key_OK) {           // Пока пользователь не введёт корректный номер месяца.
                        printf("Invalid number. Please insert month number from 1 to 12\n");
                        printf("or restart the program without -m key to get annual statistics.\n");
                        scanf("%u", &month_num);  // Повторный ввод, если предыдущий ввод был некорректным.
                        printf("Number of month inserted: %d\n", month_num);
                        if (month_num > 0 && month_num <= 12) {
                            m_key_OK = 1;
                            printf("Month selected: %s\n\n", months_str[month_num]);
                            break;
                        }
                    }
                    break;
            }
        }

        /* Пояснения для юзера, который не ввёл ключ -f. */
        if(!f_key_OK && !h_key_OK) {
            printf("-f key and file name are obligatory to use this program properly.\n");
            printf("Please restart program with -h key to get help.\n\n");
        }

        /* Переменные для записи искомых значений. */
        int32_t max_temp = 101;
        int32_t min_temp = 101;
        float mean_temp = 101.0;

        /* Главная "рабочая лошадка" - функция, записывающая искомые значения в переменные. */
        if (f_key_OK) {
            Temp.find_max_min_mean(&max_temp, &min_temp, &mean_temp, f, m_key_OK, month_num);
        }

        /* Вывод искомых значений. */
        if (f_key_OK) {
            printf("\n");
            if (!m_key_OK) {
                printf("Annual statistics are as follows:\n");
            } else {
                printf("Statistics for %s are as follows:\n", months_str[month_num]);
            }
            printf("Maximal temperature: %d\n", max_temp);
            printf("Minimal temperature: %d\n", min_temp);
            printf("Average temperature: %.2f\n\n", mean_temp);
        }

        /* Пользователь уведомляется о завершении обработки данных. */
        printf("Run finished. Press any key to quit.");
        getc(stdin);
    #endif


/************************ ТЕСТ ************************/

    #ifdef TEST
        printf("Test: can program process both correct and incorrect reports?\n");

        /* Создание файла с фиксированным текстом для обработки. */
        FILE *test_f = fopen("test_input.txt", "w");
        WRITE_TO_TEST_FILE(test_f);  // Макрос из temp_func.h, записывающий в файл заранее заготовленные строки.
        fclose(test_f);

        /* Переменные для записи искомых значений. */
        int32_t max_temp = 101;
        int32_t min_temp = 101;
        float mean_temp = 101.0;

        test_f = fopen("test_input.txt", "r");

        /* Главная "рабочая лошадка" - функция, записывающая искомые значения в переменные. */
        Temp.find_max_min_mean(&max_temp, &min_temp, &mean_temp, test_f, 0, 0);

        fclose(test_f);

        printf("\n");
        printf("Test results are as follows:\n");
        printf("Maximal temperature: %d\n", max_temp);
        printf("Minimal temperature: %d\n", min_temp);
        printf("Average temperature: %.2f\n\n", mean_temp);

        printf("Run finished. Press any key to quit.");
        getc(stdin);
    #endif

/*******************************************************/


    return 0;
}
