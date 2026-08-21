#include "structs.h"
#include "calculator.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK_RES \
    do{ \
        if (result != 1) { \
            printf("error - invalid input");\
            return NULL;\
        } \
    }while(0);

/* OOP FUNCTIONS */

//gets input and creates an object.
Obj *get_obj() {
    Obj *object = malloc(sizeof(Obj));
    if (object == NULL) return NULL; //allocation check

    // inner pointers allocation
    object->pos = malloc(sizeof(A_Vector));
    object->speed = malloc(sizeof(A_Vector));
    object->acc = malloc(sizeof(A_Vector));
    object->forces = NULL; // will be allocated later using realloc.
    object->force_count = 0;

    //for loops and input
    char input = '0';
    char input_inner = '0'; // for inner loops
    int result;

    //cast polar coordinates
    P_Vector caster;
    A_Vector *casted;

    void *failsafe; //for realloc

    // get mass
    printf("enter mass:\n");
    result = scanf("%f", &(object->mass));
    CHECK_RES

    // get position
    while (input != ARITHMETIC && input != POLAR) {
        printf("enter position in polar or arithmetic? (P/A)\n");
        result = scanf(" %c", &input);
        CHECK_RES

        if (input == ARITHMETIC) {
            printf("enter x:\n");
            result = scanf("%f", &(object->pos->x));
            CHECK_RES
            printf("enter y:\n");
            result = scanf("%f", &(object->pos->y));
            CHECK_RES
        }
        else if (input == POLAR) {
            printf("enter size:\n");
            result = scanf("%f", &(caster.size));
            CHECK_RES
            printf("enter direction:\n");
            result = scanf("%f", &(caster.direction));
            CHECK_RES
            caster.direction = deg_to_rad(caster.direction);
            casted = P_vec_TO_A_vec(&caster);
            object->pos->x = casted->x;
            object->pos->y = casted->y;
            free(casted);
        }
        else {
            printf("invalid input - try again.\n");
        }
    }
    input = '0';

    // get speed
    while (input != ARITHMETIC && input != POLAR) {
        printf("enter speed in polar or arithmetic? (P/A)\n");
        result = scanf(" %c", &input);
        CHECK_RES

        if (input == ARITHMETIC) {
            printf("enter v_x:\n");
            result = scanf("%f", &(object->speed->x));
            CHECK_RES
            printf("enter v_y:\n");
            result = scanf("%f", &(object->speed->y));
            CHECK_RES
        }
        else if (input == POLAR) {
            printf("enter size:\n");
            result = scanf("%f", &(caster.size));
            CHECK_RES
            printf("enter direction:\n");
            result = scanf("%f", &(caster.direction));
            CHECK_RES
            caster.direction = deg_to_rad(caster.direction);
            casted = P_vec_TO_A_vec(&caster);
            object->speed->x = casted->x;
            object->speed->y = casted->y;
            free(casted);
        }
        else {
            printf("invalid input - try again.\n");
        }
    }
    input = '0';

    // get acceleration
    while (input != YES && input != NO) {
        printf("would you like to add acceleration? (y/n) \n");
        result = scanf(" %c", &input);
        CHECK_RES

        if (input == YES) {
            input_inner = '0';
            while (input_inner != ARITHMETIC && input_inner != POLAR) {
                printf("enter acceleration in polar or arithmetic? (P/A)\n");
                result = scanf(" %c", &input_inner);
                CHECK_RES

                if (input_inner == ARITHMETIC) {
                    printf("enter a_x:\n");
                    result = scanf("%f", &(object->acc->x));
                    CHECK_RES
                    printf("enter a_y:\n");
                    result = scanf("%f", &(object->acc->y));
                    CHECK_RES
                }
                else if (input_inner == POLAR) {
                    printf("enter size:\n");
                    result = scanf("%f", &(caster.size));
                    CHECK_RES
                    printf("enter direction:\n");
                    result = scanf("%f", &(caster.direction));
                    CHECK_RES
                    caster.direction = deg_to_rad(caster.direction);
                    casted = P_vec_TO_A_vec(&caster);
                    object->acc->x = casted->x;
                    object->acc->y = casted->y;
                    free(casted);
                }
                else {
                    printf("invalid input - try again.\n");
                }
            }
        } else if (input == NO) {
            object->acc->x = 0.0f;
            object->acc->y = 0.0f;
        }
    }

    // get forces (Dynamic Array)
    input = '0';
    while (1) { // Runs until a no
        printf("would you like to add a force? (y/n)\n");
        result = scanf(" %c", &input);
        CHECK_RES

        if (input == NO) break;

        if (input == YES) {
            input_inner = '0';

            // resize the array
            object->force_count++;
            failsafe = realloc(object->forces, object->force_count * sizeof(A_Vector));
            if (failsafe == NULL) {
                free_obj(object);
                return NULL;
            }

            object->forces = failsafe;

            // pointer to current force
            A_Vector *current_force = &(object->forces[object->force_count - 1]);

            while (input_inner != ARITHMETIC && input_inner != POLAR) {
                printf("enter force in polar (P) or arithmetic(A)?:\n");
                result = scanf(" %c", &input_inner);
                CHECK_RES

                if (input_inner == ARITHMETIC) {
                    printf("enter F_x:\n");
                    result = scanf("%f", &(current_force->x));
                    CHECK_RES
                    printf("enter F_y:\n");
                    result = scanf("%f", &(current_force->y));
                    CHECK_RES
                }
                else if (input_inner == POLAR) {
                    printf("enter size:\n");
                    result = scanf("%f", &(caster.size));
                    CHECK_RES
                    printf("enter direction:\n");
                    result = scanf("%f", &(caster.direction));
                    CHECK_RES
                    caster.direction = deg_to_rad(caster.direction);
                    casted = P_vec_TO_A_vec(&caster);
                    current_force->x = casted->x;
                    current_force->y = casted->y;
                    free(casted);
                }
                else {
                    printf("invalid input - try again.\n");
                }
            }
        }
    }
    printf("enter object type character: (b/c) \n"); // WATCH OUT -> NO FAILSAFES
    result = scanf(" %c", &(object->type));
    CHECK_RES
    return object;
}

