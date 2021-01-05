#include <iostream>
#include <map>
#include <vector>
#include "House.h"

using namespace std;

vector<House*> House::houses;
HouseMap House::characteristic_to_house_map;
CatMap House::characteristic_to_category_map;

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

House* House::make_or_get_house(string& characteristic) {
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

void House::add_characteristic_and_category(string& characteristic, int cat) {
    characteristic_to_category_map[characteristic] = cat;
}

