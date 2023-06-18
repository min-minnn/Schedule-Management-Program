typedef struct DataType {
    char* date; //��¥
    char* schedule; //����
    char* category; //ī�װ�
}element;

typedef struct ListNode {
    element* data;
    struct ListNode* link; //���� ���� ����Ŵ
}ScheduleType;

typedef struct AnniNode {
    element* data;
    char totalDay[1024];
}anniversary;

ScheduleType* insert_node(ScheduleType*, element*);
ScheduleType* merge_data();

void textColor(int colorNum);
void print_schedule(int month, int day, char* category);
void TodaySchedule();
void show_category_calendar();
void print_calendar(int month);
void month_Schedule();
void sort_schedule(ScheduleType** head);
void add_schedule_menu();
void add_Category();
void delete_Category();
void show_Category();
void file_Update_User();
void file_Update_category();
void file_Init_Holidays();	// ������ ���� �ʱ�ȭ
void file_Init_UserData();	// ���� ������ ���� �ʱ�ȭ
void file_Init_Category();
void category_menu();
void anniversary_cal();
void modify_Schedule();
void delete_Schedule();
void show_schedule_menu();
int is_empty(int,int);
int category_Check(char*);