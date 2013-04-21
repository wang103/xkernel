#include "rbtree.h"

struct rb_node *minimum(struct rb_node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

struct rb_node *maximum(struct rb_node *node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

struct rb_node *predecessor(struct rb_node *node) {
    if (node->left != NULL) {
        return maximum(node->left);
    }
    rb_node *parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

struct rb_node *successor(struct rb_node *node) {
    if (node->right != NULL) {
        return minimum(node->right);
    }
    rb_node *parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

/**
 * Assume the right child of node is not NULL.
 */
static void leftRotate(rb_node *node, struct rb_root *root) {
    rb_node *y = node->right;

    node->right = y->left;
    if (y->left != NULL) {
        y->left->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == NULL) {
        root->rb_node = node;
    } else {
        if (node == node->parent->left) {
            node->parent->left = y;
        } else {
            node->parent->right = y;
        }
    }

    y->left = node;
    node->parent = y;
}

/**
 * Assume the left child of node is not NULL.
 */
static void rightRotate(rb_node *node, rb_root *root) {
    rb_node *x = node->left;

    node->left = x->right;
    if (x->right != NULL) {
        x->right->parent = node;
    }

    x->parent = node->parent;

    if (node->parent == NULL) {
        root->rb_node = node;
    } else {
        if (node == node->parent->left) {
            node->parent->left = x;
        } else {
            node->parent->right = x;
        }
    }

    x->right = node;
    node->parent = x;
}

void rb_insert_fixup(struct rb_node *node, struct rb_root *root) {
    // Correct if the property of RB tree is violated.
    while (node != root && node->parent->is_red) {
        // node is red, so is its parent, this needs to be fixed.
        // Since the root of the tree is always black, we know for sure that
        // the parent of node is not the root, and the parent of parent of node
        // exists.
        // We also know that the node's parent's parent is black.


    }

    // Ensure the root of the tree is always black.
    root->is_red = 0;
}

void rb_erase_fixup(struct rb_node *node, struct rb_root *root) {

}
