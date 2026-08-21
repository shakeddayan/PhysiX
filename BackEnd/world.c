#include "constants.h"
#include "structs.h"
#include "calculator.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* PHYSICS FUNCTIONS */
P_Vector *calc_acc(Obj *obj) {
    A_Vector *sumF = calc_force_sum(obj);
    if (sumF == NULL) return NULL;
    P_Vector *acc_res = A_vec_TO_P_vec(sumF);
    acc_res ->size /= obj->mass;
    obj -> acc = P_vec_TO_A_vec(acc_res);
    free(sumF);
    return acc_res;
}

void calc_speed(Obj *obj) {
    obj->speed->x += obj->acc->x  * dT;
    obj->speed->y += obj->acc->y  * dT;
}

void calc_pos(Obj *obj) {
    obj->pos->x += obj->speed->x * dT + (0.5f * obj->acc->x *dT*dT);
    obj->pos->y += obj->speed->y * dT + (0.5f * obj->acc->y *dT*dT);
}



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





int min() {


    Obj *object1 = get_obj();
    Obj *object3 = obj_deepcopy(object1);
    // Obj *object2 = get_obj();
    P_Vector *accel1 = calc_acc(object1);
    P_Vector *accel3 = calc_acc(object3);
    // P_Vector *accel2 = calc_acc(object2);
    char *info1 = obj_to_string(object1);
    // char *info2 = obj_to_string(object2);
    char *info3 = obj_to_string(object3);
    printf("\n\n OBJ1 \n %s", info1);
    // printf("\n\n OBJ2 \n %s", info2);
    free(info1);
    // free(info2);
    for (int i = 0; i < 60; i++) {
        calc_pos(object1);
        // calc_pos(object2);
        calc_speed(object1);
        // calc_speed(object2);
    }
    info1 = obj_to_string(object1);
    // info2 = obj_to_string(object2);
    printf("\n\n OBJ1-After 1s \n %s", info1);
    printf("acceleration (polar) : (%f, %f)", accel1 -> size, accel1-> direction);
    printf("\n\n OBJ3 \n %s", info3);
    printf("acceleration (polar) : (%f, %f)", accel3 -> size, accel3-> direction);
    // printf("\n\n OBJ2-After 1s \n %s", info2);
    // printf("acceleration (polar) : (%f, %f)", accel2 -> size, accel2-> direction);
    free(accel1);
    free(accel3);
    // free(accel2);
    free_obj(object1);
    free_obj(object3);
    // free_obj(object2);
    free(info1);
    free(info3);
    // free(info2);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

// פונקציית עזר להדפסת המצב הנוכחי של המערכים
void print_world(Data *d) {
    printf("=== World State (Count: %d / Capacity: %d) ===\n", d->count, d->capacity);
    for(int i = 0; i < d->count; i++) {
        printf("  [%d] Type: %c | Mass: %.1f | Pos: (%.1f, %.1f) | Forces: (%.1f, %.1f)\n",
               i,
               d->type[i],
               d->inv_m[i],
               d->pos_x[i], d->pos_y[i],
               d->net_force_x[i], d->net_force_y[i]);
    }
    printf("==============================================\n\n");
}

// int main() {
//     printf("--- Test 1: createData ---\n");
//     Data *world = createData();
//     if (world == NULL) {
//         printf("FAILED to create Data!\n");
//         return 1;
//     }
//     printf("Data created successfully!\n\n");
//
//     // ניצור כוח פיקטיבי שנוכל לתת לאובייקטים כדי לבדוק הקצאות
//     A_Vector dummy_force = {10.0f, -5.0f};
//
//     printf("--- Test 2: add_obj ---\n");
//     // אובייקט 1
//     Obj objA;
//     objA.type = 'A';
//     objA.mass = 1.0f;
//     objA.pos = malloc(sizeof(A_Vector));
//     objA.pos->x = 100.0f; objA.pos->y = 100.0f;
//     objA.speed = malloc(sizeof(A_Vector));
//     objA.speed->x = 0; objA.speed->y = 0;
//     objA.acc = malloc(sizeof(A_Vector));
//     objA.acc->x = 0; objA.acc->y = 0;
//     objA.force_count = 1;
//     objA.forces = &dummy_force; // מצביע לכוח הפיקטיבי שלנו
//     add_obj(&objA, world);
//
//     // אובייקט 2
//     Obj objB;
//     objB.type = 'B';
//     objB.mass = 2.0f;
//     objB.pos = malloc(sizeof(A_Vector));
//     objB.pos->x = 200.0f; objB.pos->y = 200.0f;
//     objB.speed = malloc(sizeof(A_Vector));
//     objB.speed->x = 0; objB.speed->y = 0;
//     objB.acc = malloc(sizeof(A_Vector));
//     objB.acc->x = 0; objB.acc->y = 0;
//     objB.force_count = 0; // ללא כוחות
//     objB.forces = NULL;
//     add_obj(&objB, world);
//
//     // אובייקט 3
//     Obj objC;
//     objC.type = 'C';
//     objC.mass = 3.0f;
//     objC.pos = malloc(sizeof(A_Vector));
//     objC.pos->x = 300.0f; objC.pos->y = 300.0f;
//     objC.speed = malloc(sizeof(A_Vector));
//     objC.speed->x = 0; objC.speed->y = 0;
//     objC.acc = malloc(sizeof(A_Vector));
//     objC.acc->x = 0; objC.acc->y = 0;
//     objC.force_count = 1;
//     objC.forces = &dummy_force;
//     add_obj(&objC, world);
//
//     print_world(world); // צפוי לראות A, B, C
//
//     printf("--- Test 3: rm_index (Swap and Pop) ---\n");
//     printf("Removing object at index 0 (Type A)...\n");
//     rm_index(0, world);
//
//     // אחרי המחיקה, אנחנו מצפים ש-C (שהיה באינדקס 2) יתפוס את המקום של A (אינדקס 0)
//     print_world(world); // צפוי לראות C, B
//
//     printf("Removing object at index 1 (Type B) - testing last element deletion...\n");
//     rm_index(1, world);
//
//     print_world(world); // צפוי לראות רק את C
//
//     printf("--- Test 4: Memory Cleanup ---\n");
//     // // ניקוי ידני של מה שנשאר כדי לבדוק שאין לנו זיכרון דולף (Valgrind ישמח)
//     // for (int i = 0; i < world->count; i++) {
//     //     if (world->forces[i] != NULL) {
//     //         free(world->forces[i]);
//     //     }
//     // }
//     // free(world->mass); free(world->pos); free(world->speed); free(world->acc);
//     // free(world->forces); free(world->force_count); free(world->type);
//     // free(world);
//     freeData(world);
//
//     printf("All tests completed successfully!\n");
//     return 0;
// }