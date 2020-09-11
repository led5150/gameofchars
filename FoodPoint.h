/******************************************************************************
*   Program:    FoodPoint.h
*   Created By: Matt Champlin
*   Date:       Thu Mar 12 20:57:23 EDT 2020
*               
*   Program Purpose: Represents the coordinates of a point of food.
*                    
******************************************************************************/

#ifndef _FOODPOINT_H_
#define _FOODPOINT_H_


struct food_pnt {
    food_pnt() {
        food_row = -999;
        food_col = -999;
    }
    food_pnt(int row, int col) {
        food_row = row;
        food_col = col;
    }
    int food_row;
    int food_col;
};
    
#endif
