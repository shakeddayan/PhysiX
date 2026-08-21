#ifndef STRUCTS_H
#define STRUCTS_H

#define DATA_SIZE 16
#define DATA_INC 16
// define basic objects - AoS approach
typedef struct {
    float size;
    float direction;
} P_Vector;

typedef struct {
    float x;
    float y;
} A_Vector;

typedef struct {
    float mass;
    A_Vector *pos;
    A_Vector *speed;
    A_Vector *acc;
    A_Vector *forces;
    int force_count;
    char type;
} Obj;

typedef struct {
    Obj base;
    float len;
} Box;

typedef struct {
    Obj base;
    float radius;
} Circle;

typedef struct ScreenBlock {
    int len;
    Obj *objects;
    struct ScreenBlock *Sons;
} ScreenBlock;

typedef struct {
    int g;
    A_Vector wind;
    ScreenBlock screen;
}World;


// SoA approach - one struct of data arrays, more data focused and not OOP focused.

//a struct meant to carry all objects' data.
typedef struct {
    // Arrays of data
    float *m;
    float *inv_m;
    // x arrays
    float *pos_x;
    float *speed_x;
    float *acc_x;
    float *net_force_x;
    // A_Vector *pos;
    // A_Vector *speed;
    // A_Vector *acc;
    // A_Vector **forces;
    //y arrays
    float *pos_y;
    float *speed_y;
    float *acc_y;
    float *net_force_y;

    char *type;
    // Control variables.
    int count; //how many object were added + the next free index
    int capacity; //how much objects can we carry before realloc, to prevent frequent reallocs.
}Data;


/* OOP FUNCTIONS */

//creates a Data module.
Data *createData();
//add an object to the data struct
void add_obj(Obj *obj, Data *data);
//remove an object from the data struct by the index
void rm_index(int index, Data *data);
//free a Data module
void freeData(Data *data);

//gets input and creates an object.
Obj *get_obj();
//creates a deepcopy of an objects, with no links at all.
Obj *obj_deepcopy(Obj *obj);
//frees an object
void free_obj(Obj *object);
//creates a dynamically allocated string describing an object.
char* obj_to_string(const Obj *object);
//gets input and creates a Circle.
Circle *get_circle();
//gets input and creates a Box.
Box *get_Box();
#endif //STRUCTS_H
