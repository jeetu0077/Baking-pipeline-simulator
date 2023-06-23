#include "project01.h"
#include <stdlib.h>
#define PIPE_SIZE 12
#define BREAK_TIME 10
#define BAKING_STAGE 9 // 0 indexed
#define BAKING_REST 10
#define SCALING_REST 1000

enum BreadType { None, Bagel, Baguette };
struct Queue {
    int type;
    struct Queue* next;
};

int strcomp(char* a, char* b, int length) {
    while(length--) if (*a == '\0' || *b == '\0' || *a++ != *b++) return 0;
    return 1;
}

// int strlen(char* a) {
//     if 
// }

void enterQueue(struct Queue **head, struct Queue **rear, int bread) {
    struct Queue* stage = (struct Queue*)malloc(sizeof(struct Queue));
    stage->type = bread;
    stage->next = NULL;

    if (*head == NULL) {
        *head = stage;
        *rear = *head;
    } else {
        (*rear)->next = stage;
        *rear = (*rear)->next;
    }

    return;
}

void printQueue (struct Queue* head, struct Queue* rear) {
    struct Queue* current = head;
    while (current != NULL) {
        printf("%d -> ", current->type);
        current = current->next;
    }
    return;
}

int initPipe(int a[]) {
    for (int i = 0; i < PIPE_SIZE; i++) a[i] = 0;
    return PIPE_SIZE;
}

int moveInPipe(int pipe[], struct Queue **head, int baking_count) {
    int notEmpty = 0;
    for (int i = PIPE_SIZE - 1; i > 0; i--) {
        if (i == BAKING_STAGE + 1 && pipe[i - 1] == Baguette) {
            pipe[i - 1]--;
            return 1;
        }
        pipe[i] = pipe[i - 1];
        
        // isEmpty check
        if (pipe[i] > None) {
            notEmpty = 1;
        }
    }

    if (!notEmpty && (*head) == NULL) return 0;

    // add front of queue to the pipe
    pipe[0] = (*head)->type;
    // move front of queue
    struct Queue *temp = *head;
    *head = (*head)->next;
    free(temp);
    return 1;
}

int main(int argc, char *argv[])  {
    // read input file
    FILE *fp;
    char line[1024];
    fp = fopen(argv[1], "r");

    // initiate the waiting queue
    struct Queue* head = NULL;
    struct Queue* rear = NULL;

    // initiate the pipeline
    int pipe[PIPE_SIZE];
    initPipe(pipe);
    int notEmpty = 0;

    while(fgets(line, 1024, fp) != NULL) {
        bakery_time++;
        int bread = None;
        if (strcomp(line, "Bake-Bagel", 10)) {
            bread = Bagel;
            baking_count++;
            bagel_baked++;
        } else if (strcomp(line, "Bake‐Baguette", 13)) {
            bread = Baguette;
            baking_count++;
            baguette_baked++;
        } else {
            no_request++;
        }

        // if baking_count % 1000, insert 10 no request (bubbles) into queue
        if (baking_count > 0 && baking_count % SCALING_REST == 0) {
            for (int i = 0; i < BREAK_TIME; i++) {
                enterQueue(&head, &rear, None);
            }
        } else if (baking_count % BAKING_REST == BAKING_STAGE) {
            enterQueue(&head, &rear, None);
        }
        // insert the current request into the queue
        enterQueue(&head, &rear, bread);

        // move the pipe
        notEmpty = moveInPipe(pipe, &head, baking_count);
    }

    while (notEmpty && head != NULL) {
        bakery_time++;
        notEmpty = moveInPipe(pipe, &head, 0);
    }

    //output formats
    printf("Baking count: %d\n", baking_count);
    printf(" -Bagel baked: %d\n", bagel_baked);
    printf(" -Baguette baked: %d\n", baguette_baked);
    printf("No request: %d\n", no_request);

    printf("\nHow many minutes to bake: %d\n", bakery_time);

    printf("\nPerformance (bakes/minutes): %.2f\n", performance);

    return 0;
}