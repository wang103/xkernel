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

/**
 * Fix the colors and perform necessary rotations to re-balance the tree after
 * a deletion.
 */
static void rb_erase_fixup(struct rb_node *node, struct rb_node *parent,
        int is_left_child, struct rb_root *root) {
    struct rb_node *sibling;

    while (node != root->rb_node && (node == NULL || node->is_red == 0)) {
        if (is_left_child) {
            sibling = parent->right;

            // First make sure the sibling is black.
            if (sibling->is_red) {
                // Since sibling is red, parent must be black. Swap color
                // between parent and sibling, rotate, and update sibling.
                sibling->is_red = 0;
                parent->is_red = 1;
                left_rotate(parent, root);
                sibling = parent->right;
            }

            // Now the sibling must be black.
            if ((sibling->left == NULL || sibling->left->is_red == 0) &&
                (sibling->right == NULL || sibling->right->is_red == 0)) {
                // Both left child and right child are black, now we can
                // balance it just by making sibling red.
                sibling->is_red = 1;
            }
            else {
                if (sibling->right->is_red == 0) {
                    // Make sure the sibling's left child is black, right child
                    // is red.
                    sibling->left->is_red = 0;
                    sibling->is_red = 1;
                    right_rotate(sibling, root);

                    sibling = parent->right;
                }

                // Now the sibling's left child is black, right child is red.
                sibling->is_red = parent->is_red;
                parent->is_red = 0;
                sibling->right->is_red = 0;
                left_rotate(parent, root);

                node = root;
                continue;
            }
        } else {
            sibling = parent->left;

            // First make sure the sibling is black.
            if (sibling->is_red) {
                // Since sibling is red, parent must be black. Swap color
                // between parent and sibling, rotate, and update sibling.
                sibling->is_red = 0;
                parent->is_red = 1;
                right_rotate(parent, root);
                sibling = parent->left;
            }
            
            // Now the sibling must be black.
            if ((sibling->left == NULL || sibling->left->is_red == 0) &&
                (sibling->right == NULL || sibling->right->is_red == 0)) {
                // Both left child and right child are black, now we can
                // balance it just by making sibling red.
                sibling->is_red = 1;
            }
            else {
                if (sibling->left->is_red == 0) {
                    // Make sure the sibling's right child is black, left child
                    // is red.
                    sibling->right->is_red = 0;
                    sibling->is_red = 1;
                    left_rotate(sibling, root);

                    sibling = parent->left;
                }

                // Now the sibling's right child is black, left child is red.
                sibling->is_red = parent->is_red;
                parent->is_red = 0;
                sibling->left->is_red = 0;
                right_rotate(parent, root);

                node = root;
                continue;
            }
        }
        
        // Update.
        node = parent;
        parent = parent->parent;
        if (node == parent->left) {
            is_left_child = 1;
        } else {
            is_left_child = 0;
        }
    }

    node->is_red = 0;
}

void rb_erase(struct rb_node *node, struct rb_root *root) {
    struct rb_node *spliced_out_node;
    if (node->left == NULL || node->right == NULL) {
        spliced_out_node = node;
    } else {
        spliced_out_node = successor(node);
    }

    struct rb_node *child;
    if (spliced_out_node->left != NULL) {
        child = spliced_out_node->left;
    } else {
        child = spliced_out_node->right;
    }

    if (child != NULL) {
        child->parent = spliced_out_node->parent;
    }

    int is_left_child;
    if (spliced_out_node->parent == NULL) {
        root->rb_node = child;
    } else if (spliced_out_node == spliced_out_node->parent->left) {
        spliced_out_node->parent->left = child;
        is_left_child = 1;
    } else {
        spliced_out_node->parent->right = child;
        is_left_child = 0;
    }

    int old_color = spliced_out_node->is_red;
    if (spliced_out_node != node) {
        spliced_out_node->parent = node->parent;
        if (node->parent == NULL) {
            root->rb_node = spliced_out_node;
        } else if (node == node->parent->left) {
            node->parent->left = spliced_out_node;
        } else {
            node->parent->right = spliced_out_node;
        }

        spliced_out_node->left = node->left;
        if (node->left != NULL) {
            node->left->parent = spliced_out_node;
        }

        spliced_out_node->right = node->right;
        if (node->right != NULL) {
            node->right->parent = spliced_out_node;
        }

        spliced_out_node->is_red = node->is_red;

        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
    }

    // Rebalance the tree if needed.
    if (root->rb_node != NULL && old_color == 0) {
        rb_erase_fixup(child, child->parent, is_left_child, root);
    }
}
