#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

struct node {
    int x;
    int y;
    double value;
    struct node *prev;
    struct node *next;
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

    int* memptr;
    memptr = (struct node*)malloc(sizeof(struct node));
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    free(memptr);

    int numread;
    char buffer[BUFFER_SIZE];
    while(!feof(fptr)) {
        memset(buffer, 0, BUFFER_SIZE);
        numread = fscanf(fptr, "%s", buffer);
        buffer[BUFFER_SIZE] = 0;
        printf("%d - %s\n",numread,buffer);
    }
    fclose(fptr);

    struct node example;
    example.x = 99;
    example.y = 29;
    example.value = 0.32f;
    example.prev = NULL;
    example.next = NULL;

    struct node example2;
    example2.x = 19;
    example2.y = 39;
    example2.value = 1.85f;
    example2.prev = NULL;
    example2.next = NULL;

    example.next = &example2;
    example2.prev = &example;

    printf("X: %d\n",example.x);
    printf("Y: %d\n",example.y);
    printf("Value: %f\n",example.value);
    printf("Prev: %p\n",example.prev);
    printf("Next: %p\n\n",example.next);

    printf("X: %d\n",example2.x);
    printf("Y: %d\n",example2.y);
    printf("Value: %f\n",example2.value);
    printf("Prev: %p\n",example2.prev);
    printf("Next: %p\n\n",example2.next);

    return 0;
}