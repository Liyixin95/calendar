#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <time.h>

int Days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int year = 2017,mouth = 7;
int Year_Now = 2017,Mouth_Now = 7;
int Day_Now = 0;

short COLOR_GRAY = 8;



/* 判断某年是不是闰年 */
int Year_Judge(int year)
{
	if((year%4 == 0) && (year%100 != 0) || year%400 == 0)
		return 366;
	else
		return 365;
}

/* 判断某年的一月一号是星期几 */
int Week_Judge(int year)
{
	int sum = 0,i = 1900;                         //计算1900年至输入年份的天数总和
	for(i;i<year;i++)
		sum = sum + Year_Judge(i);
	return (sum+1)%7;
	
}

/* 判断某月的一号是星期几 */
int Mouth_Judge(int year,int mouth)
{
	int sum = 0,i = 1;
	int weeks = Week_Judge(year);
	if(Year_Judge(year) == 366)
		Days[1] = 29;
	for(i;i<mouth;i++)
		sum = sum+Days[i-1];
	sum = sum+weeks;
	//printf("sum = %d\n",sum);
	return (sum)%7;
}

void Draw_Calendar(int year,int mouth)
{
	int temp = 0,i = 0,Last_Mouth = 0,Next_Mouth = 0;
	char Date_Buf[7] = {0};
	char Day_Buf[5] = {0};
	int Flag = 0;
	if(year == Year_Now && mouth == Mouth_Now)               //判断是不是今年今月
		Flag = 1;
	
	temp = Mouth_Judge(year,mouth);             //计算本月开头是星期几
	if(mouth >1)
	    Last_Mouth = Days[mouth -2] - temp +1;     //计算本月结束还需要填充几天
	else
		Last_Mouth = Days[11] - temp +1;
	
    if(mouth > 9)
	{
		sprintf(Date_Buf,"%d.%d",year,mouth);
	}
	else
	{
		sprintf(Date_Buf,"%d. %d",year,mouth);
	}
	
	clear();
	if(Flag == 1)
		attron(COLOR_PAIR(3));
	else
		attroff(COLOR_PAIR(3));
	mvaddstr(1,COLS/2-strlen(Date_Buf)/2,Date_Buf);
	attroff(COLOR_PAIR(3));
	mvaddstr(3,COLS/2-17,"  Sun  Mon  Tue  Wed  Thu  Fri  Sat");
	mvaddstr(4,COLS/2-17,"===================================");
	
	move(5,COLS/2-17);
	int y = 5;
	
	attron(COLOR_PAIR(1));
    for(i;i<temp;i++)                      //用上个月填充本月开头
	{
		if(Last_Mouth > 9)
		{
			sprintf(Day_Buf,"  %d ",Last_Mouth);
		}
		else
		{
			sprintf(Day_Buf,"   %d ",Last_Mouth);
		}
		addstr(Day_Buf);
		Last_Mouth++;
	}
	attroff(COLOR_PAIR(1));
	
	i = 1;
	while(i <= Days[mouth-1])             //本月
	{
		if(i > 9)                          //判断日期是两位数还是一位数
		{
			sprintf(Day_Buf,"  %d ",i);
		}
		else
		{
			sprintf(Day_Buf,"   %d ",i);
		}
		
		if((i + temp)%7 == 0 || (i + temp)%7 == 1)        //判断是不是周末
		{
			attron(COLOR_PAIR(2));
		}	
		else if (i == Day_Now && Flag == 1)               //判断是不是今天
		{
			attron(COLOR_PAIR(3));
		}
		else
		{
			attroff(COLOR_PAIR(2));
			attroff(COLOR_PAIR(3));
		}
		
		addstr(Day_Buf);
		if((i + temp)%7 == 0)
		{
			y = y+1;
			move(y,COLS/2-17);
		}
		i++;		
	}
	
	attron(COLOR_PAIR(1));
	Next_Mouth = 43 -i -temp;
	i = 1;
	for(i;i<=Next_Mouth;i++)    //用下个月填充本月
	{
		if(i > 9)
		{
			sprintf(Day_Buf,"  %d ",i);
		}
		else
		{
			sprintf(Day_Buf,"   %d ",i);
		}
		addstr(Day_Buf);
		if((i+temp+Days[mouth-1])%7 == 0)
		{
			y = y+1;
			move(y,COLS/2-17);
		}
	}
	move(LINES-1,0);
	attroff(COLOR_PAIR(1));
	refresh();
}

/* 获取本地时间 */
void Get_Time()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	Year_Now = (1900 + p -> tm_year);
	Mouth_Now = (1 + p -> tm_mon);
	Day_Now = p -> tm_mday;
}

void Initial()
{
	initscr();
	keypad(stdscr,TRUE);
	clear();
	echo();
	if(start_color() == OK)
	{
		init_color(COLOR_BLUE,25,34,48);
		init_pair(1,COLOR_BLUE,COLOR_BLACK);
		init_pair(2,COLOR_RED,COLOR_BLACK);
		init_pair(3,COLOR_GREEN,COLOR_BLACK);
		//init_pair(3,COLOR_CYAN,COLOR_BLACK);
	}
	Get_Time();
	Draw_Calendar(2017,7);
}
void Set_Date()                               //设置日期
{
	char Year_Input[4];
	char Mouth_Input[2];
	clear();
	//Mouth_Input[1] = 'a';
	mvaddstr(0,0,"please enter the year");
	move(1,0);
	getstr(Year_Input);
	year = 1000*(Year_Input[0] - '0') + 100*(Year_Input[1] - '0') + 10*(Year_Input[2] - '0') + (Year_Input[3] - '0');
	while(year < 1900)
	{
		clear();
		mvaddstr(0,0,"please enter again");
		move(1,0);
		getstr(Year_Input);
		year = 1000*(Year_Input[0] - '0') + 100*(Year_Input[1] - '0') + 10*(Year_Input[2] - '0') + (Year_Input[3] - '0');
	}
	
	clear();
	mvaddstr(0,0,"please enter the mouth");
	move(1,0);
	getstr(Mouth_Input);
	if(Mouth_Input[1] == '\0')                 //判断输入的是一位数还是两位数。
		mouth = Mouth_Input[0] - '0';
	else
	    mouth = 10*(Mouth_Input[0] - '0') + (Mouth_Input[1] - '0');
	while(mouth < 1 || mouth > 12)
	{
		clear();
		mvaddstr(0,0,"please enter again");
		move(1,0);
	    getstr(Mouth_Input);
		move(2,0);
		printf("%d",mouth);
		if(Mouth_Input[1] == '\0')
			mouth = Mouth_Input[0] - '0';
		else
			mouth = 10*(Mouth_Input[0] - '0') + (Mouth_Input[1] - '0');
	}
}

int main()
{
	int ch;
	Initial();
	while(1)
	{
		ch = getch();
		switch(ch)
		{
			case KEY_UP:
				year++;
			    Draw_Calendar(year,mouth);
				break;
			case KEY_DOWN:
				year--;
			    Draw_Calendar(year,mouth);
				break;
			case KEY_LEFT:
				mouth--;
			    if(mouth == 0)
			    {
				    mouth = 12;
				    year --;
			    }
			    Draw_Calendar(year,mouth);
				break;
			case KEY_RIGHT:
				mouth = mouth +1;
			    if(mouth > 12)
			    {
				    year ++;
				    mouth = 1;
			    }
			    Draw_Calendar(year,mouth);
				break;
			case 'f' :
				Set_Date();
				Draw_Calendar(year,mouth);
				break;
			case 27:
				endwin();
				return 0;
		}

	}
	
	endwin();
	return 0;
}



















