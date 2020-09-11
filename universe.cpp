/******************************************************************************
*   Program:    universe.cpp
*   Created By: Matt Champlin
*   Date:       Fri Feb 21 20:43:00 EST 2020
*               
*   Program Purpose: Implementation of Universe for Game of Chars to exist in!
*                    
******************************************************************************/


#include "universe.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "termfuncs.h"
#include <fstream>
#include <random>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

static const int DIV           = 10;
static const int TIME          = 1000000 / DIV; // one second divided by DIV
static const int QUANT_FLUX    = 75;    // cycles till new creature
static const int MUTRATE       = 250;   // cylcles till mutation
static const int PRINT_TIME    = 30;    // how long messages print for
static const int ABUNDANCE     = 1;     // how many foods drop
static const int PROLIFERATION = 1;     // how many cycles before food drops
static const double ENERGY     = 2;     // How much life each food is worth

// Default constructor
Universe::Universe()
{
    HEIGHT = 1;
    WIDTH  = 1;
    num_generations = 1; 
    num_dead = 0;
    print_me = 0;
    space = new char*[1];
    space[0] = new char[1];
}

// Parameterized constructor
Universe::Universe(int H, int W, string filename)
{
    srand(time(NULL));
    HEIGHT          = H;
    WIDTH           = W;
    NUM_FOOD        = H * W;
    num_generations = 1;
    num_dead        = 0;
    print_me        = PRINT_TIME;
    // allocate user defined height
    space = new char*[HEIGHT];
    // allocate user defined width
    for (int i = 0; i < HEIGHT; i++) {
        space[i] = new char[WIDTH];
    }
    // create board with spaces
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            space[i][j] = ' ';
        }
    }
    big_bang(filename);
}

// Destructor
Universe::~Universe()
{
    for (int i = 0; i < HEIGHT; i++) {
        delete [] space[i];
    }
    delete [] space;
}

// Starts the arrow of time!  This is the main workhorse of the program.
// Calls functions nessesary to run the simulation.
void Universe::arrow_of_time()
{
    while (true) {
        print();
        scan_universe();
        check_for_death();
        clear();    
        move_organisms();
        pass_time();
        print_messages();
        debug_organisms();
        usleep(TIME);
        screen_clear();
        screen_home();
        food_loc.clear();
    }
}

// Prints the fun messages about what is happening with all of the organisms
// in the universe!  Also prints a 
void Universe::print_messages()
{
    cout << "Time Passed: " << num_generations << endl;
    cout << "Number of dead: " << num_dead << endl;
    if (print_message and print_me > 0) {
        for (size_t i = 0; i < messages.size(); i++) {
            cout << messages[i] << endl;
        }
        print_me--;
    } else if (print_message and print_me == 0) {
        print_me      = PRINT_TIME;
        print_message = false;
        messages.clear();
    }
}

// Loops over Organism vector and checks that their size has not been reduced
// to zero, either by slowly decaying, or by getting eaten...
void Universe::check_for_death()
{
    for (size_t i = 0; i < organisms.size(); i++) {
        if (fear_the_reaper(organisms[i])) {    // check for death
            organisms.erase(organisms.begin() + i);
        }
    }
}

// Helper function that does the actual check for death per organism.
// Calls pay_respects() to print out a sad death message.
bool Universe::fear_the_reaper(Organism &O)
{
    if (O.get_num_parts() <= 0) {
        pay_respects(O);
        num_dead++;
        return true;
    }
    if (O.get_life() <= 0) {
        if (O.get_num_parts() > 1) {
            O.self_sacrifice();
            return false;
        } else {
            pay_respects(O);
            num_dead++;
            return true;
        }
    } 
    return false;
}

// Prints out the sad death message. If Jesus dies, we print a special 
// message.
void Universe::pay_respects(Organism kaput)
{
    messages.push_back(kaput.get_name() + " died!!"); 
    print_message = true;
    print_me      = PRINT_TIME;
    if (kaput.get_name() == "Jesus") {
        messages.push_back("Forgive them, for they know not "
                "what they do.");
        print_message = true;
        print_me      = PRINT_TIME;
    }
}

