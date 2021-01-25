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
    values[cat_idx] = val_idx;
    return true;
}

bool House::can_merge(House* other) {
    if (this == other) {
        return true;
    }
    bool merge_allowed = true;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        if (this->values[i] != -1) {
            if (other->values[i] != -1) {
                merge_allowed = false;
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


StreetList Street::possible_streets;

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

Street* Street::combine(Street* other_street) {
    Street* new_street = new Street();
    int success_count = 0;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        if (this->houses[i].can_merge(&other_street->houses[i])) {
            new_street->houses[i].merge(&this->houses[i]);
            new_street->houses[i].merge(&other_street->houses[i]);
            success_count++;
        }
    }
    if (success_count == TOTAL_HOUSES) {
        return new_street;
    }
    delete new_street;
    return NULL;
}

void Street::erase_street_list(StreetList& the_list) {
    for (StreetList::iterator it = the_list.begin(); it != the_list.end(); it++) {
        delete (*it);
    }
    the_list.clear();
}

void Street::make_combos(StreetList &new_streets) {
    if (possible_streets.size() == 0) {
        possible_streets = new_streets;
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
    erase_street_list(possible_streets);
    possible_streets = new_combos;
    erase_street_list(new_streets);
}

void Street::add_new_characteristics(string& char1, string& char2) {
    StreetList new_streets;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        Street* street = new Street();
        street->set_characteristic(i, char1);
        street->set_characteristic(i, char2);
        new_streets.push_back(street);
    }
}

