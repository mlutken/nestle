#include <cesl_debug/cesl_debug.h>
#include <cesl_concurrent/cesl_llfifo.h>
#include <string.h>

#define my_fifo_elem_max_size 1024
#define my_fifo_elems_max_count 4

static char my_fifo_buffer_g[my_fifo_elems_max_count][my_fifo_elem_max_size];
static llfifo_t my_fifo_g;

static int test_llfifo_push(const char* str)
{
    return cesl_llfifo_push(&my_fifo_g, str, strlen(str));
}

// static char* test_llfifo_front_strcpy(char* dst_str)
// {
//     return strncpy(dst_str, llfifo_front(&my_fifo_g), my_fifo_elem_max_size);
// }

int main()
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    test_llfifo_push("second");
    test_llfifo_push("third");
    const char* front_ptr = cesl_llfifo_front(&my_fifo_g);
    int empty = cesl_llfifo_empty(&my_fifo_g);
    int full = cesl_llfifo_full(&my_fifo_g);
    size_t size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("1: 3 elements pushed ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    test_llfifo_push("fourth");
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf( "2: 4 elements pushed ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("3: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    test_llfifo_push("fourth");
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("4: push 'fourth' again; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("5: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    test_llfifo_push("fifth");
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("6: push 'fifth' ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("7: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("8: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("9: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );

    cesl_llfifo_pop(&my_fifo_g);
    front_ptr = cesl_llfifo_front(&my_fifo_g);
    empty = cesl_llfifo_empty(&my_fifo_g);
    full = cesl_llfifo_full(&my_fifo_g);
    size = cesl_llfifo_size(&my_fifo_g);
    cesl_dprintf("10: pop (front) ; size: '%d', empty: '%d',  full: '%d', front: '%s' \n", size, empty, full, front_ptr );


    return 0;
}


int main1()
{
    cesl_dprintf("Lockless FIFO playground\n");
    const int res = cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    cesl_dprintf("llfifo_create()         : '%d'\n", res);
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));

    cesl_dprintf("llfifo_push('first')    : '%d'\n", test_llfifo_push("first"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_push('second')   : '%d'\n", test_llfifo_push("second"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_push('third')    : '%d'\n", test_llfifo_push("third"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_push('fourth')   : '%d'\n", test_llfifo_push("fourth"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));

    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));
    cesl_llfifo_pop(&my_fifo_g);
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));
    cesl_dprintf("llfifo_push('fourth')   : '%d'\n", test_llfifo_push("fourth"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));

    cesl_llfifo_pop(&my_fifo_g);
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));

    cesl_llfifo_pop(&my_fifo_g);
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));

    cesl_llfifo_pop(&my_fifo_g);
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));

    cesl_dprintf("llfifo_push('fifth')   : '%d'\n", test_llfifo_push("fifth"));
    cesl_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", cesl_llfifo_empty(&my_fifo_g), cesl_llfifo_full(&my_fifo_g));
    cesl_dprintf("llfifo_front()          : '%s'\n", cesl_llfifo_front(&my_fifo_g));

    return 0;
}
