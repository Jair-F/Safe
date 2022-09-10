#include <Arduino.h>
#include <math.h>
#include <limits.h>
#include <UTFT.h>
#include "FGUI/lib/position.hpp"
#include "FGUI/display_geometry_funcs.hpp"

// 1 rad = 57.2958 deg
#define RAD_DEG_PROPORTION 57.2958 // 1 rad in degrees
inline double
rad_to_deg(double rad)
{
    return rad * RAD_DEG_PROPORTION;
}
inline double deg_to_rad(double deg)
{
    return deg / RAD_DEG_PROPORTION;
}

double l_func_y(double m, double b, double x_pos)
{
    /*
        y = (mx + b)
    */
    if (m == 0)
        return b;
    else
        return m * x_pos + b;
};

/*
    calculate the according x_point from a function by the x_pos

    m must be not equal 0!!
*/
double l_func_x(double m, double b, double y_pos)
{
    /*
        y = mx + b  | - b
        y - b = mx  | /m
        (y - b)/m = x
    */
    return (y_pos - b) / m;
};

// calc the pitch/slope of the function with 2 points
double l_func_m(floating_position pos_1, floating_position pos_2)
{
    /*
        m = (y2 - y1) / (x2 - x1)
    */
    return (pos_2.y_pos - pos_1.y_pos) /
           (pos_2.x_pos - pos_1.x_pos);
};

// calc the b with m and a position
double l_func_b(double m, floating_position pos)
{
    /*
        y = m*x + b | - m*x
        y - m*x = b
    */
    return pos.y_pos - m * pos.x_pos;
};

// calculate the intersection piont of 2 functions
FGUI::position l_func_intersection(linerar_function func1, linerar_function func2)
{
    if (func1.m == func2.m)
    {
        // error parallell functions dont have a common point!
    }

    FGUI::position intersection_point = {0, 0};

    // calculate the x_pos
    /*
        y and x are at both the same - we want to find the common point

        our two funcions:
        y = m1*x + b1           y = m2*x + b2

        we compare them:
        m1*x + b1 = m2*x + b2   | - b1
        m1*x = m2*x + b2 - b1   | - m2*x
        m1*x - m2*x = b2 - b1
        x(m1 - m2) = b2 - b1    | /(m1 - m2)
        x = (b2 - b1)/(m1 - m2)
    */
    intersection_point.x_pos = (func2.b - func1.b) / (func1.m - func2.m);

    /*
        y = mx + b
            ||
    */
    intersection_point.y_pos = func1.m * intersection_point.x_pos + func1.b;

    return intersection_point;
};

