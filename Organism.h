/******************************************************************************
*   Program:    Organism.h
*   Created By: Matt Champlin
*   Date:       Fri Feb 21 23:29:49 EST 2020
*               
*   Program Purpose: Defines the class Organism and the things they can do
*                    
******************************************************************************/

#ifndef _ORGANISM_H_
#define _ORGANISM_H_

#include <iostream>
#include <vector>
#include "FoodPoint.h"

class Organism {
    public:
        Organism();
        Organism(int HEIGHT, int WIDTH);
        Organism(int WIDTH, int HEIGHT, char hot_bod);
        ~Organism();
        
        // FUNctions
        void evolve(int num_generations, int MUTRATE,
                    std::vector<food_pnt> food_loc,
                    int UH, int UW);
        void self_sacrifice();

        // Debugging
        void print_organism();

        // setters
        void set_name(std::string to_name);
        void set_life(int val);
        void set_death(double val);
        void set_food(int val);
        void set_x_pos(double x);
        void set_y_pos(double y);
        void set_v_speed(double val);
        void set_h_speed(double val);
        void set_body(char body_type);
        void set_num_parts(int val);
        void create_organism(std::string DNA);
        void set_char(char hot_body);
        void set_mutation(int mutation);

        // getters
        std::string get_name();
        double get_life();
        double get_death();
        int    get_food();      
        double get_x_pos();
        double get_y_pos();
        double get_v_speed();
        double get_h_speed();
        int    get_max_H();
        int    get_max_W();
        int    get_num_parts();
        int    get_mutation();
        char   get_char(int row, int col);
        char   get_body_type();
        

        void   grow();
        double random_double(int low_range, int high_range);
        double random_direction();
    private:
        static const int MAX_H    = 4;
        static const int MAX_W    = 8;
        static const int MAX_SIZE = MAX_H * MAX_W;
        

        // Organism DNA
        std::string name;
        double life;
        double death;
        int    food_found;
        double x_pos;
        double y_pos;
        double v_speed;
        double h_speed;
        char   body_type;
        int    mutation_chance;
        int    num_parts;
        char   body[MAX_H][MAX_W];

        // Organism behaviors
        void   decay();
        void   random_mutation();
        void   change_movement();
        void   move_towards_food(food_pnt closest);
        void   change_death();
        bool   random_chance();
        double percentage_of_size();
        int    get_low_range();
        int    get_high_range();

        bool   close_to_food(std::vector<food_pnt> food_loc, food_pnt &closest,
                             int UH, int UW);
        int    distance_to_food(std::vector<food_pnt> food_loc, 
                                food_pnt &closest, int UH, int UW);

        // God-like powers


        // Names God likes
        std::string names[19] = {"Jessie", "Carl", "Steve", "Bob", "Alice",
                               "James", "Christine", "Matt", "Jordan",
                               "Lauren", "Connor", "Le-a", "Cornelius",
                               "Dr. Fogglebottom", "Abofie", "Penny Lane",
                               "Rhys", "Brendon", "Chris"};

        std::string get_random_name();
        
};
    
#endif
