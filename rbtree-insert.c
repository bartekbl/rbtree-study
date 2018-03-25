#include "rbtree.h"
#include "rbtree-internal.h"
#include "rbtree-dump.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void rbtree_correct_right(struct RBTree** tree,
                                 struct RBTree** pcursor,
                                 int debug_info)
{
    struct RBTree* cursor = *pcursor;
    if (cursor->parent->parent->left != NULL && cursor->parent->parent->left->color == RBTREE_COLOR_RED)
    {
        cursor->parent->parent->left->color = RBTREE_COLOR_BLACK;
        cursor->parent->color = RBTREE_COLOR_BLACK;
        cursor->parent->parent->color = RBTREE_COLOR_RED;
        *pcursor = cursor->parent->parent;
        if (debug_info) printf("after just repaint:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);
    }
    else
    {
        if (cursor->parent->right != cursor) // We are a left child of a right child
        {
            assert (cursor->parent->left == cursor);

            struct RBTree* result = rbtree_rotate_right(tree, cursor->parent);
            cursor = result->right;

            if (debug_info) printf("after first rotation:\n");
            if (debug_info) rbtree_dump(*tree); fflush(stdout);
        }                                    // note the lack of "else" - the previous case is solved by reducing it to the next case
        assert (cursor->parent->right == cursor); // We are a right child of a right child

        // left-rotate cursor->parent->parent
        struct RBTree* result = rbtree_rotate_left(tree, cursor->parent->parent);
        if (result->parent == NULL) *tree = result;

        if (debug_info) printf("after second rotation:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);

        assert(result->color == RBTREE_COLOR_RED);
        assert(result->left != NULL);
        assert(result->left->color == RBTREE_COLOR_BLACK);
        result->color = RBTREE_COLOR_BLACK; // this is now node
        result->left->color  = RBTREE_COLOR_RED; // this is now child

        if (debug_info) printf("after rotation repaint:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);

        *pcursor = *tree;
    }
}

static void rbtree_correct_left(struct RBTree** tree,
                                struct RBTree** pcursor,
                                int debug_info)
{
    assert(pcursor != 0);
    struct RBTree* cursor = *pcursor;
    if (cursor->parent->parent->right != NULL && cursor->parent->parent->right->color == RBTREE_COLOR_RED)
    {
        cursor->parent->parent->right->color = RBTREE_COLOR_BLACK;
        cursor->parent->color = RBTREE_COLOR_BLACK;
        cursor->parent->parent->color = RBTREE_COLOR_RED;
        *pcursor = cursor->parent->parent;
        if (debug_info) printf("after just repaint:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);
    }
    else
    {
        if (cursor->parent->right == cursor) // We are a right child of a left child
        {
            assert (cursor->parent->left != cursor);

            struct RBTree* result = rbtree_rotate_left(tree, cursor->parent);
            cursor = result->left; // cursor must be pointing to lower left node whether this "if" has executed or not

            if (debug_info) printf("after first rotation:\n");
            if (debug_info) rbtree_dump(*tree); fflush(stdout);
        }                                    // note the lack of "else" - the previous case is solved by reducing it to the next case
        assert (cursor->parent->left == cursor); // We are a right child of a right child

        struct RBTree* result = rbtree_rotate_right(tree, cursor->parent->parent);
        if (result->parent == NULL) *tree = result;

        if (debug_info) printf("after second rotation:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);

        assert(result->color == RBTREE_COLOR_RED);
        assert(result->right != NULL);
        assert(result->right->color == RBTREE_COLOR_BLACK);
        result->color = RBTREE_COLOR_BLACK; // this is now node
        result->right->color  = RBTREE_COLOR_RED; // this is now child

        if (debug_info) printf("after rotation repaint:\n");
        if (debug_info) rbtree_dump(*tree); fflush(stdout);

        *pcursor = *tree;
    }
}

static void rbtree_correct(struct RBTree** tree,
                           struct RBTree* cursor,
                           int debug_info)
{
    while (1)
    {
        if (debug_info) printf("cursor:%d\n", cursor->value); fflush(stdout);
        if (cursor->parent == NULL)
        {
            assert (cursor == *tree);
            cursor->color = RBTREE_COLOR_BLACK;
            return;
        }
        if (cursor->parent->color == RBTREE_COLOR_BLACK) return;
        assert (cursor->parent->parent != NULL); // if parent is red than it must have parent because otherwise it breaks red-black tree rules
        if (cursor->parent->parent->right == cursor->parent) // we are under the node that is right child of its parent
        {
            assert (cursor->parent->parent->left != cursor->parent);
            rbtree_correct_right(tree, &cursor, debug_info);
        }
        else // we are under the node that is left child of its parent
        {
            assert (cursor->parent->parent->left == cursor->parent);
            rbtree_correct_left(tree, &cursor, debug_info);
        }
    }
}

enum RBTreeError rbtree_insert(struct RBTree** tree,
                               int             value,
                               int             debug_info)
{
    assert (tree != NULL); // we must be given valid pointer to a pointer to a tree
    assert (*tree == NULL || (*tree)->parent == NULL); // tree can be empty or we can operate on root but we can't start from the middle

    struct RBTree* newval = malloc(sizeof(struct RBTree));
    if (newval == NULL) return RBTREE_INSERT_ERROR_CANT_ALLOCATE;
    memset(newval, 0, sizeof(struct RBTree));
    newval->value = value; // left, parent, right, and color are initialized to zero which is ok (NULL, NULL, NULL and RED)

    struct RBTree* parent = NULL;
    struct RBTree** pcursor = tree;
    while (*pcursor != NULL)
    {
        struct RBTree* cursor = *pcursor;
        if (value < cursor->value) pcursor = &cursor->left;
        else                       pcursor = &cursor->right;
        parent = cursor;
    }

    newval->parent = parent;
    *pcursor = newval;

    if (debug_info) printf("after initial insertion:\n");
    if (debug_info) rbtree_dump(*tree);

    rbtree_correct(tree, newval, debug_info);

    return RBTREE_NO_ERROR;
}
