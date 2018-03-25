#include "rbtree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "rbtree-dump.h"
//#include <time.h>

enum RBTreeError rbtree_init(struct RBTree** tree)
{
    *tree = NULL;
    return 0;
}

// it takes pointer to a parent of rotated pair and returns new parent
static struct RBTree* rbtree_rotate_left(struct RBTree** tree, struct RBTree* parent)
{
    // parent      - parent node being rotated
    // child       - child node being rotated
    // grandparent - parent of a parent that stays stationary but has its child exchanged
    // grandchild  - subtree that will have its parent exchanged
    assert(parent        != NULL);
    struct RBTree* grandparent = parent->parent;
    struct RBTree* child = parent->right; // it must not be NULL for rotation to work
    assert(child != NULL);
    struct RBTree* grandchild = child->left;

    if (grandparent == NULL)
    {
        assert (*tree == parent);
        *tree = child;
    }
    else
    {
        if (grandparent->right == parent) grandparent->right = child;
        else
        {
            assert (grandparent->left == parent);
            grandparent->left = child;
        }
    }
    child->parent = grandparent;

    parent->parent = child;
    child->left = parent;

    if (grandchild != NULL) grandchild->parent = parent;
    parent->right = grandchild;

    return child;
}

// it takes pointer to a parent of rotated pair and returns new parent
static struct RBTree* rbtree_rotate_right(struct RBTree** tree, struct RBTree* parent)
{
    // parent      - parent node being rotated
    // child       - child node being rotated
    // grandparent - parent of a parent that stays stationary but has its child exchanged
    // grandchild  - subtree that will have its parent exchanged
    assert(parent        != NULL);
    struct RBTree* grandparent = parent->parent;
    struct RBTree* child = parent->left; // it must not be NULL for rotation to work
    assert(child != NULL);
    struct RBTree* grandchild = child->right;

    if (grandparent == NULL)
    {
        assert (*tree == parent);
        *tree = child;
    }
    else
    {
        if (grandparent->right == parent) grandparent->right = child;
        else
        {
            assert (grandparent->left == parent);
            grandparent->left = child;
        }
    }
    child->parent = grandparent;

    parent->parent = child;
    child->right = parent;

    if (grandchild != NULL) grandchild->parent = parent;
    parent->left = grandchild;

    return child;
}