// Populates the Universe with food and creates any life found in the 
// file "filename"
void Universe::big_bang(string filename)
{
    place_food();
    create_life(filename);
}

// Opens the user or default file "filename" and creates any organisms found
// within.
void Universe::create_life(string filename)
{   
    ifstream infile(filename);
    try {
        if (! infile.is_open()) {
            throw runtime_error("File Unable To Be Opened!");
        }
    }
    catch (runtime_error err) {
        cout << err.what() << endl;
        exit(1);
    }
    
    string newlife;
    string DNA;
    while(! infile.eof()) {
        infile >> newlife;
        if (newlife == "##NEWLIFE") {
            Organism temp;
            getline(infile, DNA);
            temp.create_organism(DNA);
            organisms.push_back(temp);
        } else {
            getline(infile, newlife);   // throw away header from file
        }
    }
}

// Does the actual printing of the Universe state to the screen.
void Universe::print()
{
    place_organisms();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << space[i][j];
        }
    cout << endl;
    }
}

// Clears the previous call to print(). Skips over cells that have a FOOD point
// in them.
void Universe::clear()
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (space[i][j] != FOOD) {
                space[i][j] = ' ';
            }
        }
    }
}

// Time keeps on tickin', tickin', tickin', into the fuuuuutuuuuurrrrre!
// Increments our num_generation counter that determines if several things
// happen:  Mutation, the placing of new food and quantum fluctuations.
void Universe::pass_time()
{
    num_generations++;
    if (num_generations % MUTRATE == 0) {
        messages.push_back("Random Mutation Occuring!  DNA Scrambling....");
        print_me      = PRINT_TIME;
        print_message = true;
    }
    evolve_organisms();
    if (num_generations % PROLIFERATION == 0) {  // make food
        for (int i = 0; i < ABUNDANCE; i++) {
            place_one_food();  
        }
    }
    if (num_generations % QUANT_FLUX == 0) {  // Spawn new Organism 
        quantum_fluctuation();
    }
}

// A new Organism is spontaneously created and placed at a random position
// due to the quantum mechanical nature of the Universe.  
void Universe::quantum_fluctuation()
{
    Organism oaky_afterbirth(WIDTH - 1, HEIGHT - 1);
    if (oaky_afterbirth.get_body_type() == '.') {
        messages.push_back("HALLELUJAH!  THE SAVIOR IS BORN!!");
        print_me      = PRINT_TIME;
        print_message = true;
    } else {
        messages.push_back("A quantum fluctuation created " 
        + oaky_afterbirth.get_name() + "!!"); 
        print_me      = PRINT_TIME;
        print_message = true;
    }
    organisms.push_back(oaky_afterbirth);
}

// returns the amount of time that has passed.
int Universe::time_passed()
{
    return num_generations;
}

// "Moves" the organism on the screen by updating the x,y position they print
// from.
void Universe::move_organisms()
{
    for (size_t i = 0; i < organisms.size(); i++) {
        organisms[i].set_x_pos(organisms[i].get_x_pos() + organisms[i].get_v_speed());
        organisms[i].set_y_pos(organisms[i].get_y_pos() + organisms[i].get_h_speed());
    }
}

// Evolves organisms by calling their evolve method. 
// (see Organism.cpp for documentation)
// Also determines if an organism should replicate, based on how big they are
// and if enough time has passed since the last chance to replicate.
void Universe::evolve_organisms()
{
    int org_size = organisms[0].get_max_H() * organisms[0].get_max_W();
    for (size_t i = 0; i < organisms.size(); i++) {
        organisms[i].evolve(num_generations, MUTRATE, food_loc, HEIGHT, WIDTH);
        if (organisms[i].get_num_parts() > org_size * .8 and num_generations % 100 == 0) {
            organisms.push_back(replicate(organisms[i]));
        }
    }
}



