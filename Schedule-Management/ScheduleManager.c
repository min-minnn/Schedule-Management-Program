#define _CRT_SECURE_NO_WARNINGS
#define MAX_ANNI_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "makeScheduleFunc.h"

anniversary* anni[MAX_ANNI_SIZE];
//기념일별 현재 날짜 기준으로 며칠 남았는지 저장하는 배열
ScheduleType* schedules[12][31]; 
//전체 일정을 저장하는 배열
int lastDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 }; 
//달별 마지막 일

typedef struct ScheduleNode { //이진 탐색 트리
    ScheduleType* data;
    struct ScheduleNode* left;
    struct ScheduleNode* right;
} ScheduleNode;

ScheduleType* insert_node(ScheduleType* head, element* item) { 
    //일정 연결 리스트 형태로 관리해서 해당 날짜에 일정 노드를 추가
    ScheduleType* p = (ScheduleType*)malloc(sizeof(ScheduleType));
    p->data = item;
    p->link = head;
    head = p;

    return head;
}

int correct_str(char* date) { //입력 받은 문자열이 날짜가 맞는지 판별하는 함수
    if (strlen(date) != 5) {
        //문자열 길이가 5인지
        return 0;
    }

    char temp[6];
    strcpy(temp, date); //입력받은 일정 복사
    char* ptr = strtok(temp, "."); //.을 기준으로 문자열을 끊음
    int month = atoi(ptr);
    ptr = strtok(NULL, ".");
    int day = atoi(ptr);

    if (month < 1 || month > 12) {
        //month가 1-12에 존재하는지
        return 0;
    }

    if (day < 1 || day > lastDay[month - 1]) {
        //month의 마지막 날 안에 존재하는지
        return 0;
    }

    return 1; //모든 오류 조건에 만족하지 않으면 1 반환
}

int is_empty(int month, int day) { //해당 날짜의 배열에 연결 리스트 없는지
    if (schedules[month][day] == NULL) { //없으면 0 반환
        return 0;
    }
    return 1; //있으면 1 반환
}