//creates a deepcopy of an objects, with no links at all.
Obj *obj_deepcopy(Obj *obj) {
    Obj *copy = malloc(sizeof(Obj));
    if (copy == NULL)
        return NULL;

    //mass
    copy->mass = obj->mass;

    //pos
    copy->pos = malloc(sizeof(A_Vector));
    if (copy->pos == NULL) {
        free(copy);
        return NULL;
    }
    copy->pos->x = obj->pos->x;
    copy->pos->y = obj->pos->y;

    //speed
    copy->speed = malloc(sizeof(A_Vector));
    if (copy->speed == NULL) {
        free(copy->pos);
        free(copy);
        return NULL;
    }
    copy->speed->x = obj->speed->x;
    copy->speed->y = obj->speed->y;

    //acc
    copy->acc = malloc(sizeof(A_Vector));
    if (copy->acc == NULL) {
        free(copy->speed);
        free(copy->pos);
        free(copy);
        return NULL;
    }
    copy->acc->x = obj->acc->x;
    copy->acc->y = obj->acc->y;

    //forces
    copy->forces = malloc(obj->force_count * sizeof(A_Vector));
    if (copy->forces == NULL) {
        free(copy->acc);
        free(copy->speed);
        free(copy->pos);
        free(copy);
        return NULL;
    }
    for (int i = 0; i < obj->force_count; i++) {
        copy->forces[i].x = obj->forces[i].x;
        copy->forces[i].y = obj->forces[i].y;
    }

    //force_count
    copy->force_count = obj->force_count;

    //type
    copy->type = obj->type;
    return copy;
}


void free_obj(Obj *object) {
    if (object == NULL) {
        return;
    }

    free(object->pos);
    free(object->speed);
    free(object->acc);

    free(object->forces);


    free(object);
}

char* obj_to_string(const Obj *object) {
    if (object == NULL) return NULL;

    // calculate approximate buffer size
    size_t buffer_size = 256 + (object->force_count * 50);

    // allocate memory
    char *str = malloc(buffer_size);
    if (str == NULL) return NULL;

    // write the basic info
    int offset = snprintf(str, buffer_size,
        "--- Object Info ---\n"
        "Type:  %c\n"
        "Mass:  %.2f\n"
        "Pos:   (%.2f, %.2f)\n"
        "Speed: (%.2f, %.2f)\n"
        "Acc:   (%.2f, %.2f)\n"
        "Forces (%d total):\n",
        object->type,
        object->mass,
        object->pos->x, object->pos->y,
        object->speed->x, object->speed->y,
        object->acc->x, object->acc->y,
        object->force_count
    );

    //add all forces to the string
    for (int i = 0; i < object->force_count; i++) {
        if (offset < buffer_size) {
            offset += snprintf(str + offset, buffer_size - offset,
                "  -> F%d: (%.2f, %.2f)\n",
                i + 1, object->forces[i].x, object->forces[i].y);
        }
    }

    return str;
}

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
void add_obj(Obj *obj, Data *data) {

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
    data->m[data->count] = obj->mass; //save inverse to reduce calculation times
    data->inv_m[data->count] = 1.0f / obj->mass; //save inverse to reduce calculation times
    data->pos_x[data->count] = obj->pos->x;
    data->pos_y[data->count] = obj->pos->y;
    data->speed_x[data->count] = obj->speed->x;
    data->speed_y[data->count] = obj->speed->y;
    data->acc_x[data->count] = obj->acc->x;
    data->acc_y[data->count] = obj->acc->y;
    data->type[data->count] = obj->type;

    //handle forces, as it is more complex.
    if (obj->force_count > 0) {
        A_Vector *net_force = calc_force_sum(obj);
        if (net_force == NULL) {
            printf("ERROR: Out of memory - couldn't save the forces of the object.");
            data->net_force_x[data->count] = 0;
            data->net_force_y[data->count] = 0;
        }
        else {
            data->net_force_x[data->count] = net_force->x;
            data->net_force_y[data->count] = net_force->y;
        }
    }
    else {
        data->net_force_x[data->count] = 0;
        data->net_force_y[data->count] = 0;
    }

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


//gets input and creates a Circle.
Circle *get_circle();
//gets input and creates a Box.
Box *get_Box();