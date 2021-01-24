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
string addresses[] = { "1", "2", "3", "4", "5" };
string colors[] = { "blue", "green", "red", "white", "yellow" };
string nationalities[] = { "Danish", "English", "German", "Swedish", "Norwegian" };
string drinks[] = { "beer", "coffee", "milk", "tea", "water" };
string cigarettes[] = { "Blend", "Bluemaster", "Dunhill", "Pall Mall", "Prince" };
string pets[] = { "bird", "cat", "dog", "fish", "horse" };

// Process the informational arrays above into more useable data structures
void process_characteristics() {
    string* array_lookup[TOTAL_CATEGORIES] = { addresses, colors, nationalities, drinks, cigarettes, pets };
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        for (int i = 0; i < TOTAL_HOUSES; i++) {
            House::add_characteristic_and_category(array_lookup[j][i], j);
        }
    }
}

void print_houses() {
    std::cout << "Houses are:\n=======\n";
    for (vector<House*>::iterator it = House::houses.begin(); it != House::houses.end(); it++) {
        House* house = *it;
        if (!house->is_defunct()) {
            house->print_info();
        }
    }

    for (vector<NeighborData*>::iterator it = NeighborData::pairs.begin(); it != NeighborData::pairs.end(); it++) {
        NeighborData* nd = *it;
        nd->print_info();
    }

    House::print_unused_characteristics();
}

bool elimination_round() {
    bool change_made = false;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (House::at_last_characteristic(i)) {
            string remaining_char = House::get_last_characteristic(i);
            House* house = House::make_or_get_house(remaining_char);
            change_made = true;
        }
    }
    return change_made;
}

bool neighbors_round() {
    bool change_made = false;
    for (vector<NeighborData*>::iterator it = NeighborData::pairs.begin(); it != NeighborData::pairs.end(); it++) {
        NeighborData* nd = *it;

        House* my_house = House::get_house(nd->char_name[0]);
        House* other_house = House::get_house(nd->char_name[1]);
        int my_addr = my_house->get_address();
        int other_addr = other_house->get_address();
        if (nd->dir == 0) {
            if (my_addr == 1) {
                other_addr = 2;
                nd->dir = 1;
            }
            if (my_addr == 5) {
                other_addr = 4;
                nd->dir = 1;
                string swap = nd->char_name[0];
                nd->char_name[0] = nd->char_name[1];
                nd->char_name[1] = swap;
            }
        }
        else {
            if (my_addr != -1) {
                other_addr = my_addr + nd->dir;
            }
        }
        if (other_addr != -1) {
            string other_addr_str = to_string(other_addr);
            House* merge_house = House::make_or_get_house(other_addr_str);
            other_house->merge(merge_house);
            change_made = (merge_house != other_house);
        }
    }
    if (change_made) {
        return true;
    }
    for (vector<NeighborData*>::iterator it = NeighborData::pairs.begin(); it != NeighborData::pairs.end(); it++) {
        NeighborData* nd = *it;
        if (nd->attempt_resolve()) {
            change_made = true;
        }
    }
    return change_made;
}

void do_rounds() {
    while (true) {
        bool elims_made = elimination_round();
        bool neighbor_changes = neighbors_round();
        print_houses();
        if (!elims_made && !neighbor_changes) {
            break;
        }
    }
}

// Links the two characteristics. If they are in different houses, the houses are merged.
void connect_characteristics(string char1, string char2) {
    House* house1 = House::get_house(char1);
    House* house2 = House::get_house(char2);
    if (house1 == NULL && house2 == NULL) {
        house1 = House::make_or_get_house(char1);
        house1->set_characteristic(char1);
        house1->set_characteristic(char2);
    }
    else if (house1 && house2 == NULL) {
        house1->set_characteristic(char2);
    }
    else if (house2 && house1 == NULL) {
        house2->set_characteristic(char1);
    }
    else {
        house1->merge(house2);
    }
    do_rounds();
}

// Provides a house with information about its neighbor
void set_neighbor(string my_char, string neighbor_char, int dir) {
    House::make_or_get_house(my_char);
    House::make_or_get_house(neighbor_char);
    NeighborData* data = new NeighborData(my_char, neighbor_char, dir);
    do_rounds();
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
#if 0
    // Simple tests
    string s1 = "Danish";
    House* h1 = make_or_get_house(s1);
    string s2 = "milk";
    House* h2 = make_or_get_house(s2);
    string s3 = "monkey"; // monkey is not part of the pet set
    House* h3 = make_or_get_house(s3);
#endif

    connect_characteristics("English", "red"); // The Englishman lives in the red house.
    connect_characteristics("Swedish", "dog"); // The Swede keeps dogs.
    connect_characteristics("Danish", "tea"); // The Dane drinks tea.
    set_neighbor("green", "white", 1); // The green house is just to the left of the white one.
    connect_characteristics("green", "coffee"); // The owner of the green house drinks coffee.
    connect_characteristics("Pall Mall", "bird"); // The Pall Mall smoker keeps birds.
    connect_characteristics("yellow", "Dunhill"); // The owner of the yellow house smokes Dunhills.
    connect_characteristics("3", "milk"); // The man in the center house drinks milk.
    connect_characteristics("Norwegian", "1"); // The Norwegian lives in the first house.
    set_neighbor("Blend", "cat", 0); // The Blend smoker has a neighbor who keeps cats.
    connect_characteristics("Bluemaster", "beer"); // The man who smokes Blue Masters drinks bier.
    set_neighbor("horse", "Dunhill", 0); // The man who keeps horses lives next to the Dunhill smoker.
    connect_characteristics("German", "Prince"); // The German smokes Prince.
    set_neighbor("Norwegian", "blue", 0); // The Norwegian lives next to the blue house.
    set_neighbor("Blend", "water", 0); // The Blend smoker has a neighbor who drinks water.
}

