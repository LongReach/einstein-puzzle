#pragma once
#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

const int TOTAL_HOUSES = 5;
const int TOTAL_VALUES = 5;
const int TOTAL_CATEGORIES = 5;

typedef map<string, int> CatMap;
typedef map<string, int> CharMap;

/*
* Represents a theoretical house. Zero or more characteristics, such as nationality, pet, or drink, are present.
* Two theoretical houses at the same address can sometimes be merged together, yielding a new combined house.
* This is possible if a) the two houses don't clash over any assigned characteristic and b) the characteristic
* being merged in isn't already at a different house.
* 
* Characteristics are described with strings, e.g. "beer", "green", or "Norwegian". Inside a House instance, 
* however, they are represented with integers, for fast comparison.
* 
*/
class House {
public:
    House();
    // Assigns a characteristic to this house, given category index (e.g. "color") and value index (e.g. "white")
    bool set_characteristic(int cat_idx, int val_idx);
    // Gets the characterisitic in given category as a string, or returns "*" if none assigned
    const string &get_characteristic(int cat_idx);
    // Returns true if the other can be merged. already_used_values is an array of sets containing the characteristics
    // already used in each category.
    bool can_merge(House* other, set<int> already_used_values[]);
    // Merges the contents of the other house into this one. The caller must first test that merge is possible.
    void merge(House* other);

    // Adds a characteristic and its category number to tracking, for easy lookup
    static void add_characteristic_and_category(const string& characteristic, int cat, int idx);
    // Given a characteristic (e.g. "horse"), returns its category index and index.
    static bool get_cat_and_idx_from_characteristic(const string& characteristic, int* ret_cat, int* ret_idx);
    static const string &get_characteristic_string(int cat_idx, int val_idx);
    
private:
    int values[TOTAL_CATEGORIES]; // contains the assigned characteristic in each category or -1, if none

private:
    static CatMap characteristic_to_category_map; // maps a characteristic to a category number
    static CharMap characteristic_to_idx_map; // maps a characteristic to a category number
    static string known_characteristics[TOTAL_CATEGORIES][TOTAL_HOUSES]; // lookup for string values of characteristics

};

