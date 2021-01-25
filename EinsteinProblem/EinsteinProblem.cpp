// EinsteinProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "House.h"

/*
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

// Links the two characteristics. If they are in different houses, the houses are merged.
void connect_characteristics(string char1, string char2) {
    cout << "Connecting characteristic " << char1 << " to " << char2 << endl;
    Street::add_new_characteristics(char1, char2);
}

void add_characteristic(string the_char) {
    cout << "Adding characteristic " << the_char << endl;
    Street::add_new_characteristics(the_char, the_char);
}

// Provides a house with information about its neighbor
void set_neighbor(string my_char, string neighbor_char, int dir) {
    cout << "Setting neighbor of " << my_char << " to " << neighbor_char << endl;
    Street::add_neighbor_pair(my_char, neighbor_char, dir);
}

void set_address(int address, string the_char) {
    cout << "Setting address of " << the_char << " to " << address << endl;
    Street::add_address(address, the_char);
}

/*
* Ideas:
* 
* As we process each fact, we put associated characteristics together into a single house.
* 
*/

int main()
{
    std::cout << "Einstein's Puzzle\n-----------------\n";

    process_characteristics();

    connect_characteristics("English", "red"); // The Englishman lives in the red house.
    connect_characteristics("Swedish", "dog"); // The Swede keeps dogs.
    connect_characteristics("Danish", "tea"); // The Dane drinks tea.
    set_neighbor("green", "white", 1); // The green house is just to the left of the white one.
    connect_characteristics("green", "coffee"); // The owner of the green house drinks coffee.
    connect_characteristics("Pall Mall", "bird"); // The Pall Mall smoker keeps birds.
    connect_characteristics("yellow", "Dunhill"); // The owner of the yellow house smokes Dunhills.
    set_address(3, "milk"); // The man in the center house drinks milk.
    set_address(1, "Norwegian"); // The Norwegian lives in the first house.
    set_neighbor("Blend", "cat", 0); // The Blend smoker has a neighbor who keeps cats.
    connect_characteristics("Bluemaster", "beer"); // The man who smokes Blue Masters drinks bier.
    set_neighbor("horse", "Dunhill", 0); // The man who keeps horses lives next to the Dunhill smoker.
    connect_characteristics("German", "Prince"); // The German smokes Prince.
    set_neighbor("Norwegian", "blue", 0); // The Norwegian lives next to the blue house.
    set_neighbor("Blend", "water", 0); // The Blend smoker has a neighbor who drinks water.
    add_characteristic("fish");
    Street::print_street_list();
}

