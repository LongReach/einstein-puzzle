// EinsteinProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "House.h"
#include "PuzzleSolver.h"
#include "PuzzleMaker.h"

/*
Original Rules:

The Englishman lives in the red house.
The Swede keeps dogs.
The Dane drinks tea.
The green house is just to the left of the white one.
The owner of the green house drinks coffee.
The Pall Mall smoker keeps birds.
The owner of the yellow house smokes Dunhills.
The man in the center house drinks milk.
The Norwegian lives in the first house.
The Blend smoker has a neighbor who keeps cats.
The man who smokes Blue Masters drinks bier.
The man who keeps horses lives next to the Dunhill smoker.
The German smokes Prince.
The Norwegian lives next to the blue house.
The Blend smoker has a neighbor who drinks water.
*/

// Arrays of characteristic info. Each one in a category is unique to a particular house.
string categories[] = { "address", "color", "nationality", "drink", "cigarette", "pet" };
string colors[] = { "blue", "green", "red", "white", "yellow" };
string nationalities[] = { "Danish", "English", "German", "Swedish", "Norwegian" };
string drinks[] = { "beer", "coffee", "milk", "tea", "water" };
string cigarettes[] = { "Blend", "Bluemaster", "Dunhill", "Pall Mall", "Prince" };
string pets[] = { "bird", "cat", "dog", "fish", "horse" };

// Process the informational arrays above into more useable data structures
void process_characteristics() {
    string* array_lookup[TOTAL_CATEGORIES] = { colors, nationalities, drinks, cigarettes, pets };
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        for (int i = 0; i < TOTAL_HOUSES; i++) {
            House::add_characteristic_and_category(array_lookup[j][i], j, i);
        }
    }
}

int main()
{
    std::cout << "Einstein's Puzzle\n-----------------\n";

    process_characteristics();

    string rules[] = { "pair 'English' 'red'", 
        "pair 'Swedish' 'dog'", 
        "pair 'Danish' 'tea'",
        "neighbor 'green' 'white' 1",
        "pair 'green' 'coffee'",
        "pair 'Pall Mall' 'bird'",
        "pair 'yellow' 'Dunhill'",
        "address 3 'milk'",
        "address 1 'Norwegian'",
        "neighbor 'Blend' 'cat'",
        "pair 'Bluemaster' 'beer'",
        "neighbor 'horse' 'Dunhill'",
        "pair 'German' 'Prince'",
        "neighbor 'Norwegian' 'blue'",
        "neighbor 'Blend' 'water'",
        "single 'fish'",
        "done"};

#if 0
    PuzzleSolver solver;
    solver.add_steps(rules);
    while (solver.run_next_step()) {

    }
    Street::print_street_list();
#endif

#if 1
    PuzzleMaker maker;
    maker.make_model_street();
    maker.make_puzzle();
#endif

#if 0
    PuzzleMaker maker;
    maker.make_model_street();
    for (int i = 0; i < 10; i++) {
        maker.make_rule();
    }
#endif
}

