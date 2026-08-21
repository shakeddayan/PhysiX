#include "calculator.h"
#include "constants.h"
#include "structs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* CALCULATOR FUNCTIONS */

float rad_to_deg(float rad) {
    return rad * 180 / PI;
}

float deg_to_rad(float deg){
    return deg * PI / 180;
}

P_Vector *A_vec_TO_P_vec(A_Vector *vector) {
    P_Vector *res = malloc(sizeof(P_Vector));
    res->direction = atanf(vector->y / vector->x);
    res->size = sqrtf(powf(vector->x, (float)2) + powf(vector->y, (float)2));
    return res;
}

A_Vector *P_vec_TO_A_vec(P_Vector *vector) {
    A_Vector *res = malloc(sizeof(A_Vector));
    res->x = vector->size * cosf(vector->direction);
    res->y = vector->size * sinf(vector->direction);
    return res;
}

A_Vector *calc_force_sum(Obj *obj) {
    return rep_vec_sum(obj->forces, obj->force_count);
}

A_Vector *vec_sum(A_Vector *vec1, A_Vector *vec2) {
    A_Vector *sum = malloc(sizeof(A_Vector));
    sum->x = vec1->x + vec2->x;
    sum->y = vec1->y + vec2->y;
    return sum;
}

void vec_sum_override(A_Vector *vec1, A_Vector *vec2) {
    vec1 -> x += vec2 -> x;
    vec1 -> y += vec2 -> y;
}

A_Vector *rep_vec_sum(A_Vector *vecs, int amount) {
    A_Vector *sum = calloc(1, sizeof(A_Vector)); //calloc so it will be zeroed.
    if (sum == NULL) {
        printf("mem alloc at 46");
        return NULL;
    }
    for (int i = 0; i < amount  && vecs != NULL; i++) {
        vec_sum_override(sum, vecs++); //sum the current vector and move the pointer.
    }
    return sum;
}