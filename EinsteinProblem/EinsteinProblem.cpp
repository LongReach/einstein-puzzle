// EinsteinProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <set>

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

using namespace std;

const int TOTAL_HOUSES = 5;
const int TOTAL_CATEGORIES = 6;

string categories[] = { "address", "color", "nationality", "drink", "cigarette", "pet" };
string addresses[] = { "1", "2", "3", "4", "5" };
string colors[] = { "blue", "green", "red", "white", "yellow" };
string nationalities[] = { "Danish", "English", "German", "Swedish", "Norwegian" };
string drinks[] = { "beer", "coffee", "milk", "tea", "water" };
string cigarettes[] = { "Blend", "Bluemaster", "Dunhill", "Pall Mall", "Prince" };
string pets[] = { "bird", "cat", "dog", "fish", "horse" };

typedef map<string, int> CatMap;
CatMap characteristic_to_category_map;

class House {
public:
    House();
    bool set_characteristic(string& the_char);
    void merge(House* other);
    bool get_remove_flag();
    void print_info();

    static vector<House*> houses;
private:
    string values[TOTAL_CATEGORIES];
    bool remove_flag;
};

typedef map<string, House*> HouseMap;
HouseMap characteristic_to_house_map;

vector<House*> House::houses;

House::House() {
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        this->values[i].assign("*");
    }
    this->remove_flag = false;
    houses.push_back(this);
}

bool House::set_characteristic(string& the_char) {
    CatMap::iterator it = characteristic_to_category_map.find(the_char);
    if (it == characteristic_to_category_map.end()) {
        cout << "cat idx not found\n";
        return false;
    }
    int cat_idx = it->second;
    cout << "cat idx is " << cat_idx << "\n";
    this->values[cat_idx] = the_char;
    characteristic_to_house_map[the_char] = this;
    return true;
}

void House::merge(House* other) {
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] == "*") {
            this->values[i].assign(other->values[i]);
            if (other->values[i] != "*") {
                characteristic_to_house_map[other->values[i]] = this;
            }
        }
    }
    other->remove_flag = true;
}

bool House::get_remove_flag() {
    return this->remove_flag;
}

void House::print_info() {
    cout << "House: ";
    bool add_comma = false;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] != "*") {
            cout << (add_comma ? "," : "") << this->values[i];
            add_comma = true;
        }
    }
    cout << "\n";
}

void process_characteristics() {
    string* array_lookup[TOTAL_CATEGORIES] = { addresses, colors, nationalities, drinks, cigarettes, pets };
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        for (int i = 0; i < TOTAL_HOUSES; i++) {
            characteristic_to_category_map[array_lookup[j][i]] = j;
        }
    }
}

House* make_or_get_house(string& characteristic) {
    House* house = NULL;
    HouseMap::iterator it = characteristic_to_house_map.find(characteristic);
    if (it != characteristic_to_house_map.end()) {
        cout << "Retrieving house with characteristic " << characteristic << "\n";
        house = it->second;
    }
    else {
        cout << "Making house with characteristic " << characteristic << "\n";
        house = new House();
        house->set_characteristic(characteristic);
    }
    return house;
}

void connect_characteristics(string char1, string char2) {
    House* house1 = make_or_get_house(char1);
    House* house2 = make_or_get_house(char2);
    house1->merge(house2);
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
    /*
    string s1 = "Danish";
    House* h1 = make_or_get_house(s1);
    string s2 = "milk";
    House* h2 = make_or_get_house(s2);
    string s3 = "butt";
    House* h3 = make_or_get_house(s3);
    */

    connect_characteristics("English", "red");
    connect_characteristics("Swedish", "dog");
    connect_characteristics("Danish", "tea");
    // missing
    connect_characteristics("green", "coffee");
    connect_characteristics("Pall Mall", "bird");
    connect_characteristics("yellow", "Dunhill");
    connect_characteristics("3", "milk");
    connect_characteristics("Norwegian", "1");

    for (vector<House*>::iterator it = House::houses.begin(); it != House::houses.end(); it++) {
        House* house = *it;
        if (!house->get_remove_flag()) {
            house->print_info();
        }
    }
}

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

