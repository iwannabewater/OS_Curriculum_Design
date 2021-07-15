/*
* 简单实现rm命令
* 1509寝W组
* 2021/6/28
*/
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>

int rm(char* file_name)
{
	char file_path[128];
	strcpy(file_path, file_name);
	struct stat st;  
	//找不到文件  
	if(lstat(file_path, &st) == -1)
	{
		return -1;
	}
	//是否为常规文件
	if(S_ISREG(st.st_mode))
	{
		//unlink失败
		if(unlink(file_path) == -1)
		{
			return -1;
		}    
	}
	//是否为文件夹
	else if(S_ISDIR(st.st_mode))
	{
		fprintf(stdout, "无法删除：其为一个目录\n");
		return -1;
	}
	return 0;
}

int main(int argc,char *argv[])
{
	//1个argc
	if (argc == 1)
	{
		fprintf(stdout, "请指定要删除的文件\n");
	}
	//n个argc
	else
	{
		for(int i = 1; i < argc; ++i)
		{
			if(rm(argv[i]) == -1)
		        {
				fprintf(stdout, "删除%s失败\n", argv[i]);    	
		        }
		}
	}
}