#ifndef RBTREEVERIFY_H
#define RBTREEVERIFY_H

// This is just a debug function so it is not optimized for speed or memory (it can use recursion!)
void rbtree_verify(const struct RBTree* tree, int expected_size);

#endif // RBTREEVERIFY_H
