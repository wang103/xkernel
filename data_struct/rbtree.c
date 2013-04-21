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
    struct rb_node *parent = node->parent;
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
    struct rb_node *parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

/**
 * Assume the right child of node is not NULL.
 */
static void left_rotate(struct rb_node *node, struct rb_root *root) {
    struct rb_node *y = node->right;

    node->right = y->left;
    if (y->left != NULL) {
        y->left->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == NULL) {
        root->rb_node = y;
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
static void right_rotate(struct rb_node *node, struct rb_root *root) {
    struct rb_node *x = node->left;

    node->left = x->right;
    if (x->right != NULL) {
        x->right->parent = node;
    }

    x->parent = node->parent;

    if (node->parent == NULL) {
        root->rb_node = x;
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
    while (node != root->rb_node && node->parent->is_red) {
        // node is red, so is its parent, this needs to be fixed.
        // Since the root of the tree is always black, we know for sure that
        // the parent of node is not the root, and the parent of parent of node
        // exists.
        // We also know that the node's parent's parent is black.

        if (node->parent == node->parent->parent->left) {
            struct rb_node *uncle = node->parent->parent->right;
            if (uncle != NULL && uncle->is_red) {
                // Uncle node is also red, just need to swap color.
                node->parent->is_red = 0;
                uncle->is_red = 0;
                uncle->parent->is_red = 1;
                node = uncle->parent;
            } else {
                // Uncle node is black, needs rotation.
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(node, root);
                }
                node->parent->is_red = 0;
                node->parent->parent->is_red = 1;
                right_rotate(node->parent->parent, root);
            }
        } else {
            struct rb_node *uncle = node->parent->parent->left;
            if (uncle != NULL && uncle->is_red) {
                // Uncle node is also red, just need to swap color.
                node->parent->is_red = 0;
                uncle->is_red = 0;
                uncle->parent->is_red = 1;
                node = uncle->parent;
            } else {
                // Uncle node is black, needs rotation.
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(node, root);
                }
                node->parent->is_red = 0;
                node->parent->parent->is_red = 1;
                left_rotate(node->parent->parent, root);
            }
        }
    }

    // Ensure the root of the tree is always black.
    root->rb_node->is_red = 0;
}

void rb_erase_fixup(struct rb_node *node, struct rb_root *root) {

}