/*
    calculate the 4 corner positions of a rectangle which is turned counter clockwise with the angle angle_alpha around the
    origin_pos which is the intersection point of the 2 diagonals in the rectangle

    a_pos is always the highest point on the y_axis and c_pos is always the lowest point on the y_axis

    the 4 positions are stored in the passed parameters
*/
void calc_turned_rect(const FGUI::position &origin_pos, uint16_t width, uint16_t height, uint16_t *angle_alpha,
                      floating_position *a_pos, floating_position *b_pos, floating_position *c_pos, floating_position *d_pos)
{
    /*
        y = mx + b
        m = pitch
        b = intersection with the y-Axis (0 , b)
    */

    while (*angle_alpha >= 180)
        *angle_alpha = *angle_alpha - 180; // rotating more than 180 deg is also representable by a angle lower than 180 at a rectangle

    // angle_alpha = 180 - angle_alpha; // change direction of turning

    /*
        if angle is bigger than 90 deg we change the width and the height so we only need to draw a rect in angle of 90 deg
        and then A y-point is always positive
    */
    while (*angle_alpha >= 90)
    {
        swap(uint16_t, height, width);
        *angle_alpha = *angle_alpha - 90;
    }

    // with pythagoras calc the diagonal length
    double diagonal_length = sqrt(pow((double)height / 2, 2) + pow((double)width / 2, 2)) * 2;

    bool a_pos_x_positive, a_pos_y_positive = true; // the y-pos is always positive at point A

    // point A and C X-Pos  -  positive or negative - slide 2

    double angle_beta = 90 - *angle_alpha;
    double check_length_2 = double(height) / 2;

    double check_length_1;
    if (angle_alpha == 0)
    {
        check_length_1 = __DBL_MAX__; // if angle is 0 this length would theoretically be ∞(endless), so set it to the max we can
    }
    else
    {
        /*
            tan(𝛽) = QP / PO | *PO
            PO * tan(𝛽) = QP

            QP = check_length_2
            PO = width / 2
        */
        check_length_1 = ((double)width / 2) * tan(deg_to_rad(angle_beta));
    }

    // the y-pos is always positive at point A
    if (check_length_1 > check_length_2) // A-X-Pos is positive
    {
        a_pos_x_positive = true;
    }
    else if (check_length_1 < check_length_2) // A-X-Pos is negative
    {
        a_pos_x_positive = false;
    }
    else // check_length_1 and check_length_2 are equal - A-X-Pos is 0
    {
        a_pos_x_positive = true;
        a_pos->x_pos = 0;
    }

    /*







    */

    // calculate the rectangle pionts A, B, C, D

    // calculating point A,C - slide 4
    /*
        tan(δ) = AF / OF
        calculate angle δ with arc_tan

        AF = height / 2
        OF = width / 2
    */
    double angle_delta = atan(((double)height / 2) / ((double)width / 2)); // angle is still in radiands!
    angle_delta = rad_to_deg(angle_delta);

    double angle_gamma = abs(90 - *angle_alpha - angle_delta); // if alpha is big enough this could be negative - therefore abs to get the angle

    /*
        OA = diagonal_length / 2

        sin(δ) = IA / OA
        sin(δ) * OA = IA

        IA = a_pos.x_pos
    */
    a_pos->x_pos = (sin(deg_to_rad(angle_gamma)) * (diagonal_length / 2));
    if (!a_pos_x_positive)
        a_pos->x_pos = -a_pos->x_pos;
    c_pos->x_pos = -a_pos->x_pos; // c_pos is the exact opposite of a_pos

    // a_pos.x_pos = origin_pos.x_pos + a_pos.x_pos;
    // c_pos.x_pos = origin_pos.x_pos + c_pos.x_pos;

    /*
        AO = diagonal_length / 2

        cos(δ) = IO / AO    | *AO
        cos(δ) * AO = IO

        IO = a_pos.y_pos
    */
    a_pos->y_pos = (cos(deg_to_rad(angle_gamma)) * (diagonal_length / 2)); // a_pos - y_pos is always positive...
    // if (!a_pos_y_positive)
    //     a_pos.y_pos = -a_pos.y_pos;
    c_pos->y_pos = -a_pos->y_pos; // c_pos is the exact opposite of a_pos

    // a_pos.y_pos = origin_pos.y_pos - a_pos.y_pos; // the y-pos of pos a is above the origin - subtract...
    // c_pos.y_pos = origin_pos.y_pos - c_pos.y_pos;

    /*



    */

    /*
        AB = height
        ∡ABF = ∡∝

        cos(∝) = BF * AB    | * AB
        BF = AB * cos(∝)

        sin(∝) = AF * AB    | * AB
        AF = AB * cos(∝)

        b_pos.x_pos = a_pos.x_pos + AF
        b_pos.y_pos = a_pos.y_pos - BF
    */
    double BF = cos(deg_to_rad(*angle_alpha)) * height;
    double AF = sin(deg_to_rad(*angle_alpha)) * height;
    b_pos->x_pos = a_pos->x_pos + AF;
    b_pos->y_pos = a_pos->y_pos - BF;

    d_pos->x_pos = c_pos->x_pos - AF;
    d_pos->y_pos = c_pos->y_pos + BF;

    // round all the positions to get a displayable solution for the display
    // a_pos.x_pos = round(a_pos.x_pos);
    // a_pos.y_pos = round(a_pos.y_pos);
    //
    // b_pos.x_pos = round(b_pos.x_pos);
    // b_pos.y_pos = round(b_pos.y_pos);
    //
    // c_pos.x_pos = round(c_pos.x_pos);
    // c_pos.y_pos = round(c_pos.y_pos);
    //
    // d_pos.x_pos = round(d_pos.x_pos);
    // d_pos.y_pos = round(d_pos.y_pos);
}

