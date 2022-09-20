/* ������: �������� ������.
 * ��������: ���������� �������, �������������� ���������� � ��������� ����� �������� ������ ������� �����������.
 * �����: ������� ������, GeekBrains, ��������� IoT, ������ 2544. */


/************ ��������� ������������� ************/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>             // ��� getopt().
#include <stdlib.h>             // ��� atoi().

#include "temp_func.h"          // ���������� � ���������, ��������� � ���������� ������ � �����������.
#include "interface_func.h"     // ���������� � ���������, ��������� � ����������� �����-������.

#define FILE_NAME_MAX_LEN 200   // ������������ ����� ��������� ����� (������) ��������������� �����.


/* ���������/���������� ��������� ������.
 * ���� ������ ����� ����������������, ��:
 *  - �������� ���������� ��������� ����� �������� (�������� �� ������� ����� �� ����� ��������������).
 *  - ��������� ������� (��� �����������) �������� ���� test_input.txt � ������������� �����������
 * (��. ������ �� temp_func.h) � ���������� ������������ � �� ������ �������� ����������� �����. */
//#define TEST


/******************** ������� ********************/

int main(int argc, char *argv[]) {

    /* ��������� �� ��������� ������ ������� � ������� �������� "�����������" �������
     * � ����� ������� ����������� �� �������� ���������. */
    printf("Initializing program...\n");
    loading_indicator(80, 3);
    printf("\n");

    /* �������� ���������� ��������� */
    #ifndef TEST

        // ����������, ����������� � ������������ � ��������� ������� � �� ����������.
        bool h_key_OK = 0;
        char file_name[FILE_NAME_MAX_LEN] = {0};
        FILE *f = 0;
        bool f_key_OK = 0;
        uint32_t month_num = 0;
        bool m_key_OK = 0;

        /* ��������� �������� ������. �������� ��������� �����:
         *   -h (������������) - ����� �������� � ����������� �� ������������� ��������� (�������� ������� �� interface_func.h).
         *   -f <���_�����> (������������) - ���� ������ � ������ (�������) ��������������� �����.
         *   -m <�����_������> (������������) - ���� ������ ������ (�� 1 �� 12), �� ������� ��������� ���������� ������.
         * ���� ����� �� ������, ��������� ���������� ������ � ������� ���������� �� ��� (���������� ���� �������). */
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

                    f = fopen(file_name, "r");   // ��������� ���������� ������� ���� � ��������� ������ (�� ���������� ������).

                    if (f) {                     // ���� ��������� �� ���� ��������� (���� ��������� ������������� ���� ����������).
                        f_key_OK = 1;
                        printf("File name is valid.\n\n");
                    }

                    while(!f) {                                   // ���� ��������� �� ���� ������� (���� ������������ �� ����� ��� (�����) ������������� �����).
                        memset(file_name, 0, FILE_NAME_MAX_LEN);  // ��������� ������ ������, ����� �������������� ������� ������� ������� �����.
                        printf("Invalid file name. Please insert existing file name.\n");
                        scanf("%s", file_name);                   // ��������� ����, ���� ���������� ���� ��� ������������.
                        printf("File name inserted: ");
                        printf("%s\n", file_name);
                        f = fopen(file_name, "r");                // ��������� ���������� ������� ���� � ����� ��������� ������ (�������).
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
                        printf("Month selected: %s\n\n", months_str[month_num]);  // ��� ������ ������ �� ������� � interface_func.c.
                    }

                    while (!m_key_OK) {           // ���� ������������ �� ����� ���������� ����� ������.
                        printf("Invalid number. Please insert month number from 1 to 12\n");
                        printf("or restart the program without -m key to get annual statistics.\n");
                        scanf("%u", &month_num);  // ��������� ����, ���� ���������� ���� ��� ������������.
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

        /* ��������� ��� �����, ������� �� ��� ���� -f. */
        if(!f_key_OK && !h_key_OK) {
            printf("-f key and file name are obligatory to use this program properly.\n");
            printf("Please restart program with -h key to get help.\n\n");
        }

        /* ���������� ��� ������ ������� ��������. */
        int32_t max_temp = 101;
        int32_t min_temp = 101;
        float mean_temp = 101.0;

        /* ������� "������� �������" - �������, ������������ ������� �������� � ����������. */
        if (f_key_OK) {
            Temp.find_max_min_mean(&max_temp, &min_temp, &mean_temp, f, m_key_OK, month_num);
        }

        /* ����� ������� ��������. */
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

        /* ������������ ������������ � ���������� ��������� ������. */
        printf("Run finished. Press any key to quit.");
        getc(stdin);
    #endif


/************************ ���� ************************/

    #ifdef TEST
        printf("Test: can program process both correct and incorrect reports?\n");

        /* �������� ����� � ������������� ������� ��� ���������. */
        FILE *test_f = fopen("test_input.txt", "w");
        WRITE_TO_TEST_FILE(test_f);  // ������ �� temp_func.h, ������������ � ���� ������� ������������� ������.
        fclose(test_f);

        /* ���������� ��� ������ ������� ��������. */
        int32_t max_temp = 101;
        int32_t min_temp = 101;
        float mean_temp = 101.0;

        test_f = fopen("test_input.txt", "r");

        /* ������� "������� �������" - �������, ������������ ������� �������� � ����������. */
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
