#pragma once
#include <Arduino.h>
#include <math.h>
#include <limits.h>
#include <UTFT.h>
#include "./lib/position.hpp"

/**
 * \defgroup DisplayGeometryFunctions
 * \ingroup FGUI
 *
 * @brief some userfull functions for calculating positions on the display+drawing.
 *
 * @details some useful functions for calculating positions on the screen + function to draw a turned rectangle
 * with the center of its diagonals, widht, height and the turned angle.
 */

/**
 * @brief type to store a linerar_function of the type "y = mx + b"
 */
struct linerar_function
{
    double m; ///< @brief pitch
    double b; ///< @brief intersection point with the y_axis
};
/**
 * @details the sambe like FGUI::positon but only in doubles to ensure a more precisely
 */
struct floating_position
{
    double x_pos, y_pos;
};

// 1 rad = 57.2958 deg
#define RAD_DEG_PROPORTION 57.2958 // 1 rad in degrees
inline double rad_to_deg(double rad);

inline double deg_to_rad(double deg);

/**
 * @brief calculate the y point of a linerar function if the x_pos,
 * the pitch and the intersection point with the y_axis is given.
 * @param m pitch
 * @param b intersection point with the y_axis
 * @param x_pos the x_pos to which we want to find th y_pos.
 * @note !! m must be not equal 0 !!
 */
double l_func_y(double m, double b, double x_pos);

/**
 * @brief calculate the according x_point from a function by the x_pos
 * @param m pitch
 * @param b intersection point with the y_axis
 * @param x_pos the y_pos to which we want to find th x_pos.
 * @note !! m must be not equal 0 !!
 */
double l_func_x(double m, double b, double y_pos);

/**
 * @brief calc the pitch/slope of the function with 2 points
 */
double l_func_m(floating_position pos_1, floating_position pos_2);

/**
 * @brief calc the b with m and a position
 * @param m pitch
 */
double l_func_b(double m, floating_position pos);

/**
 * @brief calculate the intersection point of 2 functions
 */
FGUI::position l_func_intersection(linerar_function func1, linerar_function func2);

/**
 * @brief calculate the 4 corner positions of a rectangle which is rotated clockwise with the angle angle_alpha around the
 * origin_pos which is the intersection point of the 2 diagonals in the rectangle
 * @note a_pos is always the highest point on the y_axis and c_pos is always the lowest point on the y_axis
 * @details the 4 positions are stored in the passed parameters
 * @param origin intersection point of the 2 diagonals in the rectangle
 * @param angle_alpha the angle between the horizontal line through the origin_pos and the nearest diagonal or the rectangle(0-360).
 */
void calc_rotated_rect(const FGUI::position &origin_pos, uint16_t width, uint16_t height, uint16_t *angle_alpha,
                       floating_position *a_pos, floating_position *b_pos, floating_position *c_pos, floating_position *d_pos);
/**
 * @brief fill a rectangle which is rotated over the angle alpha
 * @param origin_pos the center of the rectangle - intersections of the diagonales
 * @param width the width of the rect
 * @param height the height of the rectangle
 * @param angle_alpha the angle the rectangle is rotated(from the center of the rectangle the angle between the X-Axis and the rect) in degrees (0-360)
 */
void fill_rect(UTFT *display, FGUI::position origin_pos, uint16_t width, uint16_t height, uint16_t angle_alpha);