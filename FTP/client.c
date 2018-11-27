#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <argp.h>

#pragma mark                                                                        状态码
#define LOGIN           1
#define UPLOAD          002
#define DOWNLOAD        010
#define MKDIR           011
#define RMDIR           100
#define PWD             101
#define CD              110
#define LS              111
#define BYE             88

#pragma mark                                                                        端口号
#define PORT            8888
#define MAX             30
#define MMAX            1024

#pragma mark                                                                        mode
#define ASCII           1
#define BIT             2


#pragma mark                                                                        协议 头
typedef struct NYN {
    int status;                         //状态码s        // 1-----登录//001-----上传//010-----下载//011-----创建dir
    //100-----删除dir//101-----显示当前路径//110-----切换目录
    //111-----查看当前所有文件
    int mode;                           //上传下载选定模式//1-----ansic//2-----bit//3-----非上传下载
}CW;
typedef struct MCM {                    //s为1时选用
    char username[10];
    char password[10];
} MN;
typedef struct SQS {                    //s为2是选用
    char content[1024];//文件
} BX;
#pragma mark                                                                        协议 体
typedef struct NLM {
    CW _n;                               //协议头
    union {
        MN _m;
        BX _b;
    };
} BZ;


#pragma mark - write_server                                                         写线程
void write_server(int client_socket)
{
    int a ;
    while (1)
    {
        while (1) {
            printf("\n");
            interface();
            printf("\n");
            printf("\n");
            sleep(1);
            printf("\t\t*_*输入功能:\n");
            scanf("%d", &a);
            printf("\n");
            while (getchar() != '\n');
            switch (a) {
                case 1 :
                    preupload(client_socket);
                    break;
                case 2 :
                    predownload(client_socket);
                    break;
                case 3 :
                    precreat_dir(client_socket);
                case 4 :
                    logout();
            }
            system("clear");
        }

    }

}


#pragma mark -interface                                                             主界面
void interface()
{
    printf ("\t\t 1、上传文件\n");
    printf ("\t\t 2、下载文件\n");
    printf ("\t\t 3、创建目录\n");
    printf ("\t\t 4、删除目录\n");
    printf ("\t\t 5、显示当前路径\n");
    printf ("\t\t 6、显示当前文件夹\n");
    printf ("\t\t 7、切换目录\n");
    printf ("\t\t 8、退出\n");
}


#pragma mark - Upload()                                                             上传
void preupload(int client_socket){
    struct NLM tname;
    tname._n.status=UPLOAD;
    char name[1024];
    printf("文件名：");
    scanf("%s",&name);
    strcpy(tname._b.content,name);
    write(client_socket, &tname, sizeof(tname));

    Upload(client_socket,&tname);
}

void Upload(int client_socket,struct NLM *nlm){
    const char * a="can't open";

    FILE* fd;
    int nbyte;
    struct NLM tt;
    bzero(&tt, sizeof(BZ));

    printf("get filename : [ %s ]\n",nlm->_b.content);

    if (ASCII==nlm->_n.mode){
        if((fd=fopen(nlm->_b.content,"r")) < 0) {                                  //ACSII---error
            printf("Open file Error!\n");
            strcpy(tt._b.content,a);
            if(write(client_socket, &tt, sizeof(tt)) <0) {
                printf("Write Error!\n");
                return ;
            }
        }
        if (-1==chmod(nlm->_b.content,S_IRUSR|S_IWUSR|S_IROTH)){
            printf("can't set.");
        };
        while((nbyte=(int)read(client_socket, &tt, sizeof(tt))) > 0) {
            if(fwrite(tt._b.content,MMAX,1,fd) < 0) {
                printf("Write Error!\n");
                fclose(fd);
                return;
            }
        }
    }else if(BIT==nlm->_n.mode){
        if((fd=fopen(nlm->_b.content,"rb")) < 0) {                                  //ACSII---error
            printf("Open file Error!\n");
            strcpy(tt._b.content,a);
            if(write(client_socket, &tt, sizeof(tt)) <0) {
                printf("Write Error!\n");
                return ;
            }
        }
        if (-1==chmod(nlm->_b.content,S_IRUSR|S_IWUSR|S_IROTH)){
            printf("can't set.");
        };
        while((nbyte=(int)read(client_socket, &tt, sizeof(tt))) > 0) {
            if(fwrite(tt._b.content,MMAX,1,fd) < 0) {
                printf("Write Error!\n");
                fclose(fd);
                return;
            }
        }
    }
}

