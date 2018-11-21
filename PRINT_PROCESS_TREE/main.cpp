/*本来是要用模板类写链表的 ，但是在那个链接的时候出现了，后来知道怎么解决了,但是因为考试没时间。
所以只能偷鸡一波，时间没那么紧的话我就能做出来了。只是这个是偷鸡版的。*/
#include <dirent.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#define  length (sizeof(st)/sizeof(st[0]))
using namespace std;
int countlines(char *filename){
    ifstream readfile;
    int n=0;
    string tmp;
    readfile.open(filename,ios::in);
    if (readfile.fail()){
        return 0;
    }
    else{
        while(getline(readfile,tmp,'\n'))
            n++;
        readfile.close();
    }
    return n;

}
string readline(char *filename,int line){
    int lines,i=0;
    string temp;
    fstream file;
    file.open(filename,ios::in);
    lines=countlines(filename);
    if(line<0)
        return "错误行数，不得小于零";
    if(file.fail())
        return "文件不存在";
    if(line>lines)
        return "超出范围";
    while(getline(file,temp)&&i<line-1)
        i++;
    file.close();
    return temp;

}
string cutoff(char *filename,int o){
    string  str;
    string::size_type s;

    str=readline(filename,o);
    if(str=="文件不存在")
        return "false";
    s=str.find('\t');
    if(s == std::string::npos)
        cout<<"not found"<<endl;
    else
        str=str.substr(s).erase(0,1);//擦除制表符
        str=str.erase(str.find_last_not_of(" ") + 1);//擦除字符串后的空格
        return str;//擦除制表符
}
int check(){
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];//大小随意，能装下cmdline文件的路径即可
    char filetext[1000];//大小随意，能装下要识别的命令行文本即可
    int n=0;
    dir=opendir("/proc");//打开路径
    if(NULL!=dir) {
        //循环读取路径下的每个文件和文件夹
        while (NULL != (ptr = readdir(dir))) {
            if (0 == strcmp(ptr->d_name, ".") || (0 == strcmp(ptr->d_name, "..")))continue;
            if (0 == strcmp(ptr->d_name, "acpi") || (0 == strcmp(ptr->d_name, "bus")))continue;
            if (0 == strcmp(ptr->d_name, "fs") || (0 == strcmp(ptr->d_name, "driver")))continue;
            if (0 == strcmp(ptr->d_name, "irq") || (0 == strcmp(ptr->d_name, "mpt")))continue;
            //if (0==strcmp(ptr->d_name,"self")||(0==strcmp(ptr->d_name,"thread-self")))continue;
            if (0 == strcmp(ptr->d_name, "sys") || (0 == strcmp(ptr->d_name, "sysvipc")))continue;
            if (0 == strcmp(ptr->d_name, "net") || (0 == strcmp(ptr->d_name, "tty")))continue;
            if (0 == strcmp(ptr->d_name, "version"))continue;
            n++;
        }
        closedir(dir);
    }
    return n;
}



int main(){
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];//大小随意，能装下cmdline文件的路径即可
    char filetext[1000];//大小随意，能装下要识别的命令行文本即可
    int i=0;
    int check1;//存储数组长度
    check1=check();
    struct store{
        string name;
        string pid;
        string ppid;

    }st[check1];
    dir=opendir("/proc");//打开路径
    if(NULL!=dir){
        //循环读取路径下的每个文件和文件夹

        while(NULL!=(ptr=readdir(dir))){
            if (0==strcmp(ptr->d_name,".")||(0==strcmp(ptr->d_name,"..")))continue;
            if (0==strcmp(ptr->d_name,"acpi")||(0==strcmp(ptr->d_name,"bus")))continue;
            if (0==strcmp(ptr->d_name,"fs")||(0==strcmp(ptr->d_name,"driver")))continue;
            if (0==strcmp(ptr->d_name,"irq")||(0==strcmp(ptr->d_name,"mpt")))continue;
            //if (0==strcmp(ptr->d_name,"self")||(0==strcmp(ptr->d_name,"thread-self")))continue;
            if (0==strcmp(ptr->d_name,"sys")||(0==strcmp(ptr->d_name,"sysvipc")))continue;
            if (0==strcmp(ptr->d_name,"net")||(0==strcmp(ptr->d_name,"tty")))continue;
            if (0==strcmp(ptr->d_name,"version"))continue;
            ELOOP;
                sprintf(filepath,"/proc/%s/status",ptr->d_name);
            /*cout<<readline(filepath,1)<<endl;
            cout<<readline(filepath,6)<<std::endl;
            cout<<readline(filepath,7)<<std::endl;*/
            if(cutoff(filepath,1)=="false"){continue;}
            if(cutoff(filepath,1)=="open"){goto bu;}//神奇的goto解决了莫名其妙的进程号
            else{ cout<<cutoff(filepath,1)<<endl;
            st[i].name=cutoff(filepath,1);
            }if(cutoff(filepath,6)=="false"){ continue;}
            else{cout<<cutoff(filepath,6)<<endl;
            st[i].pid=cutoff(filepath,6);
            }if(cutoff(filepath,7)=="false"){ continue;}
            else{cout<<cutoff(filepath,7)<<endl;
            st[i].ppid=cutoff(filepath,7);
            }
            bu:
            i++;
            continue;

            //if((char ch=cutoff(filepath,1))!="false")cout<<
        }
        closedir(dir);//关闭路径
    }
    cout<<"进程数目:"<<check()<<endl;
    /*测试结构体是否完好*/
    for(int i=0;i<length;i++){
        if(sizeof(st[i])!=NULL){
            cout<<st[i].name<<" "<<st[i].pid+"  "<<st[i].ppid+" "<<endl;
        } else{break;}
    }
    /*使用了偷鸡方法，可以避免使用数据结构*/
    for(int i=0;i<length;i++){
        if(st[i].ppid=="0"){
            cout<<st[i].name<<endl;
            for(int j=i+1;j<length;j++){
                if(st[j].ppid==st[i].pid){
                    cout<<"-------------"+st[j].name+"      "+st[j].pid+"       "+st[j].ppid<<endl;
                    for(int z=j+1;z<length;z++){
                        if(st[z].ppid==st[j].pid){
                            cout<<"--------------------------"+st[z].name+"      "+st[z].pid+"       "+st[z].ppid<<endl;
                            for(int k=z+1;k<length;k++){
                                if(st[k].ppid==st[j].pid){
                                    cout<<"---------------------------------------"+st[k].name+"      "+st[k].pid+"       "+st[k].ppid<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}