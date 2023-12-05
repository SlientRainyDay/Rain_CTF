#include<stdio.h>
#include <math.h>
#include <stdio.h>
#include<unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>

char users_statment[20]="CUSTOMER";
void print_dir(char *path, int depth)
{
    struct dirent **name_list;
    int n = scandir(path,&name_list,0,alphasort);
        if(n < 0)
        { 
        printf( "scandir return %d \n",n);
        }
        else
        {
            int index=2;
            while(index < n)
            {
                printf("%s\n", name_list[index]->d_name);
                free(name_list[index++]);
            }
            free(name_list);
    }
}
void start_client() {
    printf("%s\n", "为缓解服务器压力客户端将以安全模式运行中");
    
    FILE *fp;
    char buffer[8000];
    int status;
    
    // 创建子进程
    fp = popen("/home/ctf/client", "r");
    if (fp == NULL) {
        perror("popen");
        exit(0);
    }
    
    // 持续交互直到子进程崩溃
    while (1) {
        // 读取子进程输出
        if (fgets(buffer, sizeof(buffer), fp) == NULL) {
            // 子进程崩溃或结束
            break;
        }
        
        // 打印子进程输出
        printf("%s", buffer);
    }
    
    // 获取子进程退出状态
    status = pclose(fp);
    
    // 检查子进程退出状态
    if (WIFEXITED(status)) {
        printf("子进程正常退出，退出码：%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("子进程被信号终止，信号：%d\n", WTERMSIG(status));
    }
}

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
}
void ls()
{
    char *now_dir, pwd[2]=".";

    now_dir=pwd;
        
    print_dir(now_dir,0);
}
void CMD()
{
    char choice[30];
    while(1)
    {
        printf("%s","(Safe Mode)#");
        scanf("%30s",choice);
        getchar();
        if(!strcmp(choice,"ls"))
        {
            ls();
        }
        if(!strcmp(choice,"id"))
        {
            printf("ID:%s\n",users_statment);
        }
        if(!strcmp(choice,"Safe_Mode_Key"))
        {
            printf("%s","This is your key!");
            char *flag;
                                FILE *file = fopen("/home/ctf/Nep_CTF_FLAG_FOUR", "r");
                                if (file == NULL) {
                                    printf("Could not open flag file.\n");
                                    exit(1);
                                }

                                fseek(file, 0, SEEK_END);
                                long fsize = ftell(file);
                                fseek(file, 0, SEEK_SET);

                                flag = malloc(fsize + 1);
                                fread(flag, fsize, 1, file);
                                fclose(file);
                                flag[fsize] = '\0';
            printf("%s\n",flag);
        }

    }
}
int main(int argc, char const *argv[])
{
        init();
        start_client();
        fflush(stdout);
        fflush(stdin);
        printf("%s\n","程序运行时崩溃是否重新启动:)?");
        char choice[2];
        scanf("%1s",choice);
        if(!strcmp(choice,"Y"))
        {
           start_client();
        }
        else{
            CMD();
        }
    return 0;
}