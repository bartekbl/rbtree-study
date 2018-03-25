#ifndef RBTREE_H
#define RBTREE_H

enum RBTreeError
{
    RBTREE_NO_ERROR                   =  0,
    RBTREE_INSERT_ERROR_CANT_ALLOCATE = -1,
    RBTREE_REMOVE_ERROR_NOT_FOUND     = -2,
};

enum RBTreeColor
{
    RBTREE_COLOR_RED,
    RBTREE_COLOR_BLACK
};

struct RBTree
{
    struct RBTree*   left;
    struct RBTree*   right;
    struct RBTree*   parent;
    enum RBTreeColor color;
    int              value;
};

enum RBTreeError rbtree_init(struct RBTree** tree);

enum RBTreeError rbtree_insert(struct RBTree** tree,
                               int             value,
                               int             partial_results);

// This is just a debug function so it is not optimized for speed or memory (it can use recursion!)
void rbtree_verify(const struct RBTree* tree, int expected_size);

enum RBTreeError rbtree_remove(struct RBTree** tree,
                               int             value,
                               int             partial_results);

void rbtree_destroy(struct RBTree** tree);

#endif // RBTREE_H
