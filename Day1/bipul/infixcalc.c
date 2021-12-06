#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stacks.h"

enum Operator
{
    ZERO_POINT,
    ADD,
    SUB,
    MULT,
    DIVI,
    EXP,
    BRAKO,
    BRAKC,
    NUM
};

typedef struct Node
{
    struct Node * left;
    struct Node * right;
    enum Operator type;
    int parentFlag;         // 1 is parent has been assigned
    double num;
} Node;

void printOp(enum Operator op)
{
    switch (op)
    {
        case ADD:
            printf("+");
            break;
        case SUB:
            printf("-");
            break;
        case MULT:
            printf("*");
            break;
        case DIVI:
            printf("/");
            break;
        case EXP:
            printf("^");
            break;
        case BRAKO:
            printf("(");
            break;
        case BRAKC:
            printf(")");
            break;
        default:
            break;
    }
}

double solveTree(struct Node * node)
{
    double left=0.0,right=0.0;
    if(node->left != NULL)
        left = solveTree(node->left);
    if(node->right != NULL)
        right= solveTree(node->right);
    
    switch(node->type)
    {
        case NUM:
            return node->num;
            break;
        case ADD:
            return left+right;
            break;
        case SUB:
            return left-right;
            break;
        case MULT:
            return left*right;
            break;
        case DIVI:
            return left/right;
            break;
        case BRAKO:
            return right;
        case EXP:
            return pow(left,right);
        default:
            return node->num;

    }
}

//Takes an array of unarranged nodes and arranges them in a binary tree and returns the parent node
struct Node * makeTree(struct Node * nodeStart, unsigned int size);


int main()
{
    char * expr = "1+ (2 *4-5)/8^2";

    //stack to store the nodes
    struct Stack nodes;
    createStk(&nodes,3,Node);

    char * token = expr;
    struct Node tmpNode;
    struct Node * debugNode = nodes.data;
    //Fills the stack with the nodes reading from the expression
    while(*token != '\0')
    {
        if(*token >= '0' && *token <= '9')
        {
            tmpNode.num = atof(token);
            while((*token >= '0' && *token <='9') || *token =='.')
                token ++;
            tmpNode.left=NULL;
            tmpNode.right=NULL;
            tmpNode.parentFlag = 0;
            tmpNode.type = NUM;
            pushStk(&nodes,&tmpNode,Node);
        }

        if( *token == '+' || *token == '-' || *token == '*' || *token == '/' || *token == '(' || *token == '^' || *token == ')')
        {
            switch(*token)
            {
                case '+':
                    tmpNode.type=ADD;
                    break;
                case '-':
                    tmpNode.type=SUB;
                    break;
                case '*':
                    tmpNode.type=MULT;
                    break;
                case '/':
                    tmpNode.type=DIVI;
                    break;
                case '(':
                    tmpNode.type=BRAKO;
                    break;
                case '^':
                    tmpNode.type=EXP;
                    break;
                case ')':
                    tmpNode.type=BRAKC;
                    break;
            }
            tmpNode.parentFlag=0;
            tmpNode.left=NULL;
            tmpNode.right=NULL;
            tmpNode.num = 0.0;
            pushStk(&nodes,&tmpNode,Node);
        }
        
        token++;
    }

    printf("result = %f ",solveTree(makeTree(nodes.data,nodes.last+1)));


    delStk(&nodes);

    return 0;
}

struct Node * makeTree(struct Node * nodeStart, unsigned int size)
{

    //Solves till inside brackets are cleared
    for(struct Node * currNode = nodeStart; currNode < nodeStart + size;currNode++)
    {
        if(currNode->type == BRAKO)
        {
            int openBracks = 1;
            int i = 0;
            while( (currNode + i < nodeStart + size ) && ( openBracks > 0 ))
            {
                i++;
                if(currNode[i].type == BRAKC )
                    openBracks--;
                if(currNode[i].type == BRAKO )
                    openBracks++;
            }
            //Recursively create branch inside the brackets and use that node in the bracket
            struct Node * bracNode = makeTree(currNode+1,i-1);

            //If there was a bracket close, not giving one will not give a syntax error ,
            //Make bracket close node have bracket open as parent , just for compatibility reasons 😁😁
            if(currNode[i].type == BRAKC)
            {
                currNode[i].parentFlag = 1;
            }
            currNode->left = NULL;
            currNode->right= bracNode;
            bracNode->parentFlag = 1;
            currNode += i;

        }
    }

    //Start from the highest operator type
    enum Operator currType = EXP;
    struct Node * tmp ;
    while(currType > ZERO_POINT)
    {
        for(struct Node * currNode = nodeStart; currNode < nodeStart + size; currNode ++)
        {
            if((currNode->type == currType) && (currNode->parentFlag==0))
            {
                //loop towards left of array till something happens
                //If left is NUM ,i.e is a number then grab the number, so for right works iif node is not null
                {
                    if(currNode->type == SUB)
                    {
                        int j = 8;
                    }
                }
                //finding the left most available node
                for(tmp = currNode - 1;tmp >= nodeStart ; tmp--)
                    if(tmp->parentFlag == 0 )
                        break;

                //To follow left to right associativity >= is used
                if((tmp != NULL) && ((tmp->type == NUM) || (tmp->type >= currNode->type) ))
                {
                    tmp->parentFlag = 1;
                    currNode->left = tmp;
                }

                //finding the right most available node
                for(tmp = currNode + 1;tmp < nodeStart + size ; tmp++)
                    if(tmp->parentFlag == 0 )
                        break;
                // For following left to right associativity , > is used
                if((tmp != NULL) && ((tmp->type == NUM) || (tmp->type > currNode->type) ))
                {
                    tmp->parentFlag = 1;
                    currNode->right = tmp;
                }
            }
        }
        currType--;
    }


    //Finding the most parent node of them all
    tmp = nodeStart;
    while(tmp->parentFlag && (tmp < nodeStart + size))
        tmp++;

    return tmp;
}
