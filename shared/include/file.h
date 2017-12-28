#pragma once
#include "file_s.h"

FILE_INFO *file_search(char *name, FILE_INFO *finfo, int max);

void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
