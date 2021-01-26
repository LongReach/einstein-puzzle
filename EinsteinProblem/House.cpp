#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "House.h"

using namespace std;

CatMap House::characteristic_to_category_map;
CharMap House::characteristic_to_idx_map;
string House::known_characteristics[TOTAL_CATEGORIES][TOTAL_HOUSES];

House::House() {
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        values[j] = -1;
    }
}

bool House::set_characteristic(int cat_idx, int val_idx) {
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    assert(val_idx >= 0 && val_idx < TOTAL_HOUSES);
    values[cat_idx] = val_idx;
    return true;
}

string House::get_characteristic(int cat_idx) {
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    if (values[cat_idx] == -1) {
        return "*";
    }
    return known_characteristics[cat_idx][values[cat_idx]];
}

bool House::can_merge(House* other, set<int> already_used_values[]) {
    if (this == other) {
        return true;
    }
    bool merge_allowed = true;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        // what vals have been used in current category?
        set<int>& vals_used_already = already_used_values[i];
        if (this->values[i] != -1) {
            if (this->values[i] != other->values[i] && other->values[i] != -1) {
                merge_allowed = false;
                break;
            }
        }
        else {
            // Is the value coming from the other already used on this street?
            set<int>::iterator it = vals_used_already.find(other->values[i]);
            if (it != vals_used_already.end()) {
                // Yes, it's used already, so we can't put it here
                merge_allowed = false;
                break;
            }
        }
    }
    return merge_allowed;
}

void House::merge(House* other) {
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] == -1) {
            this->values[i] = other->values[i];
        }
    }
}
    
void House::add_characteristic_and_category(string& characteristic, int cat, int idx) {
    assert(cat >= 0 && cat < TOTAL_CATEGORIES);
    assert(idx >= 0 && idx < TOTAL_HOUSES);
    characteristic_to_category_map[characteristic] = cat;
    characteristic_to_idx_map[characteristic] = idx;
    known_characteristics[cat][idx] = characteristic;
}

bool House::get_cat_and_idx_from_characteristic(string& characteristic, int* ret_cat, int* ret_idx) {
    CatMap::iterator it1 = characteristic_to_category_map.find(characteristic);
    if (it1 == characteristic_to_category_map.end()) {
        return false;
    }
    CharMap::iterator it2 = characteristic_to_idx_map.find(characteristic);
    if (it2 == characteristic_to_idx_map.end()) {
        return false;
    }
    *ret_cat = it1->second;
    *ret_idx = it2->second;
    return true;
}

string& House::get_characteristic_string(int cat_idx, int val_idx) {
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    assert(val_idx >= 0 && val_idx < TOTAL_HOUSES);
    return known_characteristics[cat_idx][val_idx];
}

StreetList Street::possible_streets;
set<int> Street::values_present[TOTAL_CATEGORIES];

// Helper function: takes string and pads out to twenty characters.
string special_format_string(const char* c_str) {
    int required_len = 20;
    int len = strlen(c_str);
    string out_str = c_str;
    int padding = required_len - len;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; i++) {
        out_str.append(" ");
    }
    return out_str;
}

Street::Street() {
}

void Street::set_characteristic(int addr, string& characteristic) {
    int cat_idx = -1;
    int val_idx = -1;
    bool success = House::get_cat_and_idx_from_characteristic(characteristic, &cat_idx, &val_idx);
    if (success) {
        houses[addr].set_characteristic(cat_idx, val_idx);
    }
}

string Street::get_characteristic(int addr, int cat_idx) {
    return houses[addr].get_characteristic(cat_idx);
}

Street* Street::combine(Street* other_street) {
    Street* new_street = new Street();
    int success_count = 0;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        House* this_house = &this->houses[i];
        House* other_house = &other_street->houses[i];
        if (this_house->can_merge(other_house, values_present)) {
            new_street->houses[i].merge(this_house);
            new_street->houses[i].merge(other_house);
            success_count++;
        }
    }
    // The merge must succeed on all five houses
    if (success_count == TOTAL_HOUSES) {
        return new_street;
    }
    delete new_street;
    return NULL;
}

