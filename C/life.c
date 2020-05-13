#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

#define DIMX 8
#define DIMY 8 // floored to nearest multiple of 8

struct tuple_node;
struct tuple_node* stack_root = NULL;
byte* state;



/*  STACK DEFINITION
 *  ================
 */
struct tuple {
    int x;
    int y;
} tuple;

struct tuple_node {
    struct tuple* tp;
    struct tuple_node* next;
} tuple_node;

struct tuple_node* node_init(int x, int y) {
    struct tuple_node* tn = (struct tuple_node*)malloc(sizeof(struct tuple_node));
    tn->tp = (struct tuple*)malloc(sizeof(struct tuple));
    tn->tp->x = x;
    tn->tp->y = y;
    tn->next = NULL;
    return tn;
}

void stack_push(int x, int y) {
    struct tuple_node* tn = node_init(x, y);
    tn->next = stack_root;
    stack_root = tn;
}

struct tuple stack_pop(void) {
    if (!stack_root) exit(1);
    struct tuple_node* tmp = stack_root;
    stack_root = stack_root->next;
    struct tuple tp = *(tmp->tp);
    free(tmp->tp);
    free(tmp);
    return tp;
}

void stack_print() {
    for (struct tuple_node* curr = stack_root; curr; curr = curr->next)
        printf("x=%d y=%d\n", curr->tp->x, curr->tp->y);
}



/*  STATE REPRESENTATION
 *  ====================
 *  "all cells are just Booleans, so why not shove 8 in a byte?"
 *  what a fool I was.
 *
 *  Uses row-major order.
 *  The column length is (arbitrarily) a multiple of 8,
 *  so store groups of 8 in a byte. This makes addressing very bit-twiddly.
 */

void state_toggle(int ii, int jj) {
    byte* cur_byte = state + (ii / 8) + (DIMX * jj / 8);
    *cur_byte ^= (1 << (ii % 8));
}

int state_read(int ii, int jj) {
    byte* cur_byte = state + (ii / 8) + (DIMX * jj / 8);
    return (*cur_byte >> (ii % 8)) & 0x01;
}

int neighbor_count(int ii, int jj) {
    int count = 0;
    // TODO: what is the most efficient order to test these?
    if (ii != 0 && jj != 0)                 // TL Corner
        count += state_read(ii-1, jj-1);
    if (jj != 0)                            // T Edge
        count += state_read(ii,   jj-1);
    if (ii != DIMX && jj != 0)              // TR Corner
        count += state_read(ii+1, jj-1);
    if (ii != DIMX)                         // R Edge
        count += state_read(ii+1, jj);
    if (ii != DIMX && jj != DIMY)           // BR Corner
        count += state_read(ii+1, jj+1);
    if (jj != DIMY)                         // B Edge
        count += state_read(ii,   jj+1);
    if (ii != 0 && jj != DIMY)              // BL Corner
        count += state_read(ii-1, jj+1);
    if (ii != 0)                            // L Edge
        count += state_read(ii-1, jj);
    return count;
}



/*  EVOLUTION
 *  =========
 */
void evolve() {
    for (int ii = 0; ii < DIMX; ii++) {
        for (int jj = 0; jj < DIMY; jj++) {
            int neighbors = neighbor_count(ii, jj);
            if (state_read(ii, jj)) {
                if (neighbors < 2)  { stack_push(ii, jj); continue; }
                if (neighbors > 3)  { stack_push(ii, jj); continue; }
            }
            else
                if (neighbors == 3) { stack_push(ii, jj); continue; }

        }
    }
    while(stack_root) {
        struct tuple tp = stack_pop();
        state_toggle(tp.x, tp.y);
        stack_root = stack_root->next;
    }
    printf("popped\n");
}

void render_board(void) {
    for(int jj = 0; jj < DIMY; jj++) {
        for(int ii = 0; ii < DIMX / 8; ii++) {
            byte* b = state + (ii / 8) + (DIMX * jj / 8);
            for (int bb = 0; bb < 8; bb++)
                printf("%d", (*b >> bb) & 0x01);
        }
        printf("\n");
    }
}

int main(void) {
    state = (byte*)calloc(DIMX * DIMY / 8, sizeof(byte));
    state_toggle(1, 0);
    state_toggle(2, 1);
    state_toggle(2, 2);
    state_toggle(1, 2);
    state_toggle(0, 2);

    for (;;) {
        render_board();
        evolve();
    }

    free(state);
    return 0;
}
