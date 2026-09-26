#ifndef BACKEND_LIBRARY_H
#define BACKEND_LIBRARY_H
#include "structs.h"

//step 1 frame with all data.
void step_physics(Data *data, float dt);
void add_gravity(Data *data, float g);
void add_aerodynamics(Data *data, float drag_coefficient, float wind_x, float wind_y);
void update_frame(Data *data, float dt);

#endif //BACKEND_LIBRARY_H