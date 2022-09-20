/* ����: temp_func.h
 * ��������: ������������ ���� � ������������ � �����������, ���������� � ���������� ������ � �����������. */


/************ ��������� ������������� ************/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#ifndef TEMP_FUNC
#define TEMP_FUNC

#define CORRECT_REPORT_ARG_NUM 6  // ���������� ����������, ������� ������ ���� ������� �� ������ ���������� �� ������� ������ �����.
#define BUFFER_STR_MAX_LEN 100    // ������������ ����� ������, � ������� �������� ������������ ������������ ������.

#define MIN_YEAR 2021
#define MAX_YEAR 2100
#define MIN_MONTH 1
#define MAX_MONTH 12
#define MIN_DAY 1
#define MAX_DAY 31
#define MIN_HOUR 0
#define MAX_HOUR 23
#define MIN_MINUTE 0
#define MAX_MINUTE 59
#define MIN_TEMP -80              // �����������, ���� ������� ����� ����� ��������� �������������� � ������ �� ����� �����������.
#define MAX_TEMP 120              // �����������, ���� ������� ����� ����� ��������� �������������� � ������ �� ����� �����������.

/* ����� ��������� �����, ������������� ����� ���������� ��� ������������������� ������� �����.
 * ����� 17 �����:
 *   - ������ 5 ������� ����������;
 *   - ��������� 6 ������� �������� ������ �������;
 *   - ��������� 6 ������� �������� ��������� �� ������ ��������.
 * ���������� ��������� �����: ����. ����������� 10, ���. ����������� -10, �����. ����������� 0, ������ � ������� 6 - 17. */
#define WRITE_TO_TEST_FILE(x)           \
    fprintf((x), "2022;1;1;0;1;-10\n"); \
    fprintf((x), "2022;1;1;0;2;-5\n");  \
    fprintf((x), "2022;1;1;0;3;0\n");   \
    fprintf((x), "2022;1;1;0;4;5\n");   \
    fprintf((x), "2022;1;1;0;5;10\n");  \
    fprintf((x), "bs;1;1;0;5;25\n");    \
    fprintf((x), "2022;bs;1;0;5;25\n"); \
    fprintf((x), "2022;1;bs;0;5;25\n"); \
    fprintf((x), "2022;1;1;bs;5;25\n"); \
    fprintf((x), "2022;1;1;0;bs;25\n"); \
    fprintf((x), "2022;1;1;0;5;bs\n");  \
    fprintf((x), "1889;1;1;0;1;25\n");  \
    fprintf((x), "2022;13;1;0;2;25\n"); \
    fprintf((x), "2022;1;0;0;3;25\n");  \
    fprintf((x), "2022;1;1;24;4;25\n"); \
    fprintf((x), "2022;1;1;0;-1;25\n"); \
    fprintf((x), "2022;1;1;0;0;300\n"); \


/********************* ���� *********************/

// ����������� ��� ������ ��� �������� ������� ������� �����������.
typedef struct report {
    int32_t y, m, d, h, mi, t;
} report;

/* ����������� ��� ������, ���������� ������ ��������� �� �������. �� ���� - ��������� ������� ���������
 * �����������, ��������� ��� ������ ����������. �������� ������������� ������ ���, ������ �������� �����������. */
typedef struct temp_func {
    uint32_t (*scan_report)(report *, FILE *);
    void (*print_report)(report *);
    bool (*check_values)(report *);
    void (*find_max_min_mean)(int32_t *, int32_t *, float *, FILE *, bool, uint32_t);
} temp_func;


/******************** ������� ********************/

/* UPD: � ���� ������ �������, ��� �� � ����� ������� ��� ������� static, �� ��������� ������ ������ �� ������������� �����. */
// static uint32_t scan_report(report *rep, FILE *f);
// static void print_report(report *rep);  // � ��������� ������ �� ������������, �� �� ������ ������ �������.
// static bool check_values(report *rep);
// static find_max_min_mean(int32_t *max_temp, int32_t *min_temp, float *mean_temp, FILE *f, bool m_key_OK, uint32_t month_num);


/****************** ���������� ******************/

// ���������� �������� �������.
temp_func Temp;


#endif
