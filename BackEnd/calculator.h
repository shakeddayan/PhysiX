#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "structs.h"

/* CALCULATOR FUNCTIONS */

// converts an angle from radians to degrees
float rad_to_deg(float rad);
// converts an angle from degrees to radians
float deg_to_rad(float deg);
//converts an arithmetic version of a vector to a polar version
P_Vector *A_vec_TO_P_vec(A_Vector *vector);
//converts a polar version of a vector to an arithmetic version
A_Vector *P_vec_TO_A_vec(P_Vector *vector);
// calculates the total force applied on an object. useful for newton's second law: F=ma
A_Vector *calc_force_sum(Obj *obj);
// calculates a repeated sum of arithmetic vectors.
A_Vector *rep_vec_sum(A_Vector *vecs, int amount);
//calculates a sum of 2 arithmetic vectors
A_Vector *vec_sum(A_Vector *vec1, A_Vector *vec2);
//calculates a sum of 2 arithmetic vectors and stores the answer in vec1.
void vec_sum_override(A_Vector *vec1, A_Vector *vec2);
#endif //CALCULATOR_H
