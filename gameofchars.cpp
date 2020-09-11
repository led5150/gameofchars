/******************************************************************************
*   Program:    gameofchars.cpp
*   Created By: Matt Champlin
*   Date:       Fri Feb 21 20:44:17 EST 2020
*               
*   Program Purpose: Runs the GameOfChars!
*                    
******************************************************************************/

#include <iostream>
#include "universe.h"
#include "unistd.h"
#include <stdlib.h>

using namespace std;

void run_universe(int argc, char *argv[]);
bool input_is_ok(int argc, char *argv[]);
void usage_message(string error);

static const int DEFAULT_HEIGHT = 20;
static const int DEFAULT_WIDTH  = 80;

int main(int argc, char *argv[])
{    
    if (input_is_ok(argc, argv)){
        run_universe(argc, argv);
    } 
    return 0;
}

// Check user input for mistakes. Note: if argc == 2, we let universe.cpp
// check that the file is ok.
bool input_is_ok(int argc, char *argv[])
{
    if (argc == 3) {
        if (! atoi(argv[1]) or ! atoi(argv[2])) {
            usage_message("HEIGHT and WIDTH arguments must be integers!");
            return false;
        }
    }
    if (argc > 4) {
        usage_message("Too many arguments!");
        return false;
    }
    return true;
}

// Runs the type of universe the user specified with command line arguments
void run_universe(int argc, char *argv[])
{
    if (argc == 1) {
        Universe U(DEFAULT_HEIGHT, DEFAULT_WIDTH, "creatures.txt");
        U.arrow_of_time();
    }
    if (argc == 2) {
        Universe U(DEFAULT_HEIGHT, DEFAULT_WIDTH, argv[1]);
        U.arrow_of_time();
    }
    if (argc == 3) {
        Universe U(atoi(argv[1]), atoi(argv[2]), "creatures.txt");
        U.arrow_of_time();
    }
    if (argc == 4) {
        Universe U(atoi(argv[1]), atoi(argv[2]), argv[3]);
        U.arrow_of_time();
    }
}

// Prints a simple usage message and the error that caused it.
void usage_message(string error) {
    cout << "Usage: ./gameofchars [HEIGHT] [WIDTH] [filename]" << endl;
    cout << "       ./gameofchars filename" << endl;
    cout << "Error: " << error << endl;
}