// for debugging or for fun!
// Prints some information about each organisms state.  Uncomment below for
// more verbose detail.
void Universe::print_organisms()
{
    cout << "\nNUM OF ORGANISMS: " << organisms.size() << endl;
    for (size_t i = 0; i < organisms.size(); i++) {
        cout << "Name: " << organisms[i].get_name() 
             << "  Char: " << organisms[i].get_body_type() 
             << "  Death: " << organisms[i].get_death() 
             << "  Mutation: " << organisms[i].get_mutation()
             << "  Parts: " << organisms[i].get_num_parts()
             << "  Food: " << organisms[i].get_food() 
            //  << "  x_pos: " << organisms[i].get_x_pos() 
            //  << "  y_pos: " << organisms[i].get_y_pos() 
            //  << "  v_speed: " << organisms[i].get_v_speed() 
            //  << "  h_speed: " << organisms[i].get_h_speed() 
             << "  Life: " << organisms[i].get_life() << endl;
    }
}

// Returns value mod m, unless value is less than 0.  
unsigned Universe::modulo(int value, unsigned m) {
    int mod = value % (int)m;
    if (value < 0) {
        mod += m;
    }
    return mod;
}

// Places the organisms in the Universe by updating the "space" 2D array of 
// chars. Does some other checks like if they are on top of a food space,
// or if they have bumped into another organism.
void Universe::place_organisms()
{
    int row;
    int col = 0;
    
    for (size_t i = 0; i < organisms.size(); i++) {
        row = 0;
        int x = (int)organisms[i].get_x_pos();
        int y = (int)organisms[i].get_y_pos();
        for(int j = x; j < organisms[i].get_max_H() + x; j++) {
            for (int k = y; k < organisms[i].get_max_W() + y; k++) {
                if (organisms[i].get_char(row, col) != '\n') {
                    int ROW = modulo(j, HEIGHT);
                    int COL = modulo(k, WIDTH);
                    if (ROW == HEIGHT) {
                        ROW = 0;
                    }
                    if (COL == WIDTH) {
                        COL = 0;
                    }
                    if (is_food(ROW, COL)) {    // if any part of the organism
                        eat_food(organisms[i]); // touches food, it eats it.
                    }
                    // if the organism bumps into someone not like them, they
                    // might just throw fists.
                    if (is_organism(ROW, COL)) {
                        char my_body = space[ROW][COL];
                        if (organisms[i].get_body_type() != my_body) {
                            fight(organisms[i], find_best_match(i - 1, my_body));
                        } else {
                            if ((organisms[i].get_food() > 500) ) { // Make a baby
                                organisms[i].set_food(0);
                                organisms.push_back(bumpin_uglies(organisms[i]));
                            }
                        }
                    }
                    if (organisms[i].get_num_parts() > 0) {
                        place_at(ROW, COL, organisms[i].get_char(row, col));
                    }
                }
                col++;
            }
            row++;
            col = 0;
        }
    }
}

//  Organism replication at 70% cost to size and 20% cost to life.
Organism Universe::replicate(Organism &O) 
{
    // Parent is left with 70% of their size after replicating.
    for (int i = 0; i < (O.get_num_parts()) * .7; i++) { 
        O.self_sacrifice();
    }
    // Replicant is made in the parents image, but at -20% of the parents life
    O.set_life(O.get_life() * .8);
    Organism baby = O;
    baby.set_life(baby.get_life() * .7);
    baby.set_x_pos(rand() % HEIGHT - 1);
    baby.set_y_pos(rand() % WIDTH - 1);
    messages.push_back(O.get_name() + " replicated!");
    return baby;
}

// Creates a new baby Organism! Awww, how cute!  Reduces the size of the
// "Parent" Organism, since it drains the life out of you to have kids.
// Called if an organism bumps into one of it's own kind, and it has eaten 
// enough food to have the energy to procreate.  If so, we make a baby!
Organism Universe::bumpin_uglies(Organism &Parent)
{
    Organism baby = Parent;
    while (baby.get_num_parts() > 2 ) {
        baby.self_sacrifice();
    }
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd_h(0, HEIGHT - 1);
    uniform_int_distribution<int> rnd_w(0, WIDTH - 1);
    Parent.set_food(1);
    baby.set_food(1);
    baby.set_x_pos(rnd_h(rng));
    baby.set_y_pos(rnd_w(rng));
    messages.push_back("Baby " + baby.get_name() + " was born!");
    return baby;

}

