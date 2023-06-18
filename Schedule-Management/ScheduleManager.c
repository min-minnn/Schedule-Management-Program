#define _CRT_SECURE_NO_WARNINGS
#define MAX_ANNI_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "makeScheduleFunc.h"

anniversary* anni[MAX_ANNI_SIZE];
//����Ϻ� ���� ��¥ �������� ��ĥ ���Ҵ��� �����ϴ� �迭
ScheduleType* schedules[12][31]; 
//��ü ������ �����ϴ� �迭
int lastDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 }; 
//�޺� ������ ��

typedef struct ScheduleNode { //���� Ž�� Ʈ��
    ScheduleType* data;
    struct ScheduleNode* left;
    struct ScheduleNode* right;
} ScheduleNode;

ScheduleType* insert_node(ScheduleType* head, element* item) { 
    //���� ���� ����Ʈ ���·� �����ؼ� �ش� ��¥�� ���� ��带 �߰�
    ScheduleType* p = (ScheduleType*)malloc(sizeof(ScheduleType));
    p->data = item;
    p->link = head;
    head = p;

    return head;
}

int correct_str(char* date) { //�Է� ���� ���ڿ��� ��¥�� �´��� �Ǻ��ϴ� �Լ�
    if (strlen(date) != 5) {
        //���ڿ� ���̰� 5����
        return 0;
    }

    char temp[6];
    strcpy(temp, date); //�Է¹��� ���� ����
    char* ptr = strtok(temp, "."); //.�� �������� ���ڿ��� ����
    int month = atoi(ptr);
    ptr = strtok(NULL, ".");
    int day = atoi(ptr);

    if (month < 1 || month > 12) {
        //month�� 1-12�� �����ϴ���
        return 0;
    }

    if (day < 1 || day > lastDay[month - 1]) {
        //month�� ������ �� �ȿ� �����ϴ���
        return 0;
    }

    return 1; //��� ���� ���ǿ� �������� ������ 1 ��ȯ
}

int is_empty(int month, int day) { //�ش� ��¥�� �迭�� ���� ����Ʈ ������
    if (schedules[month][day] == NULL) { //������ 0 ��ȯ
        return 0;
    }
    return 1; //������ 1 ��ȯ
}

