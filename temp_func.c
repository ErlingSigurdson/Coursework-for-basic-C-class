/* ����: temp_func.c
 * ��������: ������ �������, ��������� � ���������� ������ � �����������. */


/************ ��������� ������������� ************/

#include "temp_func.h"
#include "interface_func.h"


/******************** ������� ********************/

static uint32_t scan_report(report *rep, FILE *f) {
    return fscanf(f, "%u; %u ;%u ;%u ;%u ;%d ", &(rep->y), &(rep->m), &(rep->d), &(rep->h), &(rep->mi), &(rep->t));
}

static void print_report(report *rep) {  // � ��������� ������ �� ������������, �� �� ������ ������ �������.
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

    /* ��������� ����������, ����������� � ����� � ��������� ������ �� �����. */
    report buffer_report;                       // �������� ���������, � ������� �� ������� ������������ ���������� ������ (������ �� ���������� �� ������� ����� �����).
    char buffer_str[BUFFER_STR_MAX_LEN] = {0};  // �������� ������, � ������� �� ������� ������������ ������������ ������ (����������� ������ �����).
    bool initial_read = 0;                      // ��������� ���������� ������ � ����������� � *max_temp � *min_temp �� ������� �� ����������� ������ � ��������� ����, ����� ��� ��������� ���� ��������.
    int64_t sum_temp = 0;                       // ����� ��������� ���������� ����������� ������.
    uint32_t items_read = 0;                    // ���������� ����������, ��������� �� ������ ����� (� ���������� �� ������� ������ ��� ������ ���� ����� 6).
    uint32_t correct_reports_read = 0;          // ���������� ��������� ���������� �������.
    uint32_t line_num = 0;                      // ���������� ��������� ����� �����.
    bool match = 0;                             // ���� ������������ ������ ����� ��������� ������. ����� ������ ������, ���� ������� ����� �� �������� �������������.
    uint64_t cursor_pos = ftell(f);             // ����������� ��������� �������. ������������, ����� �������� ������������ ������ (������) �������.

    /* ��������� �� ��������� ������ ������� � ������� �������� "�����������" �������
    * � ����� ������� ����������� �� �������� ���������. */
    printf("Initializing data processing...\n");
    loading_indicator(80, 3);
    printf("\n");

    while((items_read = Temp.scan_report(&buffer_report, f)) != EOF) {
        ++line_num;

        match = 0;
        if (!m_key_OK || buffer_report.m == month_num) {  // �������� �� ������������ ��������� ������.
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


/****************** ���������� ******************/

// ����������� �������� �������.
temp_func Temp = {
    .scan_report = &scan_report,
    .print_report = &print_report,
    .check_values = &check_values,
    .find_max_min_mean = &find_max_min_mean
};
