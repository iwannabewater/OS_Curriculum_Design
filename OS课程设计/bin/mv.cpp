/*
* 简单实现mv命令
* 1509寝W组
* 2021/6/28
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

//获取文件名
char* getFileName(char* fileName)
{
	char tp[100], *name = (char*)malloc(sizeof(char));
	int i, j=0;
	for(i = strlen(fileName) - 1; i >= 0; --i)
	{
		if(fileName[i] == '/')
		{
			break;
		}
	}
	for(i++; i < strlen(fileName); ++i)
	{
		tp[j++]=fileName[i];
	}
	tp[j] = '\0';
	strcpy(name, tp);
	//printf("%s\n",name);
	return name;
}

//main
int main(int argc,char * argv[])
{
	struct stat st;
	if(argc != 3)
	{
		fprintf(stdout, "参数错误:参数数量错误\n");
		return 0;
	}
	//查看参数1相关
	if(stat(argv[1], &st) == -1 || S_ISDIR(st.st_mode))
	{
		fprintf(stdout, "参数错误:参数1不是文件\n");
		return 0;
	}
	//查看参数2相关
	if(stat(argv[2], &st) != -1)
	{ 
		//查看是不是存在
		if(S_ISDIR(st.st_mode))
		{
			strcpy(argv[2]+strlen(argv[2]), "/");
			strcpy(argv[2]+strlen(argv[2]), getFileName(argv[1]));
			strcpy(argv[2]+strlen(argv[2]), "\0");
		}
		else
		{
			fprintf(stdout, "参数错误:参数2已存在\n");
			return 0;
		}
	}
	//直接使用rename
	if(rename(argv[1], argv[2]) == -1)
	{
		fprintf(stdout, "移动失败\n");
		return 0;
	}
	return 0;
}