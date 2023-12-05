#include "head.h"
long long int sockfd;
long long int quit = 0;
#define true 1;
pthread_t pthreadRecv;
pthread_t pthreadSend;
int login_or_not=0;
char *card_group[10];
void *shop_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                /* 打印消息同时打印是哪个客户端打印的 */
                printf("商品信息:\n%s\n", msg.message);
                
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *shop_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    sprintf(msg.message,"%s:%s:%s:%s","Action","Shop","Value","message");
    send(sockfd, &msg, sizeof(msg), 0);
    while (1)
    {
        /* 读取输入 */
        scanf("%510s", message);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s","Action","Shop","Value",message);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            /* 如果是quit的话令quit变量为true,这样主程序会结束 */
            if (!strcmp(message, "back"))
            {
                quit = true;
                memset(msg.message, 0, sizeof(msg.message));
                printf("%s\n","你已退出商店");
                return;
                
            }
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *message_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                printf("%s\n", msg.message);
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *check_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
               ;
            }
            else
            {
                ;
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", "Check failed");
            exit(0);
        }
    }
}
void *message_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    sprintf(msg.message,"%s:%s:%s:%s","Action","Message","Value","message");
    /* 安全起先还是加了一个\0 */
    long long int len = strlen(msg.message);
    msg.message[len + 1] = '\0';
    send(sockfd, &msg, sizeof(msg), 0);
    getchar();
    quit = true;
    
}
void *secret_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                printf("(UID:%d:Username:%s:%s):%s\n", (msg.cli.fd),(msg.cli.username),(msg.cli.statement), msg.message);
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *secret_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    char id[110];
    while (1)
    {
        /* 读取输入 */
        printf("%s","私聊对象UID: ");
        scanf("%100s",id);
        getchar();
        printf("%s",">>: ");
        scanf("%510s", message);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s:%s","Action","Secret","Value",message,id);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            /* 如果是quit的话令quit变量为true,这样主程序会结束 */
            if (!strcmp(message, "back"))
            {
                quit = true;
                memset(msg.message, 0, sizeof(msg.message));
                printf("%s\n","你已退出私聊");
                return;
                
            }
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *bot_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                if(!strcmp(msg.message,"RemoteVIPApplicationCertificationHasPassed"))
                {
                    printf("%s:","你已成功申请为VIP用户,请保管好您的VIP码(此码仅往后不会再出现,请妥善保存)");
                    char *vip;
                                FILE *file = fopen("/home/ctf/Nep_CTF_FLAG_TWO", "r");
                                if (file == NULL) {
                                    printf("Could not open flag file.\n");
                                    exit(1);
                                }

                                fseek(file, 0, SEEK_END);
                                long fsize = ftell(file);
                                fseek(file, 0, SEEK_SET);

                                vip = malloc(fsize + 1);
                                fread(vip, fsize, 1, file);
                                fclose(file);
                                vip[fsize] = '\0';
                    printf("%s\n",vip);

                }
                else{
                    printf("Bot say:%s\n",msg.message);
                }
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *bot_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    while (1)
    {
        /* 读取输入 */
        scanf("%510s", message);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s","Action","BotRemoteHelp","Value",message);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            /* 如果是quit的话令quit变量为true,这样主程序会结束 */
            if (!strcmp(message, "back"))
            {
                quit = true;
                memset(msg.message, 0, sizeof(msg.message));
                printf("%s\n","你已退出AI协助服务");
                return;
                
            }
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *chart_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                /* 打印消息同时打印是哪个客户端打印的 */
                printf("(UID:%d:Username:%s:%s):%s\n", (msg.cli.fd),(msg.cli.username),(msg.cli.statement), msg.message);
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *chart_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    sprintf(msg.message,"%s:%s:%s:%s","Action","ReadyChart","Value","message");
    send(sockfd, &msg, sizeof(msg), 0);
    while (1)
    {
        /* 读取输入 */
        scanf("%510s", message);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s","Action","Chart","Value",message);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            /* 如果是quit的话令quit变量为true,这样主程序会结束 */
            if (!strcmp(message, "back"))
            {
                quit = true;
                memset(msg.message, 0, sizeof(msg.message));
                printf("%s\n","你已退出聊天室");
                return;
                
            }
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *roll_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                /* 打印消息同时打印是哪个客户端打印的 */
                printf("\n恭喜你抽到了%s\n",  msg.message);
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *roll_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    while (1)
    {
        /* 读取输入 */
        scanf("%510s", message);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s","Action","RollCard","Value",message);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            /* 如果是quit的话令quit变量为true,这样主程序会结束 */
            if (!strcmp(message, "back"))
            {
                quit = true;
                memset(msg.message, 0, sizeof(msg.message));
                printf("%s\n","你已退出抽卡界面");
                return;
                
            }
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *start_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                /* 打印消息同时打印是哪个客户端打印的 */
                printf("\n%s\n",  msg.message);
                quit = true;
                return;
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *start_pthread_send(void *arg)
{
    msg_t msg;
    bzero(&msg, 0);
    char message[512];
    char skill[10];
    while (1)
    {
        /* 读取输入 */
        scanf("%510s", message);
        scanf("%10s", skill);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s:%s","Action","Start","Value",message,skill);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}
void *login_pthread_recv(void *arg)
{
    msg_t msg; // 接收消息的结构体，我会放到head.h文件中解释
    bzero(&msg, 0); // 初始化
    while (1) //循环
    {
        /* 阻塞接收函数,ret返回值用处来表明接收消息是否正常 */
        /* 同时也能知道其他客户端是否还在不在聊天室,比如下文的等于0 */
        long long int ret = recv(sockfd, &msg, sizeof(msg), 0); 
        if (ret > 0)
        {
            /* type变量是一个区分,1为正常消息,0为不正常,通常是有客户端主动正常退出 */
            if (msg.type == 1 && strlen(msg.message) > 0)
            {
                /* 打印消息同时打印是哪个客户端打印的 */
                printf("\n注册结果:%s\n",  msg.message);
                
                if(!strcmp(msg.message,"true"))
                {
                    quit = true;
                    memset(msg.message, 0, sizeof(msg.message));
                    printf("%s\n","你已退出登录注册界面");
                    login_or_not=1;
                    return;
                }
            }
            else
            {
                printf("\n%s\n", msg.message);
            }
        }
        /* 为0就是某某客户端断开连接 */
        else if (ret == 0)
        {
            printf("\n%s\n", msg.message);
        }
    }
}

void *login_pthread_send(void *arg)
{


    msg_t msg;
    bzero(&msg, 0);
    char username[6];
    char password[6];
    while (1)
    {
        /* 读取输入 */
        scanf("%6s",username);
        getchar();
        scanf("%6s",password);
        /* 防止读取到一个回车就发送 */
        if (getchar() == '\n' && msg.message[0] != '\n')
        {
            sprintf(msg.message,"%s:%s:%s:%s:%s","Action","Login","Value",username,password);
            /* 安全起先还是加了一个\0 */
            long long int len = strlen(msg.message);
            msg.message[len + 1] = '\0';
            send(sockfd, &msg, sizeof(msg), 0);
            memset(msg.message, 0, sizeof(msg.message));
        }
    }
}

struct sockaddr_in serverSock;
void start_me() {
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    
    while (1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("Socket creation failed");
            exit(0);
        }
        
        
        socklen_t serverLen = sizeof(struct sockaddr_in);
        serverSock.sin_addr.s_addr = htonl(INADDR_ANY);
        serverSock.sin_port = htons(PORT);
        serverSock.sin_family = AF_INET;

        if (connect(sockfd, (struct sockaddr *)&serverSock, serverLen) == -1) {
            perror("Connection failed");
            exit(0);
            close(sockfd);
            sleep(1);  // Wait before attempting to reconnect
            continue;  // Retry the connection
        }

        printf("%s\n", "欢迎来到NepCTF CardFight，在这里你可以进行大厅聊天以及卡牌对战，当你胜利后会获取到金币码，金币足够后可以进行商店物品兑换(flag暂未上架)");
        printf("%s\n", "你可以输入help来获取帮助");

        menu();
        
        close(sockfd);
        sleep(1);  // Wait before attempting to reconnect
    }
}
void help()
{
    printf("%s\n","输入Login进行登录和注册,数据存储在sqlite中请不要担心丢失，输入back返回上级菜单");
    printf("%s\n","输入Chart进入大厅聊天室(服务器不支持空格)，输入back返回菜单选择");
    printf("%s\n","输入Start可以进行卡牌对战，输入back返回菜单选择");
    printf("%s\n","输入Shop进入商店，输入back返回菜单选择");
    printf("%s\n","输入Message获取个人数据，输入back返回上级菜单");
    printf("%s\n","输入Secret进入私聊模式，输入back返回上级菜单");
    printf("%s\n","输入RollCard进行抽卡，输入back返回上级菜单");
    printf("%s\n","输入Bot获取远程AI协助,输入back返回上级菜单");
    printf("%s\n","sqlite只存储用户名密码");
}

void Check()
{
    pthread_create(&pthreadSend, NULL, message_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, check_pthread_recv, NULL);
    while (!quit)
        ;
}
void Start()
{
     printf("%s\n","欢迎进入战斗模式,你只有一回次出手机会,请分别按照角色下标,角色技能下标进行攻击大BOSS T佬。");
    pthread_create(&pthreadSend, NULL, start_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, start_pthread_recv, NULL);
    while (!quit)
        ;
}
void Secret()
{
    printf("%s\n","欢迎进入私聊模式");
    pthread_create(&pthreadSend, NULL, secret_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, secret_pthread_recv, NULL);
    while (!quit)
        ;
}
void Chart()
{

    printf("%s\n","欢迎加入聊天室");
    pthread_create(&pthreadSend, NULL, chart_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, chart_pthread_recv, NULL);
    while (!quit)
        ;
}
void RollCard()
{
    printf("%s\n","欢迎来到抽卡界面，我们目前只支持单抽呢亲,一次100Nep币");
    printf("%s","输入Roll抽卡 >>: ");
    pthread_create(&pthreadSend, NULL, roll_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, roll_pthread_recv, NULL);
    while (!quit)
        ;
}
void Login()
{
    printf("%s\n","欢迎注册&登录Nep CardFight system");
    printf("%s\n","请按次序输入用户名和密码(均为6个字符)");
    pthread_create(&pthreadSend, NULL, login_pthread_send, NULL);
    pthread_create(&pthreadRecv, NULL, login_pthread_recv, NULL);

    while (!quit)
        ;
}
void Shop()
{
    printf("%s\n","欢迎来到Nep商店");
    pthread_create(&pthreadSend,NULL,shop_pthread_send,NULL);
    pthread_create(&pthreadRecv,NULL,shop_pthread_recv,NULL);
    while (!quit)
        ;
}
void Bot()
{
    printf("%s\n","欢迎使用远程AI协助功能");
    pthread_create(&pthreadSend,NULL,bot_pthread_send,NULL);
    pthread_create(&pthreadRecv,NULL,bot_pthread_recv,NULL);
    while (!quit)
        ;
}
void Message()
{
    pthread_create(&pthreadSend,NULL,message_pthread_send,NULL);
    pthread_create(&pthreadRecv,NULL,message_pthread_recv,NULL);
    while (!quit)
        ;
}

void menu()
{
    char buf[1024];
    msg_t msg;
    bzero(&msg, 0);
    
    while(1)
    {
        quit=0;
        
        scanf("%1024s",buf);
        getchar();
        if(!strcmp(buf,"help"))
        {
            help();
        }
        if(!strcmp(buf,"Chart"))
        {   
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Chart();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"RollCard"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            RollCard();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Shop"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Shop();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Login"))
        {
            Login();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Bot"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Bot();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Secret"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Secret();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
         if(!strcmp(buf,"Message"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Message();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Start"))
        {
            if(login_or_not==0)
            {
                printf("%s\n","请登录!");
                continue;
            }
            Start();
            if(quit)
            {
                pthread_cancel(pthreadRecv);
                pthread_cancel(pthreadSend);
            }
        }
        if(!strcmp(buf,"Check"))
        {
            Check();
        }
    }
    

    
    
    return 0;
}

int main(long long int argc, char const *argv[])
{

    start_me();
}
