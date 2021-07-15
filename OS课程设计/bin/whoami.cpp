/*
* 简单实现whoami命令
* 1509寝W组
* 2021/6/27
*/
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>

int main(int argc, char* argv[])
{
	struct passwd* pass;
	pass = getpwuid(getuid());
	printf("%s\n",pass->pw_name);
	return 0;
}