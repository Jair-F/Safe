#pragma once
#include <Arduino.h>
#include <math.h>
#include <limits.h>
#include <UTFT.h>
#include "UI/lib/position.hpp"

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
    return m * x_pos + b;
};

// calculate the according x_point from a function by the x_pos
double l_func_x(double m, double b, double y_pos)
{
    /*
        y = mx + b  | - b
        y - b = mx  | /m
        (y - b)/m = x
    */
    return (y_pos - b) / m;
};

// calc the pitch of the function with 2 points
double l_func_m(UI::position pos_1, UI::position pos_2)
{
    /*
        m = (y2 - y1) / (x2 - x1)
    */
    return (pos_2.y_pos - pos_1.y_pos) /
           (pos_2.x_pos - pos_2.x_pos);
};

// calc the b with m and a position
double l_func_b(double m, UI::position pos)
{
    /*
        y - y1 = m(x - x1)
        y - y1 = mx - mx1   | + y1
        y = mx - mx1 + y1
        ^   ^
        |   |
        this falls down becaus we insert 0 for x:
        y = m*0 - mx1 + y1

        b = mx1 + y1
    */
    return m * pos.x_pos + pos.y_pos;
};

// calculate the intersection piont of 2 functions
UI::position l_func_intersection(linerar_function func1, linerar_function func2)
{
    if (func1.m == func2.m)
    {
        // error parallell functions dont have a common point!
    }

    UI::position intersection_point = {0, 0};

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
    @param origin_pos the center of the rectangle - intersections of the diagonales
    @param width the width of the rect
    @param the height of the rectangle
    @param the angle the rectangle is rotated(from the center of the rectangle the angle between the X-Axis and the rect) in degrees (0-90)
*/
void fill_rect(UTFT *display, UI::position origin_pos, uint16_t width, uint16_t height, uint16_t angle_alpha)
{
    /*
        y = mx + b
        m = pitch
        b = intersection with the y-Axis (0 , b)
    */

    if (angle_alpha >= 180)
        angle_alpha = angle_alpha - 180; // rotating more than 180 deg is also representable by a angle lower than 180 at a rectangle

    /*
        if angle is bigger than 90 deg we change the width and the height so we only need to draw a rect in angle of 90 deg
        and then A y-point is always positive
    */
    if (angle_alpha >= 90)
    {
        swap(uint16_t, height, width);
        angle_alpha = angle_alpha - 90;
    }

    floating_position a_pos, b_pos, c_pos, d_pos; // the corner positions of the rectangle
    linerar_function AB, BC, CE, EA;

    // with pythagoras calc the diagonal length
    double diagonal_length = sqrt(pow((double)height / 2, 2) + pow((double)width / 2, 2)) * 2;

    {
        bool a_pos_x_positive, a_pos_y_positive = true; // the y-pos is always positive at point A
        bool b_pos_x_positive = true, b_pos_y_positive; // the x-pos is always positive at point B
        bool c_pos_x_positive, c_pos_y_positive = false;
        bool d_pos_x_positive = false, d_pos_y_positive;

        // point A and C X-Pos  -  positive or negative - slide 2

        double angle_beta = 90 - angle_alpha;
        double check_length_2 = double(height) / 2;

        /*
            tan(𝛽) = AP / PO | *PO
            PO * tan(𝛽) = AP

            AP = check_length_2
            PO = width / 2
        */
        double check_length_1;
        if (angle_alpha == 0)
        {
            check_length_1 = __DBL_MAX__;
        }
        else
        {
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
            a_pos.x_pos = 0;
        }

        /*
            C is on the opposite size - the opposite...
        */
        c_pos_x_positive = !a_pos_x_positive;

        /*







        */

        // point B and D X-Pos  -  positive or negative - slide 3

        angle_beta = 90 - angle_alpha;
        check_length_2 = double(width) / 2;
        /*
            ∡𝛽 = 90 - ∝

            tan(𝛽) = GH / OG    | * OG
            OG * tan(𝛽) = GH

            GH = check_length_1
            OG = height / 2
        */
        if (angle_alpha == 0)
        {
            check_length_1 = __DBL_MAX__;
        }
        else
        {
            check_length_1 = ((double)height / 2) * tan(deg_to_rad(angle_beta));
        }

        // the X-pos is always positive at point B
        if (check_length_1 > check_length_2) // A-Y-pos is negative
        {
            b_pos_y_positive = false;
        }
        else if (check_length_1 < check_length_2) // A-Y-pos is positive
        {
            b_pos_y_positive = true;
        }
        else // check_length_1 and check_length_2 are equal - B-Y-Pos is 0
        {
            b_pos_y_positive = true;
            b_pos.y_pos = 0;
        }

        /*
            D is on the opposite size - the opposite...
        */
        d_pos_y_positive = !b_pos_y_positive;

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
        double angle_delta = atan(((double)height / 2) / ((double)width / 2));
        angle_delta = rad_to_deg(angle_delta);

        double angle_gamma = abs(90 - angle_alpha - angle_delta);

        /*
            OA = diagonal_length / 2

            sin(δ) = IA / OA
            sin(δ) * OA = IA

            IA = a_pos.x_pos
        */
        a_pos.x_pos = (sin(deg_to_rad(angle_gamma)) * (diagonal_length / 2));
        if (!a_pos_x_positive)
            a_pos.x_pos = -a_pos.x_pos;
        c_pos.x_pos = -a_pos.x_pos; // c_pos is the exact opposite of a_pos

        a_pos.x_pos = origin_pos.x_pos + a_pos.x_pos;
        c_pos.x_pos = origin_pos.x_pos + c_pos.x_pos;

        /*
            AO = diagonal_length / 2

            cos(δ) = IO / AO    | *AO
            cos(δ) * AO = IO

            IO = a_pos.y_pos
        */
        a_pos.y_pos = (cos(deg_to_rad(angle_gamma)) * (diagonal_length / 2));
        if (!a_pos_y_positive)
            a_pos.y_pos = -a_pos.y_pos;
        c_pos.y_pos = -a_pos.y_pos; // c_pos is the exact opposite of a_pos

        a_pos.y_pos = origin_pos.y_pos + a_pos.y_pos;
        c_pos.y_pos = origin_pos.y_pos + c_pos.y_pos;

        /*



        */

        /*
            AB = height
            ∡ABF = ∡∝


            b_pos.x_pos = a_pos.x_pos + AF
            b_pos.y_pos = a_pos.y_pos - BF
        */
        double BF = cos(deg_to_rad(angle_alpha)) * height;
        double AF = sin(deg_to_rad(angle_alpha)) * height;
        b_pos.x_pos = a_pos.x_pos + AF;
        b_pos.y_pos = a_pos.y_pos - BF;

        // if (!b_pos_x_positive)
        //     b_pos.x_pos = -b_pos.x_pos;
        // if (!b_pos_y_positive)
        //     b_pos.y_pos = -b_pos.y_pos;

        d_pos.x_pos = c_pos.x_pos - AF;
        d_pos.y_pos = c_pos.y_pos + BF;

        a_pos.x_pos = round(a_pos.x_pos);
        a_pos.y_pos = round(a_pos.y_pos);

        b_pos.x_pos = round(b_pos.x_pos);
        b_pos.y_pos = round(b_pos.y_pos);

        c_pos.x_pos = round(c_pos.x_pos);
        c_pos.y_pos = round(c_pos.y_pos);

        d_pos.x_pos = round(d_pos.x_pos);
        d_pos.y_pos = round(d_pos.y_pos);
    }

    display->drawLine(a_pos.x_pos, a_pos.y_pos, b_pos.x_pos, b_pos.y_pos);
    display->drawLine(b_pos.x_pos, b_pos.y_pos, c_pos.x_pos, c_pos.y_pos);
    display->drawLine(c_pos.x_pos, c_pos.y_pos, d_pos.x_pos, d_pos.y_pos);
    display->drawLine(d_pos.x_pos, d_pos.y_pos, a_pos.x_pos, a_pos.y_pos);
}