// EinsteinProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

// Links the two characteristics. If they are in different houses, the houses are merged.
void connect_characteristics(string char1, string char2) {
    House* house1 = House::make_or_get_house(char1);
    House* house2 = House::make_or_get_house(char2);
    house1->merge(house2);
}

// Provides a house with information about its neighbor
void set_neighbor(string my_char, string neighbor_char, int dir) {
    House* my_house = House::make_or_get_house(my_char);
    House* neighbor = House::make_or_get_house(neighbor_char);
    my_house->set_neighbor(neighbor, dir);
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
    // TODO
    // The Blend smoker has a neighbor who keeps cats.
    // The man who smokes Blue Masters drinks bier.
    // The man who keeps horses lives next to the Dunhill smoker.
    // The German smokes Prince.
    // The Norwegian lives next to the blue house.
    // The Blend smoker has a neighbor who drinks water.

    for (vector<House*>::iterator it = House::houses.begin(); it != House::houses.end(); it++) {
        House* house = *it;
        if (!house->is_defunct()) {
            house->print_info();
        }
    }
}

