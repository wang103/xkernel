/**
 * Test all the data structures.
 */

#include <assert.h>
#include <stddef.h>
#include "linked_list.h"
#include "queue.h"
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

}

void test_rbtree() {
    struct rb_root root = RB_ROOT;
}

int main() {
    test_linked_list();
    
    test_queue();

    test_rbtree();

    printf("All tests successfully passed!\n");

    return 0;
}
