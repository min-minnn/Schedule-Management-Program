#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>S
#include "makeScheduleFunc.h"

int main() {
    int sel; //�޴� ����
    file_Init_Holidays();	// ������ ���� �ʱ�ȭ
    file_Init_UserData();	// ���� ������ ���� �ʱ�ȭ
    file_Init_Category();

    while (1) {
        anniversary_cal();

        printf("[ 2023�⵵ ���� ���� ���α׷� ]\n");
        TodaySchedule();

        printf("------- < �� �� > -------\n");
        printf(" 1. ���� ���\n");
        printf(" 2. ���� ����\n");
        printf(" 3. ���� ����\n\n");
        printf(" 4. ���� ����\n");
        printf(" 5. ī�װ� ����\n");
        printf(" 0. ����\n");
        printf("-------------------------\n");
        printf(">> ");
        scanf("%d", &sel);

        switch (sel)
        {
        case 1:
            system("cls");
            add_schedule_menu(); //���� ��� �޴�
            break;
        case 2:
            system("cls");
            delete_Schedule(); //���� ����
            system("cls");
            break;
        case 3:
            system("cls");
            modify_Schedule(); //���� ����
            break;
        case 4:
            system("cls");
            show_schedule_menu(); //���� ���� �޴�
            break;
        case 5:
            system("cls");
            category_menu(); // ī�װ� ���� �޴�
            break;
        case 0:
            printf("���α׷��� �����մϴ�.");
            file_Update_category(); //ī�װ� ������Ʈ
            file_Update_User(); //����� ���� ������ ������Ʈ
            return;
            break;
        default:
            printf("���� �޴��� �����ϼ̽��ϴ�. �ٽ� ������ �ּ���.\n\n");
            break;
        }
    }
}