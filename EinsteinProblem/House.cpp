#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "House.h"

using namespace std;

vector<House*> House::houses;
HouseMap House::characteristic_to_house_map;
CatMap House::characteristic_to_category_map;
string House::known_characteristics[TOTAL_CATEGORIES][TOTAL_HOUSES];
set<string> House::remaining_values[TOTAL_CATEGORIES]; // values that might yet be assigned
int House::id_counter = 0;

House::House() {
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        this->values[j].assign("*");
    }
    this->merged_into = NULL;
    this->id = id_counter++;
    houses.push_back(this);
}

bool House::set_characteristic(string& the_char) {
    // TODO:
    // If the characteristics is an address, make sure we propagate that info to neighbors

    CatMap::iterator it = characteristic_to_category_map.find(the_char);
    if (it == characteristic_to_category_map.end()) {
        //cout << "cat idx not found\n";
        assert(0);
        return false;
    }
    int cat_idx = it->second;
    //cout << "cat idx is " << cat_idx << "\n";
    this->values[cat_idx] = the_char;
    characteristic_to_house_map[the_char] = this;
    //assert(num_erased == 1);

    return true;
}

int House::get_address() {
    if (this->values[0] == "*") return -1;
    return atoi(this->values[0].c_str());
}

bool House::match_address(int address) {
    string addr_str = to_string(address);
    if (this->values[0] != "*") {
        // This house has an address
        if (this->values[0] == addr_str) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        // This house doesn't not have an address. Is there another house entry here?
        House* house_at_addr = House::get_house(addr_str);
        if (!house_at_addr) {
            return true;
        }
        return (can_merge(house_at_addr));
    }
}

bool House::can_merge(House* other) {
    assert(!this->is_defunct());
    other = other->resolve_pointer();
    if (this == other) {
        return true;
    }
    bool merge_allowed = true;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] != "*") {
            if (other->values[i] != "*") {
                merge_allowed = false;
            }
        }
    }
    return merge_allowed;
}
    
void House::merge(House* other) {
    assert(!this->is_defunct());
    other = other->resolve_pointer();
    if (this == other) {
        return;
    }
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] == "*") {
            this->values[i].assign(other->values[i]);
            if (other->values[i] != "*") {
                characteristic_to_house_map[other->values[i]] = this;
            }
        }
        else {
            assert(other->values[i] == this->values[i] || other->values[i] == "*");
        }
    }
    other->merged_into = this;
}

bool House::is_defunct() {
    return (this->merged_into != NULL);
}

House *House::resolve_pointer()
{
    House* ptr = this;
    while (ptr->merged_into)
    {
        ptr = this->merged_into;
    }
    return ptr;
}

int House::get_id() {
    return this->id;
}

void House::print_info() {
    cout << "House  " << this->id << ": ";
    bool add_comma = false;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] == "*") {
            cout << (add_comma ? "," : "") << "*";
        }
        else {
            cout << (add_comma ? "," : "") << this->values[i];
        }
        add_comma = true;
    }
    cout << "\n";
}

House* House::get_house(string& characteristic) {
    House* house = NULL;
    HouseMap::iterator it = characteristic_to_house_map.find(characteristic);
    if (it != characteristic_to_house_map.end()) {
        //cout << "Retrieving house with characteristic " << characteristic << "\n";
        house = it->second;
    }
    return house;
}

House* House::make_or_get_house(string& characteristic) {
    House* house = NULL;
    HouseMap::iterator it = characteristic_to_house_map.find(characteristic);
    if (it != characteristic_to_house_map.end()) {
        //cout << "Retrieving house with characteristic " << characteristic << "\n";
        house = it->second;
    }
    else {
        //cout << "Making house with characteristic " << characteristic << "\n";
        house = new House();
        house->set_characteristic(characteristic);
    }
    return house;
}

void House::add_characteristic_and_category(string& characteristic, int cat, int idx) {
    characteristic_to_category_map[characteristic] = cat;
    known_characteristics[cat][idx] = characteristic;
    remaining_values[cat].insert(characteristic);
}

void House::disassociate(string& characteristic) {
    CatMap::iterator it = characteristic_to_category_map.find(characteristic);
    if (it == characteristic_to_category_map.end()) {
        //cout << "cat idx not found\n";
        assert(0);
    }
    int cat_idx = it->second;
    remaining_values[cat_idx].erase(characteristic);
}

vector<NeighborData*> NeighborData::pairs;

NeighborData::NeighborData(string &char1, string &char2, int d) {
    char_name[0] = char1;
    char_name[1] = char2;
    dir = d;
    pairs.push_back(this);
}

bool NeighborData::attempt_resolve() {
    int num_passes = (this->dir == 0) ? 2 : 1;

    House* house1 = House::get_house(char_name[0]);
    House* house2 = House::get_house(char_name[1]);
    if (house1->get_address() != -1 && house2->get_address() != -1) {
        return false;
    }
    int first_resolve_addr = -1;
    int first_resolve_pass_num = -1;
    int resolve_count = 0;

    for (int pass = 0; pass < num_passes; pass++) {
        House* left = (pass == 0) ? house1 : house2;
        House* right = (pass == 0) ? house2 : house1;
        for (int addr = 1; addr <= 4; addr++) {
            bool match_l = left->match_address(addr);
            bool match_r = right->match_address(addr + 1);
            if (match_l && match_r) {
                resolve_count++;
                first_resolve_pass_num = pass;
                first_resolve_addr = addr;
            }
        }
    }
    if (resolve_count == 1) {
        House* left = (first_resolve_pass_num == 0) ? house1 : house2;
        House* right = (first_resolve_pass_num == 0) ? house2 : house1;
        string addr = to_string(first_resolve_addr);
        merge_characteristic(left, addr);
        House::disassociate(addr);
        addr = to_string(first_resolve_addr+1);
        merge_characteristic(right, addr);
        House::disassociate(addr);
        if (this->dir == 0) {
            this->dir = 1;
            if (first_resolve_pass_num == 1) {
                string swap = char_name[0];
                char_name[0] = char_name[1];
                char_name[1] = swap;
            }
        }
        return true;
    }

    return false;
}

void NeighborData::print_info() {
    House* house1 = House::get_house(this->char_name[0]);
    House* house2 = House::get_house(this->char_name[1]);
    cout << "Neighbors: " << house1->get_id() << " and " << house2->get_id() << " dir=" << this->dir << "\n";
}

void NeighborData::merge_characteristic(House* house, string& characteristic) {
    House* other_house = House::get_house(characteristic);
    if (other_house) {
        house->merge(other_house);
    }
    else {
        house->set_characteristic(characteristic);
    }
}

 
