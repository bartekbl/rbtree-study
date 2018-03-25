#include "rbtree.h"
#include <stdio.h>
#include <assert.h>

static void rbtree_verify_recursive(const struct RBTree *tree, int* min, int* max, int* max_black_level, int black_level, int *tree_size)
{
    assert (tree != NULL);
    (*tree_size)++;
    if (tree->color == RBTREE_COLOR_BLACK) black_level++;
    if (tree->left == NULL)
    {
        *min = tree->value;
        assert (*max_black_level == -1 || *max_black_level > 0);
        if (*max_black_level == -1) *max_black_level = black_level;
        else
        {
            if (black_level != *max_black_level) printf("tree error: max black node level %d different to previous %d\n", black_level, *max_black_level); fflush(stdout);
        }
    }
    else
    {
        // parent test
        if (tree->left->parent != tree)
        {
            printf("tree error: left node %d has wrong parent (should be %d)", tree->left->value, tree->value); fflush(stdout);
        }

        // min/max value test
        int left_min;
        int left_max;
        rbtree_verify_recursive(tree->left, &left_min, &left_max, max_black_level, black_level, tree_size);
        if (left_max > tree->value)
        {
            printf("tree error: left subtree has value %d greater than this node %d", left_max, tree->value); fflush(stdout);
        }
        *min = left_min;

        // R/B test
        if (tree->color == RBTREE_COLOR_RED && tree->left->color == RBTREE_COLOR_RED)
        {
            printf("tree error: two consecutive red nodes: %d, %d", tree->value, tree->left->value); fflush(stdout);
        }

        // TODO: add number of blacks to leaf
    }
    if (tree->right == NULL)
    {
        *max = tree->value;
        assert (*max_black_level == -1 || *max_black_level > 0);
        if (*max_black_level == -1) *max_black_level = black_level;
        else
        {
            if (black_level != *max_black_level) printf("tree error: max black node level %d different to previos %d ", black_level, *max_black_level); fflush(stdout);
        }
    }
    else
    {
        int right_min;
        int right_max;
        if (tree->right->parent != tree)
        {
            printf("tree error: right node %d has wrong parent (should be %d)", tree->left->value, tree->value); fflush(stdout);
        }
        rbtree_verify_recursive(tree->right, &right_min, &right_max, max_black_level, black_level, tree_size);
        if (right_min < tree->value)
        {
            printf("tree error: left subtree has value %d lesser than this node %d", right_min, tree->value); fflush(stdout);
        }
        *max = right_max;

        // R/B test
        if (tree->color == RBTREE_COLOR_RED && tree->right->color == RBTREE_COLOR_RED)
        {
            printf("tree error: two consecutive red nodes: %d, %d", tree->value, tree->right->value); fflush(stdout);
        }

        // TODO: add number of blacks to leaf
    }
    assert (*min <= tree->value);
    assert (*max >= tree->value);
}

void rbtree_verify(const struct RBTree* tree, int expected_size)
{
    printf("Verify tree...\n"); fflush(stdout);
    if (tree == NULL)
    {
        printf("Verify tree finished: min=N/A, max=N/A, max_black_level=0, tree_size=0\n"); fflush(stdout);
        return;
    }
    int min = tree->value;
    int max = tree->value;
    int max_black_level = -1;
    int tree_size = 0;
    if (tree->parent != NULL) printf("tree error: root[%d] has parent\n", tree->value); fflush(stdout);
    if (tree->color != RBTREE_COLOR_BLACK) printf("tree error: root[%d] is red\n", tree->value); fflush(stdout);
    rbtree_verify_recursive(tree, &min, &max, &max_black_level, 0, &tree_size);
    if (expected_size >= 0 && tree_size != expected_size) printf("tree error: size is different than expected (expected %d got %d)\n", expected_size, tree_size); fflush(stdout);
    printf("Verify tree finished: min=%d, max=%d, max_black_level=%d, tree_size=%d\n", min, max, max_black_level, tree_size); fflush(stdout);
}
