/******************************************************************************
*   Program:    universe.h
*   Created By: Matt Champlin
*   Date:       Fri Feb 21 20:43:00 EST 2020
*               
*   Program Purpose: This is the header file for the Universe class.
*                    We define functions that the universe has, and 
*                    the "physics" of how the universe works.
*                    
******************************************************************************/

#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include <iostream>
#include <vector>
#include "Organism.h"
#include "FoodPoint.h"


using namespace std;
using namespace std::chrono;

class Universe {
    public:
        Universe();
        Universe(int height, int width, std::string filename);
        ~Universe();

        void arrow_of_time();
        void print();
        void place_at(int x, int y, char to_place);
    
    private:
    
        static const char FOOD = '.';
        int  HEIGHT;
        int  WIDTH;
        int  NUM_FOOD;
        int  num_generations;
        int  num_dead;
        int  print_me;
        std::vector<std::string> messages;
        bool print_message;

        // The UNIVERSE!!
        char **space;
        
        // God-Like Powers
        void place_organisms();
        void move_organisms();
        void clear();
        void pass_time();
        void check_for_death();
        int  time_passed();
        bool is_food(int row, int col);
        bool is_organism(int row, int col);
        void place_food();
        void place_one_food();
        void big_bang(std::string filename);
        void create_life(std::string filename);
        void print_organisms();
        void evolve_organisms();
        void eat_food(Organism &O);
        bool fear_the_reaper(Organism &O);
        void pay_respects(Organism kaput);
        void quantum_fluctuation();
        bool we_can_mate(Organism &one, Organism &two);
        void fight(Organism &one, Organism &two);
        void print_messages();
        
        Organism &find_best_match(int index, char to_find);
        Organism replicate(Organism &O);
        Organism bumpin_uglies(Organism &Parent);

        void debug_organisms();
        void debug_print();
        unsigned modulo(int value, unsigned m);
        
        void scan_universe();

        std::vector<Organism> organisms;
        std::vector<food_pnt> food_loc;

};
    
#endif
