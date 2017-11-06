#include "asmfunc_def.h"

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

unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned end);

void memory_manager_init(MEM_MAN* man);
unsigned int memory_manager_total(MEM_MAN* man);
unsigned int memory_manager_alloc(MEM_MAN* man, unsigned int size);
int memory_manager_free(MEM_MAN* man, unsigned int addr, unsigned int size);