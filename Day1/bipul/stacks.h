#ifndef ADVANCE_C_WORKSHOP_STACKS_H
#define ADVANCE_C_WORKSHOP_STACKS_H

#define createStk(stackP,size,type) (type*)createStack(stackP,size,sizeof(type))
#define pushStk(stackP,dataP,type) *(type*)pushStack(stackP,dataP,sizeof(type))
#define popStk(stackP,type) *(type*)popStack(stackP,sizeof(type))
#define peekStk(stackP,type) *(type*)peekStack(stackP,sizeof(type))
#define delStk(stackP) delStack(stackP)
#define clearStk(stackP) clearStack(stackP)
#define getStk(stackP,index,type) *(type*)getStack(stackP,index,sizeof(type))

struct Stack {
    void *data;
    int last;
    int capacity;
};
void * createStack(struct Stack* s, unsigned int cap, unsigned int unitSize);
void * pushStack(struct Stack * s, void* n, unsigned int unitSize);;
void * popStack(struct Stack * s, unsigned int unitSize);
void * peekStack(struct Stack * s, unsigned int unitSize);
void * clearStack(struct Stack * s);
void delStack(struct Stack * s);
void * getStack(struct Stack * s, int index, unsigned int unitSize);


#endif //ADVANCE_C_WORKSHOP_STACKS_H
