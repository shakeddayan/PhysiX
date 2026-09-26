#include "structs.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>

//step 1 frame with all data.
void step_physics(Data *data, float dt) {
    int n = data->count;

    //restricted pointers, to enable SIMD:
    float * restrict px = data->pos_x;
    float * restrict py = data->pos_y;
    float * restrict vx = data->speed_x;
    float * restrict vy = data->speed_y;
    float * restrict ax = data->acc_x;
    float * restrict ay = data->acc_y;
    float * restrict fx = data->net_force_x;
    float * restrict fy = data->net_force_y;
    float * restrict inv_m = data->inv_m;


    const float half_dT_sq = 0.5f* dt * dt;

    //a flat loop, to make sure SIMD is active.
    #pragma GCC ivdep
    for (int i = 0; i < n; i++) {

        // calculate acceleration
        ax[i] = fx[i]*inv_m[i];
        ay[i] = fy[i]*inv_m[i];

        //calculate position
        px[i] += vx[i] * dt + ax[i] * half_dT_sq;
        py[i] += vy[i] * dt + ay[i] * half_dT_sq;

        //calculate speed
        vx[i] += ax[i] * dt;
        vy[i] += ay[i] * dt;

        //zero the forces
        fx[i] = 0.0f;
        fy[i] = 0.0f;
    }
}

void add_gravity(Data *data, float g) {
    int n = data->count;

    float * restrict m = data->m;
    float * restrict fy = data->net_force_y;

    //make sure SIMD is active
    #pragma GCC ivdep
    for (int i = 0; i < n; i++) {
        fy[i] -= g * m[i];
    }

}
void add_aerodynamics(Data *data, float drag_coefficient, float wind_x, float wind_y) {
    int n = data ->count;

    float * restrict vx = data->speed_x;
    float * restrict vy = data->speed_y;
    float * restrict fx = data->net_force_x;
    float * restrict fy = data->net_force_y;

    float rel_v_x, rel_v_y;

    //make sure SIMD is active
    #pragma GCC ivdep
    for (int i = 0; i < n; i++) {
        //calculate relative speed
        rel_v_x = vx[i] - wind_x;
        rel_v_y = vy[i] - wind_y;

        //apply drag against relative velocity
        fx[i] -= drag_coefficient * rel_v_x;
        fy[i] -= drag_coefficient * rel_v_y;
    }
}


//the actual step in the game loop.
void update_frame(Data *data, float dt) {
    //if needed, add specific forces.
    //apply global forces - gravity, wind, etc.
    add_gravity(data, 9.8f);
    add_aerodynamics(data, 0.05f, 50.0f, 0.0f);
    //check collision step - later
    //step the physics values - integration.
    step_physics(data, dt);
}