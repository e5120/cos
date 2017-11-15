#include "../include/layer.h"

LAYER_CTL* layer_control_init(MEM_MAN* memman, unsigned char *vram, int xsize, int ysize){
  LAYER_CTL* ctl;
  int i;
  ctl = (LAYER_CTL*)memory_manage_alloc_4k(memman, sizeof(LAYER_CTL));
  if(ctl == 0){
    goto err;
  }
  ctl->map = (unsigned char*)memory_manage_alloc_4k(memman, xsize * ysize);
  if(ctl->map == 0){
    memory_manage_free(memman, (int)ctl, sizeof(LAYER_CTL));
    goto err;
  }
  ctl->vram = vram;
  ctl->xsize = xsize;
  ctl->ysize = ysize;
  ctl->top = -1;
  for(i = 0; i < MAX_LAYERS; ++i){
    ctl->sheets0[i].flags = 0;
    ctl->sheets0[i].ctl = ctl;
  }

err:
  return ctl;
}

LAYER* layer_alloc(LAYER_CTL* ctl){
  LAYER *layer;
  int i;
  for(i = 0; i < MAX_LAYERS; ++i){
    if(ctl->sheets0[i].flags == 0){
      layer = &ctl->sheets0[i];
      layer->flags = LAYER_USED;
      layer->height = -1;
      return layer;
    }
  }
  return 0;
}

void layer_setbuf(LAYER* layer, unsigned char* buf, int xsize, int ysize, int col_inv){
  layer->buf = buf;
  layer->bxsize = xsize;
  layer->bysize = ysize;
  layer->color_inv = col_inv;
  return;
}

void layer_refreshmap(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0){
  int h, bx, by, vx, vy, bx0, by0, bx1, by1;
  unsigned char *buf, sid, *map = ctl->map;
  LAYER* layer;
  if(vx0 < 0){
    vx0 = 0;
  }
  if(vy0 < 0){
    vy0 = 0;
  }
  if(vx1 > ctl->xsize){
    vx1 = ctl->xsize;
  }
  if(vy1 > ctl->ysize){
    vy1 = ctl->ysize;
  }
  for(h = h0; h <= ctl->top; ++h){
    layer = ctl->sheets[h];
    // アドレスを引き算し、その結果を各レイヤーの番号として使用
    sid = layer - ctl->sheets0;
    buf = layer->buf;
    bx0 = vx0 - layer->vx;
    by0 = vy0 - layer->vy;
    bx1 = vx1 - layer->vx;
    by1 = vy1 - layer->vy;
    if(bx0 < 0){
      bx0 = 0;
    }
    if(by0 < 0){
      by0 = 0;
    }
    if(bx1 > layer->bxsize){
      bx1 = layer->bxsize;
    }
    if(by1 > layer->bysize){
      by1 = layer->bysize;
    }
    for(by = by0; by < by1; ++by){
      vy = layer->vy + by;
      for(bx = bx0; bx < bx1; ++bx){
        vx = layer->vx + bx;
        if(buf[by * layer->bxsize + bx] != layer->color_inv){
          map[vy * ctl->xsize + vx] = sid;
        }
      }
    }
  }
  return;
}

void layer_refreshsub(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1){
  int h, bx, by, vx, vy, bx0, by0, bx1, by1;
  unsigned char *buf, *vram = ctl->vram;
  unsigned char *map = ctl->map, sid;
  LAYER *layer;

  if(vx0 < 0){
    vx0 = 0;
  }
  if(vy0 < 0){
    vy0 = 0;
  }
  if(vx1 > ctl->xsize){
    vx1 = ctl->xsize;
  }
  if(vy1 > ctl->ysize){
    vy1 = ctl->ysize;
  }

  for(h = h0; h <= h1; ++h){
    layer = ctl->sheets[h];
    buf = layer->buf;
    sid = layer - ctl->sheets0;
    bx0 = vx0 - layer->vx;
    by0 = vy0 - layer->vy;
    bx1 = vx1 - layer->vx;
    by1 = vy1 - layer->vy;
    if(bx0 < 0){bx0 = 0;}
    if(by0 < 0){by0 = 0;}
    if(bx1 > layer->bxsize){bx1 = layer->bxsize;}
    if(by1 > layer->bysize){by1 = layer->bysize;}

    for(by = by0; by < by1; ++by){
      vy = layer->vy + by;
      for(bx = bx0; bx < bx1; ++bx){
        vx = layer->vx + bx;
        if(map[vy * ctl->xsize + vx] == sid){
          vram[vy * ctl->xsize + vx] = buf[by * layer->bxsize + bx];
        }
      }
    }
  }
  return;
}

void layer_updown(LAYER* layer, int height){
  LAYER_CTL* ctl = layer->ctl;
  int h, old = layer->height;
  if(height > ctl->top + 1){
    height = ctl->top + 1;
  }
  if(height < -1){
    height = -1;
  }
  layer->height = height;

  if(old > height){
    if(height >= 0){
      for(h = old; h > height; --h){
        ctl->sheets[h] = ctl->sheets[h-1];
        ctl->sheets[h]->height = h;
      }
      ctl->sheets[height] = layer;
      layer_refreshmap(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, height + 1);
      layer_refreshsub(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, height + 1, old);
    }
    else{
      if(ctl->top > old){
        for(h = old; h < ctl->top; ++h){
          ctl->sheets[h] = ctl->sheets[h+1];
          ctl->sheets[h]->height = h;
        }
      }
      --ctl->top;
      layer_refreshmap(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, 0);
      layer_refreshsub(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, 0, old - 1);
    }
  }
  else if(old < height){
    if(old >= 0){
      for(h = old; h < height ; ++h){
        ctl->sheets[h] = ctl->sheets[h+1];
        ctl->sheets[h]->height = h;
      }
      ctl->sheets[height] = layer;
    }
    else{
      for(h = ctl->top; h >= height; --h){
        ctl->sheets[h+1] = ctl->sheets[h];
        ctl->sheets[h+1]->height = h + 1;
      }
      ctl->sheets[height] = layer;
      ++ctl->top;
    }
    layer_refreshmap(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, height);
    layer_refreshsub(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize, height, height);
  }
  return;
}

void layer_refresh(LAYER* layer, int bx0, int by0, int bx1, int by1){
  if(layer->height >= 0){
    layer_refreshsub(layer->ctl, layer->vx + bx0, layer->vy + by0, layer->vx + bx1, layer->vy + by1, layer->height, layer->height);
  }
  return;
}


void layer_slide(LAYER* layer, int vx, int vy){
  LAYER_CTL *ctl = layer->ctl;
  int old_vx = layer->vx, old_vy = layer->vy;
  layer->vx = vx;
  layer->vy = vy;
  if(layer->height >= 0){
    layer_refreshmap(ctl, old_vx, old_vy, old_vx + layer->bxsize, old_vy + layer->bysize, 0);
    layer_refreshmap(ctl, vx, vy, vx + layer->bxsize, vy + layer->bysize, layer->height);
    layer_refreshsub(ctl, old_vx, old_vy, old_vx + layer->bxsize, old_vy + layer->bysize, 0, layer->height - 1);
    layer_refreshsub(ctl, vx, vy, vx + layer->bxsize, vy + layer->bysize, layer->height, layer->height);
  }
  return;
}

void layer_free(LAYER* layer){
  if(layer->height >= 0){
    layer_updown(layer, -1);
  }
  layer->flags = 0;
  return;
}


