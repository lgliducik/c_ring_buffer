#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct BufferItem {
  unsigned char* data;
  unsigned int size;
  bool empty;
};

struct RingBuffer
{
  BufferItem* items;  // buffer for items
  int head;  // number of head in body
  int tail;
  int max_count;  // maximum items in buffer
  int max_item_size;  //maximus message size for item in buffer
};

#define MAX_BUFFER_ITEM_COUNT      3
#define MAX_MESSAGE_SIZE           512 * 1024
#define ERR_RING_BUFFER_FULL       -1
#define ERR_RING_BUFFER_EMPTY      -2
#define ERR_RING_BUFFER_SIZE_SMALL -3


RingBuffer init_ring_buffer(int max_count, int max_item_size){
  RingBuffer result;
  BufferItem* item;

  result.max_count = max_count;
  result.max_item_size = max_item_size;
  result.items = (BufferItem*)malloc(sizeof(BufferItem) * max_count);
  for(int i = 0; i < max_count; i++){
    item = &result.items[i];
    item->data = (unsigned char*)malloc(max_item_size);
    item->empty = true;
  }
  result.head = 0;
  result.tail = 0;
  return result;
}

void print_ring_buffer(RingBuffer *ring_buffer){
  printf("**********ring_buffer**********\n");
  printf("head=%d tail=%d\n", ring_buffer->head, ring_buffer->tail);
  printf("***********");
}

int put_ring_buffer(RingBuffer *ring_buffer, char *data, int data_size){
  // check for full buffer
if(ring_buffer->head == ring_buffer->tail &&
   ring_buffer->items->empty == false){
    return ERR_RING_BUFFER_FULL;
  }
  memcpy(data, ring_buffer->items[ring_buffer->head].data, data_size);
  ring_buffer->items[ring_buffer->head].size = data_size;
  ring_buffer->items[ring_buffer->head].empty = false;
  ring_buffer->head = (ring_buffer->head + 1) % ring_buffer->max_count;
  return data_size;
}

int get_ring_buffer(RingBuffer *ring_buffer, char *buf, int buf_size){
  BufferItem* target = &(ring_buffer->items[ring_buffer->tail]);

  if(ring_buffer->tail == ring_buffer->head &&
   ring_buffer->items->empty == true){
    return ERR_RING_BUFFER_EMPTY;
  }
  if(buf_size < target->size){
    return ERR_RING_BUFFER_SIZE_SMALL;
  }
  memcpy(target->data, buf, target->size);
  ring_buffer->items[ring_buffer->tail].empty = true;
  ring_buffer->tail = (ring_buffer->tail + 1) % ring_buffer->max_count;
  return target->size;
}

int main(int argc, char** argv){
  //init buffer
  RingBuffer ring_buffer =  init_ring_buffer(MAX_BUFFER_ITEM_COUNT,
                                             MAX_MESSAGE_SIZE);
  char *data = (char*)malloc(10);
  put_ring_buffer(&ring_buffer, data, 10);
  print_ring_buffer(&ring_buffer);
  put_ring_buffer(&ring_buffer, data, 10);
  print_ring_buffer(&ring_buffer);
  put_ring_buffer(&ring_buffer, data, 10);
  print_ring_buffer(&ring_buffer);
  get_ring_buffer(&ring_buffer, data, 10);
  print_ring_buffer(&ring_buffer);
  return 0;
}
