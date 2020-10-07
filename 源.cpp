#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<random>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define MAX 65534

unsigned char S[256]; //����S
char T[256];    //����T
int Key[256];   //������ɵ���Կ
int KeyStream[MAX]; //��Կ
char PlainText[MAX];
char CryptoText[MAX];
const char* WordList = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

void init_S()
// ��ʼ��S;
{
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
}

void init_Key() {
   

    for (int i = 0; i < 256; i++) {   //��ʼ��T[]
        T[i] = WordList[i % 62];
    }


}

void  permute_S()
{
    // �û�S;
    int temp;
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}


//��ͨģʽ�µļ��ܺ���
bool Encryption(unsigned char* S, long size, char* file_name,int mode)//�ӽ����ļ� ��Ϊ�ǶԺ����㣬����ֻ��Ҫһ������,��mode���Ƽ��ܻ��ǽ��� 
{
    init_S();
    init_Key();
    permute_S();
    FILE* pr;                      //������ļ�ָ��pr 
    pr = fopen(file_name, "rb");    //ֻ�� 	
    if (pr == NULL)                 //������ļ��������˳�
    {
        printf( "�û������ļ���ʧ�ܣ�\n" );
        return false;
    }

    FILE* pw;                      //����д�ļ�ָ��pw
    unsigned char* data;			           //�洢ԭʼ�ļ����ݣ�������������г䵱���������ԭ���е������    
    unsigned char i, j, temp;
    long num;
    data = (unsigned char*)malloc(size * sizeof(unsigned char));//�����ڴ� 

    //pw = fopen("", "wb");
    if (mode == 1)
       pw = fopen("encrypted.txt", "wb");    //ģʽΪֻд��ˢ���ļ� 
    else
       pw = fopen("decrypted.txt", "wb");    //ģʽΪֻд��ˢ���ļ�

    if (pw == NULL)                            //������ļ��������˳�
    {
       printf( "����ļ�ʱ����\n" );
        return false;
    }
    fread(data, sizeof(char), size, pr);//��ȡ��������(��Ҫ��/���ܵ��ļ�) 
    for (num = 0, i = 0, j = 0; num < size; num++) //��λ�������� 
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
      printf( "������ɣ�����encryption.txt�ļ�\n" );
   else
      printf( "������ɣ�����decryption.txt�ļ�\n" );

    fwrite(data, sizeof(char), size, pw);//д��������� 
    fclose(pw);//�ر��ļ��� 
    fclose(pr);
    free(data);//�ͷ��ڴ� 
    return true;
}

//����ģʽ�µļ��ܺ���
bool EncryptionSpeedMode(unsigned char* S, long size, char* file_name, int mode)//�ӽ����ļ� ��Ϊ�ǶԺ����㣬����ֻ��Ҫһ������,��mode���Ƽ��ܻ��ǽ��� 
{
    FILE* pr;                      //������ļ�ָ��pr 
    pr = fopen(file_name, "rb");    //ֻ�� 	
    time_t t1,t2;
    if (pr == NULL)                 //������ļ��������˳�
    {
        printf("�û������ļ���ʧ�ܣ�\n");
        return false;
    }

    FILE* pw;                      //����д�ļ�ָ��pw
    unsigned char* data;			           //�洢ԭʼ�ļ����ݣ�������������г䵱���������ԭ���е������    
    unsigned char i, j, temp;
    long num;
    data = (unsigned char*)malloc(size * sizeof(unsigned char));//�����ڴ� 

    //pw = fopen("", "wb");
    if (mode == 1)
        pw = fopen("encrypted.txt", "wb");    //ģʽΪֻд��ˢ���ļ� 
    else
        pw = fopen("decrypted.txt", "wb");    //ģʽΪֻд��ˢ���ļ�

    if (pw == NULL)                            //������ļ��������˳�
    {
        printf("����ļ�ʱ����\n");
        return false;
    }
    fread(data, sizeof(char), size, pr);//��ȡ��������(��Ҫ��/���ܵ��ļ�) 
    
    t1 = clock();//��ʼ����
    
        init_S();
        init_Key();
        permute_S();
        for (num = 0, i = 0, j = 0; num < size; num++) //��λ�������� 
        {
            temp = S[i];
            j = (j + temp) % 256;
            S[i] = S[j];
            S[j] = temp;
            temp = (temp + S[i]) % 256;
            data[num] ^= S[temp];
            i += 1;
        }
  
    t2 = clock();//��������

   if (mode == 1)
        printf("������ɣ�����encryption�ļ�\n");
    else
        printf("������ɣ�����decryption�ļ�\n");

    fwrite(data, sizeof(unsigned char), size, pw);//д��������� 
    printf("RC4�㷨������ʱ��Ϊ%f ms\n", difftime(t2, t1));
    fclose(pw);//�ر��ļ��� 
    fclose(pr);
    free(data);//�ͷ��ڴ� 
    return true;
}


long file_size(char *filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("�ļ���ʧ��");
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
    printf("��ѡ��RC4����ģʽ��\n 0�������Ե���ͨ����ģʽ��\n 1�������ԵĲ���ģʽ");
    scanf("%d", &speedmode);
    if (speedmode == 0) {
        char filename1[] = "test.txt";//��Ҫ���ܵ��ļ�
        char filename2[] = "encrypted.txt";//��Ҫ���ܵ��ļ�
        long filesize = file_size(filename1);
        Encryption(S, filesize, filename1, 1);//����
        Encryption(S, filesize, filename2, 2);//����
    }
    else {
        char filename1[] = "test.txt";//��Ҫ���ܵ��ļ�
        char filename2[] = "encrypted.txt";//��Ҫ���ܵ��ļ�
        long filesize = file_size(filename1);
        EncryptionSpeedMode(S, filesize, filename1, 1);//����
        EncryptionSpeedMode(S, filesize, filename2, 2);//����
    }
    /*char filename1[] = "test.txt";//��Ҫ���ܵ��ļ�
    char filename2[] = "encrypted.txt";//��Ҫ���ܵ��ļ�
    long filesize = file_size(filename1);
    EncryptionSpeedMode(S, filesize, filename1, 1);//����*/
    return 0;
}
