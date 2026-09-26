#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "structs.h"
#include "world.h"

// נגדיר את ה-dT פעם אחת כאן כדי ששאר הפונקציות ישתמשו בו
const float dT = 0.016f; 

// =========================================================================
// הנחה: המבנה Data והפונקציות step_physics, add_gravity, add_aerodynamics 
// מוגדרים כאן למעלה, או מיובאים מקובץ Header.
// חובה לוודא ש-step_physics עושה כעת שימוש ב-inv_m לכפל ולא לחילוק!
// =========================================================================

int main() {
    const int NUM_OBJECTS = 1000000; 
    const int NUM_FRAMES = 1000;     

    printf("--- SIMD Physics Engine Benchmark (Box2D Style) ---\n");
    printf("Allocating memory for %d objects...\n", NUM_OBJECTS);

    Data *world = calloc(1, sizeof(Data));
    world->count = NUM_OBJECTS;
    world->capacity = NUM_OBJECTS;

    world->m = malloc(NUM_OBJECTS * sizeof(float));
    world->inv_m = malloc(NUM_OBJECTS * sizeof(float)); // המערך החדש!
    world->pos_x = calloc(NUM_OBJECTS, sizeof(float));
    world->pos_y = calloc(NUM_OBJECTS, sizeof(float));
    world->speed_x = calloc(NUM_OBJECTS, sizeof(float));
    world->speed_y = calloc(NUM_OBJECTS, sizeof(float));
    world->acc_x = calloc(NUM_OBJECTS, sizeof(float));
    world->acc_y = calloc(NUM_OBJECTS, sizeof(float));
    world->net_force_x = calloc(NUM_OBJECTS, sizeof(float));
    world->net_force_y = calloc(NUM_OBJECTS, sizeof(float));

    if (!world->m || !world->inv_m || !world->net_force_y) {
        printf("ERROR: Not enough memory!\n");
        return 1;
    }

    // --- אתחול נתונים ---
    // כולם מתחילים עם מהירות ימינה, מסה 1.0
    for (int i = 0; i < NUM_OBJECTS; i++) {
        world->m[i] = 1.0f;
        world->inv_m[i] = 1.0f / world->m[i]; // חישוב ההופכי מראש
        world->pos_x[i] = 0.0f;
        world->pos_y[i] = 0.0f;
        world->speed_x[i] = 10.0f; 
        world->speed_y[i] = 0.0f;
    }

    printf("Starting simulation: %d frames...\n", NUM_FRAMES);

    clock_t start_time = clock();

    // הלולאה המרכזית (Game Loop)
    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        
        //changed to the main running function (to be) from the front end.
        update_frame(world, dT);
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\n--- Benchmark Results ---\n");
    printf("Total time for %d frames: %.4f seconds\n", NUM_FRAMES, time_spent);
    printf("Time per frame: %.4f milliseconds\n", (time_spent / NUM_FRAMES) * 1000.0);
    
    double updates_per_sec = (NUM_OBJECTS * (double)NUM_FRAMES) / time_spent;
    printf("Performance: %.2f Million Object-Updates per second\n", updates_per_sec / 1000000.0);

    // --- אימות פיזיקלי (Validation) ---
    printf("\n--- Physics Validation (Object 0) ---\n");
    
    // סימולציה סקלרית (רגילה) של אובייקט בודד כדי למצוא את התוצאה המצופה
    float exp_px = 0.0f, exp_py = 0.0f;
    float exp_vx = 10.0f, exp_vy = 0.0f;
    float exp_m = 1.0f, exp_inv_m = 1.0f / exp_m;
    float half_dt_sq = 0.5f * dT * dT;

    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        float fx = 0.0f, fy = 0.0f;
        
        // כבידה
        fy -= 9.8f * exp_m;
        
        // גרר
        float rel_vx = exp_vx - 50.0f;
        float rel_vy = exp_vy - 0.0f;
        fx -= 0.05f * rel_vx;
        fy -= 0.05f * rel_vy;
        
        // אינטגרציה
        float ax = fx * exp_inv_m;
        float ay = fy * exp_inv_m;
        exp_px += exp_vx * dT + ax * half_dt_sq;
        exp_py += exp_vy * dT + ay * half_dt_sq;
        exp_vx += ax * dT;
        exp_vy += ay * dT;
    }

    printf("Expected Final Position: X = %10.3f, Y = %10.3f\n", exp_px, exp_py);
    printf("Actual Final Position:   X = %10.3f, Y = %10.3f\n", world->pos_x[0], world->pos_y[0]);

    // נבדוק שההפרש קטן מאוד (התחשבות באובדן דיוק קל של Float ב-Fast Math)
    if (fabs(world->pos_x[0] - exp_px) < 0.1f && fabs(world->pos_y[0] - exp_py) < 0.1f) {
        printf(">>> PASSED: SIMD processing exactly matches scalar physics! <<<\n");
    } else {
        printf(">>> FAILED: Position mismatch. Check your SIMD arrays and calculations. <<<\n");
    }

    // ניקוי
    free(world->m); free(world->inv_m); 
    free(world->pos_x); free(world->pos_y);
    free(world->speed_x); free(world->speed_y); 
    free(world->acc_x); free(world->acc_y);
    free(world->net_force_x); free(world->net_force_y); 
    free(world);

    return 0;
}