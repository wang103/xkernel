/**
 * Test all the data structures.
 */

#include <assert.h>
#include <stddef.h>
#include "linked_list.h"
#include "rbtree.h"

typedef struct _os {
    int os_number;
    struct list_head list;
} os;

void test_linked_list() {
    // Declare a list head.
    LIST_HEAD(os_list);

    assert(list_empty(&os_list) == 1);

    // Add a new node to the list.
    os *new_os_1 = (os *)malloc(sizeof(os));
    new_os_1->os_number = 0;
    list_add(&new_os_1->list, &os_list);

    // Add one more node to the list.
    os *new_os_2 = (os *)malloc(sizeof(os));
    new_os_2->os_number = 1;
    list_add_tail(&new_os_2->list, &os_list);

    assert(list_empty(&os_list) == 0);

    struct _os *cur_os;
    int counter = 0;
    list_for_each_entry(cur_os, &os_list, list) {
        assert(cur_os->os_number == counter);
        counter++;
    }

    // Remove the added nodes.
    list_del(&new_os_1->list);
    list_del(&new_os_2->list);

    assert(list_empty(&os_list) == 1);

    free(new_os_2);
    free(new_os_1);
}

void test_queue() {
    // Queue is not tested here, instead it's done in xkernel.c.
}

typedef struct _character {
    int char_number;
    struct rb_node node;
} character;

void test_rbtree() {
    struct rb_root root = RB_ROOT;

    assert(RB_EMPTY_ROOT(&root) == 1);

    // Add a new node to the tree.
    character *new_char_1 = (character *)malloc(sizeof(character));
    new_char_1->char_number = 1;
    rb_link_node(&(new_char_1->node), root.rb_node, &(root.rb_node));
    rb_insert_fixup(&new_char_1->node, &root);

    assert(RB_EMPTY_ROOT(&root) == 0);

    // Add one more node to the tree.
    character *new_char_2 = (character *)malloc(sizeof(character));
    new_char_2->char_number = 2;
    rb_link_node(&(new_char_2->node), root.rb_node, &(root.rb_node->right));
    rb_insert_fixup(&new_char_2->node, &root);

    assert(RB_EMPTY_ROOT(&root) == 0);

    // Add one more node to the tree.
    character *new_char_3 = (character *)malloc(sizeof(character));
    new_char_3->char_number = 3;
    rb_link_node(&(new_char_3->node), &(new_char_2->node), &(new_char_2->node.right));
    rb_insert_fixup(&new_char_3->node, &root);

    assert(RB_EMPTY_ROOT(&root) == 0);

    character *cur_char;
    cur_char = rb_entry(root.rb_node, character, node);
    assert(cur_char->char_number == 2);
    cur_char = rb_entry(root.rb_node->left, character, node);
    assert(cur_char->char_number == 1);
    cur_char = rb_entry(root.rb_node->right, character, node);
    assert(cur_char->char_number == 3);

    // Remove the added nodes.
    rb_erase(&new_char_2->node, &root);
    cur_char = rb_entry(root.rb_node, character, node);
    assert(cur_char->char_number == 3);
    cur_char = rb_entry(root.rb_node->left, character, node);
    assert(cur_char->char_number == 1);
    assert(root.rb_node->right == NULL);

    rb_erase(&new_char_3->node, &root);
    cur_char = rb_entry(root.rb_node, character, node);
    assert(cur_char->char_number == 1);
    assert(root.rb_node->left == NULL);
    assert(root.rb_node->right == NULL);

    rb_erase(&new_char_1->node, &root);
    assert(root.rb_node == NULL);

    assert(RB_EMPTY_ROOT(&root) == 1);

    free(new_char_3);
    free(new_char_2);
    free(new_char_1);
}

void test_map() {
    // Map is not tested here, instead it's done in xkernel.c.
}

int main() {
    test_linked_list();
    
    test_queue();

    test_rbtree();

    test_map();

    printf("All tests successfully passed!\n");

    return 0;
}
