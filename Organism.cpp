/******************************************************************************
*   Program:    Organism.cpp
*   Created By: Matt Champlin
*   Date:       Fri Feb 21 23:29:49 EST 2020
*               
*   Program Purpose: Implementation of an Organism.
*                    
******************************************************************************/


#include <iostream>
#include "Organism.h"
#include <sstream>
#include <chrono>
#include <random>
#include <unistd.h>
#include <cstdlib>

using namespace std;

static const int DIST = 50; // distance a food has to be for an organism
                            // to not move towards it.

// Default Constructor
Organism::Organism()
{
    srand(time(NULL));
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    name      = "Default";
    life      = -1;
    x_pos     = 0;
    y_pos     = 0;
    v_speed   = 0;
    h_speed   = 0;
    num_parts = 1;
    
    for (int i = 0; i < MAX_H; i++) {
        for (int j = 0; j < MAX_W; j++) {
            body[i][j] = '\n';
        }
    }
    body[0][0] = 'x';
}

// Parameterized Constructor.
// Used for creating organisms from file.
Organism::Organism(int WIDTH, int HEIGHT, char hot_bod)
{
    // create some random number generators
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(5, 99);
    uniform_int_distribution<int> hot_body(33, 126);
    uniform_int_distribution<int> mut(1, 70);
    uniform_int_distribution<int> row(0, HEIGHT);
    uniform_int_distribution<int> col(0, WIDTH);
    uniform_int_distribution<int> grw(0, 10);

    // set Organism traits
    name            = get_random_name();
    life            = rnd(rng);
    death           = random_double(5, 70);
    food_found      = 1;
    x_pos           = row(rng);
    y_pos           = col(rng);
    v_speed         = random_direction();
    h_speed         = random_direction();
    body_type       = hot_bod;
    mutation_chance = mut(rng);
    num_parts       = 0;

    // clear body
    for (int i = 0; i < MAX_H; i++) {
        for (int j = 0; j < MAX_W; j++) {
            body[i][j] = '\n';
        }
    }
    int growth = grw(rng);
    for (int i = 0; i < growth; i++) {
        grow();
    }
}

// Another Parameterized Constructor.
// Used for creating random Organisms as a result of a quantum_fluctuation()
Organism::Organism(int WIDTH, int HEIGHT)
{
    // create some random number generators
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> lfe(10, 99);
    uniform_int_distribution<int> hot_body(33, 125);
    uniform_int_distribution<int> mut(1, 70);
    uniform_int_distribution<int> row(0, HEIGHT);
    uniform_int_distribution<int> col(0, WIDTH);

    // set Organism traits
    name            = get_random_name();
    life            = lfe(rng);
    death           = random_double(15, 70);
    food_found      = 1;
    x_pos           = row(rng);
    y_pos           = col(rng);
    v_speed         = random_direction();
    h_speed         = random_direction();
    body_type       = hot_body(rng);
    mutation_chance = mut(rng);
    num_parts       = 1;

    // clear body
    for (int i = 0; i < MAX_H; i++) {
        for (int j = 0; j < MAX_W; j++) {
            body[i][j] = '\n';
        }
    }

    // check for Jesus!
    if (body_type == '.') {
        life = 32;
        name = "Jesus";
        for (int i = 0; i < MAX_H; i++) {
            for (int j = 0; j < MAX_W; j++) {
                body[i][j] = '.';
            }
        }
    }

    // place one char of body type
    body[0][0] = body_type;
}

// Memory handled by Vector class.
Organism::~Organism()
{
    // nothing to do
}

// Prints the organism. For Debugging.
void Organism::print_organism()
{
    for (int i = 0; i < MAX_H; i++) {
        for (int j = 0; j < MAX_W; j++) {
            if (body[i][j] != '\n') {
                cout << body[i][j];
            }
        }
    cout << endl;
    }
}

/********** GETTERS ***********/

// All getters and setters have function names that document themselves.  
// They do exactly as their name suggests.


string Organism::get_name()
{
    return name;
}

double Organism::get_x_pos()
{
    return x_pos;
}

double Organism::get_y_pos()
{
    return y_pos;
}

double Organism::get_life()
{
    return life;
}

double Organism::get_death()
{
    return death;
}

int Organism::get_food()
{
    return food_found;
}

int Organism::get_max_H()
{
    return MAX_H;
}

int Organism::get_max_W()
{
    return MAX_W;
}

char Organism::get_char(int row, int col)
{
    return body[row][col];
}

