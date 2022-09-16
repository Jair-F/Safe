#pragma once
#include <Arduino.h>
#include <math.h>
#include <limits.h>
#include <UTFT.h>
#include "./lib/position.hpp"

struct linerar_function
{
    double m; // pitch
    double b; // intersection point with the y_axis
};
struct floating_position
{
    double x_pos, y_pos;
};

// 1 rad = 57.2958 deg
#define RAD_DEG_PROPORTION 57.2958 // 1 rad in degrees
inline double rad_to_deg(double rad);

inline double deg_to_rad(double deg);

double l_func_y(double m, double b, double x_pos);

/*
    calculate the according x_point from a function by the x_pos

    m must be not equal 0!!
*/
double l_func_x(double m, double b, double y_pos);

// calc the pitch/slope of the function with 2 points
double l_func_m(floating_position pos_1, floating_position pos_2);

// calc the b with m and a position
double l_func_b(double m, floating_position pos);

// calculate the intersection piont of 2 functions
FGUI::position l_func_intersection(linerar_function func1, linerar_function func2);

/*
    calculate the 4 corner positions of a rectangle which is turned counter clockwise with the angle angle_alpha around the
    origin_pos which is the intersection point of the 2 diagonals in the rectangle

    a_pos is always the highest point on the y_axis and c_pos is always the lowest point on the y_axis

    the 4 positions are stored in the passed parameters
*/
void calc_turned_rect(const FGUI::position &origin_pos, uint16_t width, uint16_t height, uint16_t *angle_alpha,
                      floating_position *a_pos, floating_position *b_pos, floating_position *c_pos, floating_position *d_pos);
/*
    @param origin_pos the center of the rectangle - intersections of the diagonales
    @param width the width of the rect
    @param the height of the rectangle
    @param the angle the rectangle is rotated(from the center of the rectangle the angle between the X-Axis and the rect) in degrees (0-90)
*/
void fill_rect(UTFT *display, FGUI::position origin_pos, uint16_t width, uint16_t height, uint16_t angle_alpha);