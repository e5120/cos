#include "../include/memory.h"

unsigned int memtest(unsigned int start, unsigned int end){
  char flag486 = 0;
  unsigned int eflag, cr0, i;

  eflag = io_load_eflags();
  eflag |= EFLAGS_AC_BIT;
  io_store_eflags(eflag);
  if((eflag & EFLAGS_AC_BIT) != 0){
    flag486 = 1;
  }
  eflag &= ~EFLAGS_AC_BIT;
  io_store_eflags(eflag);

  if(flag486 != 0){
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE;
    store_cr0(cr0);
  }

  i = memtest_sub(start, end);

  if(flag486 != 0){
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE;
    store_cr0(cr0);
  }
  return i;
}

/*
unsigned int memtest_sub(unsigned int start, unsigned end){
  unsigned int i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
  // 4KBずつメモリが使えるかチェック
  for(i = start; i <= end; i += 0x1000){
    p = (unsigned int*)(i + 0xffc);   // 4KBのうち末尾の4バイトをチェック
    old = *p;
    *p = pat0;
    *p ^= 0xffffffff;
    if(*p != pat1){
not_memory:
      *p = old;
      break;
    }
    *p ^= 0xffffffff;
    if(*p != pat0){
      goto not_memory;
    }
    *p = old;
  }
  return i;
}
*/

void memory_manage_init(MEM_MAN* man){
  man->frees = 0;       // 空き情報
  man->maxfrees = 0;    // 状況観察用
  man->lostsize = 0;    // 解放に失敗した合計サイズ
  man->losts = 0;       // 解放に失敗した回数
  return;
}

unsigned int memory_manage_total(MEM_MAN* man){
  unsigned int i, t = 0;
  for(i = 0; i < man->frees; ++i){
    t += man->free[i].size;
  }
  return t;
}

unsigned int memory_manage_alloc(MEM_MAN* man, unsigned int size){
  unsigned int i, a;
  for(i = 0; i < man->frees; ++i){
    if(man->free[i].size >= size){
      a = man->free[i].addr;
      man->free[i].addr += size;
      man->free[i].size -= size;
      if(man->free[i].size == 0){
        --man->frees;
        for(; i < man->frees; ++i){
          man->free[i] = man->free[i + 1];
        }
      }
      // 確保
      return a;
    }
  }
  // 空きが無い
  return 0;
}

int memory_manage_free(MEM_MAN* man, unsigned int addr, unsigned int size){
  // 解放
  int i, j;
  // アドレス順になるように解放する場所を決定
  for(i = 0; i < man->frees; ++i){
    if(man->free[i].addr > addr){
      break;
    }
  }
  if(i > 0){
    //前がある
    if(man->free[i - 1].addr + man->free[i-1].size == addr){
      // 前の空き領域にまとめられる
      man->free[i - 1].size += size;
      if(i < man->frees){
        // 後ろもある
        if(addr + size == man->free[i].addr){
          // 後ろの空き領域ともまとめられる
          man->free[i - 1].size += man->free[i].size;
          // man->free[i]の削除　＆　free[i]がなくなったので前へ詰める
          --man->frees;
          for(; i < man->frees; ++i){
            man->free[i] = man->free[i + 1];
          }
        }
      }
      return 0;
    }
  }
  // 前とはまとめられない
  if (i < man->frees){
    // 後ろとはまとめられる
    if(addr + size == man->free[i].addr){
      man->free[i].addr = addr;
      man->free[i].size += size;
      return 0;
    }
  }
  // 後ろともまとめられない
  if(man->frees < MEMMANAGER_FREES){
    for(j = man->frees; j > i; --j){
      man->free[j] = man->free[j - 1];
    }
    ++man->frees;
    if(man->maxfrees < man->frees){
      man->maxfrees = man->frees;
    }
    man->free[i].addr = addr;
    man->free[i].size = size;
    return 0;   // 正常終了
  }
  // 後ろにずらせなかった
  ++man->losts;
  man->lostsize += size;
  return -1;    // 失敗終了
}

unsigned int memory_manage_alloc_4k(MEM_MAN* man, unsigned int size){
    unsigned int a;
    size = (size + 0xfff) & 0xfffff000;
    a = memory_manage_alloc(man, size);
    return a;
}

int memory_manage_free_4k(MEM_MAN* man, unsigned int addr, unsigned int size){
  int i;
  size = (size + 0xfff) & 0xfffff000;
  i = memory_manage_free(man, addr, size);
  return i;
}