// Finds the Organism that must have been bumped into. Since we print
// organisms in accending order, the Organism printed on "top" will have
// bumped into some organism printed before them. We find it, and return it.
Organism &Universe::find_best_match(int index, char to_find)
{
    for (int i = index; i >= 0; i--) {
        if (organisms[i].get_body_type() == to_find) {
            return organisms[i];
        } 
    }
    return organisms[0];
}

// Checks a "space" cell for an organism body part.
bool Universe::is_organism(int row, int col)
{
    char to_compare = space[row][col];
    if (to_compare != FOOD and to_compare != ' ') {
        return true;
    } else {
        return false;
    }
}

// Checks if the Organisms are of similar type, thus able to procreate.
bool Universe::we_can_mate(Organism &one, Organism &two)
{
    if (one.get_name() == two.get_name() and 
        one.get_body_type() == two.get_body_type()) 
    {
    messages.push_back(one.get_name() + " and " 
            + two.get_name() + " are Mating!!");
    print_message = true;
    print_me      = PRINT_TIME;
        return true;
    }
        return false;
}

// The first rule about Organism fight club: Document Organism fight club!
// Organisms fight if one is 2x larger than the other.  If so, the big 
// bully wins and eats a peice of the smaller guy.
// We push a message to the messages vector to print out that a conflict
// occured.
void Universe::fight(Organism &one, Organism &two) 
{   
    int one_size = one.get_num_parts();
    int two_size = two.get_num_parts();
    string one_name = one.get_name();
    string two_name = two.get_name();
    if (one_size >= two_size * 2) {
        two.self_sacrifice();
        one.grow();
        messages.push_back("Fight!! " + one_name + " won and ate a peice of "
             + two_name + "! " + two_name + " reduced to: " 
                        + to_string(two.get_num_parts()));
    } else if (two_size >= one_size * 2) {
        one.self_sacrifice();
        two.grow();
        messages.push_back("Fight!! " + two_name + " won and ate a peice of "
                         + one_name + "! " + one_name + " reduced to: " 
                         + to_string(one.get_num_parts()));
       
    } 
}

// Organisms eat food and gain ENERGY life.  Unless they are Jesus, since
// He is represented by a '.' character, which is food. Thus, Jesus will 
// give of Himself to the Universe, by not eating and leaving a trail of
// food!
void Universe::eat_food(Organism &O)
{   
    if (O.get_name() != "Jesus") {
        O.set_food(O.get_food() + 1);
        O.set_life(O.get_life() + ENERGY);
    }
}

// Checks a "space" cell for food.
bool Universe::is_food(int row, int col)
{
    if (space[row][col] == FOOD) {
        return true;
    } else {
        return false;
    }
}

// places a char "to_place" into space.
void Universe::place_at(int x, int y, char to_place)
{
    space[x][y] = to_place;
}

// Puts food '.' in random places in space.
void Universe::place_food()
{
    for (int i = 0; i < NUM_FOOD * .03; i++) {
        place_at(rand() % HEIGHT, rand() % WIDTH, FOOD);
    }
}

// Places one food '.' at a random place in space.
void Universe::place_one_food()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd_h(0, HEIGHT - 1);
    uniform_int_distribution<int> rnd_w(0, WIDTH - 1);
    place_at(rnd_h(rng), rnd_w(rng), FOOD);
    
}

// Calls simple print organisms for debugging.
void Universe::debug_organisms() 
{
    print_organisms();
}

// Prints the universe once.
void Universe::debug_print() 
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << space[i][j];
        }
    cout << endl;
    }
}

// Scans the universe for all food, and updates the food_loc vector with their
// positions.
void Universe::scan_universe()
{
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j++) {
            if (space[i][j] == FOOD) {
                food_pnt yum(i, j);
                food_loc.push_back(yum);
            }
        }
    }
}

