/**
 * Test all the data structures.
 */

#include "linked_list.h"

typedef struct _os {
    int os_number;
    struct list_head list;
} os;

int main() {
    /******************************************
     *          Test the linked list          *
     ******************************************/

    // Declare a list head.
    LIST_HEAD(os_list);

    // Add a new element to the list.
    os *new_os_1 = (os *)malloc(sizeof(os));
    new_os_1->os_number = 0;
    list_add(&new_os_1->list, &os_list);

    // Add one more element to the list.
    os *new_os_2 = (os *)malloc(sizeof(os));
    new_os_2->os_number = 1;
    list_add_tail(&new_os_2->list, &os_list);


    free(new_os_2);
    free(new_os_1);

    printf("All tests successfully passed!\n");

    return 0;
}
