#ifndef STRUCTS_H
#define STRUCTS_H

#define DATA_SIZE 16
#define DATA_INC 16

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

/* MANAGE DATA STRUCT */

//creates a Data module.
Data *createData();
//add an object to the data struct
void add_obj(
    Data *data,
    float mass, float pos_x, float pos_y
    , float speed_x, float speed_y,
    char type
    );
//remove an object from the data struct by the index
void rm_index(int index, Data *data);
//free a Data module
void freeData(Data *data);

#endif //STRUCTS_H
