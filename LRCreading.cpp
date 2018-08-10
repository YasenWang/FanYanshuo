#include<stdio.h>
#include<Windows.h>

#define LRC_PATH "C:\\Users\\wangz\\Desktop\\SimpleLove.lrc"
#define LRC_SHOW_LINENUMBER 5		//0-9行 当值为0时，只显示1行歌词
#define SCREAM_CLEAN " "
#define ShowTime 1				//显示时间
#pragma warning (disable : 4996)

typedef struct lrc
{
	int min;
	int sec;
	char cur_num[100];
	struct lrc *head;
	struct lrc *next;
}LRC;

typedef struct clock
{
	int m;//分
	int s;// 秒
}CLOCK;
CLOCK GlobalTime = { 0 };
//使光标移动到指定位置
void move(int x, int y)
{
	HANDLE h1;
	COORD pos;								//pos为结构体变量
	pos.X = x;
	pos.Y = y;
	h1 = GetStdHandle(STD_OUTPUT_HANDLE);	//从特定的输出中获得句柄
	SetConsoleCursorPosition(h1, pos);		//实现对光标位置的控制
}
//清屏
void CleanScreamLine()
{
	for (int i = 0; i < 50; i++)
	{
		printf(SCREAM_CLEAN);
	}
}

//设置颜色
void set_color(int color)
{
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, color);
}
//创建链表头结点
LRC *creat_head()
{
	LRC *p = (LRC *)malloc(sizeof(LRC));
	p->head = NULL;
	return p;
}
//创建链表头 
void insert_head(LRC *p1, LRC *p2)
{
	if (p1->head == NULL)
	{
		p1->head = p2;
	}
	else
	{
		p2->next = p1->head;
		p1->head = p2;
	}
}
//创建链表尾
void insert_tail(LRC *p1, LRC *p2)
{
	LRC *p;
	if (p1->head == NULL)
	{
		p1->head = p2;
	}
	else
	{
		p = p1->head;
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = p2;
	}
}
//创建关于时间的结点
LRC *data_creat(int min, int sec, char data[])
{
	LRC *p;
	p = (LRC *)malloc(sizeof(LRC));

	p->min = min;
	p->sec = sec;

	memset(p->cur_num, '\0', sizeof(p->cur_num));
	strcpy(p->cur_num, data);
	p->next = NULL;

	return p;
}
//读取lrc中数据，加入到链表中
LRC *creat_list(FILE *fp)
{
	LRC *p1 = creat_head();

	LRC *p2 = NULL;
	int min, sec;
	char buffer[100] = { 0 };
	char *p = NULL;

	p = fgets(buffer, 100, fp);
	while (p != NULL)
	{
		if (buffer[0] == '[' && buffer[1] >= '0' && buffer[1] <= '9')
		{
			min = (buffer[1] - '0') * 10 + (buffer[2] - '0');
			sec = (buffer[4] - '0') * 10 + (buffer[5] - '0');

			p2 = data_creat(min, sec, buffer);
			insert_tail(p1, p2);
		}
		p = fgets(buffer, 100, fp);
	}
	return p1;
}


void ShowTimeByCount()
{
	GlobalTime.s++;
	if (GlobalTime.s == 60)
	{
		GlobalTime.m++;
		GlobalTime.s = 0;
	}
#if ShowTime
	move(30, 2);
	set_color(2);
	printf("%2d:%2d\n", GlobalTime.m, GlobalTime.s);
#endif
}

void ShowSingleLRC(char *singlelrc,int line)
{
	if (line == 0)										//首行颜色不同
	{
		move(30, 10 - line);
		set_color(5);
		CleanScreamLine();
		move(30, 10 - line);
		printf("%s", singlelrc);
	}
	else if (line > 0 && line < LRC_SHOW_LINENUMBER)	//后四行歌词
	{
		move(30, 10 - line);
		set_color(2);
		CleanScreamLine();
		move(30, 10 - line);
		printf("%s", singlelrc);
	}
	else printf("ShowSingleLine ERROR!\n");
}

void yasen_show(LRC *mylrc)
{


	int time_pre = 0;
	int time_next = 0;
	int detime = 0;
	int count = 0;
	LRC *p = mylrc->head;
	LRC lrcwords[LRC_SHOW_LINENUMBER] = { 0 };

	int lrc_time1 = 0;
	int lrc_time2 = 0;
	int LRCline = 0;

	while (p != NULL)
	{
		//get detime
		lrc_time1 = (p->min) * 60 + p->sec;
		detime = (lrc_time1 - lrc_time2) * 1000;
		lrc_time2 = lrc_time1;

		//get timepre
		time_pre = detime - time_next;
		while (time_pre >= 1000)
		{
			time_pre -= 1000;
			Sleep(1000);
			//showtime
			ShowTimeByCount();
		}
		time_next = 1000 - time_pre;

		Sleep(time_pre);

		strcpy(lrcwords[count].cur_num, p->cur_num);
		for (int i = 0; i < LRC_SHOW_LINENUMBER; i++)
		{
			//printf("%d : %d\n", (count + i) % LRC_SHOW_LINENUMBER,i);
			ShowSingleLRC(lrcwords[(count - i + LRC_SHOW_LINENUMBER) % LRC_SHOW_LINENUMBER].cur_num, i);
		}
		count = (count + 1) % (LRC_SHOW_LINENUMBER);

		Sleep(time_next - LRC_SHOW_LINENUMBER);

		//showtime
		ShowTimeByCount();

		p = p->next;
	}

}

void yasen_show1(LRC *mylrc)
{


	int time_pre = 0;
	int time_next = 0;
	int detime = 0;
	int count = 0;
	LRC *p = mylrc->head;
	LRC lrcwords[LRC_SHOW_LINENUMBER] = { 0 };

	int lrc_time1 = 0;
	int lrc_time2 = 0;
	int LRCline = 0;

	//get detime
	lrc_time1 = (p->min) * 60 + p->sec;
	detime = (lrc_time1 - lrc_time2) * 1000;
	lrc_time2 = lrc_time1;

	while (p != NULL)
	{
		
		//get timepre
		time_pre = detime - time_next;
		while (time_pre >= 1000)
		{
			time_pre -= 1000;
			Sleep(1000);
			//showtime
			ShowTimeByCount();
		}
		time_next = 1000 - time_pre;

		Sleep(time_pre);

		strcpy(lrcwords[count].cur_num, p->cur_num);
		for (int i = 0; i < LRC_SHOW_LINENUMBER; i++)
		{
			//printf("%d : %d\n", (count + i) % LRC_SHOW_LINENUMBER,i);
			ShowSingleLRC(lrcwords[(count - i + LRC_SHOW_LINENUMBER) % LRC_SHOW_LINENUMBER].cur_num, i);
		}
		count = (count + 1) % (LRC_SHOW_LINENUMBER);

		



		Sleep(time_next - LRC_SHOW_LINENUMBER);

		//showtime
		ShowTimeByCount();

		p = p->next;
		//get detime
		lrc_time1 = (p->min) * 60 + p->sec;
		detime = (lrc_time1 - lrc_time2) * 1000;
		lrc_time2 = lrc_time1;
		
	}

}


int main()
{
	FILE *fp = NULL;					//文件读取声明
	LRC *p;								//歌词Node
	CLOCK m_clock;						//时间声明
	m_clock.m = 0;						//时间初始化
	m_clock.s = 0;
	fp = fopen(LRC_PATH, "rb");			//打开歌词文本，打开方式 “rb”
	if (fp == NULL)	printf("error\n");	//文件打开成功
	p = creat_list(fp);
	yasen_show1(p);
	
	
	system("pause");
}

