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
    layer_refresh(ctl);
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
    layer_refresh(ctl);
  }
  return;
}

void layer_refresh(LAYER_CTL* ctl){
  int h, bx, by, vx, vy;
  unsigned char *buf, c, *vram = ctl->vram;
  LAYER *layer;
  for(h = 0; h <= ctl->top; ++h){
    layer = ctl->sheets[h];
    buf = layer->buf;
    for(by = 0; by < layer->bysize; ++by){
      vy = layer->vy + by;
      for(bx = 0; bx < layer->bxsize; ++bx){
        vx = layer->vx + bx;
        c = buf[by * layer->bxsize  + bx];
        if(c != layer->color_inv){
          vram[vy * ctl->xsize + vx] = c;
        }
      }
    }
  }
  return;
}

void layer_slide(LAYER_CTL* ctl, LAYER* layer, int vx, int vy){
  layer->vx = vx;
  layer->vy = vy;
  if(layer->height >= 0){
    layer_refresh(ctl);
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


