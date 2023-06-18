#define _CRT_SECURE_NO_WARNINGS
#define MAX_ELEMENT_SIZE 20
#define MAX_CATEGORY_NAME_SIZE 15
#define MAX_CATEGORY_SIZE 20
#define MAX_ANNI_SIZE 20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "makeScheduleFunc.h"

anniversary* anni[MAX_ANNI_SIZE];

void textColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

enum ColorType {    // ���� �� ����
    DarkBlue = 1,
    DarkGreen = 2,
    DarkSkyBlue = 3,
    DarkRed = 4,
    DarkPurple = 5,
    DarkYellow = 6,
    DarkGray = 8,
    Blue = 9,
    Green = 10,
    SkyBlue = 11,
    Red = 12,
    Purple = 13,
    Yellow = 14,
    White = 7
};

ScheduleType* schedules[12][31];
char categoryList[MAX_CATEGORY_SIZE][MAX_CATEGORY_NAME_SIZE];   // ī�װ� ����Ʈ
int categoryIndex = 0;   // ī�װ� ��� ���� ����
int fixedData = 0;   // ī�װ� ������ "������","��������","�Ϲ�"

void file_Init_Holidays() // ������, �������� ���� �ʱ�ȭ
{
    char* date = (char*)malloc(6 * sizeof(char));
    char* date_temp = (char*)malloc(6 * sizeof(char));
    char* schedule_temp = (char*)malloc(1024 * sizeof(char));
    char* category_temp = (char*)malloc(20 * sizeof(char));
    ScheduleType* head; //���� ó�� ��� ����Ű�� head ���

    FILE* pfile = fopen("CSV//holidayss.csv", "r");   // ���� open
    char str_tmp[1024];

    if (pfile != NULL)   // ������ �����Ѵٸ�
    {
        fgets(str_tmp, 1024, pfile);   // ���Ͽ��� ���پ� �о�ͼ� ���ڿ��� ��ȯ

        while (fgets(str_tmp, 1024, pfile) != NULL) {   // ������ ������ // ���Ͽ��� ������ �о�ͼ� ���ڿ��� ��ȯ
            element* item = (element*)malloc(sizeof(element)); //���� �Ҵ�

            item->date = (char*)malloc(6 * sizeof(char)); //���� �Ҵ�
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(10 * sizeof(char));

            char* ptr = strtok(str_tmp, ",");   // ���ڿ��� ,�� �����ؼ� ������   
            strcpy(date_temp, ptr);

            ptr = strtok(NULL, ",");
            strcpy(schedule_temp, ptr);

            ptr = strtok(NULL, ",");
            strcpy(category_temp, ptr);

            strcpy(date, date_temp);
            char* pptr = strtok(date, ".");   // ���ڿ��� .�� �����ؼ� ������ 
            int month = atoi(pptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
            pptr = strtok(NULL, ".");
            int day = atoi(pptr) - 1;

            if (is_empty(month, day) == 0) { //�ش� ��¥�� ������ �������� ������
                head = NULL;
            }
            else { //�ش� ��¥�� ������ �����ϸ�
                head = schedules[month][day];
            }

            item = merge_data(date_temp, schedule_temp, category_temp);

            head = insert_node(head, item); //��带 insert

            schedules[month][day] = head;
        }
    }
    fclose(pfile);
}

void file_Init_UserData() // ���� ���� �ʱ�ȭ
{
    char* date = (char*)malloc(6 * sizeof(char));
    char* date_temp = (char*)malloc(6 * sizeof(char));
    char* schedule_temp = (char*)malloc(1024 * sizeof(char));
    char* category_temp = (char*)malloc(20 * sizeof(char));
    ScheduleType* head; //���� ó�� ��� ����Ű�� head ���

    FILE* pfile = fopen("CSV//userData.csv", "r");   // ���� open
    char str_tmp[1024];

    if (pfile != NULL)   // ������ �����Ѵٸ�
    {
        while (fgets(str_tmp, 1024, pfile) != NULL) {   // ������ ������ // ���Ͽ��� ������ �о�ͼ� ���ڿ��� ��ȯ
            element* item = (element*)malloc(sizeof(element)); //���� �Ҵ�

            item->date = (char*)malloc(6 * sizeof(char)); //���� �Ҵ�
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(10 * sizeof(char));

            if (strchr(str_tmp, ',')) {
                char* ptr = strtok(str_tmp, ",");   // ���ڿ��� ,�� �����ؼ� ������   
                strcpy(date_temp, ptr);

                ptr = strtok(NULL, ",");
                strcpy(schedule_temp, ptr);

                ptr = strtok(NULL, ",");
                strcpy(category_temp, ptr);

                strcpy(date, date_temp);
                char* pptr = strtok(date, ".");   // ���ڿ��� .�� �����ؼ� ������ 
                int month = atoi(pptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
                pptr = strtok(NULL, ".");
                int day = atoi(pptr) - 1;

                if (is_empty(month, day) == 0) { //�ش� ��¥�� ������ �������� ������
                    head = NULL;
                }
                else { //�ش� ��¥�� ������ �����ϸ�
                    head = schedules[month][day];
                }

                item = merge_data(date_temp, schedule_temp, category_temp);

                head = insert_node(head, item); //��带 insert

                schedules[month][day] = head;
            }
        }
    }
    fclose(pfile);
}

void file_Update_User() // ���� ������Ʈ
{
    FILE* pfile = fopen("CSV//userData.csv", "w"); // ���� open

    if (pfile != NULL) // ������ �����Ѵٸ�
    {
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 31; j++) {
                ScheduleType* head;

                if (is_empty(i, j) == 0) {
                    head = NULL;
                }
                else {
                    head = schedules[i][j];
                }

                sort_schedule(&head);

                if (head != NULL) {
                    for (ScheduleType* p = head; p != NULL; p = p->link) {
                        char date_temp[7] = "";
                        char* schedule_temp = (char*)malloc(1024 * sizeof(char));
                        char* category_temp = (char*)malloc(20 * sizeof(char));
                        char month[MAX_ELEMENT_SIZE];
                        char day[MAX_ELEMENT_SIZE];

                        if (strcmp(p->data->category, "������\n") == 0 ||
                            strcmp(p->data->category, "��������\n") == 0) {
                            continue;
                        }

                        element* item = (element*)malloc(sizeof(element)); // ���� �Ҵ�

                        item->date = (char*)malloc(6 * sizeof(char)); // ���� �Ҵ�
                        item->schedule = (char*)malloc(1024 * sizeof(char));
                        item->category = (char*)malloc(10 * sizeof(char));

                        _itoa(i + 1, month, 10);
                        _itoa(j + 1, day, 10);

                        strcat(date_temp, month);
                        strcat(date_temp, ".");
                        strcat(date_temp, day);

                        strcpy(schedule_temp, p->data->schedule);
                        strcpy(category_temp, p->data->category);

                        item = merge_data(date_temp, schedule_temp, category_temp);

                        fprintf(pfile, "%s,", item->date);
                        fprintf(pfile, "%s,", item->schedule);
                        fprintf(pfile, "%s", item->category);

                        free(item->date);
                        free(item->schedule);
                        free(item->category);
                        free(item);
                        free(schedule_temp);
                        free(category_temp);
                    }
                }
            }
        }
    }
    fclose(pfile);
}

