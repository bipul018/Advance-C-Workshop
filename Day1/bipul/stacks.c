#include "stacks.h"
#include <stdlib.h>
#include <string.h>
void * createStack(struct Stack * s, unsigned int cap, unsigned int unitSize)
{
    s->data = malloc(cap * unitSize);
    s->capacity = cap;
    s->last = -1;
}
void * pushStack(struct Stack * s, void* n, unsigned int unitSize)
{
    if( s->last >= ( s->capacity - 1))
    {
        s->capacity *= 2;
        s->data = realloc(s->data,s->capacity * unitSize);
    }
    s->last++;
    memcpy(s->data + unitSize * s->last,n,unitSize);
    return s->data + unitSize * s->last;
}
void * popStack(struct Stack * s,unsigned int unitSize)
{
    if(s->last <= s->capacity / 4)
    {
        s->capacity /= 2;
        s->data = realloc(s->data,s->capacity * unitSize);
    }
    s->last--;
    return s->data + (s->last + 1) * unitSize;
}

void * peekStack(struct Stack * s, unsigned int unitSize)
{
    return s->data + s->last * unitSize;
}

void * clearStack(struct Stack * s)
{
    s->last = -1;
    return s->data;
}

void delStack(struct Stack * s)
{
    s->last = -1;
    s->capacity = 0;
    free(s->data);
}

void * getStack(struct Stack * s, int index, unsigned int unitSize)
{
    return s->data + index * unitSize;
}