double Organism::get_v_speed()
{
    return v_speed;
}

double Organism::get_h_speed()
{
    return h_speed;
}

int Organism::get_num_parts()
{
    return num_parts;
}

char Organism::get_body_type()
{
    return body_type;
}

int Organism::get_mutation()
{
    return mutation_chance;
}

/********** SETTERS ***********/

void Organism::set_name(string to_name)
{
    name = to_name;
}

void Organism::set_x_pos(double x)
{
    x_pos = x;
}

void Organism::set_y_pos(double y)
{
    y_pos = y;
}

void Organism::set_life(int val)
{
    life = val;
}

void Organism::set_death(double val)
{
    death = val;
}

void Organism::set_v_speed(double val)
{
    v_speed = val;
}

void Organism::set_h_speed(double val)
{
    h_speed = val;
}

void Organism::set_food(int val)
{
    food_found = val;
}

void Organism::set_char(char hot_body)
{
    body_type = hot_body;
}

void Organism::set_body(char body_type)
{
    body[0][0] = body_type;
}

void Organism::set_mutation(int mutation)
{
    mutation_chance = mutation;
}

void Organism::set_num_parts(int val)
{
    num_parts = val;
}

/************************* FUN-ctions!! **********************/

// Creates an organism from a string of DNA.  Sets member variables to 
// values held in the DNA string.
void Organism::create_organism(string DNA)
{
    stringstream ss(DNA);
    ss >> name >> life >> death >> food_found >> x_pos >> y_pos 
       >> v_speed >> h_speed >> body_type >> mutation_chance;
    set_body(body_type);
}

// Determines if an Organism is close to a point in space that contains
// a food.
bool Organism::close_to_food(vector<food_pnt> food_loc, food_pnt &closest,
                              int UH, int UW)
{
    int distance = distance_to_food(food_loc, closest, UH, UW);
    if (distance < DIST) {
        return true;
    } else {
        return false;
    }
}

// Returns the percentage of the MAX_SIZE an Organism size currently is
double Organism::percentage_of_size()
{
    double parts = get_num_parts();
    double size  = MAX_SIZE;
    return parts / size; 
}

// Returns an int based on the Organisms size to use to determine its
// speed. Used to set the Minimum speed value. Lower numbers are 
// slower.
int Organism::get_low_range()
{
    double size  = percentage_of_size() * 100;
    if (size <= 20) {
        return 75;
    } else if (size > 20 and size <= 40) {
        return 50;
    } else if (size > 40 and size <= 65) {
        return 30;
    } else {
        return 10;
    }
}

// Returns an int based on the Organisms size to use to determine its
// speed. Used to set the Maximum speed value.  Higher numbers are 
// faster.
int Organism::get_high_range()
{
    double size  = percentage_of_size() * 100;
    if (size <= 20) {
        return 100;
    } else if (size > 20 and size <= 40) {
        return 80;
    } else if (size > 40 and size <= 65) {
        return 60;
    } else {
        return 30;
    }
}

// Moves the organism towards the food_pnt that is closest to it!  Sets the 
// vertical and horizontal speed to direct it towards that point.
// The larger an Organism grows, the slower it moves.
void Organism::move_towards_food(food_pnt closest)
{
    int org_row = get_x_pos();
    int org_col = get_y_pos();
    int rng_low = get_low_range();
    int rng_hgh = get_high_range();

    // Movement Up and Down
    if (org_row < closest.food_row) {
        set_v_speed(random_double(rng_low, rng_hgh));  // POSITVIE
    } else if (org_row == closest.food_row) {
        set_v_speed(0);
    } else if (org_row > closest.food_row){
        set_v_speed(random_double(rng_low, rng_hgh) * -1); // NEGATIVE
    }

    // Movement Left and Right
    if (org_col < closest.food_col) {
        set_h_speed(random_double(rng_low, rng_hgh));
    } else if (org_col == closest.food_col) {
        set_h_speed(0);
    } else if (org_col > closest.food_col) {
        set_h_speed(random_double(rng_low, rng_hgh) * -1);   
    }

    // Randomize if food found!
    // if (org_row == closest.food_row and org_col == closest.food_col) {
    //     change_movement();
    // }
}   