void add_schedule() { //���� ���
    char key; //���� ���� �Է� �޴� ����
    char str[6] = ""; 
    char date_temp[6] = "";
    char month_temp[3];
    char day_temp[3];
    element* item = (element*)malloc(sizeof(element)); //���� �Ҵ�
    ScheduleType* head; //���� ó�� ��� ����Ű�� head ���

    item->date = (char*)malloc(6 * sizeof(char)); //���� �Ҵ�
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    printf("[ ���� ��� ]\n");

    while (1) {
        printf("��¥[ex) 01.01]: ");
        scanf("%s", item->date); //��¥ �Է� ����

        if (correct_str(item->date)) {
            break; //���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // ���ڿ��� .�� �����ؼ� ������ 
    int month = atoi(ptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
    ptr = strtok(NULL, "."); 
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //�ش� ��¥�� ������ �������� ������
        head = NULL;
    }
    else { //�ش� ��¥�� ������ �����ϸ�
        head = schedules[month][day];
    }

    printf("����: ");
    scanf(" %[^\n]", item->schedule); //������ ������ ���ڿ� ����
    printf("\n");

    while (1) {
        show_Category(); //ī�װ� ��� �����ִ� �Լ�
        printf("ī�װ�: ");
        scanf(" %[^\n]", item->category); //ī�װ� �Է� ����
        if (category_Check(item->category) == 1) { //ī�װ� �����ϸ� 1 ��ȯ
            strcat(item->category, "\n"); //�Է¹��� ī�װ��� \n ���� �߰��ؼ� ��������
            break;
        }
        else {
            printf("���� ī�װ��Դϴ�. �ٽ� �Է��� �ּ���.\n");
        }
    }

    _itoa(month + 1, month_temp, 10);   // ������ ���ڷ�
    _itoa(day + 1, day_temp, 10);

    strcat(str, month_temp);
    strcat(str, ".");
    strcat(str, day_temp); //��¥ �������� ��ġ��
    strcpy(item->date, str);

    head = insert_node(head, item); //��带 insert

    schedules[month][day] = head; //���� �����ϴ� �迭�� �ش� ��¥ ���� ���� ��������

    printf("\n");
    printf("\"%s\" ������ ��ϵǾ����ϴ�.\n", item->schedule);
    printf("\n");

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

int day_of_week(int month, int day) { //���� ��ȯ�� �ִ� �Լ�
    int result;
    int year = 2023;
    int mm = month + 1;
    int dd = day + 1;

    if (mm < 3) {
        mm += 12;
        year -= 1;
    }

    int k = year % 100;
    int j = year / 100;

    int h = (dd + 13 * (mm + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

    result = (h + 6) % 7;

    switch (result + 1)
    {
    case 1: //�Ͽ���
        return 1;
        break;
    case 2: //������
        return 2;
        break;
    case 3: //ȭ����
        return 3;
        break;
    case 4: //������
        return 4;
        break;
    case 5: //�����
        return 5;
        break;
    case 6: //�ݿ���
        return 6;
        break;
    case 7: //�����
        return 7;
        break;
    default:
        break;
    }
}

ScheduleType* merge_data(char* date, char* schedule, char* category) { 
    //�ݺ����� ��Ͽ��� �Է¹��� ������ ��¥, ���� ����, ī�װ��� �ϳ��� �����༭ ��ȯ����
    element* item = (element*)malloc(sizeof(element));

    item->date = (char*)malloc(6 * sizeof(char));
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    strcpy(item->date, date);
    strcpy(item->schedule, schedule);
    strcpy(item->category, category);

    return item;
}

void add_recurring_schedule() { //�Է��� �Ⱓ �ȿ� ������ �����ؼ� �ݺ� ������ ����ϰ� �ϴ� �Լ�
    char key; //���� ���� �ޱ� ���� ����
    int startMonth = 0, endMonth = 0, startDay = 0, endDay = 0, totalMonth = 0; //�Ⱓ�� �Է� ���� ����
    int selecWeek, dayOfTheWeek = 0; //�ݺ��� ������ ���ϰ� 
    char* start = (char*)malloc(6 * sizeof(char)); //���� ���� ����
    char* end = (char*)malloc(6 * sizeof(char)); //������ ���� ����
    char month[6]; //�� ����
    char day[6]; //�� ����
    char* schedule = (char*)malloc(1024 * sizeof(char));
    char* category = (char*)malloc(20 * sizeof(char));
    ScheduleType* head;

    printf("[ �ݺ� ���� ��� ]\n");

    while (1) {
        printf("���� ��¥[ex) 01.01]: ");
        scanf("%s", start); //��¥ �Է� ����

        if (correct_str(start)) {
            break; //���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    while (1) {
        printf("���� ��¥[ex) 01.01]: ");
        scanf("%s", end); //��¥ �Է� ����

        if (correct_str(end)) {
            break; //���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    printf("����: ");
    scanf(" %[^\n]", schedule); //������ ������ ���ڿ� ����
    printf("\n");

    while (1) {
        show_Category();
        printf("ī�װ�: ");
        scanf(" %[^\n]", category);
        if (category_Check(category) == 1) {
            strcat(category, "\n");
            break;
        }
        else {
            printf("���� ī�װ��Դϴ�. �ٽ� �Է��� �ּ���.\n");
        }
    }
    printf("\n");

    printf("----- < ���� >-----\n");
    printf(" 1. �Ͽ���\n");
    printf(" 2. ������\n");
    printf(" 3. ȭ����\n");
    printf(" 4. ������\n");
    printf(" 5. �����\n");
    printf(" 6. �ݿ���\n");
    printf(" 7. �����\n");
    printf("------------------\n");
    printf("�ݺ��� ������ �����ϼ���.: ");
    scanf("%d", &selecWeek);

    char* ptr = strtok(start, ".");   // ���ڿ��� .�� �����ؼ� ������ 
    startMonth = atoi(ptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
    ptr = strtok(NULL, ".");
    startDay = atoi(ptr) - 1;

    char* pptr = strtok(end, ".");   // ���ڿ��� .�� �����ؼ� ������ 
    endMonth = atoi(pptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
    pptr = strtok(NULL, ".");
    endDay = atoi(pptr) - 1;

    totalMonth = endMonth - startMonth;

    if (totalMonth == 0) { //���� ���� �ݺ��Ǵ� ������ ����ȴٸ�
        for (int i = startDay; i < endDay; i++) {
            element* item = (element*)malloc(sizeof(element));
            item->date = (char*)malloc(6 * sizeof(char));
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(20 * sizeof(char));

            dayOfTheWeek = day_of_week(startMonth, i);

            if (selecWeek == dayOfTheWeek) {
                char str[6] = "";

                _itoa(startMonth + 1, month, 10);   // ������ ���ڷ�
                _itoa(i + 1, day, 10);

                strcat(str, month);
                strcat(str, ".");
                strcat(str, day); //��¥ �������� ��ġ��

                item = merge_data(str, schedule, category);

                if (is_empty(startMonth, i) == 0) { //�ش� ��¥�� ������ �����ϴ���
                    head = NULL; //�������� ������ head = NULL;
                    head = insert_node(head, item);
                    schedules[startMonth][i] = head;
                }
                else {
                    head = schedules[startMonth][i]; //������ ��
                    head = insert_node(head, item);
                    schedules[startMonth][i] = head;
                }
            }
        }
    }
    else if (totalMonth > 0) { //�ݺ� ������ �Ⱓ�� ���� �ٲ�ٸ�
        int j = startDay;
        for (int i = startMonth; i <= endMonth; i++) { //�� �ٲ�� �� ����
            element* item = (element*)malloc(sizeof(element));
            item->date = (char*)malloc(6 * sizeof(char));
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(20 * sizeof(char));

            while (((j <= lastDay[i] && (i < endMonth && j <= lastDay[i])) || (i == endMonth && j <= endDay))) { 
                //i�� ���� �� ���̸� j�� �ش� �� �������� ���� �ʰ�, ���� �ް� �������� �Է¹��� ���� �ϱ����� ������
                dayOfTheWeek = day_of_week(i, j);

                if (selecWeek == dayOfTheWeek) {
                    char str[6] = "";

                    _itoa(i + 1, month, 10);   // ������ ���ڷ�
                    _itoa(j + 1, day, 10);

                    strcat(str, month);
                    strcat(str, ".");
                    strcat(str, day); //��¥ �������� ��ġ��

                    item = merge_data(str, schedule, category); //���� ��¥, ����, ī�װ� ���ļ� ��ȯ ����

                    if (is_empty(i, j) == 0) {
                        head = NULL;
                        head = insert_node(head, item);
                        schedules[i][j] = head;
                    }
                    else {
                        head = schedules[i][j];
                        head = insert_node(head, item);
                        schedules[i][j] = head;
                    }
                }
                j++;
            }
            j = 0;
        }
    }

    printf("\n");
    printf("\"%s\" ������ ��ϵǾ����ϴ�.\n", schedule);
    printf("\n");

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void add_anniversary() { // ����� ����ϴ� �Լ�
    char key;
    char str[6] = "";
    char date_temp[6] = "";
    char month_temp[3];
    char day_temp[3];
    element* item = (element*)malloc(sizeof(element)); //���� �Ҵ�
    ScheduleType* head; //���� ó�� ��� ����Ű�� head ���

    item->date = (char*)malloc(6 * sizeof(char)); //���� �Ҵ�
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    printf("[ ����� ��� ]\n");

    while (1) {
        printf("����� ��¥[ex) 01.01]: ");
        scanf("%s", item->date); //��¥ �Է� ����

        if (correct_str(item->date)) {
            break; //���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // ���ڿ��� .�� �����ؼ� ������ 
    int month = atoi(ptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //�ش� ��¥�� ������ �������� ������
        head = NULL;
    }
    else { //�ش� ��¥�� ������ �����ϸ�
        head = schedules[month][day];
    }

    printf("����: ");
    scanf(" %[^\n]", item->schedule); //������ ������ ���ڿ� ����

    strcpy(item->category, "�����\n");

    _itoa(month + 1, month_temp, 10);   // ������ ���ڷ�
    _itoa(day + 1, day_temp, 10);

    strcat(str, month_temp);
    strcat(str, ".");
    strcat(str, day_temp); //��¥ �������� ��ġ��
    strcpy(item->date, str);

    head = insert_node(head, item); //��带 insert

    schedules[month][day] = head; //���� �����ϴ� �迭�� �ش� ��¥ ���� ���� ��������

    printf("\n");
    printf("\"%s\" ������� ��ϵǾ����ϴ�.\n", item->schedule);
    printf("\n");

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

int total_days(int year, int month, int day) { //�� �ϼ� �����ִ� �Լ�
    int i;
    long total = 0L;

    total = (year - 1) * 365L + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;

    if (!(year % 4) && year % 100 || !(year % 400))
        lastDay[1]++;
    for (i = 0; i < month - 1; i++)
        total += lastDay[i];
    total += day;

    return total;
}

void anniversary_cal() {
    time_t timer = time(NULL);
    struct tm* t;
    t = localtime(&timer);

    int anniIndex = 0; // anni �迭�� �ε��� ���� �߰�

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 31; j++) {
            ScheduleType* head;

            if (is_empty(i, j) == 0) {
                head = NULL;
            }
            else {
                head = schedules[i][j];
            }

            if (head != NULL) {
                for (ScheduleType* p = head; p != NULL; p = p->link) {
                    if (strcmp(p->data->category, "�����\n") == 0) {
                        // anni �迭�� �Ҵ� �� �� ä���
                        anni[anniIndex] = (anniversary*)malloc(sizeof(anniversary));
                        anni[anniIndex]->data = (element*)malloc(sizeof(element));
                        anni[anniIndex]->data->date = (char*)malloc(6 * sizeof(char));
                        anni[anniIndex]->data->schedule = (char*)malloc(1024 * sizeof(char));
                        anni[anniIndex]->data->category = (char*)malloc(20 * sizeof(char));

                        char str[6] = "";
                        int totalDay = 0;
                        char str_totalDay[10] = "";
                        char date_temp[1024] = "";

                        strcpy(str, p->data->date);
                        char* ptr = strtok(str, ".");
                        int month = atoi(ptr);
                        ptr = strtok(NULL, ".");
                        int day = atoi(ptr);

                        if (t->tm_mon + 1 > month || (t->tm_mon + 1 == month && t->tm_mday > day)) {
                            //������� ���� ��¥�κ��� ���� �ִ� ������̶��
                            totalDay = total_days(2023, t->tm_mon + 1, t->tm_mday) - total_days(2023, month, day);
                            //��ĥ �������� ���
                            strcat(date_temp, "D + ");
                            _itoa(totalDay, str_totalDay, 10);
                            strcat(date_temp, str_totalDay);

                            strcpy(anni[anniIndex]->totalDay, date_temp);
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }
                        else if (t->tm_mon + 1 == month && t->tm_mday == day) { //����� ��¥�� �����ϰ� ������
                            strcpy(anni[anniIndex]->totalDay, "D - Day");
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }
                        else { //����� ��¥�� ���� ��¥�κ��� �Ŀ� �ִ� ������̶��
                            totalDay = total_days(2023, month, day) - total_days(2023, t->tm_mon + 1, t->tm_mday);
                            strcat(date_temp, "D - ");
                            _itoa(totalDay, str_totalDay, 10);
                            strcat(date_temp, str_totalDay);

                            strcpy(anni[anniIndex]->totalDay, date_temp);
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }

                        anniIndex++; // �ε��� ����
                    }
                }
            }
        }
    }
}

void add_schedule_menu() { //���� ��� �޴� ����ִ� �Լ�
    int sel;

    while (1) {
        printf("------< ���� ��� > --------\n");
        printf(" 1. ���� ���\n");
        printf(" 2. �ݺ� ���� ���\n");
        printf(" 3. ����� ���\n");
        printf(" 0. ���ư���\n");
        printf("-----------------------------\n");
        printf(">> ");
        scanf("%d", &sel);

        switch (sel)
        {
        case 1:
            system("cls");
            add_schedule();
            system("cls");
            return;
            break;
        case 2:
            system("cls");
            add_recurring_schedule();
            system("cls");
            return;
            break;
        case 3:
            system("cls");
            add_anniversary();
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

ScheduleNode* createScheduleNode(ScheduleType* data) {  // ������ ����Ž��Ʈ�� ���·� �������
    ScheduleNode* newNode = (ScheduleNode*)malloc(sizeof(ScheduleNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertScheduleNode(ScheduleNode** root, ScheduleType* data) { // ������ ���� Ž�� Ʈ���� ����
    if (*root == NULL) {
        *root = createScheduleNode(data);
        return;
    }

    int compare = strcmp(data->data->schedule, (*root)->data->data->schedule); //�������� ���ؼ� ��ȯ�� ����

    if (compare < 0) {
        insertScheduleNode(&(*root)->left, data);
    }
    else { 
        insertScheduleNode(&(*root)->right, data);
    }
}

void inorderTraversal(ScheduleNode* root, ScheduleType** head) { 
    //���� Ž�� Ʈ���� ���� ��ȸ�Ͽ� ������������ ���ĵ� ������ ���� ����Ʈ ����
    if (root == NULL) { //������ ������ ����
        return;
    }

    inorderTraversal(root->left, head);

    ScheduleType* data = root->data;

    if (*head == NULL) {
        *head = data;
        data->link = NULL;
    }
    else {
        ScheduleType* temp = *head;

        while (temp->link != NULL) {
            temp = temp->link;
        }

        temp->link = data;
        data->link = NULL;
    }

    inorderTraversal(root->right, head);
}

void sort_schedule(ScheduleType** head) { //�־��� ���� ����Ʈ ���� Ž�� Ʈ�� ����Ͽ� ����
    if (*head == NULL || (*head)->link == NULL) {
        return;
    }

    ScheduleNode* root = NULL;
    ScheduleNode* anni_root = NULL;
    ScheduleType* current = *head;

    while (current != NULL) {
        ScheduleType* next = current->link;
        if (strcmp(current->data->category, "�����\n") != 0) { //ī�װ��� '�����'�� �ش����� �ʴ� ���鳢�� ����
            insertScheduleNode(&root, current);
            current = next;
        }
        else { //ī�װ��� '�����'�� �ش��ϴ� ���鳢�� ����
            insertScheduleNode(&anni_root, current);
            current = next;
        }
    }

    *head = NULL; //������ �����͵� �־��ֱ� ���� head�� ����ֱ�
    inorderTraversal(anni_root, head); //'�����'�� �ش��ϴ� ������ ù ��尡 �ǵ���
    inorderTraversal(root, head);
}

void print_date() {
    char key;

    for (int k = 0; k < MAX_ANNI_SIZE; k++) { //����� ���
        if (anni[k] != NULL) {
            printf("%s, %s, %s", anni[k]->totalDay, anni[k]->data->schedule, anni[k]->data->category);
        }
    }

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
                    // ������� �ƴ� ��� ���
                    if (strcmp(p->data->category, "�����\n") != 0) {
                        printf("%d, %d, %s, %s, %s", i, j, p->data->date, p->data->schedule, p->data->category);
                    }
                }
            }
        }
    }

    printf("\n");
    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");

    while (1) {
        key = getchar();

        if (key == 'q') {
            system("cls");
            return;
        }
    }
}

void modify_Schedule()
{
    char key;
    char date_temp[6] = "";
    char* delete_schedule;
    char* change_schedule;

    element* item = (element*)malloc(sizeof(element)); //���� �Ҵ�
    ScheduleType* head; //���� ó�� ��� ����Ű�� head ���1

    item->date = (char*)malloc(6 * sizeof(char)); //���� �Ҵ�
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));
    change_schedule = (char*)malloc(1024 * sizeof(char));
    delete_schedule = (char*)malloc(1024 * sizeof(char));

    while (1) {

        printf("������ ��¥[ex) 01.01]: ");
        scanf("%s", item->date);

        if (correct_str(item->date)) {
            break; //���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // ���ڿ��� .�� �����ؼ� ������ 
    int month = atoi(ptr) - 1;   //atoi �Լ� ����ؼ� ���ڿ��� ������ ��ȯ
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //�ش� ��¥�� ������ �������� ������
        head = NULL;
    }
    else { //�ش� ��¥�� ������ �����ϸ�
        head = schedules[month][day];
    }

    if (head != NULL) {
        int ck = 0;
        for (ScheduleType* p = head; p != NULL; p = p->link) {
            if (strcmp(p->data->category, "������\n") != 0 &&
                strcmp(p->data->category, "��������\n") != 0 &&
                strcmp(p->data->category, "�����\n") != 0)
                printf(" %s %s %s", p->data->date, p->data->schedule, p->data->category);
        }
        while (ck != 1)
        {
            printf("�ٲٰ� ���� ������ �Է� : ");
            while (getchar() != '\n');
            gets(delete_schedule);
            printf("\n");

            for (ScheduleType* p = head; p != NULL; p = p->link) {
                if (strcmp(p->data->schedule, delete_schedule) == 0)
                    ck = 1;
            }
            if (!ck)
            {
                printf("�ٽ� �Է�.\n");
            }

        }

        printf("�ٲ�� �� ������ �Է� : ");
        gets(change_schedule);

        for (ScheduleType* p = head; p != NULL; p = p->link) {
            if (strcmp(p->data->schedule, delete_schedule) == 0)
                strcpy(p->data->schedule, change_schedule);
        }
    }

    printf("\n");

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            system("cls");
            return;
        }
    }
}

void delete_Schedule()
{
    ScheduleType* head = NULL;
    element* item = (element*)malloc(sizeof(element));
    char date_temp[6] = "";
    char key;

    item->date = (char*)malloc(6 * sizeof(char));
    item->schedule = (char*)malloc(1024 * sizeof(char));

    // item->date �ʱ�ȭ
    strcpy(item->date, "");

    printf("[ ���� ���� ]\n");

    // ��¥ �Է� �ޱ�
    while (1) {
        printf("������ ��¥[ex) 01.01]: ");
        scanf("%s", date_temp);

        if (correct_str(date_temp)) {
            break; // ���ڿ� ������ ������ break;
        }
        else {
            printf("�ùٸ� ��¥ ������ �ƴմϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    strcpy(item->date, date_temp);
    char* ptr = strtok(date_temp, ".");
    int month = atoi(ptr) - 1;
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    // ������ ���ų� ī�װ��� "������"�̰ų� "��������"�� ���
    if (is_empty(month, day) == 0 || (schedules[month][day] != NULL && (strcmp(schedules[month][day]->data->category, "������\n") == 0 || strcmp(schedules[month][day]->data->category, "��������\n") == 0)))
    {
        printf("�ش� ��¥���� ���� ������ ������ �����ϴ�.\n");
        free(item->date);
        free(item->schedule);
        free(item);

        printf("\n�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
        while (1)
        {
            key = getchar();
            if (key == 'q')
            {
                return;
            }
        }
    }
    else {
        head = schedules[month][day];
    }

    ScheduleType* current = head;
    ScheduleType* prev = NULL;

    int count = 0;

    // ���� ������ ���� ��� ���
    printf("\n----- ���� ������ ���� ��� -----\n");
    while (current != NULL) {
        if (strcmp(current->data->category, "������\n") != 0 && strcmp(current->data->category, "��������\n") != 0) {
            printf("%d. %s\n", count + 1, current->data->schedule);
            count++;
        }
        current = current->link;
    }
    printf("---------------------------------\n");

    int choice;

    // ������ ���� ����
    while (1) {
        printf("\n������ ���� ��ȣ�� �Է��ϼ���: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > count) {
            printf("��ȿ�� ��ȣ�� �Է����ּ���.\n");
        }
        else {
            break;
        }
    }

    current = head;
    prev = NULL;
    count = 0;

    // ������ ���� ��ġ�� �̵�
    while (current != NULL) {
        if (strcmp(current->data->category, "������\n") != 0 && strcmp(current->data->category, "��������\n") != 0) {
            count++;
        }

        if (count == choice) {
            break;
        }

        prev = current;
        current = current->link;
    }

    // ���� ����
    if (current == NULL) {
        printf("������ �������� �ʽ��ϴ�.\n");
        free(item->date);
        free(item->schedule);
        free(item);

        printf("\n�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
        while (1)
        {
            key = getchar();
            if (key == 'q')
            {
                return;
            }
        }
    }

    // ������ ������ ù ��° ����� ���
    if (prev == NULL)
    {
        head = current->link;
    }
    // ������ ������ ù ��° ��尡 �ƴ� ���
    else
    {
        prev->link = current->link;
    }

    schedules[month][day] = head;

    printf("\n");
    printf("[%s] ������ �����Ǿ����ϴ�.\n", current->data->schedule);
    printf("\n");

    // �޸� ����
    free(current->data->date);
    free(current->data->schedule);
    free(current->data);
    free(current);
    free(item->date);
    free(item->schedule);
    free(item);

    printf("�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1)
    {
        key = getchar();
        if (key == 'q')
        {
            return;
        }
    }
}

void search_Schedule()
{
    char key;
    element* item = (element*)malloc(sizeof(element));
    item->schedule = (char*)malloc(1024 * sizeof(char));

    printf("[ ���� �˻� ]\n");

    // ���� ���� �Է� �ޱ�
    printf("���� ����: ");
    scanf(" %[^\n]s", item->schedule);
    printf("\n");

    // �˻��� ������ �ش��ϴ� ��¥ ���
    int found = 0; // ������ ã�Ҵ��� ���θ� ��Ÿ���� ����

    printf("�˻��� ����:\n");

    for (int month = 0; month < 12; month++) {
        for (int day = 0; day < lastDay[month]; day++) {
            ScheduleType* current = schedules[month][day];
            while (current != NULL) {
                if (strcmp(current->data->schedule, item->schedule) == 0) {
                    print_schedule(month + 1, day + 1, current->data->category);
                    printf(" %d�� %d�� %s\n", month + 1, day + 1, item->schedule);
                    found = 1;
                    break;
                }
                current = current->link;
            }
        }
    }

    if (!found) {
        printf("�˻��� ������ ã�� �� �����ϴ�.\n");
    }

    printf("\n�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void TodaySchedule() {  // ���� ���� ����ִ� �Լ�
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon;
    int day = tm.tm_mday - 1;

    printf("\n-------------------------\n");
    printf("%d-%d-%d\n\n", year, tm.tm_mon + 1, tm.tm_mday);

    // ����� ���
    for (int k = 0; k < MAX_ANNI_SIZE; k++) {
        if (anni[k] != NULL) {
            textColor(6);
            printf("�� ");
            textColor(7);
            printf("%s %s %s\n\n", anni[k]->data->date, anni[k]->totalDay, anni[k]->data->schedule);
        }
    }

    // ���� ���� ���
    if (is_empty(month, day) == 0) {
        printf("���� ������ �����ϴ�.\n");
    }
    else {
        ScheduleType* curr = schedules[month][day];
        while (curr != NULL) {
            print_schedule(month, day, curr->data->category);
            printf("%s\n", curr->data->schedule);
            curr = curr->link;
        }
    }
    printf("\n-------------------------\n\n");
}

void print_calendar(int month)    // �޷� ���
{
    int daysInMonth, startDay, endDay, i, j, k, l, day, count;

    // ������ �� ���� ���� ���� ���
    if (month == 2)
        daysInMonth = 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        daysInMonth = 30;
    else
        daysInMonth = 31;

    // ��¥ �� �ð� ��� ����
    struct tm firstDay, lastDay;
    firstDay.tm_year = 2023 - 1900;
    firstDay.tm_mon = month - 1;
    firstDay.tm_mday = 1;
    firstDay.tm_hour = 12;
    firstDay.tm_min = 0;
    firstDay.tm_sec = 0;
    mktime(&firstDay);

    lastDay.tm_year = firstDay.tm_year;
    lastDay.tm_mon = firstDay.tm_mon;
    lastDay.tm_mday = daysInMonth;
    lastDay.tm_hour = 12;
    lastDay.tm_min = 0;
    lastDay.tm_sec = 0;
    mktime(&lastDay);

    startDay = firstDay.tm_wday;
    endDay = lastDay.tm_wday;

    // �޷� ���
    printf("\t     2023�� %d��\n", month);
    printf("  ��   ��   ȭ   ��   ��   ��   ��  \n");
    printf("������������������������������������������������������������������������\n");
    printf("��");

    for (i = 0; i < startDay; i++)
        printf("    ��");    // ��¥���� �� ĭ
    for (day = 1, j = i; day <= daysInMonth; day++, j++) {
        if (day == 1) {     // 1��
            printf("%2d", day);
        }
        else                // ������ �� ��
            printf("��%2d", day);

        if (j % 7 == 6) {   // ������̸� ���� '��'�� �ݾ��ְ� ĭ ����
            printf("  ��");
            if (day == 30)
                break;
            printf("\n������������������������������������������������������������������������\n");
        }
        else
            printf("  ");
    }

    if (endDay % 7 != 6) {
        while (endDay % 7 != 6) {
            printf("��    ");
            endDay++;
        }
        printf("��");
    }
    printf("\n������������������������������������������������������������������������\n\n");
}

void month_Schedule()   // ���� ����
{
    char key;
    int month = 0;

    printf("[ ���� ���� ]\n");

    // Ȯ���� �� �Է�
    while (1) {
        printf("�� ��: ");
        scanf(" %d", &month);
        if (month < 1 || month>12) {
            printf("�ش��ϴ� ���� �����ϴ�. �ٽ� �Է����ּ���.\n");
        }
        else
            break;
    }
    system("cls");
    print_calendar(month);

    // ���ϴ� �� ���� ����
    for (int i = month - 1; i < month; i++) {
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
                    print_schedule(i, j, p->data->category);
                    printf("%s %s\n", p->data->date, p->data->schedule);
                }
            }
        }
    }

    printf("\n�ʱ� �޴��� ���ư����� 'q'�� �Է��ϼ���.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void show_schedule_menu() {
    int sel;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (1) {
        print_calendar(tm.tm_mon + 1);

        printf("------- < ���� ���� > -------\n");
        printf(" 1. ���� ����\n");
        printf(" 2. ī�װ��� ���� ����\n");
        printf(" 3. ���� �˻�\n");
        printf(" 0. ���ư���\n");
        printf("-----------------------------\n");
        printf(">> ");
        scanf("%d", &sel);

        switch (sel)
        {
        case 1:
            system("cls");
            // print_date();
            month_Schedule();
            system("cls");
            return;
            break;
        case 2:
            system("cls");
            show_category_calendar();
            system("cls");
            return;
            break;
        case 3:
            system("cls");
            search_Schedule();
            system("cls");
            return;
            break;
        case 0:
            system("cls");
            return;
            break;
        default:
            system("cls");
            printf("���� �޴��� �����ϼ̽��ϴ�. �ٽ� ������ �ּ���.\n\n");
            break;
        }
    }
}