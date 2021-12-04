#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mergeSort(int arr[],int size)
{
    if(size<2)
        return;

    int sizeA=size/2;
    int sizeB=size-sizeA;

    int * leftSide = (int *) malloc(size*sizeof(int));
    int * rightSide = leftSide + sizeA;

    memcpy(leftSide,arr,size*sizeof(int));

    mergeSort(leftSide,sizeA);
    mergeSort(rightSide,sizeB);

    //counters
    int t=0,i=0,j=0;
    for(t = 0;t<size;t++)
    {
        if(i>=sizeA)
        {
            memcpy(arr+t,rightSide+j,sizeof(int)*(sizeB-j));
            break;
        }
        else if(j>=sizeB)
        {
            memcpy(arr+t,leftSide+i,sizeof(int)*(sizeA-i));
            break;
        }
        else if(leftSide[i]<rightSide[j])
        {
            arr[t]=leftSide[i];
            i++;
        }
        else
        {
            arr[t]=rightSide[j];
            j++;
        }
    }
    free(leftSide);


}

int readNumFile(char* filename, int * nLines, int ** sizes, int ** numbers) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fclose(fp);

    char *buffer = (char *) malloc(size + 1);
    fp = fopen(filename, "rb");
    fread(buffer, 1, size, fp);
    fclose(fp);

    buffer[size] = '\0';

    *nLines = 0;
    for(unsigned int i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            (*nLines)++;
        }
    }
    //for last line
    (*nLines)++;

    *sizes = (int *) malloc((*nLines) * sizeof(int));

    //Count the number of numbers in each line and the total
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int totalNum = 0;
    char* prevCh = buffer;
    for(char* pCh = buffer; pCh < buffer + size; pCh++) {
        if(*pCh == ',')
            i ++;
        else if(*pCh == '\n') {
            i++;
            (*sizes)[j] = i;
            j++;
            totalNum+=i;
            i = 0;
        }
    }
    //For the last line
    {
        i++;
        (*sizes)[j] = i;
        j++;
        totalNum+=i;
    }

    //Now finally allocating and reading the numbers
    *numbers = (int *) malloc(totalNum * sizeof(int));
    memset(*numbers, 0, totalNum * sizeof(int));
    int num = 0;
    i = 0;
    for(char * pCh = buffer; ( pCh <buffer + size) && (i < totalNum) ; pCh++){
        if(*pCh >= '0' && *pCh <= '9') {
            while(*pCh >= '0' && *pCh <= '9') {
                num = num * 10 + (*pCh - '0');
                pCh++;
            }
            (*numbers)[i] = num;
            if(num == 4384){
                printf("%d\t",num);
            }
            num = 0;
            i++;
            //printf("%d ",(*numbers)[i-1]);

        }

    }
    free(buffer);

    return totalNum;
}
int main(int argc, char * argv[])
{
/*
    int nArr[10]={4,2,4,5,1,3,2,4,5,6};
    mergeSort(nArr,10);
    for(int i=0;i<10;i++)
        printf("%d ",nArr[i]);
    printf("\n");
    return 0;
*/

    int * sizes;
    int * numbers;
    int nLines;
    int totalNum = 0;

    if(argc > 1) {
        totalNum = readNumFile(argv[1], &nLines, &sizes, &numbers);
    } else {
        totalNum = readNumFile("mergesort_input.csv", &nLines, &sizes, &numbers);
    }

    unsigned int prev = 0;
    for(unsigned int i = 0; i<nLines; i++){
        mergeSort(numbers+prev,sizes[i]);
        prev += sizes[i];
    }

    //Output file
    FILE *fp ;
    if(argc > 2) {
        fp = fopen(argv[2], "wb");
    } else {
        fp = fopen("mergesort_output.csv", "wb");
    }
    for(unsigned int i = 0; i<nLines; i++){
        for(unsigned int j = 0; j<sizes[i]; j++){
            fprintf(fp,"%d",numbers[i*sizes[i]+j]);
            if(j != sizes[i]-1)
                fprintf(fp,",");
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    free(numbers);
    free(sizes);
    return 0;
}