// Finds the distance to the closest food point.
int Organism::distance_to_food(vector<food_pnt> food_loc, food_pnt &closest,
                                int UH, int UW)
{
    int min = 999;
    int size = food_loc.size();
    int row  = abs((int)get_x_pos() % UH);
    int col  = abs((int)get_y_pos() % UW);
    
    if (size > 0) {
        closest = food_loc[0];
        min = abs(row - food_loc[0].food_row) +
              abs(col - food_loc[0].food_col);

        for (int i = 0; i < size; i++) {
            int next = abs(row - food_loc[i].food_row) +
                       abs(col - food_loc[i].food_col);
            if (next < min) {
                min = next;
                closest = food_loc[i];
            }
        } 
    }
    return min;
}

// An organism evolves over time.  And attempts to extend it's life by
// moving towards the closest food source. It will change direction ramdomly 
// if it is not within 999 spaces of food.  Also, after a given number
// of generations (MUTERATE), it will randomly mutate.
void Organism::evolve(int num_generations, int MUTRATE, 
                        vector <food_pnt> food_loc, int UH, int UW)
{
    food_pnt closest;
    life = life - death;
    if (food_found % 11 == 0) {
        food_found++;
        grow();
    }
    if (close_to_food(food_loc, closest, UH, UW)) {
        // cout << get_name() << " IS MOVING TOWARDS FOOD!!!" << endl;
        move_towards_food(closest);
    } else  {
        change_movement();
    }
    if (num_generations % MUTRATE == 0) {
        random_mutation();
    }
}
// Returns a double in range -100 to 100 used to set the vspeed or hspeed
// of an Organism.
double Organism::random_direction()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(-100, 100);
    return (rnd(rng) * .01);
    
}

// Returns a random double between low_range and high_range.
double Organism::random_double(int low_range, int high_range)
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(low_range, high_range);
    return (rnd(rng)) * .01;
}

// Change and organisms movement by randomly setting their v speed and 
// h speed.
void Organism::change_movement()
{
    set_v_speed(random_direction());
    set_h_speed(random_direction());
}

// Grow the organism by giving them one more body part. If the body part
// is placed somewhere they already have one, we have a chance of 
// increasing their mutation rate, else, they simply get +30 life.
void Organism::grow()
{   
    int row = rand() % MAX_H;
    int col = rand() % MAX_W;
    if (body[row][col] != body_type) {
        body[row][col]  = body_type;
        num_parts++;
    } else {
        if (random_chance()) {
            mutation_chance += 2;
            if (mutation_chance > 99) {
                mutation_chance = 50;
            }
        } else {
            life += 30;
        }
    }
}

// The law of entropy is inescapable...  
// Organisms lose a random body part or lose 10 life.
void Organism::decay()
{   
    int row = 0, col = 0;
    while (row == 0 and col == 0) {
        row = rand() % MAX_H;
        col = rand() % MAX_W;
    }
    if (body[row][col] == body_type) {
        body[row][col] = '\n';
        life += 10;   // lose a body part but absorb the nutrients..yum
        num_parts--;
    } else {
        life = life - 10;
    }
}

// Returns true if a random number is less than their mutation rate.
// Thus, the higher the mutation rate, the more likely this is to return
// true.
bool Organism::random_chance()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(0, 99);
    if (rnd(rng) < mutation_chance) {
        return true;
    }
        return false;
}

// Either grow or decay an Organism based on their Mutation rate.
void Organism::random_mutation()
{
    if (random_chance()){
        grow();
    } else {
        decay();
    }
    // mess with death and life rates here... ie diseases vs
    // eating right and exercise.
    if (random_chance()) {
        change_death();
    }
    
}

// Might be good, might be bad.  We randomly add or subtract from the
// rate at which organisms decay.
void Organism::change_death()
{
    if (random_chance()) {
        death -= random_double(1, 15);
        if (death < .1) {
            death = .1;
        }
    } else {
        death += random_double(1, 99);
    }
}

// Picks a random name from the names array.
string Organism::get_random_name()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(0, 18);
    return names[rnd(rng)];
}

// You gotta do what you gotta do. This method is called whenever an
// organism splits, makes a baby, or it's "life" drops to 0. 
// It reduces their size -1, but adds 10 life per part removed.
void Organism::self_sacrifice()
{
    bool found_part = false;
    for (int i = 0; i < MAX_H; i++) {
        for (int j = 0; j < MAX_W; j++) {
            if (i + j != 0 and body[i][j] == body_type) {
                body[i][j] = '\n';
                found_part = true;
                break;
            }
        }
        if (found_part == true) {
            break;
        }
    }
    life = life + 10;
    num_parts--;
}