void file_Init_Category()   // ī�װ� ���� �ʱ�ȭ
{
    strcpy(categoryList[fixedData++], "������\n");
    strcpy(categoryList[fixedData++], "��������\n");
    strcpy(categoryList[fixedData++], "�Ϲ�\n");
    strcpy(categoryList[fixedData++], "�����\n");

    FILE* pfile = fopen("CSV//categoryList.csv", "r");   // ���� open
    char str_tmp[1024];
    int i = 0;
    if (pfile != NULL)   // ������ �����Ѵٸ�
    {
        while (fgets(str_tmp, 1024, pfile) != NULL) {   // ������ ������ // ���Ͽ��� ������ �о�ͼ� ���ڿ��� ��ȯ   
            str_tmp[strcspn(str_tmp, "\n")] = '\0';   // "\n" -> "\0"
            strcpy(categoryList[categoryIndex++], str_tmp);
        }
        fclose(pfile);
    }
}

void file_Update_category()   // ī�װ� ���� ������Ʈ
{
    FILE* pfile = fopen("CSV//categoryList.csv", "w");   // ���� open

    if (pfile != NULL)   // ������ �����Ѵٸ�
        for (int i = 0; i < categoryIndex; i++)
        {
            fputs(categoryList[i], pfile);
            fputs("\n", pfile);
        }

    fclose(pfile);
}

