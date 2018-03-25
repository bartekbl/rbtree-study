#include "rbtree.h"
#include "rbtree-dump.h"
#include "rbtree-verify.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    srand(time(0));
    struct RBTree* root = NULL;

    rbtree_init(&root);
    rbtree_verify(root, -1);
    rbtree_dump(root); fflush(stdout);
    FILE* testfile = fopen("test.txt", "r");
    while (1)
    {
        char buf[10] = {0};
        int value;
        int ret = fscanf(testfile, "%s %d\n", buf, &value);
        if (ret != 2) break;
        if (!strcmp(buf, "insert"))
        {
            printf("insert %d\n", value);
            rbtree_insert(&root, value, 0);
        }
        if (!strcmp(buf, "remove"))
        {
            printf("remove %d\n", value);
            rbtree_remove(&root, value, 0);
        }
        rbtree_verify(root, -1);
        rbtree_dump(root); fflush(stdout);
    }
    rbtree_remove(&root, 61, 1);
    rbtree_verify(root, -1);
    rbtree_dump(root); fflush(stdout);
    rbtree_remove(&root, 51, 1);
    rbtree_verify(root, -1);
    rbtree_dump(root); fflush(stdout);
    fclose(testfile);

    rbtree_destroy(&root);


    // Quick test
    printf("Quick test...\n\n"); fflush(stdout);
    rbtree_init(&root);

    rbtree_verify(root, 0); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 5, 0);
    rbtree_verify(root, 1); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 2, 0);
    rbtree_verify(root, 2); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 3, 0);
    rbtree_verify(root, 3); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 6, 1);
    rbtree_verify(root, 4); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 8, 1);
    rbtree_verify(root, 5); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 1, 0);
    rbtree_insert(&root, 1, 0);
    rbtree_insert(&root, 1, 0);
    rbtree_insert(&root, 1, 0);
    rbtree_insert(&root, 4, 0);
    rbtree_insert(&root, 5, 0);
    rbtree_insert(&root, 7, 0);
    rbtree_insert(&root, 9, 0);
    rbtree_verify(root, 13); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_remove(&root, 5, 0);
    rbtree_verify(root, 12); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_remove(&root, 6, 1);
    rbtree_verify(root, 11); printf("\n"); fflush(stdout);


    rbtree_dump(root); fflush(stdout);

    rbtree_destroy(&root);
    printf("\nQuick test finished.\n\n"); fflush(stdout);

    // Sequence 1 test
    printf("Sequence 1 test...\n\n"); fflush(stdout);
    rbtree_init(&root);

    rbtree_verify(root, 0);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 15, 0);
    rbtree_verify(root, 1); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 13, 0);
    rbtree_verify(root, 2); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 70, 0);
    rbtree_verify(root, 3); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 93, 0);
    rbtree_verify(root, 4); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 43, 0);
    rbtree_verify(root, 5); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 23, 0);
    rbtree_verify(root, 6); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 50, 0);
    rbtree_verify(root, 7); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 20, 0);
    rbtree_verify(root, 8); printf("\n"); fflush(stdout);

    rbtree_dump(root); fflush(stdout);

    rbtree_destroy(&root);
    printf("Sequence 1 test finished.\n\n"); fflush(stdout);

    // Sequence 2 test
    printf("Sequence 2 test...\n\n"); fflush(stdout);
    rbtree_init(&root);

    rbtree_verify(root, 0);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 92, 0);
    rbtree_verify(root, 1); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 45, 0);
    rbtree_verify(root, 2); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 20, 0);
    rbtree_verify(root, 3); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 88, 0);
    rbtree_verify(root, 4); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 15, 0);
    rbtree_verify(root, 5); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 30, 0);
    rbtree_verify(root, 6); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 26, 0);
    rbtree_verify(root, 7); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 30, 0);
    rbtree_verify(root, 8); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 43, 0);
    rbtree_verify(root, 9); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 66, 0);
    rbtree_verify(root, 10); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 82, 0);
    rbtree_verify(root, 11); printf("\n"); fflush(stdout);
    rbtree_dump(root); printf("\n"); fflush(stdout);
    rbtree_insert(&root, 85, 1);
    rbtree_verify(root, 12); printf("\n"); fflush(stdout);

    rbtree_dump(root); fflush(stdout);

    rbtree_destroy(&root);
    printf("Sequence 2 test finished.\n\n"); fflush(stdout);

    // Insertion correctness test
    printf("Insertion correctness test...\n\n"); fflush(stdout);
    for (int i = 0; i < 20; i++)
    {
        if (i > 0) printf("\n Test number %d \n\n", i); fflush(stdout);
        rbtree_init(&root);

        for (int j = 0; j < 30; j++)
        {
            rbtree_dump(root); printf("\n"); fflush(stdout);
            int new_value = rand() % 100;
            printf("insert: %d\n\n", new_value);
            rbtree_insert(&root, new_value, 0);
        }

        rbtree_dump(root);

        rbtree_destroy(&root);
    }
    printf("\nInsertion correctness test finished.\n"); fflush(stdout);

    // If following randomizing code produces an error simply copy its output, filter it for lines starting with insert or remove and than store as file "test.txt" so that you can repeat the bad case
    // If you want to break at the last line, simply remove it from the file and add or remove the value ba code. Or instead of removing line from file you can simply append a letter to it so that it is not recognized as an insert or remove.

    // Insertion/deletion correctness test
    printf("Insertion/deletion test...\n\n"); fflush(stdout);
    rbtree_init(&root);
