#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "House.h"

using namespace std;

CatMap House::characteristic_to_category_map_;
CharMap House::characteristic_to_idx_map_;
string House::known_characteristics_[TOTAL_CATEGORIES][TOTAL_VALUES];

House::House() {
    for (int j = 0; j < TOTAL_CATEGORIES; j++) {
        values_[j] = -1;
    }
}

bool House::set_characteristic(int cat_idx, int val_idx) {
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    assert(val_idx >= 0 && val_idx < TOTAL_VALUES);
    values_[cat_idx] = val_idx;
    return true;
}

const string &House::get_characteristic(int cat_idx) {
    static const string star = "*";
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    if (values_[cat_idx] == -1) {
        return star;
    }
    return known_characteristics_[cat_idx][values_[cat_idx]];
}

bool House::can_merge(House* other, set<int> already_used_values[]) {
    if (this == other) {
        return true;
    }
    bool merge_allowed = true;
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        // what vals have been used in current category?
        set<int>& vals_used_already = already_used_values[i];
        if (this->values_[i] != -1) {
            if (this->values_[i] != other->values_[i] && other->values_[i] != -1) {
                merge_allowed = false;
                break;
            }
        }
        else {
            // Is the value coming from the other already used on this street?
            set<int>::iterator it = vals_used_already.find(other->values_[i]);
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
        if (this->values_[i] == -1) {
            this->values_[i] = other->values_[i];
        }
    }
}
    
void House::add_characteristic_and_category(const string& characteristic, int cat, int idx) {
    assert(cat >= 0 && cat < TOTAL_CATEGORIES);
    assert(idx >= 0 && idx < TOTAL_VALUES);
    characteristic_to_category_map_[characteristic] = cat;
    characteristic_to_idx_map_[characteristic] = idx;
    known_characteristics_[cat][idx] = characteristic;
}

bool House::get_cat_and_idx_from_characteristic(const string& characteristic, int* ret_cat, int* ret_idx) {
    CatMap::iterator it1 = characteristic_to_category_map_.find(characteristic);
    if (it1 == characteristic_to_category_map_.end()) {
        return false;
    }
    CharMap::iterator it2 = characteristic_to_idx_map_.find(characteristic);
    if (it2 == characteristic_to_idx_map_.end()) {
        return false;
    }
    *ret_cat = it1->second;
    *ret_idx = it2->second;
    return true;
}

const string& House::get_characteristic_string(int cat_idx, int val_idx) {
    assert(cat_idx >= 0 && cat_idx < TOTAL_CATEGORIES);
    assert(val_idx >= 0 && val_idx < TOTAL_VALUES);
    return known_characteristics_[cat_idx][val_idx];
}

