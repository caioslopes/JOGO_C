#ifndef QUEUE_H_
#define QUEUE_H_

#include "utils.h"

#define QUEUE_MAX_SIZE 10

typedef struct element* Element;

typedef struct queue* Queue;

void init_element(Element *element);

void update_element(Element element, int x, int y);

void init_queue(Queue* queue);

int get_size(Queue queue);

bool is_full(Queue queue);

bool is_empty(Queue queue);

bool enqueue(Queue queue, Element new_element);

Element dequeue(Queue queue);

void close_queue(Queue *queue);

void close_element(Element *element);

#endif