#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>S
#include "makeScheduleFunc.h"

int main() {
    int sel; //메뉴 선택
    file_Init_Holidays();	// 공유일 파일 초기화
    file_Init_UserData();	// 유저 데이터 파일 초기화
    file_Init_Category();

    while (1) {
        anniversary_cal();

        printf("[ 2023년도 일정 관리 프로그램 ]\n");
        TodaySchedule();

        printf("------- < 메 뉴 > -------\n");
        printf(" 1. 일정 등록\n");
        printf(" 2. 일정 삭제\n");
        printf(" 3. 일정 수정\n\n");
        printf(" 4. 일정 보기\n");
        printf(" 5. 카테고리 관리\n");
        printf(" 0. 종료\n");
        printf("-------------------------\n");
        printf(">> ");
        scanf("%d", &sel);

        switch (sel)
        {
        case 1:
            system("cls");
            add_schedule_menu(); //일정 등록 메뉴
            break;
        case 2:
            system("cls");
            delete_Schedule(); //일정 삭제
            system("cls");
            break;
        case 3:
            system("cls");
            modify_Schedule(); //일정 수정
            break;
        case 4:
            system("cls");
            show_schedule_menu(); //일정 보기 메뉴
            break;
        case 5:
            system("cls");
            category_menu(); // 카테고리 관리 메뉴
            break;
        case 0:
            printf("프로그램을 종료합니다.");
            file_Update_category(); //카테고리 업데이트
            file_Update_User(); //사용자 일정 데이터 업데이트
            return;
            break;
        default:
            printf("없는 메뉴를 선택하셨습니다. 다시 선택해 주세요.\n\n");
            break;
        }
    }
}