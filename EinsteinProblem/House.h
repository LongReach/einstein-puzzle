#pragma once
#include <set>
#include <string>

using namespace std;

const int TOTAL_HOUSES = 5;
const int TOTAL_CATEGORIES = 5;

typedef map<string, int> CatMap;
typedef map<string, int> CharMap;

/*
* Represents a theoretical house. One or more characteristics, such as nationality, pet, or drink, are present. When two different characteristics
* are determined to be in the same actual house, the two theoretical houses will be combined into one. This process continues until it's known
* where all the characteristics reside.
* 
* A characteristic is represented with a string, e.g. "beer", "green", or "Norwegian". Fortunately, there are no characteristics in different
* categories that have the same name (e.g. no one keeps "milk" as a pet).
* 
* Each house is aware of its neighbors, if known. 
* 
* Setting address of house: 
*/
class House {
public:
    House();
    // Assigns a characteristic to this house
    bool set_characteristic(int cat_idx, int val_idx);
    bool can_merge(House* other);
    void merge(House* other);

    // Adds a characteristic and its category number to tracking, for easy lookup
    static void add_characteristic_and_category(string& characteristic, int cat, int idx);
    static bool get_cat_and_idx_from_characteristic(string& characteristic, int* ret_cat, int* ret_idx);
    
private:
    int values[TOTAL_CATEGORIES]; // contains the assigned characteristic in each category or -1, if none

private:
    static CatMap characteristic_to_category_map; // maps a characteristic to a category number
    static CharMap characteristic_to_idx_map; // maps a characteristic to a category number
    static string known_characteristics[TOTAL_CATEGORIES][TOTAL_HOUSES];

};

class Street;
typedef vector<Street*> StreetList;

class Street {
public:
    Street();
    void set_characteristic(int addr, string& characteristic);
    Street* combine(Street* other_street);

    static void erase_street_list(StreetList& the_list);
    static void make_combos(StreetList &new_streets);
    static void add_new_characteristics(string& char1, string& char2);

private:
    House houses[TOTAL_HOUSES];

    static StreetList possible_streets;
};