static void rbtree_correct_right(struct RBTree** tree,
                                 struct RBTree** pcursor,
                                 int partial_results)
{
    struct RBTree* cursor = *pcursor;
    if (cursor->parent->parent->left != NULL && cursor->parent->parent->left->color == RBTREE_COLOR_RED)
    {
        cursor->parent->parent->left->color = RBTREE_COLOR_BLACK;
        cursor->parent->color = RBTREE_COLOR_BLACK;
        cursor->parent->parent->color = RBTREE_COLOR_RED;
        *pcursor = cursor->parent->parent;
        if (partial_results) printf("after just repaint:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);
    }
    else
    {
        if (cursor->parent->right != cursor) // We are a left child of a right child
        {
            assert (cursor->parent->left == cursor);

            struct RBTree* result = rbtree_rotate_right(tree, cursor->parent);
            cursor = result->right;

            if (partial_results) printf("after first rotation:\n");
            if (partial_results) rbtree_dump(*tree); fflush(stdout);
        }                                    // note the lack of "else" - the previous case is solved by reducing it to the next case
        assert (cursor->parent->right == cursor); // We are a right child of a right child

        // left-rotate cursor->parent->parent
        struct RBTree* result = rbtree_rotate_left(tree, cursor->parent->parent);
        if (result->parent == NULL) *tree = result;

        if (partial_results) printf("after second rotation:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);

        assert(result->color == RBTREE_COLOR_RED);
        assert(result->left != NULL);
        assert(result->left->color == RBTREE_COLOR_BLACK);
        result->color = RBTREE_COLOR_BLACK; // this is now node
        result->left->color  = RBTREE_COLOR_RED; // this is now child

        if (partial_results) printf("after rotation repaint:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);

        *pcursor = *tree;
    }
}

static void rbtree_correct_left(struct RBTree** tree,
                                struct RBTree** pcursor,
                                int partial_results)
{
    assert(pcursor != 0);
    struct RBTree* cursor = *pcursor;
    if (cursor->parent->parent->right != NULL && cursor->parent->parent->right->color == RBTREE_COLOR_RED)
    {
        cursor->parent->parent->right->color = RBTREE_COLOR_BLACK;
        cursor->parent->color = RBTREE_COLOR_BLACK;
        cursor->parent->parent->color = RBTREE_COLOR_RED;
        *pcursor = cursor->parent->parent;
        if (partial_results) printf("after just repaint:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);
    }
    else
    {
        if (cursor->parent->right == cursor) // We are a right child of a left child
        {
            assert (cursor->parent->left != cursor);

            struct RBTree* result = rbtree_rotate_left(tree, cursor->parent);
            cursor = result->left; // cursor must be pointing to lower left node whether this "if" has executed or not

            if (partial_results) printf("after first rotation:\n");
            if (partial_results) rbtree_dump(*tree); fflush(stdout);
        }                                    // note the lack of "else" - the previous case is solved by reducing it to the next case
        assert (cursor->parent->left == cursor); // We are a right child of a right child

        struct RBTree* result = rbtree_rotate_right(tree, cursor->parent->parent);
        if (result->parent == NULL) *tree = result;

        if (partial_results) printf("after second rotation:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);

        assert(result->color == RBTREE_COLOR_RED);
        assert(result->right != NULL);
        assert(result->right->color == RBTREE_COLOR_BLACK);
        result->color = RBTREE_COLOR_BLACK; // this is now node
        result->right->color  = RBTREE_COLOR_RED; // this is now child

        if (partial_results) printf("after rotation repaint:\n");
        if (partial_results) rbtree_dump(*tree); fflush(stdout);

        *pcursor = *tree;
    }
}

static void rbtree_correct(struct RBTree** tree,
                           struct RBTree* cursor,
                           int partial_results)
{
    while (1)
    {
        if (partial_results) printf("cursor:%d\n", cursor->value); fflush(stdout);
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
            rbtree_correct_right(tree, &cursor, partial_results);
        }
        else // we are under the node that is left child of its parent
        {
            assert (cursor->parent->parent->left == cursor->parent);
            rbtree_correct_left(tree, &cursor, partial_results);
        }
    }
}

enum RBTreeError rbtree_insert(struct RBTree** tree,
                               int             value,
                               int             partial_results)
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

    if (partial_results) printf("after initial insertion:\n");
    if (partial_results) rbtree_dump(*tree);

    rbtree_correct(tree, newval, partial_results);

    return RBTREE_NO_ERROR;
}

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
            if (black_level != *max_black_level) printf("tree error: max black node level %d different to previos %d ", black_level, *max_black_level); fflush(stdout);
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

//static void rbtree_remove_swap_nodes(struct RBTree* node1, struct RBTree* node2)
//{
//    assert(node1 != NULL);
//    assert(node2 != NULL);
//    assert(node1 != node2);
//    if (node1 == node2->left || node1 == node2->right)
//    {
//        struct RBTree* temp =
//    }
//}

static void rbtree_remove_swap_remote_left_subtree(struct RBTree** tree, struct RBTree* node, struct RBTree* descendant)
{
    assert (descendant->parent       != NULL      ); // we use "rbtree_remove_swap_remote_left_subtree function" only in one place in "remove" function so this assertion is safe
    assert (descendant->parent->left == descendant); // we use "rbtree_remove_swap_remote_left_subtree function" only in one place in "remove" function so this assertion is safe
    assert (descendant->left         == NULL      ); // we use "rbtree_remove_swap_remote_left_subtree function" only in one place in "remove" function so this assertion is safe
    assert (node->left               != NULL      ); // we use "rbtree_remove_swap_remote_left_subtree function" only in one place in "remove" function so this assertion is safe
    assert (node->right              != NULL      ); // we use "rbtree_remove_swap_remote_left_subtree function" only in one place in "remove" function so this assertion is safe

    struct RBTree* temp;

    // swap outgoing parent pointers
    temp = node->parent;
    node->parent = descendant->parent;
    descendant->parent = temp;
    // fix incoming parent pointers
    assert (node->parent->left == descendant);
    node->parent->left = node;
    if (descendant->parent == NULL)
    {
        assert (*tree == node);
        *tree = descendant;
    }
    else
    {
        if (descendant->parent->right == node) descendant->parent->right = descendant;
        else
        {
            assert (descendant->parent->left == node);
            descendant->parent->left = descendant;
        }
    }

    // swap outgoing right pointers
    temp = node->right;
    node->right = descendant->right;
    descendant->right = temp;
    // fix incoming right pointers
    assert(descendant->right != NULL);
    descendant->right->parent = descendant;
    if (node->right != NULL) node->right->parent = node;

    // swap outgoing left pointers
    descendant->left = node->left;
    node->left = NULL;
    // fix incoming left pointers
    if (descendant->left != NULL) descendant->left->parent = descendant; // TODO if assertion "node->left != NULL" doesn't fail, this if is not needed

    enum RBTreeColor tempc = node->color;
    node->color = descendant->color;
    descendant->color = tempc;
}

static void rbtree_remove_swap_adjacent_right_child(struct RBTree** tree, struct RBTree* node, struct RBTree* child)
{
    assert (child->parent == node);
    assert (node->right == child);
    assert (child->left == NULL); // we use "rbtree_remove_swap_adjacent_right_child function" only in one place in "remove" function so this assertion is safe
    assert (node->left != NULL); // we use "rbtree_remove_swap_adjacent_right_child function" only in one place in "remove" function so this assertion is safe

    // swap outgoing parent pointers
    child->parent = node->parent;
    node->parent = child;
    // fix incoming parent pointers
    if (child->parent == NULL)
    {
        assert (*tree == node);
        *tree = child;
    }
    else
    {
        if (child->parent->right == node) child->parent->right = child;
        else
        {
            assert (child->parent->left == node);
            child->parent->left = child;
        }
    }

    // swap outgoing right pointers
    node->right = child->right;
    child->right = node;
    // fix incoming right pointers
    if (node->right != NULL) node->right->parent = node;

    // swap outgoing left pointers
    child->left = node->left;
    node->left = NULL;
    // fix incoming left pointers
    if (child->left != NULL) child->left->parent = child; // TODO: if assertion "node->left != NULL" doesn't fail, this "if" is not needed

    enum RBTreeColor temp = node->color;
    node->color = child->color;
    child->color = temp;
}

static void rbtree_remove_find_replacement_and_swap(struct RBTree** tree, struct RBTree* cursor, int partial_results)
{
    struct RBTree* replacement = cursor->right;
    if (replacement->left == NULL) // swap adjecent nodes
    {
        rbtree_remove_swap_adjacent_right_child(tree, cursor, replacement);

        if (partial_results) printf("after swap adjecent, cursor=%d\n", cursor->value); fflush(stdout);
        if (partial_results) rbtree_dump(*tree); fflush(stdout);
    }
    else // find leftmost node and swap separate nodes
    {
        while (replacement->left != NULL) replacement = replacement->left;

        rbtree_remove_swap_remote_left_subtree(tree, cursor, replacement);
        if (partial_results) printf("after swap remote, cursor=%d\n", cursor->value); fflush(stdout);
        if (partial_results) rbtree_dump(*tree); fflush(stdout);
    }
}

static void rbtree_remove_snip_red_leaf(struct RBTree* cursor)
{
    assert (cursor->left == NULL);
    assert (cursor->right == NULL);
    assert (cursor->parent != NULL);
    if (cursor->parent->right == cursor) cursor->parent->right = NULL;
    else
    {
        assert (cursor->parent->left == cursor);
        cursor->parent->left = NULL;
    }
    free(cursor);
}

static void rbtree_remove_cut_out_black_node(struct RBTree** tree, struct RBTree* cursor)
{
    assert (cursor->left == NULL || cursor->right == NULL);
    assert (cursor->left != NULL || cursor->right != NULL);

    struct RBTree* child = cursor->left;
    if (child == NULL) child = cursor->right;

    assert (child != NULL);
    assert (child->color == RBTREE_COLOR_RED);

    child->parent = cursor->parent;
    if (cursor->parent == NULL) *tree = child;
    else
    {
        if (cursor->parent->right == cursor) cursor->parent->right = child;
        else
        {
            assert (cursor->parent->left == cursor);
            cursor->parent->left = child;
        }
    }
    child->color = RBTREE_COLOR_BLACK;
    free(cursor);
}

enum RBTreeError rbtree_remove(struct RBTree** tree,
                               int             value,
                               int             partial_results)
{
    assert (tree != NULL); // we must be given valid pointer to a pointer to a tree
    assert (*tree == NULL || (*tree)->parent == NULL); // tree can be empty or we can operate on root but we can't start from the middle

    struct RBTree* cursor = *tree;

    while (1)
    {
        if (cursor == NULL) return RBTREE_REMOVE_ERROR_NOT_FOUND;

        if (value == cursor->value) break; // Found it!

        if (value < cursor->value) cursor = cursor->left;
        else                       cursor = cursor->right;
    }

    if (partial_results) printf("found node: %d\n", cursor->value); fflush(stdout);

    if (cursor->left != NULL && cursor->right != NULL) rbtree_remove_find_replacement_and_swap(tree, cursor, partial_results);

    assert (cursor->left == NULL || cursor->right == NULL);
    if (cursor->color == RBTREE_COLOR_RED)
    {
        rbtree_remove_snip_red_leaf(cursor);
        return RBTREE_NO_ERROR;
    }
    if (cursor->color == RBTREE_COLOR_BLACK && (cursor->left != NULL || cursor->right != NULL))
    {
        rbtree_remove_cut_out_black_node(tree, cursor);
        return RBTREE_NO_ERROR;
    }

    assert (cursor->left == NULL);
    assert (cursor->right == NULL);
    assert (cursor->color == RBTREE_COLOR_BLACK);
    // delete lone black node
    struct RBTree* to_delete = cursor;
    if (cursor->parent == NULL)
    {
        assert (cursor->left == NULL);
        assert (cursor->right == NULL);
        assert (*tree == cursor);
        free(cursor);
        *tree = NULL;
        return RBTREE_NO_ERROR;
    }
    while (1)
    {
        struct RBTree* parent = cursor->parent;
        if (parent == NULL)
        {
            assert (*tree == cursor);
            assert (cursor != to_delete);
            assert (to_delete->parent != NULL);
            if (to_delete->parent->right == to_delete) to_delete->parent->right = NULL;
            else
            {
                assert (to_delete->parent->left == to_delete);
                to_delete->parent->left = NULL;
            }
            free(to_delete);
            return RBTREE_NO_ERROR;
        }
        if (parent->right == cursor)
        {
            struct RBTree* sibling = parent->left;
            assert (sibling != NULL);
            if (sibling->color == RBTREE_COLOR_RED)
            {
                sibling->color = RBTREE_COLOR_BLACK;
                parent->color = RBTREE_COLOR_RED;
                if (partial_results) printf("after recoloring red sibling, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                rbtree_rotate_right(tree, cursor->parent);
                if (partial_results) printf("after rotating red sibling right, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
            }
            else if (sibling->color == RBTREE_COLOR_BLACK &&
                     (sibling->left == NULL || sibling->left->color  == RBTREE_COLOR_BLACK) &&
                     (sibling->right == NULL || sibling->right->color == RBTREE_COLOR_BLACK))
            {
                assert ((sibling->left == NULL && sibling->right == NULL) || (sibling->left != NULL && sibling->right != NULL));
                sibling->color = RBTREE_COLOR_RED;
                if (partial_results) printf("after painting black sibling red, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                if (parent->color == RBTREE_COLOR_BLACK) cursor = parent;
                else
                {
                    assert (parent->color == RBTREE_COLOR_RED);
                    parent->color = RBTREE_COLOR_BLACK;
                    if (partial_results) printf("after painting red parent black, cursor=%d\n", cursor->value); fflush(stdout);
                    if (partial_results) rbtree_dump(*tree); fflush(stdout);
                    assert (to_delete->parent != NULL);
                    if (to_delete->parent->right == to_delete) to_delete->parent->right = NULL;
                    else
                    {
                        assert (to_delete->parent->left == to_delete);
                        to_delete->parent->left = NULL;
                    }
                    free(to_delete);
                    return RBTREE_NO_ERROR;
                }
            }
            else
            {
                assert (sibling->color == RBTREE_COLOR_BLACK);
                assert ((sibling->left  != NULL && sibling->left->color  == RBTREE_COLOR_RED) ||
                        (sibling->right != NULL && sibling->right->color == RBTREE_COLOR_RED));
                if (sibling->left == NULL || sibling->left->color == RBTREE_COLOR_BLACK)
                {
                    assert (sibling->right != NULL && sibling->right->color == RBTREE_COLOR_RED);
                    rbtree_rotate_left(tree, sibling);
                    if (partial_results) printf("after left prerotation, cursor=%d\n", cursor->value); fflush(stdout);
                    if (partial_results) rbtree_dump(*tree); fflush(stdout);
                    sibling = parent->left;
                    assert (sibling != NULL);
                }
                assert (sibling->left != NULL); // It was either there in the first place or it was ariginally a sibling but was moved to sibling->right due to rotation
                sibling->left->color = RBTREE_COLOR_BLACK;
                sibling->color = parent->color;
                parent->color = RBTREE_COLOR_BLACK;
                if (partial_results) printf("after recoloration, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                rbtree_rotate_right(tree, parent);
                if (partial_results) printf("after right rotate, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                assert (to_delete->parent != NULL);
                if (to_delete->parent->right == to_delete) to_delete->parent->right = NULL;
                else
                {
                    assert (to_delete->parent->left == to_delete);
                    to_delete->parent->left = NULL;
                }
                free(to_delete);
                return RBTREE_NO_ERROR;
            }
        }
        else
        {
            assert (parent->left == cursor);
            // proceed with mirror of above
            struct RBTree* sibling = parent->right;
            assert (sibling != NULL);
            if (sibling->color == RBTREE_COLOR_RED)
            {
                sibling->color = RBTREE_COLOR_BLACK;
                parent->color = RBTREE_COLOR_RED;
                if (partial_results) printf("after recoloring red sibling, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                rbtree_rotate_left(tree, cursor->parent);
                if (partial_results) printf("after rotating red sibling left, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
            }
            else if (sibling->color == RBTREE_COLOR_BLACK &&
                     (sibling->left == NULL || sibling->left->color  == RBTREE_COLOR_BLACK) &&
                     (sibling->right == NULL || sibling->right->color == RBTREE_COLOR_BLACK))
            {
                assert ((sibling->left == NULL && sibling->right == NULL) || (sibling->left != NULL && sibling->right != NULL));
                sibling->color = RBTREE_COLOR_RED;
                if (partial_results) printf("after painting black sibling red, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                if (parent->color == RBTREE_COLOR_BLACK) cursor = parent;
                else
                {
                    assert (parent->color == RBTREE_COLOR_RED);
                    parent->color = RBTREE_COLOR_BLACK;
                    if (partial_results) printf("after painting red parent black, cursor=%d\n", cursor->value); fflush(stdout);
                    if (partial_results) rbtree_dump(*tree); fflush(stdout);
                    assert (to_delete->parent != NULL);
                    if (to_delete->parent->right == to_delete) to_delete->parent->right = NULL;
                    else
                    {
                        assert (to_delete->parent->left == to_delete);
                        to_delete->parent->left = NULL;
                    }
                    free(to_delete);
                    return RBTREE_NO_ERROR;
                }
            }
            else
            {
                assert (sibling->color == RBTREE_COLOR_BLACK);
                assert ((sibling->left  != NULL && sibling->left->color  == RBTREE_COLOR_RED) ||
                        (sibling->right != NULL && sibling->right->color == RBTREE_COLOR_RED));
                if (sibling->right == NULL || sibling->right->color == RBTREE_COLOR_BLACK)
                {
                    assert (sibling->left != NULL && sibling->left->color == RBTREE_COLOR_RED);
                    rbtree_rotate_right(tree, sibling);
                    if (partial_results) printf("after right prerotation, cursor=%d\n", cursor->value); fflush(stdout);
                    if (partial_results) rbtree_dump(*tree); fflush(stdout);
                    sibling = parent->right;
                    assert (sibling != NULL);
                }
                assert (sibling->right != NULL); // It was either there in the first place or it was ariginally a sibling but was moved to sibling->right due to rotation
                sibling->right->color = RBTREE_COLOR_BLACK;
                sibling->color = parent->color;
                parent->color = RBTREE_COLOR_BLACK;
                if (partial_results) printf("after recoloration, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                rbtree_rotate_left(tree, parent);
                if (partial_results) printf("after right rotate, cursor=%d\n", cursor->value); fflush(stdout);
                if (partial_results) rbtree_dump(*tree); fflush(stdout);
                assert (to_delete->parent != NULL);
                if (to_delete->parent->right == to_delete) to_delete->parent->right = NULL;
                else
                {
                    assert (to_delete->parent->left == to_delete);
                    to_delete->parent->left = NULL;
                }
                free(to_delete);
                return RBTREE_NO_ERROR;
            }
        }
    }
}

void rbtree_destroy(struct RBTree** tree)
{
    struct RBTree* cursor = *tree;
    while (1)
    {
        if      (cursor->left  != NULL) cursor = cursor->left;
        else if (cursor->right != NULL) cursor = cursor->right;
        else
        {
            struct RBTree* parent = cursor->parent;
            free(cursor);
            // Here is dengerous part. Cursor is already freed but either parent->left or parent->right is still pointing to it.
            if (parent == NULL) break;
            if (parent->right == cursor) parent->right = NULL;
            else
            {
                assert (parent->left == cursor);
                parent->left = NULL;
            }
            cursor = parent;
        }
    }
    *tree = NULL;
}
