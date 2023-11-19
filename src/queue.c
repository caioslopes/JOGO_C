#include "../lib/queue.h"

struct element{
    int x;
    int y;
};

struct queue{
    Element items[QUEUE_MAX_SIZE];
    int last, first, size;
};

void init_element(Element *element){
    Element e;
    e = malloc(sizeof(struct element));
    e->x = 0;
    e->y = 0;

    if(element != NULL){
        *element = e;
    }
}

void update_element(Element element, int x, int y){
    element->x = x;
    element->y = y;
}

void init_queue(Queue* queue){
    Queue q;
    q = malloc(sizeof(struct queue));
    q->last = -1;
    q->first = -1;
    q->size = 0;

    if(queue != NULL){
        *queue = q;
    }
}

int get_size(Queue queue){
    return queue->size;
}

bool is_full(Queue queue){
    return queue->size == QUEUE_MAX_SIZE;
}

bool is_empty(Queue queue){
    return queue->size == 0;
}

bool enqueue(Queue queue, Element new_element){
    bool answer = false;
    if(! is_full(queue)){
        if(queue->last == QUEUE_MAX_SIZE - 1){
            queue->last = 0;
        }else{
            queue->last += 1;
        }
        queue->items[queue->last] = new_element;
        queue->size += 1;
        answer = true;
    }
    return answer;
}

Element dequeue(Queue queue){
    Element answer = NULL;
    if(!is_empty(queue)){
        if(queue->first == QUEUE_MAX_SIZE - 1){
            queue->first = 0;
        }else{
            queue->first += 1;
        }
        queue->size -= 1;
        answer = queue->items[queue->first];
    }
    return answer;
}

void close_queue(Queue *queue){
    if(queue != NULL){
        free(*queue);
    }
}

void close_element(Element *element){
    if(element != NULL){
        free(*element);
    }
}