int category_Check(char* category)   // ī�װ� ���� ����
{
    int tmp = 0;
    for (int i = 0; i < categoryIndex; i++)
    {
        if (strcmp(categoryList[i], category) == 0)
            return tmp = 1;
    }
    return tmp;
}

void show_Category()   // ī�װ� ����Ʈ ���
{
    int cnt = 1;
    printf("------- < ī�װ� ��� > -------\n");
    for (int i = fixedData; i < categoryIndex; i++)
    {
        printf("%d. %s\n", cnt++, categoryList[i]);
    }
    printf("---------------------------------\n");
}

void add_Category()   // ī�װ� �߰�
{
    char category[MAX_CATEGORY_NAME_SIZE];
    show_Category();
    printf("�߰��Ͻ� ī�װ� �̸��� �Է��ϼ��� : ");
    scanf(" %[^\n]s", category);
    printf("\n\"%s\" �߰� �Ϸ�\n\n", category);
    strcpy(categoryList[categoryIndex++], category);

    char key;

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void delete_Category()   // ī�װ� ����
{
    if (fixedData == categoryIndex)
    {
        printf("������ ī�װ��� �����ϴ�.\n");

        char key;

        printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
        while (1) {
            key = getchar();
            if (key == 'q') {
                return;
            }
        }
    }

    char category[MAX_CATEGORY_NAME_SIZE];
    show_Category();
    while (1) {
        printf("�����Ͻ� ī�װ� �̸��� �Է��ϼ��� : ");
        scanf(" %[^\n]s", category);

        if (category_Check(category) == 1)
        {
            strcat(category, "\n");
            for (int i = 0; i < 12; i++) {
                for (int j = 0; j < 31; j++) {
                    ScheduleType* head = schedules[i][j];

                    if (is_empty(i, j) == 0) {
                        head = NULL;
                    }

                    sort_schedule(&head);

                    if (head != NULL) {
                        for (ScheduleType* p = head; p != NULL; p = p->link) {
                            if (strcmp(schedules[i][j]->data->category, category) == 0)
                                strcpy(schedules[i][j]->data->category, "�Ϲ�\n");
                        }
                    }
                }
            }
            break;
        }
        else {
            printf("���� ī�װ��Դϴ�.\n");
        }
    }

    int index = 0;

    for (int i = fixedData; i < categoryIndex; i++)
    {
        strcat(categoryList[i], "\n");
        if (strcmp(categoryList[i], category) == 0)
        {
            index = i;
            break;
        }
    }
    for (int i = index; i < categoryIndex - 1; i++)
    {
        strcpy(categoryList[i], categoryList[i + 1]);
    }
    categoryIndex--;

    char key;

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void print_schedule(int month, int day, const char* category) {      // ī�װ��� ���� ����
    char colorlist[MAX_CATEGORY_SIZE] = { 0, 0, 0, 0, Green, SkyBlue, Purple,Yellow,Blue,DarkGreen,Red,DarkPurple, DarkGray };
    // ���� ī�װ��� ���� ����
    if (strcmp(category, "������\n") == 0 || strcmp(category, "��������\n") == 0) {
        textColor(DarkRed);
    }
    else if (strcmp(category, "�Ϲ�\n") == 0) {
        textColor(DarkBlue);
    }
    else if (strcmp(category, "�����\n") == 0) {
        textColor(DarkYellow);
    }
    else {
        // ������ ī�װ����� ������� �ٸ� �������� ����
        int i;
        for (i = fixedData; i < MAX_CATEGORY_SIZE; i++) {
            if (strncmp(category, categoryList[i], strlen(categoryList[i])) == 0) {
                textColor(colorlist[i]); // ������ i�� ��ġ�ϴ� ������ ����
                break;
            }
        }
        if (i >= MAX_CATEGORY_SIZE) {
            textColor(White);  // �⺻ ����
        }
    }

    // ���õ� ������ �̿��� ��� �ڵ�
    printf("�� ");
    textColor(White);   // ���� �ʱ�ȭ
}

void show_category_calendar()   // ī�װ��� ���� ����
{
    int sel;
    int cnt = 1;

    // ��� ī�װ� ��� ���
    printf("[ ī�װ��� ���� ���� ]\n\n");
    printf("------- < ī�װ� ��� > -------\n");
    for (int i = 0; i < categoryIndex; i++)
    {
        printf("%d. %s\n", cnt++, categoryList[i]);
    }
    printf("---------------------------------\n");

    do {
        printf(">> ");
        scanf("%d", &sel);

        if (sel < 1 || sel > categoryIndex) {
            printf("��ȿ�� ī�װ��� �����ϼ���.\n");
        }
    } while (sel < 1 || sel > categoryIndex);

    system("cls");

    printf("------- < %s ��� > -------\n", categoryList[sel - 1]);
    int found = 0;  // ������ �ִ��� Ȯ���ϱ� ����

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 31; j++) {
            ScheduleType* head;

            if (is_empty(i, j) == 0) {
                head = NULL;
            }
            else {
                head = schedules[i][j];
            }

            sort_schedule(&head);

            ScheduleType* p = head;
            while (p != NULL) {
                if (strncmp(categoryList[sel - 1], p->data->category, strlen(categoryList[sel - 1])) == 0) {
                    print_schedule(i, j, p->data->category);
                    if (categoryList[3] == categoryList[sel - 1]) {     // ������� ���̿� �Բ� ���
                        for (int k = 0; k < MAX_ANNI_SIZE; k++) {
                            if (anni[k] != NULL) {
                                printf("%s %s %s\n", p->data->date, anni[k]->totalDay, anni[k]->data->schedule);
                            }
                        }
                    }
                    else
                        printf("%s %s\n", p->data->date, p->data->schedule);
                    found = 1;
                }
                p = p->link;
            }
        }
    }

    if (!found) {
        printf("\n������ ī�װ��� ���� ������ �����ϴ�.\n\n");
    }

    printf("--------------------------------\n\n");

    char key;

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}


void category_menu() {
    int sel;

    while (1)
    {
        printf("------ < ī�װ� ���� > ------\n");
        printf(" 1. ī�װ� �߰�\n");
        printf(" 2. ī�װ� ����\n");
        printf(" 3. ī�װ� ��� ��ȸ\n");
        printf(" 0. ���ư���\n");
        printf("-------------------------------\n");
        printf(">> ");
        scanf("%d", &sel);

        switch (sel)
        {
        case 1:
            system("cls");
            add_Category();
            system("cls");
            return;
            break;
        case 2:
            system("cls");
            delete_Category();
            system("cls");
            return;
            break;
        case 3:
            system("cls");
            show_Category();

            char key;

            printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
            while (1) {
                key = getchar();
                if (key == 'q') {
                    break;
                }
            }
            system("cls");
            return;
            break;
        case 0:
            system("cls");
            return;
            break;
        default:
            printf("���� �޴��� �����ϼ̽��ϴ�. �ٽ� ������ �ּ���.\n\n");
            break;
        }
    }
}