#define MAX_TREE_SIZE 100
#define MAX_NUMBER 100
#define MAX_BATCH_SIZE 40
#define BATCHES_NUMBER 100
    int numberset[MAX_TREE_SIZE] = { 0 };
    int tree_size = 0;
    printf("Prefill half tree size...\n\n"); fflush(stdout);
    for (int i = 0; i < MAX_TREE_SIZE / 2; i++) // fill tree half full of numbers
    {
        int to_insert = rand() % MAX_NUMBER;
        printf("insert %d\n\n", to_insert); fflush(stdout);
        rbtree_insert(&root, to_insert, 0);
        for (int j = 0; j < MAX_TREE_SIZE; j++) if (numberset[j] > to_insert || j >= tree_size) { int temp = to_insert; to_insert = numberset[j]; numberset[j] = temp; }
        tree_size++;
        rbtree_verify(root, tree_size);
    }
    rbtree_verify(root, tree_size);
    rbtree_dump(root);
    printf("Grind the tree...\n\n"); fflush(stdout);
    for (int i = 0; i < BATCHES_NUMBER; i++)
    {
        int batch_size = rand() % MAX_BATCH_SIZE;
        for (int k = 0; k < batch_size && tree_size < MAX_TREE_SIZE; k++)
        {
            int to_insert = rand() % MAX_NUMBER;
            printf("insert %d\n\n", to_insert); fflush(stdout);
            rbtree_insert(&root, to_insert, 0);
            for (int j = 0; j < MAX_TREE_SIZE; j++) if (numberset[j] > to_insert || j >= tree_size) { int temp = to_insert; to_insert = numberset[j]; numberset[j] = temp; }
            tree_size++;
            rbtree_verify(root, tree_size);
            rbtree_dump(root);
        }
        batch_size = rand() % MAX_BATCH_SIZE;
        for (int k = 0; k < batch_size && tree_size > 0; k++)
        {
            int to_remove_index = rand() % tree_size;
            printf("remove %d\n\n", numberset[to_remove_index]); fflush(stdout);
            rbtree_remove(&root, numberset[to_remove_index], 0);
            for (int j = to_remove_index + 1; j < MAX_TREE_SIZE; j++) numberset[j - 1] = numberset[j];
            tree_size--;
            rbtree_verify(root, tree_size);
            rbtree_dump(root);
        }
    }
    rbtree_destroy(&root);
    printf("\nInsertion/deletion test finished.\n"); fflush(stdout);

//    // Memory test
//    printf("Memory test...\n"); fflush(stdout);
//    for (int i = 0; i < 10000; i++)
//    {
//        rbtree_init(&root);

//        for (int j = 0; j < 100000; j++)
//        {
//            rbtree_insert(&root, rand() % 100);
//        }

//        destroy(&root);
//    }
//    printf("Memory test finished.\n"); fflush(stdout);

    return 0;
}
