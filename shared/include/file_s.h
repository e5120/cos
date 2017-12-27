#pragma once

typedef struct FILEINFO{
    unsigned char name[8];
    unsigned char ext[3];
    unsigned char type;
    char reserve[10];
    unsigned short time;
    unsigned short date;
    unsigned short clustno;
    unsigned int size;
}FILE_INFO;