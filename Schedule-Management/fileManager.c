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

enum ColorType {    // 글자 색 지정
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
char categoryList[MAX_CATEGORY_SIZE][MAX_CATEGORY_NAME_SIZE];   // 카테고리 리스트
int categoryIndex = 0;   // 카테고리 요소 접근 변수
int fixedData = 0;   // 카테고리 고정값 "공휴일","나라기념일","일반"

void file_Init_Holidays() // 공휴일, 나라기념일 파일 초기화
{
    char* date = (char*)malloc(6 * sizeof(char));
    char* date_temp = (char*)malloc(6 * sizeof(char));
    char* schedule_temp = (char*)malloc(1024 * sizeof(char));
    char* category_temp = (char*)malloc(20 * sizeof(char));
    ScheduleType* head; //가장 처음 노드 가르키는 head 노드

    FILE* pfile = fopen("CSV//holidayss.csv", "r");   // 파일 open
    char str_tmp[1024];

    if (pfile != NULL)   // 파일이 존재한다면
    {
        fgets(str_tmp, 1024, pfile);   // 파일에서 한줄씩 읽어와서 문자열로 반환

        while (fgets(str_tmp, 1024, pfile) != NULL) {   // 파일의 끝까지 // 파일에서 한줄을 읽어와서 문자열로 반환
            element* item = (element*)malloc(sizeof(element)); //동적 할당

            item->date = (char*)malloc(6 * sizeof(char)); //동적 할당
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(10 * sizeof(char));

            char* ptr = strtok(str_tmp, ",");   // 문자열을 ,로 구분해서 나눠줌   
            strcpy(date_temp, ptr);

            ptr = strtok(NULL, ",");
            strcpy(schedule_temp, ptr);

            ptr = strtok(NULL, ",");
            strcpy(category_temp, ptr);

            strcpy(date, date_temp);
            char* pptr = strtok(date, ".");   // 문자열을 .로 구분해서 나눠줌 
            int month = atoi(pptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
            pptr = strtok(NULL, ".");
            int day = atoi(pptr) - 1;

            if (is_empty(month, day) == 0) { //해당 날짜에 일정이 존재하지 않으면
                head = NULL;
            }
            else { //해당 날짜에 일정이 존재하면
                head = schedules[month][day];
            }

            item = merge_data(date_temp, schedule_temp, category_temp);

            head = insert_node(head, item); //노드를 insert

            schedules[month][day] = head;
        }
    }
    fclose(pfile);
}

void file_Init_UserData() // 유저 파일 초기화
{
    char* date = (char*)malloc(6 * sizeof(char));
    char* date_temp = (char*)malloc(6 * sizeof(char));
    char* schedule_temp = (char*)malloc(1024 * sizeof(char));
    char* category_temp = (char*)malloc(20 * sizeof(char));
    ScheduleType* head; //가장 처음 노드 가르키는 head 노드

    FILE* pfile = fopen("CSV//userData.csv", "r");   // 파일 open
    char str_tmp[1024];

    if (pfile != NULL)   // 파일이 존재한다면
    {
        while (fgets(str_tmp, 1024, pfile) != NULL) {   // 파일의 끝까지 // 파일에서 한줄을 읽어와서 문자열로 반환
            element* item = (element*)malloc(sizeof(element)); //동적 할당

            item->date = (char*)malloc(6 * sizeof(char)); //동적 할당
            item->schedule = (char*)malloc(1024 * sizeof(char));
            item->category = (char*)malloc(10 * sizeof(char));

            if (strchr(str_tmp, ',')) {
                char* ptr = strtok(str_tmp, ",");   // 문자열을 ,로 구분해서 나눠줌   
                strcpy(date_temp, ptr);

                ptr = strtok(NULL, ",");
                strcpy(schedule_temp, ptr);

                ptr = strtok(NULL, ",");
                strcpy(category_temp, ptr);

                strcpy(date, date_temp);
                char* pptr = strtok(date, ".");   // 문자열을 .로 구분해서 나눠줌 
                int month = atoi(pptr) - 1;   //atoi 함수 사용해서 문자열을 정수로 변환
                pptr = strtok(NULL, ".");
                int day = atoi(pptr) - 1;

                if (is_empty(month, day) == 0) { //해당 날짜에 일정이 존재하지 않으면
                    head = NULL;
                }
                else { //해당 날짜에 일정이 존재하면
                    head = schedules[month][day];
                }

                item = merge_data(date_temp, schedule_temp, category_temp);

                head = insert_node(head, item); //노드를 insert

                schedules[month][day] = head;
            }
        }
    }
    fclose(pfile);
}

void file_Update_User() // 파일 업데이트
{
    FILE* pfile = fopen("CSV//userData.csv", "w"); // 파일 open

    if (pfile != NULL) // 파일이 존재한다면
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

                        if (strcmp(p->data->category, "공휴일\n") == 0 ||
                            strcmp(p->data->category, "나라기념일\n") == 0) {
                            continue;
                        }

                        element* item = (element*)malloc(sizeof(element)); // 동적 할당

                        item->date = (char*)malloc(6 * sizeof(char)); // 동적 할당
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

void file_Init_Category()   // 카테고리 파일 초기화
{
    strcpy(categoryList[fixedData++], "공휴일\n");
    strcpy(categoryList[fixedData++], "나라기념일\n");
    strcpy(categoryList[fixedData++], "일반\n");
    strcpy(categoryList[fixedData++], "기념일\n");

    FILE* pfile = fopen("CSV//categoryList.csv", "r");   // 파일 open
    char str_tmp[1024];
    int i = 0;
    if (pfile != NULL)   // 파일이 존재한다면
    {
        while (fgets(str_tmp, 1024, pfile) != NULL) {   // 파일의 끝까지 // 파일에서 한줄을 읽어와서 문자열로 반환   
            str_tmp[strcspn(str_tmp, "\n")] = '\0';   // "\n" -> "\0"
            strcpy(categoryList[categoryIndex++], str_tmp);
        }
        fclose(pfile);
    }
}

void file_Update_category()   // 카테고리 파일 업데이트
{
    FILE* pfile = fopen("CSV//categoryList.csv", "w");   // 파일 open

    if (pfile != NULL)   // 파일이 존재한다면
        for (int i = 0; i < categoryIndex; i++)
        {
            fputs(categoryList[i], pfile);
            fputs("\n", pfile);
        }

    fclose(pfile);
}

int category_Check(char* category)   // 카테고리 존재 여부
{
    int tmp = 0;
    for (int i = 0; i < categoryIndex; i++)
    {
        if (strcmp(categoryList[i], category) == 0)
            return tmp = 1;
    }
    return tmp;
}

void show_Category()   // 카테고리 리스트 출력
{
    int cnt = 1;
    printf("------- < 카테고리 목록 > -------\n");
    for (int i = fixedData; i < categoryIndex; i++)
    {
        printf("%d. %s\n", cnt++, categoryList[i]);
    }
    printf("---------------------------------\n");
}

void add_Category()   // 카테고리 추가
{
    char category[MAX_CATEGORY_NAME_SIZE];
    show_Category();
    printf("추가하실 카테고리 이름을 입력하세요 : ");
    scanf(" %[^\n]s", category);
    printf("\n\"%s\" 추가 완료\n\n", category);
    strcpy(categoryList[categoryIndex++], category);

    char key;

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void delete_Category()   // 카테고리 삭제
{
    if (fixedData == categoryIndex)
    {
        printf("삭제할 카테고리가 없습니다.\n");

        char key;

        printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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
        printf("삭제하실 카테고리 이름을 입력하세요 : ");
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
                                strcpy(schedules[i][j]->data->category, "일반\n");
                        }
                    }
                }
            }
            break;
        }
        else {
            printf("없는 카테고리입니다.\n");
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

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
    while (1) {
        key = getchar();
        if (key == 'q') {
            return;
        }
    }
}

void print_schedule(int month, int day, const char* category) {      // 카테고리별 색상 지정
    char colorlist[MAX_CATEGORY_SIZE] = { 0, 0, 0, 0, Green, SkyBlue, Purple,Yellow,Blue,DarkGreen,Red,DarkPurple, DarkGray };
    // 고정 카테고리는 색상 고정
    if (strcmp(category, "공휴일\n") == 0 || strcmp(category, "나라기념일\n") == 0) {
        textColor(DarkRed);
    }
    else if (strcmp(category, "일반\n") == 0) {
        textColor(DarkBlue);
    }
    else if (strcmp(category, "기념일\n") == 0) {
        textColor(DarkYellow);
    }
    else {
        // 나머지 카테고리들은 순서대로 다른 색상으로 지정
        int i;
        for (i = fixedData; i < MAX_CATEGORY_SIZE; i++) {
            if (strncmp(category, categoryList[i], strlen(categoryList[i])) == 0) {
                textColor(colorlist[i]); // 색상은 i와 일치하는 값으로 설정
                break;
            }
        }
        if (i >= MAX_CATEGORY_SIZE) {
            textColor(White);  // 기본 색상
        }
    }

    // 선택된 색상을 이용한 출력 코드
    printf("■ ");
    textColor(White);   // 색상 초기화
}

void show_category_calendar()   // 카테고리별 일정 보기
{
    int sel;
    int cnt = 1;

    // 모든 카테고리 목록 출력
    printf("[ 카테고리별 일정 보기 ]\n\n");
    printf("------- < 카테고리 목록 > -------\n");
    for (int i = 0; i < categoryIndex; i++)
    {
        printf("%d. %s\n", cnt++, categoryList[i]);
    }
    printf("---------------------------------\n");

    do {
        printf(">> ");
        scanf("%d", &sel);

        if (sel < 1 || sel > categoryIndex) {
            printf("유효한 카테고리를 선택하세요.\n");
        }
    } while (sel < 1 || sel > categoryIndex);

    system("cls");

    printf("------- < %s 목록 > -------\n", categoryList[sel - 1]);
    int found = 0;  // 일정이 있는지 확인하기 위함

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
                    if (categoryList[3] == categoryList[sel - 1]) {     // 기념일은 디데이와 함께 출력
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
        printf("\n선택한 카테고리에 대한 일정이 없습니다.\n\n");
    }

    printf("--------------------------------\n\n");

    char key;

    printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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
        printf("------ < 카테고리 관리 > ------\n");
        printf(" 1. 카테고리 추가\n");
        printf(" 2. 카테고리 삭제\n");
        printf(" 3. 카테고리 목록 조회\n");
        printf(" 0. 돌아가기\n");
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

            printf("초기 메뉴로 돌아가려면 'q'를 입력하세요.\n");
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
            printf("없는 메뉴를 선택하셨습니다. 다시 선택해 주세요.\n\n");
            break;
        }
    }
}