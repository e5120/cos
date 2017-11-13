#pragma once

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000

#define MEMMANAGER_FREES    4090
#define MEMMANAGER_ADDR     0x003c0000

typedef struct MEMORY_FREE_INFO{
    unsigned int addr;
    unsigned int size;
}FREE_INFO;

typedef struct MEMORY_MANAGER{
    int frees;
    int maxfrees;
    int lostsize;
    int losts;
    FREE_INFO free[MEMMANAGER_FREES];
}MEM_MAN;
