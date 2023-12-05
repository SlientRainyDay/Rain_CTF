#include "head.h"
#include <sqlite3.h>
msg_t phead;

long long int sockfd;
sqlite3 *db;
char *zErrMsg = 0;
int  rc;
char *sql;
char shop_list[1000];

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
void delNodeFromList(msg_t *phead, long long int fd)
{
    if (phead == NULL)
        return;
    cli_t *p = &phead->cli;
    cli_t *pDel = NULL;
    /* 遍历链表找到要删除客户端的节点令pDel指向它 */
    while (p->next != NULL)
    {
        if (p->next->fd == fd)
        {
            pDel = p->next;
            break;
        }
        p = p->next;
    }
    p->next = pDel->next;
    pDel->next = NULL;
    free(pDel);
    pDel = NULL;

}
character cNode[100];
char* Int2String(long long int num,char *str)//10进制 
{
    int i = 0;//指示填充str 
    if(num<0)//如果num为负数，将num变正 
    {
        num = -num;
        str[i++] = '-';
    } 
    //转换 
    do
    {
        str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
        num /= 10;//去掉最低位    
    }while(num);//num不为0继续循环
    
    str[i] = '\0';
    
    //确定开始调整的位置 
    int j = 0;
    if(str[0]=='-')//如果有负号，负号不用调整 
    {
        j = 1;//从第二位开始调整 
        ++i;//由于有负号，所以交换的对称轴也要后移1位 
    }
    //对称交换 
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//返回转换后的值 
}
void *pthread_common(void *arg)
{
    char *info[10];
    for(int i=0;i<10;i++)
    {
        info[i]=(char *)malloc(512);
    }
    /*传递过来的pNode结构体中包含客户端的信息*/
    cli_t *pNode = (cli_t *)arg;
    for(int i=0;i<100;i++)
    {
        pNode->characters[i]=(char *)malloc(0x30);
    }
    //init users messages
    pNode->money=10000;
    pNode->characters_count=0;
    strcpy(pNode->characters[pNode->characters_count],"HRP");
    pNode->characters_count++;
    strcpy(pNode->characters[pNode->characters_count],"gxh191");
    pNode->characters_count++;
    strcpy(pNode->characters[pNode->characters_count],"YameMres");
    pNode->characters_count++;
    /*打印给服务端显示*/
    printf("客户端(%s:%d)加入服务器\n", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
    /*如果客户端链表大于1就转发某某加入聊天室的信息,并通知其他客户端*/
    if (phead.size > 1)
    {
        cli_t *p = &phead.cli;
        msg_t msg;
        bzero(&msg, 0);
        msg.cli = *pNode;
        msg.type = 0;
        sprintf(msg.message, "客户端(%s:%d)加入服务器", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
    }
    /*死循环,接收和转发消息或者处理客户端消息*/
    while (1)
    {
        msg_t msgs;
        bzero(&msgs, 0);
        long long int ret = recv(pNode->fd, &msgs, sizeof(msgs), 0);
        /* 这里对客户端链表上锁,以防止两个或多个客户端同时退出造成 */
        /* 服务端崩溃或者转发消息失败 */
        pthread_mutex_lock(&phead.lock);
        if (ret > 0)
        {
            /*先对发来quit消息的客户端进行处理
            if (!strcmp("back", msgs.message))
            {
                phead.size--;
                msgs.type = 0;
                printf("客户端(%s:%d)退出聊天室\n", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
                sprintf(msgs.message, "客户端(%s:%d)退出聊天室", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
                cli_t *p = &phead.cli;
                while (p->next != NULL)
                {
                    p = p->next;
                    if (p->fd == pNode->fd)
                    {
                        continue;
                    }
                    //send(p->fd, &msgs, sizeof(msgs), 0);
                }
                pthread_mutex_unlock(&phead.lock);
                delNodeFromList(&phead, pNode->fd);
                close(pNode->fd);
                pthread_cancel(pthread_self());
                bzero(&msgs, 0);
                break;
            }*/
            if(ret>0) 
            {
                printf("%s say:%s\n",inet_ntoa(pNode->client.sin_addr),msgs.message);
                msgs.cli = *pNode;
                cli_t *p = &phead.cli;
                cli_t *CheckChart = &phead.cli;

                /* type = 1就是为了能够客户端那边打印客户端消息 */
                msgs.type = 1; 


                int count=0;
                
                char *temp=strtok(msgs.message,":");
                strcpy(&info[count],&temp);
                
                //字符串切割，奇数下标是信息内容，偶数下标是信息标签;
                while(info[count])
                {
                    printf("%s\n",temp);
                    count++;
                    temp=strtok(NULL,":");
                    if(temp==NULL)
                    {
                        break;
                    }
                    strcpy(info[count],temp);                    

                }
                char message[500];
                strcpy(message,info[3]);
                strcpy(msgs.message,message);
                if(!strcmp(info[1],"ReadyChart"))
                {
                    while (CheckChart->next != NULL)
                    {
                        CheckChart = CheckChart->next;
                        if (CheckChart->fd == pNode->fd)
                            strcpy(CheckChart->path,"Chart");
                    }
    
                    pthread_mutex_unlock(&phead.lock);
                    continue;
                }
                else if(!strcmp(info[1],"Chart"))
                {
                    CheckChart = &phead.cli;
                    if(!strcmp(msgs.message,"back"))
                    {
                        while (CheckChart->next != NULL)
                        {
                            CheckChart = CheckChart->next;
                            if (CheckChart->fd == pNode->fd)
                                strcpy(CheckChart->path,"NoChart");
                        }
                        pthread_mutex_unlock(&phead.lock);
                        continue;
                    }
                   
                    while (p->next != NULL)
                    {
                        p = p->next;
                        printf("p->path:%s\n",p->path);
                        if (p->fd == pNode->fd)
                            //continue发全部，单发自己就直接发
                            continue;
                        if(!strcmp(p->path,"Chart"))
                        {
                            send(p->fd, &msgs, sizeof(msgs), 0);
                        }
                        
                    }
                    /* 正常发送消息结束后解锁链表 */
                    pthread_mutex_unlock(&phead.lock);
                }
                else if(!strcmp(info[1],"Login"))
                {
                    /*表中插入数据*/
                            char sql1[0x100];
                            sprintf(sql1,"INSERT INTO user (Username,Password,Statement)VALUES ('%s', '%s','customer');",info[3],info[4]);
                            printf("\n\n%s\n",sql1);
                           rc = sqlite3_exec(db, sql1, callback, 0, &zErrMsg);
                           if( rc != SQLITE_OK )
                           {
                           fprintf(stderr, "SQL error: %s\n", zErrMsg);
                              sqlite3_free(zErrMsg);
                           }else
                           {
                              fprintf(stdout, "Insert data successfully\n");
                           }
                           sql="select * from user;";
                           rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                           if( rc != SQLITE_OK )
                           {
                           fprintf(stderr, "SQL error: %s\n", zErrMsg);
                              sqlite3_free(zErrMsg);
                           }else
                           {
                              fprintf(stdout, "Insert data successfully\n");
                           }
                            strcpy(pNode->username,info[3]);
                            strcpy(pNode->password,info[4]);
                            strcpy(pNode->statement,"customer");
                            strcpy(msgs.message,"true");
                            send(pNode->fd, &msgs, sizeof(msgs), 0);
                    /* 正常发送消息结束后解锁链表 */
                    pthread_mutex_unlock(&phead.lock);
                }
               else if(!strcmp(info[1],"RollCard")&&!strcmp(info[3],"Roll")&&strcmp(info[3],"back"))
                {
                    if(pNode->money-100<0||pNode->characters_count>=50)
                    {
                       ;
                    }
                    else{
                        int a;
                        srand((unsigned)time(NULL));
                        a = rand()%10;
                        while(a>3)
                        {
                            a = rand()%10;
                        }
                        pNode->money=pNode->money-100;
                        strcpy(msgs.message,cNode[a].name);
                        strcpy(pNode->characters[pNode->characters_count],cNode[a].name);
                        pNode->characters_count++;
                        send(pNode->fd, &msgs, sizeof(msgs), 0);
                        pthread_mutex_unlock(&phead.lock);
                    }
                    
                }
                else if(!strcmp(info[1],"Shop")&&strcmp(info[3],"back"))
                {
                    if(!strcmp(info[3],"1")&&(pNode->money-1000>0))
                    {
                        pNode->money=pNode->money-1000;
                        strcpy(msgs.message,cNode[3].name);
                        strcpy(pNode->characters[pNode->characters_count],cNode[3].name);
                        pNode->characters_count++;
                        strcpy(msgs.message,"你成功购买了花花");
                        send(pNode->fd, &msgs, sizeof(msgs), 0);
                        pthread_mutex_unlock(&phead.lock);
                        
                    }
                    if(!strcmp(info[3],"2"))
                    {
                        strcpy(msgs.message,cNode[4].name);
                        strcpy(pNode->characters[pNode->characters_count],cNode[4].name);
                        pNode->characters_count++;
                        strcpy(msgs.message,"你成功购买了HRP");
                        send(pNode->fd, &msgs, sizeof(msgs), 0);
                        pthread_mutex_unlock(&phead.lock);
                        
                    }
                    if(!strcmp(info[3],"3"))
                    {
                        strcpy(msgs.message,cNode[1].name);
                        strcpy(pNode->characters[pNode->characters_count],cNode[1].name);
                        pNode->characters_count++;
                        strcpy(msgs.message,"你成功购买了gxh");
                        send(pNode->fd, &msgs, sizeof(msgs), 0);
                        pthread_mutex_unlock(&phead.lock);
                        
                    }
                    if(!strcmp(info[3],"999"))
                    {
                        char sql1[0x100];
                        sprintf(sql1,"select * from user where Username='%s' and Statement ='root'",pNode->username);
                        printf("\n\n%s\n",sql1);
                        char **tb;
                        int rows = 0;  
                        int cols = 0;
                        rc = sqlite3_get_table(db, sql1, &tb, &rows, &cols, NULL);
                        if(rows>0)
                        {
                            //exp 1'--
                            char *flag;
                                FILE *file = fopen("/home/ctf/Nep_CTF_FLAG_ONE", "r");
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
                            strcpy(msgs.message,flag);
                            send(pNode->fd, &msgs, sizeof(msgs), 0);
                            pthread_mutex_unlock(&phead.lock);
                        }
                        else{
                            strcpy(msgs.message,"flag仅供root用户");
                            send(pNode->fd, &msgs, sizeof(msgs), 0);
                            pthread_mutex_unlock(&phead.lock);
                        }
                        
                    }
                    strcpy(msgs.message,shop_list);
                    send(pNode->fd, &msgs, sizeof(msgs), 0);
                    pthread_mutex_unlock(&phead.lock);

                }
                else if(!strcmp(info[1],"BotRemoteHelp")&&strcmp(info[3],"back"))
                {
                    strcpy(msgs.message,"'远程AI协助服务正在开发中!'");
                    send(pNode->fd, &msgs, sizeof(msgs), 0);
                    pthread_mutex_unlock(&phead.lock);
                }
                else if(!strcmp(info[1],"Secret"))
                {
                    strcpy(msgs.message,info[3]);
                    int uid=atoi(info[4]);
                    if(uid>=5&&uid<1000)
                    {
                        send(uid, &msgs, sizeof(msgs), 0);
                    }
                    pthread_mutex_unlock(&phead.lock);
                }
                else if(!strcmp(info[1],"Start"))
                {
                    int cha=0;
                    int sk=0;
                    cha=atoi(info[3]);
                    sk=atoi(info[4]);
                    int check=0;
                    for(int i=0;i<5;i++)
                    {
                        if(!strcmp(pNode->characters[cha],cNode[i].name)&&( (sk>=0) && (sk<=1) &&( (cha>=0) && (cha<=pNode->characters_count))))
                        {
                            int res=cNode[4].blood-(-cNode[i].skill_hurt[sk]);
                            printf("Hurt:%d\n",res);
                            if(res<=0)
                            {
                                char *flag;
                                FILE *file = fopen("/home/ctf/Nep_CTF_FLAG_THREE", "r");
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
                                strcpy(msgs.message,flag);
                                send(pNode->fd, &msgs, sizeof(msgs), 0);
                                pthread_mutex_unlock(&phead.lock);
                                check=1;
                            }
                        }
                    }
                    if(check==0)
                    {
                        strcpy(msgs.message,"YOU LOSE!");
                        send(pNode->fd, &msgs, sizeof(msgs), 0);
                        pthread_mutex_unlock(&phead.lock);
                    }
                    
                }
                else if(!strcmp(info[1],"Message"))
                {
                    char temp[0x1000]="拥有角色列表:\n";
                    for(int i=0;i<pNode->characters_count;i++)
                    {
                        strcat(temp,pNode->characters[i]);
                        strcat(temp,"\n");
                    }
                    strcat(temp,"金钱余量：");
                    char money[100];
                    strcat(temp,Int2String(pNode->money,money));
                    strcat(temp,"\n");
                    strcat(temp,"全体角色信息:\n");
                    char temp_hurt[10];
                    for(int i=0;i<5;i++)
                    {
                        strcat(temp,"Name:\n");
                        strcat(temp,cNode[i].name);
                        strcat(temp,"\n");
                        strcat(temp,"Skill Name:\n");
                        strcat(temp,cNode[i].skill[0]);
                        strcat(temp,"\n");
                        strcat(temp,"Skill Hurt:\n");
                        strcat(temp,Int2String(cNode[i].skill_hurt[0],temp_hurt));
                        strcat(temp,"\n");
                        strcat(temp,"Skill Name:\n");
                        strcat(temp,cNode[i].skill[1]);
                        strcat(temp,"\n");
                        strcat(temp,"Skill Hurt:\n");
                        strcat(temp,Int2String(cNode[i].skill_hurt[1],temp_hurt));
                        strcat(temp,"\n");
                        strcat(temp,"Blood VALUES:\n");
                        strcat(temp,Int2String(cNode[i].blood,temp_hurt));
                        strcat(temp,"\n");
                    }
                    strcat(temp,"\n");
                    strcat(temp,"TSign[Missing_BackToBackOSpinQuad20ComboPerfectClear_Power]");
                    strcat(temp,"\n");
                    strcpy(msgs.message,temp);

                    send(pNode->fd, &msgs, sizeof(msgs), 0);
                    pthread_mutex_unlock(&phead.lock);
                }
                  else{
                    //strcpy(msgs.message,"发生错误，但是服务连接正常。");
                    //send(pNode->fd, &msgs, sizeof(msgs), 0);
                    /* 正常发送消息结束后解锁链表 */
                    pthread_mutex_unlock(&phead.lock);
                }
            }
        }
        /* 这就是ret小于等于0的情况了,表示客户端意外退出 */
        /* 其处理方法和上边quit的处理相同,不过就是发送消息改为了 */
        /* 意外退出而已 */
        else
        {
            phead.size--;
            msgs.type = 0;
            printf("客户端(%s:%d)意外退出服务器\n", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
            sprintf(msgs.message, "客户端(%s:%d)意外退出服务器", inet_ntoa(pNode->client.sin_addr), ntohs(pNode->client.sin_port));
            cli_t *p = &phead.cli;
            
            pthread_mutex_unlock(&phead.lock);
            delNodeFromList(&phead, pNode->fd);
            pthread_cancel(pthread_self());
            close(pNode->fd);
            bzero(&msgs, 0);
            break;
        }
    }
}

void *pthread_accept(void *arg)
{
    struct sockaddr_in clientSock;
    socklen_t clientLen = sizeof(struct sockaddr_in);
    printf("===========聊天室已创建===========\n");
    while (1)
    {
        long long int acceptFd = accept(sockfd, (struct sockaddr *)&clientSock, &clientLen);
        /*pNode保存客户端信息,并将这个结构体传递给common的线程*/
        /*common线程是把客户端加入到链表中之后再创建的用于接收和转发聊天*/
        cli_t *pNode = (cli_t *)malloc(sizeof(cli_t));
        pNode->client = clientSock;
        pNode->fd = acceptFd;
        pNode->next = NULL;
        /*采用头插法*/
        if (phead.size > 0)
        {
            cli_t *p = phead.cli.next;
            phead.cli.next = pNode;
            pNode->next = p;
        }
        else if (phead.size == 0)
        {
            phead.cli.next = pNode;
        }
        phead.size++;
        pthread_t commonPthread;
        /*创建聊天线程*/
        pthread_create(&commonPthread, NULL, pthread_common, (void *)pNode);
        pthread_detach(commonPthread);
    }
}
void init()
{
                        rc = sqlite3_open("users.db", &db);
                       if( rc )
                       {
                          fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                          exit(0);
                       }else
                           {
                              fprintf(stdout, "Opened database successfully\n");
                           }
                            /* 创建一个表*/

                           sql = "CREATE TABLE user("  \
                                 "Username CHAR(50) PRIMARY KEY     NOT NULL," \
                                 "Password CHAR(50) NOT NULL," \
                                 "Statement CHAR(50) NOT NULL);";
                           rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                           if( rc != SQLITE_OK )
                           {
                           fprintf(stderr, "SQL error: %s\n", zErrMsg);
                              sqlite3_free(zErrMsg);
                           }else
                           {
                              fprintf(stdout, "Table created successfully\n");
                           }
    strcpy(shop_list,"1.花花(1000)\n2.HRP(0)\n3.gxh191(0)\n");
    strcpy(cNode[0].name,"H3h3QAQ");
    cNode[0].skill[0]=(char *)malloc(0x30);
    cNode[0].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[0].skill[0],"自动化渗透木马");
    strcpy(cNode[0].skill[1],"log4j");
    cNode[0].skill_hurt[0]=10;
    cNode[0].skill_hurt[1]=2047483649;
    cNode[0].level=0;
    cNode[0].how_much=-100;
    cNode[0].blood=1;

    strcpy(cNode[1].name,"gxh191");
    cNode[1].skill[0]=(char *)malloc(0x30);
    cNode[1].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[1].skill[0],"PWN YOUR HEART");
    strcpy(cNode[1].skill[1],"数据溢出");
    cNode[1].skill_hurt[0]=15;
    cNode[1].skill_hurt[1]=40;
    cNode[1].level=3;
    cNode[1].how_much=0;
    cNode[1].blood=200;

    strcpy(cNode[2].name,"YameMres");
    cNode[2].skill[0]=(char *)malloc(0x30);
    cNode[2].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[2].skill[0],"shellcode attack");
    strcpy(cNode[2].skill[1],"数据溢出");
    cNode[2].skill_hurt[0]=50;
    cNode[2].skill_hurt[1]=40;
    cNode[2].level=4;
    cNode[2].how_much=0;
    cNode[2].blood=400;

    strcpy(cNode[3].name,"花花");
    cNode[3].skill[0]=(char *)malloc(0x30);
    cNode[3].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[3].skill[0],"免死金牌");
    strcpy(cNode[3].skill[1],"Nepnep YOU!");
    cNode[3].skill_hurt[0]=0;
    cNode[3].skill_hurt[1]=200;
    cNode[3].level=5;
    cNode[3].how_much=1000;
    cNode[3].blood=1000;

    //被动伤害*-1
    strcpy(cNode[4].name,"ThTsOd");
    cNode[4].skill[0]=(char *)malloc(0x30);
    cNode[4].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[4].skill[0],"咕咕咕");
    strcpy(cNode[4].skill[1],"杀虫剂替身");
    cNode[4].skill_hurt[0]=6666666;
    cNode[4].skill_hurt[1]=0;
    cNode[4].level=999;
    cNode[4].how_much=0;
    cNode[4].blood=99999999;

    strcpy(cNode[5].name,"Author-HRP-有问题联系QQ：201381254");
    cNode[5].skill[0]=(char *)malloc(0x30);
    cNode[5].skill[1]=(char *)malloc(0x30);
    strcpy(cNode[5].skill[0],"嘤嘤嘤");
    strcpy(cNode[5].skill[1],"开摆");
    cNode[5].skill_hurt[0]=0;
    cNode[5].skill_hurt[1]=0;
    cNode[5].level=5;
    cNode[5].how_much=99999999;
    cNode[5].blood=99999999;
    setbuf(stdin,0);
    setbuf(stdout,0);
}
int main()
{
    init();
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverSock;
    socklen_t serverLen = sizeof(struct sockaddr_in);
    serverSock.sin_addr.s_addr = htonl(INADDR_ANY);
    serverSock.sin_port = htons(PORT);
    serverSock.sin_family = AF_INET;
    bind(sockfd, (struct sockaddr *)&serverSock, serverLen);

    listen(sockfd, 0);

    pthread_t acceptPthread;
    pthread_create(&acceptPthread, NULL, pthread_accept, NULL);
    while (1)
        ;
    return 0;
}
