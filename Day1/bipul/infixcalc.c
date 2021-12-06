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

int main(int argc , char * argv[])
{
    //Taking input from file
    FILE * fin, * fout;
    if(argc > 1)
        fin = fopen(argv[1],"rb");
    else
        fin = fopen("infix_input.txt","rb");

    if(fin == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }

    //Reading into buffer
    char * buffer;
    fseek(fin,SEEK_SET,SEEK_END);
    long fSize = ftell(fin);
    fseek(fin,SEEK_SET,SEEK_SET);
    buffer = (char *)malloc(sizeof(char)*fSize);
    fread(buffer,sizeof(char),fSize,fin);
    fclose(fin);


    //stack to store the nodes
    struct Stack nodes;
    createStk(&nodes,10,Node);

    //Stack to store the size of the nodes in each line
    struct Stack nodeSizes;
    createStk(&nodeSizes,3,int);


    char * token = buffer;
    struct Node tmpNode;
    int lineNodeCount = 0;
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
            lineNodeCount++;
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
            lineNodeCount++;
        }
        else if (*token == '\n' || *token == '\r')
        {
            pushStk(&nodeSizes,&lineNodeCount,int);
            lineNodeCount = 0;
        }
        token++;
    }
    //For last line
    {
        pushStk(&nodeSizes,&lineNodeCount,int);
        lineNodeCount = 0;
    }
    free(buffer);

    //Opening file for output
    if(argc > 2)
        fout = fopen(argv[2],"wb");
    else
        fout = fopen("infix_output.txt","wb");

    for(int i = 0; i < nodeSizes.last + 1; i++)
    {
        int stackPos = (i == 0)?0: getStk(&nodeSizes,i-1,int);
        int stackSize = getStk(&nodeSizes,i,int);
        Node * startNode = makeTree(&getStk(&nodes,stackPos,Node),stackSize);
        if(startNode != NULL)
        {
            fprintf(fout,"%lf\n",solveTree(startNode));
        }
        else
        {
            fprintf(fout,"Error\n");
        }
    }

    fclose(fout);
    delStk(&nodes);
    delStk(&nodeSizes);

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

    //Start from the highest operator type and work down to the lowest
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
