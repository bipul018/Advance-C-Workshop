#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct realStack
{
    int top;
    int capacity;
    double *array;
};

void initializeStack(struct realStack *s, int capacity)
{
    s->capacity = capacity;
    s->top = -1;
    s->array = (double *)malloc(s->capacity * sizeof(double));
}
double push(struct realStack *s, double item)
{
    if (s->top == s->capacity - 1)
    {
        s->array = (double *)realloc(s->array, (s->capacity * 2) * sizeof(double));
        s->capacity *= 2;
    }
    s->array[++s->top] = item;
    return item;
}
double pop(struct realStack *s)
{
    if (s->top == -1)
    {
        printf("Stack is empty\n");
        return 0;
    }
    if(s->top <= s->capacity/4)
    {
        s->array = (double *)realloc(s->array, (s->capacity/2 + 1) * sizeof(double));
        s->capacity /= 2;
        s->capacity ++;
    }
    s->top--;
    return s->array[s->top+1];
}
void releaseStack(struct realStack *s)
{
    free(s->array);
}

void resetStack(struct realStack *s)
{
    s->top = -1;
}

int main(int argc, char *argv[])
{

    //Obtaining Source file
    char filename[100]="expressions.txt";

    //If given in command line, uses that file
    if(argc>1)
    {
        strcpy(filename,argv[1]);
    }
    FILE *fp;
    fp=fopen(filename,"rb");
    if(fp==NULL)
    {
        printf("File not found\n");
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
  
    // calculating the size of the file
    long int fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //Allocating memory to store the file
    char* fileBuffer = (char*)malloc(fileSize + 2);
    memset(fileBuffer, 0, fileSize + 2);
    int size=0;
    fread(fileBuffer, fileSize, 1, fp);
    fclose(fp);

    //Creating result file
    FILE *fp1;
    char outputFile[100]="results.txt";

    //If given in command line, uses that file
    if(argc>2)
    {
        strcpy(outputFile,argv[2]);
    }
    fp1=fopen(outputFile,"w");
    if(fp1==NULL)
    {
        printf("File not found\n");
        return 0;
    }

    char *p = fileBuffer;

    struct realStack s;
    initializeStack(&s, 10);
    while(*p != '\0')
    {
        int calcLineFound = 0;
        while(*p != '\n' && *p != '\0')
        {

            if(*p >= '0' && *p <='9')
            {
                calcLineFound = 1;
                double num = 0.0;
                double factor = 1.0;
                while((*p >= '0' && *p <='9') || (*p == '.' && factor == 1.0))
                {
                    if(*p == '.' && factor == 1.0)
                    {
                        factor = 0.1;
                    }
                    else
                    {
                        if(factor < 1.0)
                        {
                            num += (*p - '0') * factor;
                            factor /= 10.0;
                        }
                        else
                        {
                            num = num * 10 + (*p - '0');
                        }
                    }
                    p++;
                }
                push(&s,num);

            }

            if(*p=='+' || *p=='-' || *p=='*' || *p=='/')
            {
                if(s.top<1)
                {
                    printf("Invalid Expression\n");
                    return 0;
                }
                double num1 = pop(&s);
                double num2 = pop(&s);
                double result = 0.0;
                if(*p=='+')
                {
                    result = num2 + num1;
                }
                else if(*p=='-')
                {
                    result = num2 - num1;
                }
                else if(*p=='*')
                {
                    result = num2 * num1;
                }
                else if(*p=='/')
                {
                    result = num2 / num1;
                }
                push(&s,result);
            }

            p++;
        }
        //Skipping all the newlines
        while(*p == '\n' )
        {
            p++;
        }
        if(calcLineFound == 1)
            fprintf(fp1,"%lf\n",pop(&s));
        resetStack(&s);
    }
    releaseStack(&s);
    free(fileBuffer);
    fclose(fp1);
    return 0;
}
