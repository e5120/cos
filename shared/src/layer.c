#include "../include/layer.h"

LAYER_CTL* layer_control_init(MEM_MAN* memman, unsigned char *vram, int xsize, int ysize){
  LAYER_CTL* ctl;
  int i;
  ctl = (LAYER_CTL*)memory_manage_alloc_4k(memman, sizeof(LAYER_CTL));
  if(ctl == 0){
    goto err;
  }
  ctl->vram = vram;
  ctl->xsize = xsize;
  ctl->ysize = ysize;
  ctl->top = -1;
  for(i = 0; i < MAX_LAYERS; ++i){
    ctl->sheets0[i].flags = 0;
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

void layer_updown(LAYER_CTL* ctl, LAYER* layer, int height){
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
    }
    else{
      if(ctl->top > old){
        for(h = old; h < ctl->top; ++h){
          ctl->sheets[h] = ctl->sheets[h+1];
          ctl->sheets[h]->height = h;
        }
      }
      --ctl->top;
    }
    layer_refreshsub(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize);
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
    layer_refreshsub(ctl, layer->vx, layer->vy, layer->vx + layer->bxsize, layer->vy + layer->bysize);
  }
  return;
}

void layer_refresh(LAYER_CTL* ctl, LAYER* layer, int bx0, int by0, int bx1, int by1){
  if(layer->height >= 0){
    layer_refreshsub(ctl, layer->vx + bx0, layer->vy + by0, layer->vx + bx1, layer->vy + by1);
  }
  return;
}

void layer_refreshsub(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1){
  int h, bx, by, vx, vy, bx0, by0, bx1, by1;
  unsigned char *buf, c, *vram = ctl->vram;
  LAYER *layer;
  for(h = 0; h <= ctl->top; ++h){
    layer = ctl->sheets[h];
    buf = layer->buf;
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
        c = buf[by * layer->bxsize + bx];
        if(c != layer->color_inv){
          vram[vy * ctl->xsize + vx] = c;
        }
      }
    }
  }
  return;
}


void layer_slide(LAYER_CTL* ctl, LAYER* layer, int vx, int vy){
  int old_vx = layer->vx, old_vy = layer->vy;
  layer->vx = vx;
  layer->vy = vy;
  if(layer->height >= 0){
    layer_refreshsub(ctl, old_vx, old_vy, old_vx + layer->bxsize, old_vy + layer->bysize);
    layer_refreshsub(ctl, vx, vy, vx + layer->bxsize, vy + layer->bysize);
  }
  return;
}

void layer_free(LAYER_CTL* ctl, LAYER* layer){
  if(layer->height >= 0){
    layer_updown(ctl, layer, -1);
  }
  layer->flags = 0;
  return;
}


