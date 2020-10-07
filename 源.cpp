#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<random>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define MAX 65534

unsigned char S[256]; //向量S
char T[256];    //向量T
int Key[256];   //随机生成的密钥
int KeyStream[MAX]; //密钥
char PlainText[MAX];
char CryptoText[MAX];
const char* WordList = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

void init_S()
// 初始化S;
{
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
}

void init_Key() {
   

    for (int i = 0; i < 256; i++) {   //初始化T[]
        T[i] = WordList[i % 62];
    }


}

void  permute_S()
{
    // 置换S;
    int temp;
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}


//普通模式下的加密函数
bool Encryption(unsigned char* S, long size, char* file_name,int mode)//加解密文件 因为是对合运算，所以只需要一个函数,用mode控制加密还是解密 
{
    init_S();
    init_Key();
    permute_S();
    FILE* pr;                      //定义读文件指针pr 
    pr = fopen(file_name, "rb");    //只读 	
    if (pr == NULL)                 //如果打开文件错误，则退出
    {
        printf( "用户输入文件打开失败！\n" );
        return false;
    }

    FILE* pw;                      //定义写文件指针pw
    unsigned char* data;			           //存储原始文件数据，并在运算过程中充当输出序列与原序列的异或结果    
    unsigned char i, j, temp;
    long num;
    data = (unsigned char*)malloc(size * sizeof(unsigned char));//分配内存 

    //pw = fopen("", "wb");
    if (mode == 1)
       pw = fopen("encrypted.txt", "wb");    //模式为只写且刷新文件 
    else
       pw = fopen("decrypted.txt", "wb");    //模式为只写且刷新文件

    if (pw == NULL)                            //如果打开文件错误，则退出
    {
       printf( "输出文件时出错！\n" );
        return false;
    }
    fread(data, sizeof(char), size, pr);//读取二进制流(需要加/解密的文件) 
    for (num = 0, i = 0, j = 0; num < size; num++) //逐位生成序列 
    {
        temp = S[i];
        j = (j + temp) % 256;
        S[i] = S[j];
        S[j] = temp;
        temp = (temp + S[i]) % 256;
        data[num] ^= S[temp];
        i += 1;
    }

   if (mode == 1)
      printf( "功能完成，生成encryption.txt文件\n" );
   else
      printf( "功能完成，生成decryption.txt文件\n" );

    fwrite(data, sizeof(char), size, pw);//写入二进制流 
    fclose(pw);//关闭文件流 
    fclose(pr);
    free(data);//释放内存 
    return true;
}

//测速模式下的加密函数
bool EncryptionSpeedMode(unsigned char* S, long size, char* file_name, int mode)//加解密文件 因为是对合运算，所以只需要一个函数,用mode控制加密还是解密 
{
    FILE* pr;                      //定义读文件指针pr 
    pr = fopen(file_name, "rb");    //只读 	
    time_t t1,t2;
    if (pr == NULL)                 //如果打开文件错误，则退出
    {
        printf("用户输入文件打开失败！\n");
        return false;
    }

    FILE* pw;                      //定义写文件指针pw
    unsigned char* data;			           //存储原始文件数据，并在运算过程中充当输出序列与原序列的异或结果    
    unsigned char i, j, temp;
    long num;
    data = (unsigned char*)malloc(size * sizeof(unsigned char));//分配内存 

    //pw = fopen("", "wb");
    if (mode == 1)
        pw = fopen("encrypted.txt", "wb");    //模式为只写且刷新文件 
    else
        pw = fopen("decrypted.txt", "wb");    //模式为只写且刷新文件

    if (pw == NULL)                            //如果打开文件错误，则退出
    {
        printf("输出文件时出错！\n");
        return false;
    }
    fread(data, sizeof(char), size, pr);//读取二进制流(需要加/解密的文件) 
    
    t1 = clock();//开始测速
    
        init_S();
        init_Key();
        permute_S();
        for (num = 0, i = 0, j = 0; num < size; num++) //逐位生成序列 
        {
            temp = S[i];
            j = (j + temp) % 256;
            S[i] = S[j];
            S[j] = temp;
            temp = (temp + S[i]) % 256;
            data[num] ^= S[temp];
            i += 1;
        }
  
    t2 = clock();//结束测速

   if (mode == 1)
        printf("功能完成，生成encryption文件\n");
    else
        printf("功能完成，生成decryption文件\n");

    fwrite(data, sizeof(unsigned char), size, pw);//写入二进制流 
    printf("RC4算法的运行时间为%f ms\n", difftime(t2, t1));
    fclose(pw);//关闭文件流 
    fclose(pr);
    free(data);//释放内存 
    return true;
}


long file_size(char *filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("文件打开失败");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

int main()
{
   
    int speedmode = 0;
    printf("请选择RC4加密模式：\n 0：带回显的普通加密模式；\n 1：带回显的测速模式");
    scanf("%d", &speedmode);
    if (speedmode == 0) {
        char filename1[] = "test.txt";//需要加密的文件
        char filename2[] = "encrypted.txt";//需要解密的文件
        long filesize = file_size(filename1);
        Encryption(S, filesize, filename1, 1);//加密
        Encryption(S, filesize, filename2, 2);//解密
    }
    else {
        char filename1[] = "test.txt";//需要加密的文件
        char filename2[] = "encrypted.txt";//需要解密的文件
        long filesize = file_size(filename1);
        EncryptionSpeedMode(S, filesize, filename1, 1);//加密
        EncryptionSpeedMode(S, filesize, filename2, 2);//解密
    }
    /*char filename1[] = "test.txt";//需要加密的文件
    char filename2[] = "encrypted.txt";//需要解密的文件
    long filesize = file_size(filename1);
    EncryptionSpeedMode(S, filesize, filename1, 1);//加密*/
    return 0;
}
