/*
* 简单实现cat命令
* 1509寝W组
* 2021/6/28
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

//获取内容
void copy(int fdin, int fdout)
{
	char buffer[BUFF_SIZE];
	int size;
	//读取fdin的内容放在fdout的中
	while(size = read(fdin, buffer, BUFF_SIZE))
	{
		if(write(fdout, buffer, size) != size)
		{
			fprintf(stdout, "写入错误\n");
			exit(1);
		}                                                                                                                                                 
	}
	//当读入出现问题
	if(size<0)
	{
		fprintf(stdout, "读入错误\n");
  		exit(1);
	}
}

//main
int main(int argc, char *argv[])
{
	//接收键盘输入与向屏幕输出
	int fd_in = STDIN_FILENO;
	int fd_out = STDOUT_FILENO;
	//1个argc
	if (argc == 1)
	{
		copy(fd_in, fd_out);
		close(fd_in);
	}
	//n个argc
	else
	{
		for(int i = 1; i < argc; ++i)
		{
		        //令输入文件为读入文件
			fd_in = open(argv[i], O_RDONLY);
			if(fd_in < 0)
			{
				fprintf(stdout, "打开%s文件错误\n", argv[i]);
				continue;
			}
			copy(fd_in, fd_out);
			close(fd_in);
		}
	}
	return 0;
}