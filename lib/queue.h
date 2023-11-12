#include "default.h"

#define QUEUE_MAX_SIZE 10

typedef struct queue* Queue;

void init_queue(Queue* queue);

bool is_full(Queue queue);

bool is_empty(Queue queue);  

int get_size(Queue queue);

bool enqueue(Queue queue, int new_element);

int dequeue(Queue queue);