#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

struct node {
    int value;
    struct node *low;
    struct node *hi;
};

int main(int argc,char **argv) {

    for (int c = 0 ; c < argc; c++) {
        printf("%d : %s\n",c,argv[c]);
    }

    FILE *fptr;
    fptr = fopen("random.txt","r");

    if(fptr == NULL) {
        printf("File open error!");
        exit(1);
    }

    struct node* memptr;
    memptr = (struct node*)malloc(sizeof(struct node));
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    free(memptr);

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
    fclose(fptr);

    struct node example;

    return 0;
}