void Street::print_info() {
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        string addr = to_string(i + 1);
        cout << special_format_string(addr.c_str());
    }
    cout << endl;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        cout << special_format_string("---------");
    }
    cout << endl;
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        for (int i = 0; i < TOTAL_HOUSES; i++) {
            string str_val = houses[i].get_characteristic(j);
            cout << special_format_string(str_val.c_str());
        }
        cout << endl;
    }
}

void Street::reset() {
    erase_street_list(possible_streets);
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        values_present[i].clear();
    }
}

int Street::get_possible_streets_count() {
    return possible_streets.size();
}

void Street::erase_street_list(StreetList& the_list) {
    for (StreetList::iterator it = the_list.begin(); it != the_list.end(); it++) {
        delete (*it);
    }
    the_list.clear();
}

void Street::print_street_list(StreetList* the_list, bool quiet) {
    if (the_list == NULL) {
        the_list = &possible_streets;
    }
    if (!quiet) {
        cout << "Printing street list of size: " << the_list->size() << endl;
        for (StreetList::iterator it = the_list->begin(); it != the_list->end(); it++) {
            (*it)->print_info();
            cout << endl;
        }
    }
    cout << "Finished street list of size: " << the_list->size() << endl << endl;
}

bool Street::have_used_all_values() {
    for (int cat = 0; cat < TOTAL_CATEGORIES; cat++) {
        if (values_present[cat].size() < TOTAL_HOUSES) return false;
    }
    return true;
}

void Street::make_combos(StreetList &new_streets, string& char1, string& char2) {
    int cat_idxs[2];
    int val_idxs[2];
    for (int i = 0; i < 2; i++) {
        bool success = House::get_cat_and_idx_from_characteristic((i == 0) ? char1 : char2, &cat_idxs[i], &val_idxs[i]);
        assert(success);
    }

    if (possible_streets.size() == 0) {
        // No possible streets exist yet
        possible_streets = new_streets;
        for (int i = 0; i < 2; i++) {
            values_present[cat_idxs[i]].insert(val_idxs[i]);
        }
        return;
    }

    StreetList new_combos;
    for (StreetList::iterator outer = possible_streets.begin(); outer != possible_streets.end(); outer++) {
        for (StreetList::iterator inner = new_streets.begin(); inner != new_streets.end(); inner++) {
            Street* new_street = (*outer)->combine(*inner);
            if (new_street) {
                new_combos.push_back(new_street);
            }
        }
    }
    // Erase both the old list of possible streets and the list of proposals to merge in. Frees the memory.
    erase_street_list(possible_streets);
    possible_streets = new_combos;
    erase_street_list(new_streets);
    for (int i = 0; i < 2; i++) {
        values_present[cat_idxs[i]].insert(val_idxs[i]);
    }
}

void Street::add_new_characteristics(string& char1, string& char2) {
    // Generate five proposals
    StreetList new_streets;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        Street* street = new Street();
        street->set_characteristic(i, char1);
        street->set_characteristic(i, char2);
        new_streets.push_back(street);
    }
    make_combos(new_streets, char1, char2);
    //print_street_list(&possible_streets, true);
}

void Street::add_neighbor_pair(string& char1, string& char2, int dir) {
    assert(dir == 0 || dir == 1);
    // Generate four proposals (since neighbors are next to each other, it's four, not five).
    StreetList new_streets;
    for (int i = 0; i < TOTAL_HOUSES-1; i++) {
        Street* street = new Street();
        street->set_characteristic(i, char1);
        street->set_characteristic(i+1, char2);
        new_streets.push_back(street);
    }
    // If left-right relationship of neighbors not known, generate four more proposals
    if (dir == 0) {
        for (int i = 0; i < TOTAL_HOUSES - 1; i++) {
            Street* street = new Street();
            street->set_characteristic(i, char2);
            street->set_characteristic(i + 1, char1);
            new_streets.push_back(street);
        }
    }

    make_combos(new_streets, char1, char2);
    //print_street_list(&possible_streets, true);
}

void Street::add_address(int address, string& the_char) {
    address--;
    assert(address >= 0 && address < TOTAL_CATEGORIES);
    StreetList new_streets;
    Street* street = new Street();
    street->set_characteristic(address, the_char);
    new_streets.push_back(street);
    make_combos(new_streets, the_char, the_char);
    //print_street_list(&possible_streets, true);
}
