#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "House.h"

using namespace std;

vector<House*> House::houses;
HouseMap House::characteristic_to_house_map;
CatMap House::characteristic_to_category_map;
int House::id_counter = 0;

House::House() {
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        this->values[i].assign("*");
    }
    this->merged_into = NULL;
    this->ordered_neighbors[0] = NULL;
    this->ordered_neighbors[1] = NULL;
    this->id = id_counter++;
    houses.push_back(this);
}

bool House::set_characteristic(string& the_char) {
    // TODO:
    // If the characteristics is an address, make sure we propagate that info to neighbors

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

void House::set_address(int address) {
    this->values[0].assign(to_string(address));
    // TODO:
    // Make sure the address gets propagated to neighbors
}

int House::get_address() {
    if (this->values[0] == "*") return -1;
    return atoi(this->values[0].c_str());
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
    // TODO: make sure we pull in the neighbor information from the other
    other->merged_into = this;
}

void House::set_neighbor(House* other, int dir)
{
    if (dir == 0) {
        // The other could be a neighbor on either side
    }
    else {
        // The other is on a specific side
        int idx = (dir == -1) ? 0 : 1;
        if (this->ordered_neighbors[idx] != other) {
            this->ordered_neighbors[idx] = other;
            int my_address = this->get_address();
            if (my_address != -1) {
                other->set_address(my_address + dir);
            }
            // Make sure the other knows that I'm their neighbor
            other->set_neighbor(this, dir * -1);
        }
    }
}

bool House::is_defunct() {
    return (this->merged_into != NULL);
}

void House::update_stale_neighbor_pointers()
{
    // Go through the neighbor pointers and make sure they're fresh. This takes
    // advantage of the merge_into member.
}

void House::print_info() {
    cout << "House  " << this->id << ": ";
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

