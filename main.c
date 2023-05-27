#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int value;
    struct node *low;
    struct node *hi;
};

int *readdata(char *filename) {

    // try to open file
    FILE *fptr;
    fptr = fopen(filename,"r");

    if(fptr == NULL) {
        printf("File open error!");
        exit(1);
    }


    // check filesize, reserve memory and zerofill-it
    fseek(fptr, 0, SEEK_END);
    long int fsize = ftell(fptr);
    int *memptr = (int*)malloc(fsize+1);
 
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    memset(memptr, 0, fsize+1);
    

    // read filedata to allocated memory
    fseek(fptr, 0, 0);
    fread(memptr,1,fsize,fptr);


    // close file and return allocated memory
    fclose(fptr);
    return memptr;
}

int main(int argc,char **argv) {
/*
    for (int c = 0 ; c < argc; c++) {
        printf("%d : %s\n",c,argv[c]);
    }
*/

    char *memptr = (char *) readdata("random2.txt");
    printf("Mempointer: %p\n",memptr);
    printf("%s",memptr);
    printf("\n");
    printf("%d\n",strlen(memptr));

/*
    int count = 0;
    while( memptr[count] != 0 && count < 8) {
        printf("%d,%c\n",count, (unsigned char) memptr[count]);
        count ++ ;
    }
*/
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
