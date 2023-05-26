#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

struct node {
    int value;
    struct node *low;
    struct node *hi;
};

int *readdata(char *filename) {
    FILE *fptr;
    printf("Filename: %s\n",filename);
    fptr = fopen(filename,"r");

    if(fptr == NULL) {
        printf("File open error!");
        exit(1);
    }

    fseek(fptr, 0, SEEK_END);
    long int fsize = ftell(fptr);
    printf("File size:%ld\n",fsize);
    int *memptr = (int*)malloc(fsize+1);
 
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    memset(memptr, 0, fsize+1);
    fseek(fptr, 0, 0);
    size_t bytesread = fread(memptr,1,fsize,fptr);
    printf("Bytes read:%ld\n",bytesread);

    fclose(fptr);
    return memptr;
}

int main(int argc,char **argv) {
/*
    for (int c = 0 ; c < argc; c++) {
        printf("%d : %s\n",c,argv[c]);
    }
*/

    int *memptr = readdata("random2.txt");
    printf("Mempointer: %p\n",memptr);

    int count = 0;
    printf("%d",memptr[count]);
    while( memptr[count] != 0) {
        printf("%d,%c\n",count, (char) memptr[count]);
        count ++ ;
    }
    free(memptr);

/*
    struct node* memptr;
    memptr = (struct node*)malloc(sizeof(struct node));
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }


    struct node* treebase;

    int numread;
    char buffer[BUFFER_SIZE];
    while(!feof(fptr)) {
        memset(buffer, 0, BUFFER_SIZE);
        numread = fscanf(fptr, "%s", buffer);
        buffer[BUFFER_SIZE] = 0;    // ensure string ends
        if (numread == 1) {
            int value = atoi(buffer);
            printf("%d\n",value);          
            }
        // reserve new node 
        // append it to current tree based on value

    }
*/

//    struct node example;

    return 0;
}
