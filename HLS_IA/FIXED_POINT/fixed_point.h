#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>



int32_t float_to_fixed(float f);
void float_to_fixed_1d(float* ft, int32_t* fxt, int width);
void float_to_fixed_2d(float* ft, int32_t* fxt, int height, int width);
void float_to_fixed_3d(float* ft, int32_t* fxt, int depth, int height, int width);
void float_to_fixed_4d(float* ft, int32_t* fxt, int size, int depth, int height, int width);
float fixed_to_float(int32_t fixed);
int32_t change_fixed_format(int32_t fixed, uint8_t old_frac_bits, uint8_t new_frac_bits);
void set_fractional_bits(uint8_t frac_bits);
uint8_t get_fractional_bits(void);
int32_t fixed_add(int32_t a, int32_t b);
int32_t fixed_sub(int32_t a, int32_t b);
int32_t fixed_mul(int32_t a, int32_t b);
int32_t fixed_div(int32_t a, int32_t b);
void print_fixed(int32_t fixed);

#endif