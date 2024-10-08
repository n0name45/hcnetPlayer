#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // для usleep
#include <sys/types.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include "HCNetSDK.h"

// Функция для получения символа без ожидания нажатия Enter
int getch(void) {
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

inline unsigned long int lof(FILE *file) {
    long currentPos = ftell(file);   // Сохраняем текущую позицию
    fseek(file, 0, SEEK_END);         // Переходим в конец файла
    long length = ftell(file);        // Получаем длину файла
    fseek(file, currentPos, SEEK_SET); // Возвращаемся на предыдущую позицию
    return (unsigned long int)length;  // Возвращаем длину
}

void CALLBACK fVoiceDataCallBack(LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void *pUser) {}

#define N 160
#define S 20

int main(int argc, char *argv[]) {
    if (argc >= 6) {
        NET_DVR_Init();
        NET_DVR_DEVICEINFO_V30 struDeviceInfo;
        LONG lUserID = NET_DVR_Login_V30(argv[1], atoi(argv[2]), argv[3], argv[4], &struDeviceInfo);
        
        if (lUserID == -1) {
            printf("Login error, %d\n", NET_DVR_GetLastError());
            return -2;
        }

        LONG lVoiceHandle = NET_DVR_StartVoiceCom_MR_V30(lUserID, 1, fVoiceDataCallBack, NULL);
        
        if (lVoiceHandle < 0) {
            printf("NET_DVR_StartVoiceCom_MR_V30 fail %d\n", NET_DVR_GetLastError());
            NET_DVR_Logout_V30(lUserID);
            return -3;
        }

        FILE *fp1 = fopen(argv[5], "rb");
        if (fp1 == NULL) {
            fprintf(stderr, "Open sound file %s failed\n", argv[5]);
            return -4;
        }
        
        int i = lof(fp1);
        char *buffer = (char *)malloc(i + N);
        memset(buffer, 0, i + N);
        fread(buffer, 1, i, fp1);
        
        for (int j = 0; j < i;) {
            NET_DVR_VoiceComSendData(lVoiceHandle, buffer, N);
            usleep(S * 1000); // Используем usleep вместо Sleep
            buffer += N;
            j += N;
        }

        free(buffer - N); // Возвращаем указатель в начало
        fclose(fp1);    
        usleep(S * 50000); // Используем usleep вместо Sleep
        NET_DVR_StopVoiceCom(lVoiceHandle);
        NET_DVR_Logout_V30(lUserID);
        NET_DVR_Cleanup();
        return 0;
    }
    
    printf("Hikvision soundwave player v1.0 (c)iTuneDVR 2018-10-22\nUsage: HSWP.exe 192.168.1.251 8000 admin admin12345 output.wav\n");
}