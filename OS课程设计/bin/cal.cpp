/*
* 简单实现cal命令
* 1509寝W组
* 2021/6/28
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//获取本月有几天
int monthOfDay(int year, int month);

//获取从1900/1/1到目标日期的天数 (1900/1/1 周一)
int getDay(int year, int month);

//打印表
void showCal(int year, int month, int day, int days);
 
//main
int main(int argc, char *argv[])
{
	//参数数量错误
	if(argc != 1)
	{
		fprintf(stdout, "参数错误\n");
	}
	//年月日
	int year, month, day, days;
	//时间结构体相关
	time_t t;
    	struct tm *p;
	time(&t);
	p = gmtime(&t);
 	//获取时间
 	year = p->tm_year + 1900;
 	month = p->tm_mon+1;
 	day = p->tm_mday;
	//获取从1900/1/1到目标日期的天数
	days = getDay(year, month);
   	//打印表
	showCal(year, month, day, days);
	return 0;
}

//获取本月有几天
int monthOfDay(int year, int month)
{
	int day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
	{
		day[1] = 29;//二月
	}
 	return day[month - 1];
}

//获取从1900/1/1到目标日期的天数 (1900/1/1 周一)
int getDay(int year, int month)
{
	int days = 0;
	for(int i = 1900; i < year; ++i)//年
	{
		if(i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
		{
			days += 366;
		}
		else
		{
			days += 365;
		}
	}
	for(int i = 1; i < month; i++)//月
	{
		days = days + monthOfDay(year, i);
	}
	days += 1;//日
	return days;//绿色
}

//打印表
void showCal(int year, int month, int day, int days)
{
	int mdays;
	//获取本月有几天
	mdays=monthOfDay(year, month);
	//打印头
	printf("      %02d月 %d      \n", month, year);
	printf("日 一 二 三 四 五 六\n");
	//把1号置于正确的位置
	for(int i = 0; i < days % 7; ++i)
	{
		printf("%2s "," ");
	}
	//循环输出日期
	for(int i = 1; i <= mdays; ++i)
	{
		if(i == day)
		{
			printf("\033[1;40;32m%2d\033[0m ",i);//绿色
		}
		else
		{
			printf("%2d ",i);
		}
		if(days % 7 == 6)
		{
			printf("\n");
		}
	       days++;
   	}
   	printf("\n\n");
}





