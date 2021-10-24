#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Path {
    Stack *vertices;
    uint32_t length;
};

Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->vertices = stack_create(VERTICES + 1);
        p->length = 0;
    }
    return p;
}
void path_delete(Path **p) {
    if (*p) {
        stack_delete(&(*p)->vertices);
        free(*p);
        *p = NULL;
    }
}
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t hold = 0;
    stack_peek(p->vertices, &hold);

    if (stack_push(p->vertices, v) == true) {
        if (hold != v) {
            p->length += graph_edge_weight(G, hold, v);
        }
        return true;
    } else {
        return false;
    }
}
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_pop(p->vertices, v) == true) {
        uint32_t hold = 0;
        stack_peek(p->vertices, &hold);
        if (hold != *v) {
            p->length -= graph_edge_weight(G, hold, *v);
        }
        return true;
    } else {
        return false;
    }
}
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}
uint32_t path_length(Path *p) {
    return p->length;
}
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d", p->length);
    fprintf(outfile, "Path: ");
    stack_print(p->vertices, outfile, cities);
}