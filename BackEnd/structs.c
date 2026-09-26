#include "structs.h"
#include <stdio.h>
#include <stdlib.h>


//creates a Data module.
Data *createData() {
    //malloc the base struct
    Data *d = calloc(1, sizeof(Data)); //zeros all pointers
    if (d == NULL)
        return NULL;

    //malloc all arrays
    d->m = malloc(DATA_SIZE * sizeof(float));
    d->inv_m = malloc(DATA_SIZE * sizeof(float));
    d->pos_x = malloc(DATA_SIZE * sizeof(float));
    d->pos_y = malloc(DATA_SIZE * sizeof(float));
    d->speed_x = malloc(DATA_SIZE * sizeof(float));
    d->speed_y = malloc(DATA_SIZE * sizeof(float));
    d->acc_x = malloc(DATA_SIZE * sizeof(float));
    d->acc_y = malloc(DATA_SIZE * sizeof(float));
    d->net_force_x = malloc(DATA_SIZE * sizeof(float));
    d->net_force_y = malloc(DATA_SIZE * sizeof(float));
    d->type = malloc(DATA_SIZE * sizeof(char));

    //any of the allocations failed: (one of them is NULL)
    if (!d->inv_m || !d->pos_x || !d->pos_y ||
        !d->speed_x || !d->speed_y || !d->acc_x || !d->acc_y ||
        !d->net_force_x || !d->net_force_y || !d->type) {
        free(d->inv_m);
        free(d->m);
        free(d->pos_x); free(d->pos_y);
        free(d->speed_x); free(d->speed_y);
        free(d->acc_x); free(d->acc_x);
        free(d->net_force_x); free(d->net_force_y);
        free(d->type);
        free(d);
        return NULL;
    }

    //control variables
    d->count = 0;
    d->capacity = DATA_SIZE;
    return d;
}

//add an object to the data struct
void add_obj(
    Data *data,
    float mass, float pos_x, float pos_y
    , float speed_x, float speed_y,
    char type
    ) {

    void *failsafe; //for safe reallocs

    //check need to realloc
    if (data->count == data->capacity) { //if reached full capacity usage

        //realloc mass
        failsafe = realloc(data->m, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->m = failsafe;

        failsafe = realloc(data->inv_m, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->inv_m = failsafe;

        //realloc pos
        failsafe = realloc(data->pos_x, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->pos_x = failsafe;

        failsafe = realloc(data->pos_y, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->pos_y = failsafe;

        //realloc speed
        failsafe = realloc(data->speed_x, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->speed_x = failsafe;

        failsafe = realloc(data->speed_y, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->speed_y = failsafe;

        //realloc acc
        failsafe = realloc(data->acc_x, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->acc_x = failsafe;

        failsafe = realloc(data->acc_y, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->acc_y = failsafe;

        //realloc forces
        failsafe = realloc(data->net_force_x, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->net_force_x = failsafe;

        //realloc force_count
        failsafe = realloc(data->net_force_y, sizeof(float)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->net_force_y = failsafe;

        //realloc type
        failsafe = realloc(data->type, sizeof(char)*(data->capacity + DATA_INC));
        if (failsafe == NULL) {
            printf("ERROR: Out of memory. Can't add another object.");
            return;
        }
        data->type = failsafe;

        data->capacity += DATA_INC; //update the capacity if all went well.
    }


    //copy part
    data->m[data->count] = mass; //save inverse to reduce calculation times
    data->inv_m[data->count] = 1.0f / mass; //save inverse to reduce calculation times
    data->pos_x[data->count] = pos_x;
    data->pos_y[data->count] = pos_y;
    data->speed_x[data->count] = speed_x;
    data->speed_y[data->count] = speed_y;
    data->type[data->count] = type;

    //handle forces and accelerations later, as it is more complex and dynamic at runtime.
    data->acc_x[data->count] = 0;
    data->acc_y[data->count] = 0;

    data->net_force_x[data->count] = 0;
    data->net_force_y[data->count] = 0;

    data->count++; //update counter
}

//remove an object from the data struct by the index
void rm_index(int index, Data *data) {

    if (index < 0 || index >= data->count)
        return;

    data->count--;

    if (index != data->count) {
        data->m[index] = data->m[data->count];
        data->inv_m[index] = data->inv_m[data->count];
        data->pos_x[index] = data->pos_x[data->count];
        data->pos_y[index] = data->pos_y[data->count];
        data->speed_x[index] = data->speed_x[data->count];
        data->speed_y[index] = data->speed_y[data->count];
        data->acc_x[index] = data->acc_x[data->count];
        data->acc_y[index] = data->acc_y[data->count];
        data->net_force_x[index] = data->net_force_x[data->count];
        data->net_force_y[index] = data->net_force_y[data->count];
        data->type[index] = data->type[data->count];
    }
}
//free a Data module
void freeData(Data *data) {
    free(data->m);
    free(data->inv_m);
    free(data->pos_x); free(data->pos_y);
    free(data->speed_x); free(data->speed_y);
    free(data->acc_x); free(data->acc_y);
    free(data->net_force_x); free(data->net_force_y);
    free(data->type);
    free(data);
}