#pragma mark - Download()                                                             下载
void predownload(int client_socket){
    struct NLM tname;
    tname._n.status=DOWNLOAD;
    char name[1024];
    printf("文件名：");
    scanf("%s",&name);
    strcpy(tname._b.content,name);
    write(client_socket, &tname, sizeof(tname));

    Download(client_socket,&tname);
}
void Download(int client_socket,struct NLM *nlm){

    const char * a="can't open";

    FILE * fd;
    int nbyte;
    struct NLM tt;
    bzero(&tt, sizeof(BZ));

    printf("get filename : [ %s ]\n",nlm->_b.content);

    if (ASCII==nlm->_n.mode){
        if((fd=fopen(nlm->_b.content,"r")) < 0) {                                  //ACSII---error
            printf("Open file Error!\n");
            strcpy(tt._b.content,a);
            if(write(client_socket, &tt, sizeof(tt)) <0) {
                printf("Write Error!\n");
                return ;
            }
        }
        while(0<(nbyte=(int)fread(tt._b.content,MMAX,1,fd))){
            if(write(client_socket, tt._b.content, MMAX) < 0) {
                printf("Write Error! At commd_put 1!\n");
                fclose(fd);
                return;
            }
        }
    } else if (BIT==nlm->_n.mode){
        if((fd=fopen(nlm->_b.content,"rb")) < 0) {                                  //BIT---error
            printf("Open file Error!\n");
            strcpy(tt._b.content,a);
            if(write(client_socket, &tt, sizeof(tt)) <0) {
                printf("Write Error!\n");
                return ;
            }
        }
        while(0<(nbyte=(int)fread(tt._b.content,MMAX,1,fd))){
            if(write(client_socket, tt._b.content, MMAX) < 0) {
                printf("Write Error! At commd_put 1!\n");
                fclose(fd);
                return;
            }
        }
    }
}


#pragma precreat_dir()
void precreat_dir(int client_socket){
    struct NLM tmp;
    tmp._n.status=MKDIR;
    Creat_dir(client_socket,&tmp);
}
void Creat_dir(int client_socket,struct NLM *nlm){
    struct NLM tt;
    write(client_socket, &tt, sizeof(tt));
}


#pragma mark - logout                                                               退出
void logout()
{
    system("clear");
    exit(0);
}


#pragma mark - main()                                                               入口函数
int main( int argc, char** argv )
{
    // 创建与服务器通信的套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror ("client_socket");
        return -1;
    }

    // 连接服务器
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family  = AF_INET;     // 设置地址族
    addr.sin_port    = htons(PORT); // 设置本地端口
    inet_aton("127.0.0.1",&(addr.sin_addr));


    // 连接服务器，如果成功，返回0，如果失败，返回-1
    // 成功的情况下，可以通过socketfd与服务器进行通信
    int ret = connect(client_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror ("connect");
        return -1;
    }

    printf ("成功连上服务器\n");

    //开始进行操作；
    struct NLM psw;
    char t[256];
    char username[10];
    char password[10];
    char port[10];
    int i;
    int count1;
    int count2;
    while(1) {                                             //登录
        if(psw._n.status==LOGIN) {
            if (psw._b.content == '1002')break;                   //判断是否已经登录成功
            if (psw._b.content == '1003') {
                printf("密码不正确\n");
                logout();
            }
            if (psw._b.content == '1004') {
                printf("该用户不存在\n");
                logout();
            }
        }
        printf("正在验证用户名和密码\n");
        strcpy(t, argv[1]);
        int length = strlen(t);
        if (t[0]=='f'&&t[1]=='t'&&t[2]=='p'&&t[3]==':'&&t[4]=='/'&&t[5]=='/') {
            printf("ftp头格式正确\n");
            if (t[6] == '@') { printf("缺少用户名\n"); }
            count1 = 0;
            count2 = 0;
            for (i = 6; i < length; i++) {
                if (t[i] == '@')break;
                username[i - 6] = t[i];                   //获取用户名
                count1++;
            }
            for (i = 7 + count1; i < length; i++) {
                if (t[i] == ':')break;
                password[i - (7 + count1)] = t[i];        //获取密码
                count2++;
            }
            for (i = 8 + count1 + count2; i < length; i++) {
                port[i - (8 + count1 + count2)] = t[i];   //获取端口号
            }
        } else { printf("指令格式错误\n"); }
        psw._n.status = LOGIN;
        strcpy(psw._m.username,username);
        strcpy(psw._m.password,password);
        write(client_socket, &psw, sizeof(psw));     //消息写过去，等待回复，例如什么用户不存在，密码错误
        read(client_socket, &psw, sizeof(psw));      //读取来自服务器的返回的消息，进行客户端屏幕打印
    }
        printf("恭喜你登录成功\n");
        write_server(client_socket);
        // 关闭套接字
        close(client_socket);
        return 0;
    }