void add_schedule() { //일정 등록
    char key; //종료 문자 입력 받는 변수
    char str[6] = ""; 
    char date_temp[6] = "";
    char month_temp[3];
    char day_temp[3];
    element* item = (element*)malloc(sizeof(element)); //동적 할당
    ScheduleType* head; //가장 처음 노드 가르키는 head 노드

    item->date = (char*)malloc(6 * sizeof(char)); //동적 할당
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    printf("[ 일정 등록 ]\n");

    while (1) {
        printf("날짜[ex) 01.01]: ");
        scanf("%s", item->date); //날짜 입력 받음

        if (correct_str(item->date)) {
            break; //문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // 문자열을 .로 구분해서 나눠줌 
    int month = atoi(ptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
    ptr = strtok(NULL, "."); 
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //해당 날짜에 일정이 존재하지 않으면
        head = NULL;
    }
    else { //해당 날짜에 일정이 존재하면
        head = schedules[month][day];
    }

    printf("내용: ");
    scanf(" %[^\n]", item->schedule); //공백을 포함한 문자열 받음
    printf("\n");

    while (1) {
        show_Category(); //카테고리 목록 보여주는 함수
        printf("카테고리: ");
        scanf(" %[^\n]", item->category); //카테고리 입력 받음
        if (category_Check(item->category) == 1) { //카테고리 존재하면 1 반환
            strcat(item->category, "\n"); //입력받은 카테고리에 \n 문자 추가해서 저장해줌
            break;
        }
        else {
            printf("없는 카테고리입니다. 다시 입력해 주세요.\n");
        }
    }

    _itoa(month + 1, month_temp, 10);   // 정수를 문자로
    _itoa(day + 1, day_temp, 10);

    strcat(str, month_temp);
    strcat(str, ".");
    strcat(str, day_temp); //날짜 형식으로 합치기
    strcpy(item->date, str);

    head = insert_node(head, item); //노드를 insert

    schedules[month][day] = head; //일정 저장하는 배열에 해당 날짜 일정 전부 저장해줌

    printf("\n");
    printf("\"%s\" 일정이 등록되었습니다.\n", item->schedule);
    printf("\n");

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

int day_of_week(int month, int day) { //요일 반환해 주는 함수
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
    case 1: //일요일
        return 1;
        break;
    case 2: //월요일
        return 2;
        break;
    case 3: //화요일
        return 3;
        break;
    case 4: //수요일
        return 4;
        break;
    case 5: //목요일
        return 5;
        break;
    case 6: //금요일
        return 6;
        break;
    case 7: //토요일
        return 7;
        break;
    default:
        break;
    }
}

ScheduleType* merge_data(char* date, char* schedule, char* category) { 
    //반복일정 등록에서 입력받은 일정의 날짜, 일정 내용, 카테고리를 하나로 합쳐줘서 반환해줌
    element* item = (element*)malloc(sizeof(element));

    item->date = (char*)malloc(6 * sizeof(char));
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    strcpy(item->date, date);
    strcpy(item->schedule, schedule);
    strcpy(item->category, category);

    return item;
}

void add_recurring_schedule() { //입력한 기간 안에 요일을 선택해서 반복 일정을 등록하게 하는 함수
    char key; //종료 문자 받기 위한 변수
    int startMonth = 0, endMonth = 0, startDay = 0, endDay = 0, totalMonth = 0; //기간을 입력 받을 변수
    int selecWeek, dayOfTheWeek = 0; //반복을 선택한 요일과 
    char* start = (char*)malloc(6 * sizeof(char)); //시작 요일 저장
    char* end = (char*)malloc(6 * sizeof(char)); //끝나는 요일 저장
    char month[6]; //달 저장
    char day[6]; //일 저장
    char* schedule = (char*)malloc(1024 * sizeof(char));
    char* category = (char*)malloc(20 * sizeof(char));
    ScheduleType* head;

    printf("[ 반복 일정 등록 ]\n");

    while (1) {
        printf("시작 날짜[ex) 01.01]: ");
        scanf("%s", start); //날짜 입력 받음

        if (correct_str(start)) {
            break; //문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n");
        }
    }

    while (1) {
        printf("종료 날짜[ex) 01.01]: ");
        scanf("%s", end); //날짜 입력 받음

        if (correct_str(end)) {
            break; //문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n");
        }
    }

    printf("내용: ");
    scanf(" %[^\n]", schedule); //공백을 포함한 문자열 받음
    printf("\n");

    while (1) {
        show_Category();
        printf("카테고리: ");
        scanf(" %[^\n]", category);
        if (category_Check(category) == 1) {
            strcat(category, "\n");
            break;
        }
        else {
            printf("없는 카테고리입니다. 다시 입력해 주세요.\n");
        }
    }
    printf("\n");

    printf("----- < 요일 >-----\n");
    printf(" 1. 일요일\n");
    printf(" 2. 월요일\n");
    printf(" 3. 화요일\n");
    printf(" 4. 수요일\n");
    printf(" 5. 목요일\n");
    printf(" 6. 금요일\n");
    printf(" 7. 토요일\n");
    printf("------------------\n");
    printf("반복할 요일을 선택하세요.: ");
    scanf("%d", &selecWeek);

    char* ptr = strtok(start, ".");   // 문자열을 .로 구분해서 나눠줌 
    startMonth = atoi(ptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
    ptr = strtok(NULL, ".");
    startDay = atoi(ptr) - 1;

    char* pptr = strtok(end, ".");   // 문자열을 .로 구분해서 나눠줌 
    endMonth = atoi(pptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
    pptr = strtok(NULL, ".");
    endDay = atoi(pptr) - 1;

    totalMonth = endMonth - startMonth;

    if (totalMonth == 0) { //같은 날에 반복되는 일정이 종료된다면
        for (int i = startDay; i < endDay; i++) {
            element* item = (element*)malloc(sizeof(element));
            item->date = (char*)malloc(6 * sizeof(char));
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(20 * sizeof(char));

            dayOfTheWeek = day_of_week(startMonth, i);

            if (selecWeek == dayOfTheWeek) {
                char str[6] = "";

                _itoa(startMonth + 1, month, 10);   // 정수를 문자로
                _itoa(i + 1, day, 10);

                strcat(str, month);
                strcat(str, ".");
                strcat(str, day); //날짜 형식으로 합치기

                item = merge_data(str, schedule, category);

                if (is_empty(startMonth, i) == 0) { //해당 날짜에 일정이 존재하는지
                    head = NULL; //존재하지 않으면 head = NULL;
                    head = insert_node(head, item);
                    schedules[startMonth][i] = head;
                }
                else {
                    head = schedules[startMonth][i]; //존재할 때
                    head = insert_node(head, item);
                    schedules[startMonth][i] = head;
                }
            }
        }
    }
    else if (totalMonth > 0) { //반복 일정의 기간의 달이 바뀐다면
        int j = startDay;
        for (int i = startMonth; i <= endMonth; i++) { //달 바뀌는 거 적용
            element* item = (element*)malloc(sizeof(element));
            item->date = (char*)malloc(6 * sizeof(char));
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(20 * sizeof(char));

            while (((j <= lastDay[i] && (i < endMonth && j <= lastDay[i])) || (i == endMonth && j <= endDay))) { 
                //i가 종료 달 전이면 j는 해당 달 마지막날 넘지 않고, 종료 달과 같아지면 입력받은 종료 일까지만 돌도록
                dayOfTheWeek = day_of_week(i, j);

                if (selecWeek == dayOfTheWeek) {
                    char str[6] = "";

                    _itoa(i + 1, month, 10);   // 정수를 문자로
                    _itoa(j + 1, day, 10);

                    strcat(str, month);
                    strcat(str, ".");
                    strcat(str, day); //날짜 형식으로 합치기

                    item = merge_data(str, schedule, category); //일정 날짜, 내용, 카테고리 합쳐서 반환 받음

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
    printf("\"%s\" 일정이 등록되었습니다.\n", schedule);
    printf("\n");

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void add_anniversary() { // 기념일 등록하는 함수
    char key;
    char str[6] = "";
    char date_temp[6] = "";
    char month_temp[3];
    char day_temp[3];
    element* item = (element*)malloc(sizeof(element)); //동적 할당
    ScheduleType* head; //가장 처음 노드 가르키는 head 노드

    item->date = (char*)malloc(6 * sizeof(char)); //동적 할당
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));

    printf("[ 기념일 등록 ]\n");

    while (1) {
        printf("기념일 날짜[ex) 01.01]: ");
        scanf("%s", item->date); //날짜 입력 받음

        if (correct_str(item->date)) {
            break; //문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // 문자열을 .로 구분해서 나눠줌 
    int month = atoi(ptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //해당 날짜에 일정이 존재하지 않으면
        head = NULL;
    }
    else { //해당 날짜에 일정이 존재하면
        head = schedules[month][day];
    }

    printf("내용: ");
    scanf(" %[^\n]", item->schedule); //공백을 포함한 문자열 받음

    strcpy(item->category, "기념일\n");

    _itoa(month + 1, month_temp, 10);   // 정수를 문자로
    _itoa(day + 1, day_temp, 10);

    strcat(str, month_temp);
    strcat(str, ".");
    strcat(str, day_temp); //날짜 형식으로 합치기
    strcpy(item->date, str);

    head = insert_node(head, item); //노드를 insert

    schedules[month][day] = head; //일정 저장하는 배열에 해당 날짜 일정 전부 저장해줌

    printf("\n");
    printf("\"%s\" 기념일이 등록되었습니다.\n", item->schedule);
    printf("\n");

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

int total_days(int year, int month, int day) { //총 일수 구해주는 함수
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

    int anniIndex = 0; // anni 배열의 인덱스 변수 추가

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
                    if (strcmp(p->data->category, "기념일\n") == 0) {
                        // anni 배열에 할당 및 값 채우기
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
                            //기념일이 오늘 날짜로부터 전에 있는 기념일이라면
                            totalDay = total_days(2023, t->tm_mon + 1, t->tm_mday) - total_days(2023, month, day);
                            //며칠 지났는지 계산
                            strcat(date_temp, "D + ");
                            _itoa(totalDay, str_totalDay, 10);
                            strcat(date_temp, str_totalDay);

                            strcpy(anni[anniIndex]->totalDay, date_temp);
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }
                        else if (t->tm_mon + 1 == month && t->tm_mday == day) { //기념일 날짜가 오늘하고 같으면
                            strcpy(anni[anniIndex]->totalDay, "D - Day");
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }
                        else { //기념일 날짜가 오늘 날짜로부터 후에 있는 기념일이라면
                            totalDay = total_days(2023, month, day) - total_days(2023, t->tm_mon + 1, t->tm_mday);
                            strcat(date_temp, "D - ");
                            _itoa(totalDay, str_totalDay, 10);
                            strcat(date_temp, str_totalDay);

                            strcpy(anni[anniIndex]->totalDay, date_temp);
                            strcpy(anni[anniIndex]->data->date, p->data->date);
                            strcpy(anni[anniIndex]->data->schedule, p->data->schedule);
                            strcpy(anni[anniIndex]->data->category, p->data->category);
                        }

                        anniIndex++; // 인덱스 증가
                    }
                }
            }
        }
    }
}

void add_schedule_menu() { //일정 등록 메뉴 띄워주는 함수
    int sel;

    while (1) {
        printf("------< 일정 등록 > --------\n");
        printf(" 1. 일정 등록\n");
        printf(" 2. 반복 일정 등록\n");
        printf(" 3. 기념일 등록\n");
        printf(" 0. 돌아가기\n");
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
            printf("없는 메뉴를 선택하셨습니다. 다시 선택해 주세요.\n\n");
            break;
        }
    }
}

ScheduleNode* createScheduleNode(ScheduleType* data) {  // 일정을 이진탐색트리 형태로 만들어줌
    ScheduleNode* newNode = (ScheduleNode*)malloc(sizeof(ScheduleNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertScheduleNode(ScheduleNode** root, ScheduleType* data) { // 일정을 이진 탐색 트리에 삽입
    if (*root == NULL) {
        *root = createScheduleNode(data);
        return;
    }

    int compare = strcmp(data->data->schedule, (*root)->data->data->schedule); //일정별로 비교해서 반환값 받음

    if (compare < 0) {
        insertScheduleNode(&(*root)->left, data);
    }
    else { 
        insertScheduleNode(&(*root)->right, data);
    }
}

void inorderTraversal(ScheduleNode* root, ScheduleType** head) { 
    //이진 탐색 트리를 중위 순회하여 오름차순으로 정렬된 순서로 연결 리스트 구성
    if (root == NULL) { //일정이 없으면 종료
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

void sort_schedule(ScheduleType** head) { //주어진 연결 리스트 이진 탐색 트리 사용하여 정렬
    if (*head == NULL || (*head)->link == NULL) {
        return;
    }

    ScheduleNode* root = NULL;
    ScheduleNode* anni_root = NULL;
    ScheduleType* current = *head;

    while (current != NULL) {
        ScheduleType* next = current->link;
        if (strcmp(current->data->category, "기념일\n") != 0) { //카테고리가 '기념일'에 해당하지 않는 노드들끼리 정렬
            insertScheduleNode(&root, current);
            current = next;
        }
        else { //카테고리가 '기념일'에 해당하는 노드들끼리 정렬
            insertScheduleNode(&anni_root, current);
            current = next;
        }
    }

    *head = NULL; //정렬한 데이터들 넣어주기 위해 head를 비워주기
    inorderTraversal(anni_root, head); //'기념일'에 해당하는 노드들이 첫 노드가 되도록
    inorderTraversal(root, head);
}

void print_date() {
    char key;

    for (int k = 0; k < MAX_ANNI_SIZE; k++) { //기념일 출력
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
                    // 기념일이 아닌 노드 출력
                    if (strcmp(p->data->category, "기념일\n") != 0) {
                        printf("%d, %d, %s, %s, %s", i, j, p->data->date, p->data->schedule, p->data->category);
                    }
                }
            }
        }
    }

    printf("\n");
    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");

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

    element* item = (element*)malloc(sizeof(element)); //동적 할당
    ScheduleType* head; //가장 처음 노드 가르키는 head 노드1

    item->date = (char*)malloc(6 * sizeof(char)); //동적 할당
    item->schedule = (char*)malloc(1024 * sizeof(char));
    item->category = (char*)malloc(10 * sizeof(char));
    change_schedule = (char*)malloc(1024 * sizeof(char));
    delete_schedule = (char*)malloc(1024 * sizeof(char));

    while (1) {

        printf("수정할 날짜[ex) 01.01]: ");
        scanf("%s", item->date);

        if (correct_str(item->date)) {
            break; //문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n");
        }
    }

    strcpy(date_temp, item->date);
    char* ptr = strtok(date_temp, ".");   // 문자열을 .로 구분해서 나눠줌 
    int month = atoi(ptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    if (is_empty(month, day) == 0) { //해당 날짜에 일정이 존재하지 않으면
        head = NULL;
    }
    else { //해당 날짜에 일정이 존재하면
        head = schedules[month][day];
    }

    if (head != NULL) {
        int ck = 0;
        for (ScheduleType* p = head; p != NULL; p = p->link) {
            if (strcmp(p->data->category, "공휴일\n") != 0 &&
                strcmp(p->data->category, "나라기념일\n") != 0 &&
                strcmp(p->data->category, "기념일\n") != 0)
                printf(" %s %s %s", p->data->date, p->data->schedule, p->data->category);
        }
        while (ck != 1)
        {
            printf("바꾸고 싶은 일정을 입력 : ");
            while (getchar() != '\n');
            gets(delete_schedule);
            printf("\n");

            for (ScheduleType* p = head; p != NULL; p = p->link) {
                if (strcmp(p->data->schedule, delete_schedule) == 0)
                    ck = 1;
            }
            if (!ck)
            {
                printf("다시 입력.\n");
            }

        }

        printf("바뀌게 될 일정을 입력 : ");
        gets(change_schedule);

        for (ScheduleType* p = head; p != NULL; p = p->link) {
            if (strcmp(p->data->schedule, delete_schedule) == 0)
                strcpy(p->data->schedule, change_schedule);
        }
    }

    printf("\n");

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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

    // item->date 초기화
    strcpy(item->date, "");

    printf("[ 일정 삭제 ]\n");

    // 날짜 입력 받기
    while (1) {
        printf("삭제할 날짜[ex) 01.01]: ");
        scanf("%s", date_temp);

        if (correct_str(date_temp)) {
            break; // 문자열 형식이 맞으면 break;
        }
        else {
            printf("올바른 날짜 형식이 아닙니다. 다시 입력해주세요.\n");
        }
    }

    strcpy(item->date, date_temp);
    char* ptr = strtok(date_temp, ".");
    int month = atoi(ptr) - 1;
    ptr = strtok(NULL, ".");
    int day = atoi(ptr) - 1;

    // 일정이 없거나 카테고리가 "공휴일"이거나 "나라기념일"인 경우
    if (is_empty(month, day) == 0 || (schedules[month][day] != NULL && (strcmp(schedules[month][day]->data->category, "공휴일\n") == 0 || strcmp(schedules[month][day]->data->category, "나라기념일\n") == 0)))
    {
        printf("해당 날짜에는 삭제 가능한 일정이 없습니다.\n");
        free(item->date);
        free(item->schedule);
        free(item);

        printf("\n초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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

    // 삭제 가능한 일정 목록 출력
    printf("\n----- 삭제 가능한 일정 목록 -----\n");
    while (current != NULL) {
        if (strcmp(current->data->category, "공휴일\n") != 0 && strcmp(current->data->category, "나라기념일\n") != 0) {
            printf("%d. %s\n", count + 1, current->data->schedule);
            count++;
        }
        current = current->link;
    }
    printf("---------------------------------\n");

    int choice;

    // 삭제할 일정 선택
    while (1) {
        printf("\n삭제할 일정 번호를 입력하세요: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > count) {
            printf("유효한 번호를 입력해주세요.\n");
        }
        else {
            break;
        }
    }

    current = head;
    prev = NULL;
    count = 0;

    // 선택한 일정 위치로 이동
    while (current != NULL) {
        if (strcmp(current->data->category, "공휴일\n") != 0 && strcmp(current->data->category, "나라기념일\n") != 0) {
            count++;
        }

        if (count == choice) {
            break;
        }

        prev = current;
        current = current->link;
    }

    // 일정 삭제
    if (current == NULL) {
        printf("일정이 존재하지 않습니다.\n");
        free(item->date);
        free(item->schedule);
        free(item);

        printf("\n초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
        while (1)
        {
            key = getchar();
            if (key == 'q')
            {
                return;
            }
        }
    }

    // 삭제할 일정이 첫 번째 노드인 경우
    if (prev == NULL)
    {
        head = current->link;
    }
    // 삭제할 일정이 첫 번째 노드가 아닌 경우
    else
    {
        prev->link = current->link;
    }

    schedules[month][day] = head;

    printf("\n");
    printf("[%s] 일정이 삭제되었습니다.\n", current->data->schedule);
    printf("\n");

    // 메모리 해제
    free(current->data->date);
    free(current->data->schedule);
    free(current->data);
    free(current);
    free(item->date);
    free(item->schedule);
    free(item);

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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

    printf("[ 일정 검색 ]\n");

    // 일정 내용 입력 받기
    printf("일정 내용: ");
    scanf(" %[^\n]s", item->schedule);
    printf("\n");

    // 검색한 일정에 해당하는 날짜 출력
    int found = 0; // 일정을 찾았는지 여부를 나타내는 변수

    printf("검색한 일정:\n");

    for (int month = 0; month < 12; month++) {
        for (int day = 0; day < lastDay[month]; day++) {
            ScheduleType* current = schedules[month][day];
            while (current != NULL) {
                if (strcmp(current->data->schedule, item->schedule) == 0) {
                    print_schedule(month + 1, day + 1, current->data->category);
                    printf(" %d월 %d일 %s\n", month + 1, day + 1, item->schedule);
                    found = 1;
                    break;
                }
                current = current->link;
            }
        }
    }

    if (!found) {
        printf("검색한 일정을 찾을 수 없습니다.\n");
    }

    printf("\n초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void TodaySchedule() {  // 당일 일정 띄워주는 함수
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon;
    int day = tm.tm_mday - 1;

    printf("\n-------------------------\n");
    printf("%d-%d-%d\n\n", year, tm.tm_mon + 1, tm.tm_mday);

    // 기념일 출력
    for (int k = 0; k < MAX_ANNI_SIZE; k++) {
        if (anni[k] != NULL) {
            textColor(6);
            printf("■ ");
            textColor(7);
            printf("%s %s %s\n\n", anni[k]->data->date, anni[k]->totalDay, anni[k]->data->schedule);
        }
    }

    // 당일 일정 출력
    if (is_empty(month, day) == 0) {
        printf("오늘 일정이 없습니다.\n");
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

void print_calendar(int month)    // 달력 출력
{
    int daysInMonth, startDay, endDay, i, j, k, l, day, count;

    // 월별로 날 수와 시작 요일 계산
    if (month == 2)
        daysInMonth = 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        daysInMonth = 30;
    else
        daysInMonth = 31;

    // 날짜 및 시간 계산 변수
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

    // 달력 출력
    printf("\t     2023년 %d월\n", month);
    printf("  일   월   화   수   목   금   토  \n");
    printf("┌──────────────────────────────────┐\n");
    printf("│");

    for (i = 0; i < startDay; i++)
        printf("    │");    // 날짜없는 빈 칸
    for (day = 1, j = i; day <= daysInMonth; day++, j++) {
        if (day == 1) {     // 1일
            printf("%2d", day);
        }
        else                // 나머지 일 수
            printf("│%2d", day);

        if (j % 7 == 6) {   // 토요일이면 끝을 '│'로 닫아주고 칸 만듦
            printf("  │");
            if (day == 30)
                break;
            printf("\n├──────────────────────────────────┤\n");
        }
        else
            printf("  ");
    }

    if (endDay % 7 != 6) {
        while (endDay % 7 != 6) {
            printf("│    ");
            endDay++;
        }
        printf("│");
    }
    printf("\n└──────────────────────────────────┘\n\n");
}

void month_Schedule()   // 일정 보기
{
    char key;
    int month = 0;

    printf("[ 일정 보기 ]\n");

    // 확인할 달 입력
    while (1) {
        printf("몇 월: ");
        scanf(" %d", &month);
        if (month < 1 || month>12) {
            printf("해당하는 달이 없습니다. 다시 입력해주세요.\n");
        }
        else
            break;
    }
    system("cls");
    print_calendar(month);

    // 원하는 달 일정 보기
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

    printf("\n초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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

        printf("------- < 일정 보기 > -------\n");
        printf(" 1. 일정 보기\n");
        printf(" 2. 카테고리별 일정 보기\n");
        printf(" 3. 일정 검색\n");
        printf(" 0. 돌아가기\n");
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
            printf("없는 메뉴를 선택하셨습니다. 다시 선택해 주세요.\n\n");
            break;
        }
    }
}