/*
    @param origin_pos the center of the rectangle - intersections of the diagonales
    @param width the width of the rect - is not always correct due to the variable accuracy calculation limits
    @param the height of the rectangle
    @param the angle the rectangle is rotated(from the center of the rectangle the angle between the X-Axis and the rect) in degrees (0-90)
*/
void fill_rect(UTFT *display, FGUI::position origin_pos, uint16_t width, uint16_t height, uint16_t angle_alpha)
{
    floating_position a_pos, b_pos, c_pos, d_pos; // the corner positions of the rectangle
    calc_turned_rect(origin_pos, width, height, &angle_alpha, &a_pos, &b_pos, &c_pos, &d_pos);

    if (angle_alpha == 0)
    {
        display->fillRect(origin_pos.x_pos + a_pos.x_pos, origin_pos.y_pos + a_pos.y_pos, origin_pos.x_pos + c_pos.x_pos, origin_pos.y_pos + c_pos.y_pos);
    }
    else
    {
        linerar_function AB, BC, CD, DA;
        AB.m = l_func_m(a_pos, b_pos);
        AB.b = l_func_b(AB.m, a_pos);

        BC.m = l_func_m(b_pos, c_pos);
        BC.b = l_func_b(BC.m, b_pos);

        CD.m = l_func_m(c_pos, d_pos);
        CD.b = l_func_b(CD.m, c_pos);

        DA.m = l_func_m(d_pos, a_pos);
        DA.b = l_func_b(DA.m, d_pos);

        // a_pos is always the highest point and c_pos is always the lowest point on the y-axis...
        linerar_function *function_left = &CD, // CD or DA
            *function_right = &BC;             // AB or BC - depends on the actual y_pos

        for (int16_t y_pos = c_pos.y_pos; y_pos < a_pos.y_pos; ++y_pos) // c_pos - y_pos is always negative
        {
            if (y_pos > b_pos.y_pos)
                function_right = &AB;
            if (y_pos > d_pos.y_pos)
                function_left = &DA;

            // the relative y_pos in the coordinate-system - not the actual y_pos on the display like the a_pos and b_pos are
            int16_t relative_y_pos = y_pos;

            int16_t h_line_start_x_pos = round(l_func_x(function_left->m, function_left->b, relative_y_pos));
            int16_t h_line_end_x_pos = round(l_func_x(function_right->m, function_right->b, relative_y_pos));

            relative_y_pos = -relative_y_pos; // if the value is negative on the imaginary coordinate system it should go in to + on the real display

            h_line_start_x_pos = origin_pos.x_pos + h_line_start_x_pos;
            h_line_end_x_pos = origin_pos.x_pos + h_line_end_x_pos;

            display->drawHLine(h_line_start_x_pos,
                               origin_pos.y_pos + relative_y_pos, // convert it to the absolute y_pos on the display
                               abs(h_line_end_x_pos - h_line_start_x_pos));
        }
    }

    // display->drawLine(origin_pos.x_pos + d_pos.x_pos, origin_pos.y_pos + d_pos.y_pos, origin_pos.x_pos + a_pos.x_pos, origin_pos.y_pos + a_pos.y_pos);
    // display->drawLine(origin_pos.x_pos + a_pos.x_pos, origin_pos.y_pos + a_pos.y_pos, origin_pos.x_pos + b_pos.x_pos, origin_pos.y_pos + b_pos.y_pos);
    // display->drawLine(origin_pos.x_pos + b_pos.x_pos, origin_pos.y_pos + b_pos.y_pos, origin_pos.x_pos + c_pos.x_pos, origin_pos.y_pos + c_pos.y_pos);
    // display->drawLine(origin_pos.x_pos + c_pos.x_pos, origin_pos.y_pos + c_pos.y_pos, origin_pos.x_pos + d_pos.x_pos, origin_pos.y_pos + d_pos.y_pos);
}