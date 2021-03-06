#include <cassert>
#include <iostream>
#include "Street.h"
#include "House.h"

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

void Street::set_characteristic(int addr, const string& characteristic) {
    int cat_idx = -1;
    int val_idx = -1;
    bool success = House::get_cat_and_idx_from_characteristic(characteristic, &cat_idx, &val_idx);
    if (success) {
        houses_[addr].set_characteristic(cat_idx, val_idx);
    }
}

const string& Street::get_characteristic(int addr, int cat_idx) {
    return houses_[addr].get_characteristic(cat_idx);
}

Street* Street::combine(Street* other_street, set<int> values_present []) {
    Street* new_street = new Street();
    int success_count = 0;
    for (int i = 0; i < TOTAL_HOUSES; i++) {
        House* this_house = &this->houses_[i];
        House* other_house = &other_street->houses_[i];
        if (this_house->can_merge(other_house, values_present)) {
            new_street->houses_[i].merge(this_house);
            new_street->houses_[i].merge(other_house);
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
            string str_val = houses_[i].get_characteristic(j);
            cout << special_format_string(str_val.c_str());
        }
        cout << endl;
    }
}

StreetGroup::StreetGroup() {

}

void StreetGroup::reset() {
    erase_street_list(possible_streets_);
    for (int i = 0; i < TOTAL_CATEGORIES; i++) {
        values_present_[i].clear();
    }
    last_autofill_cat_ = -1;
    last_autofill_value_ = -1;
}

int StreetGroup::get_possible_streets_count() {
    return possible_streets_.size();
}

void StreetGroup::erase_street_list(StreetList& the_list) {
    for (StreetList::iterator it = the_list.begin(); it != the_list.end(); it++) {
        delete (*it);
    }
    the_list.clear();
}

void StreetGroup::print_street_list(StreetList* the_list, bool quiet) {
    if (the_list == NULL) {
        the_list = &possible_streets_;
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

bool StreetGroup::completable_streets_exist() {
    for (int cat = 0; cat < TOTAL_CATEGORIES; cat++) {
        if (values_present_[cat].size() < TOTAL_HOUSES - 1) return false;
    }
    return true;
}

string StreetGroup::get_last_autofill_value() {
    if (last_autofill_cat_ == -1 || last_autofill_value_ == -1) { return "*"; }
    return House::get_characteristic_string(last_autofill_cat_, last_autofill_value_);
}

void StreetGroup::make_combos(StreetList& new_streets, const string& char1, const string& char2) {
    int cat_idxs[2];
    int val_idxs[2];
    for (int i = 0; i < 2; i++) {
        bool success = House::get_cat_and_idx_from_characteristic((i == 0) ? char1 : char2, &cat_idxs[i], &val_idxs[i]);
        assert(success);
    }

    if (possible_streets_.size() == 0) {
        // No possible streets exist yet
        possible_streets_ = new_streets;
        for (int i = 0; i < 2; i++) {
            values_present_[cat_idxs[i]].insert(val_idxs[i]);
        }
        return;
    }

    StreetList new_combos;
    for (StreetList::iterator outer = possible_streets_.begin(); outer != possible_streets_.end(); outer++) {
        for (StreetList::iterator inner = new_streets.begin(); inner != new_streets.end(); inner++) {
            Street* new_street = (*outer)->combine(*inner, values_present_);
            if (new_street) {
                new_combos.push_back(new_street);
            }
        }
    }
    // Erase both the old list of possible streets and the list of proposals to merge in. Frees the memory.
    erase_street_list(possible_streets_);
    possible_streets_ = new_combos;
    erase_street_list(new_streets);
    for (int i = 0; i < 2; i++) {
        values_present_[cat_idxs[i]].insert(val_idxs[i]);
        if (last_autofill_cat_ == -1 && last_autofill_value_ == -1) {
            // We haven't come up with these yet, but maybe we can?
            if (completable_streets_exist()) {
                // We are at the point where each category has one or less characteristics left to add.
                if (values_present_[cat_idxs[i]].size() >= TOTAL_VALUES - 1) {
                    last_autofill_cat_ = cat_idxs[i]; // the category we just added a characteristic for
                    for (int val_idx = 0; val_idx < TOTAL_VALUES; val_idx++) {
                        if (values_present_[cat_idxs[i]].find(val_idx) == values_present_[cat_idxs[i]].end()) {
                            last_autofill_value_ = val_idx;
                        }
                    }
                    assert(last_autofill_value_ != -1);
                }
            }
        }
    }
}

void StreetGroup::add_new_characteristics(const string& char1, const string& char2) {
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

void StreetGroup::add_neighbor_pair(const string& char1, const string& char2, int dir) {
    assert(dir == 0 || dir == 1);
    // Generate four proposals (since neighbors are next to each other, it's four, not five).
    StreetList new_streets;
    for (int i = 0; i < TOTAL_HOUSES - 1; i++) {
        Street* street = new Street();
        street->set_characteristic(i, char1);
        street->set_characteristic(i + 1, char2);
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

void StreetGroup::add_address(int address, const string& the_char) {
    address--;
    assert(address >= 0 && address < TOTAL_CATEGORIES);
    StreetList new_streets;
    Street* street = new Street();
    street->set_characteristic(address, the_char);
    new_streets.push_back(street);
    make_combos(new_streets, the_char, the_char);
    //print_street_list(&possible_streets, true);
}
