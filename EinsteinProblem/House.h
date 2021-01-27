#pragma once
#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

const int TOTAL_HOUSES = 5;
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

/*
* A Street represents five houses in order from left to right. No characteristic is present more than once on a
* street. The class maintains a static list of all the possible valid streets it knows so far. As new relationship
* information is added, such as neighbor relationships or characteristics colocated at a single house, combinations
* that don't work (such as two different nationalities in one house) are discarded.
* 
* The class keeps track of characteristics that have already been assigned for quick lookup, so they won't be used
* again.
*/
class Street;
typedef vector<Street*> StreetList;

class Street {
public:
    Street();
    // Assigns a characteristic to house at the given address.
    void set_characteristic(int addr, const string& characteristic);
    const string &get_characteristic(int addr, int cat_idx);
    // Combines this street with the other, if possible, returns new Street object. If not possible, returns NULL.
    Street* combine(Street* other_street);
    // Prints out pretty-formatted info about contents of houses on this street
    void print_info();

    static void reset();

    static int get_possible_streets_count();
    // Given a list of streets, delete all the Street objects
    static void erase_street_list(StreetList& the_list);
    // Given a list of streets, print them all.
    // If no list given, use the static possible_streets list. If quiet = true, print less info.
    static void print_street_list(StreetList* the_list = NULL, bool quiet = false);
    static bool completable_streets_exist();
    // If 4/5 characteristics in a category have been assigned, the 5th one can be autofilled. The last autofilled
    // value is the one that comes from the last category to be autofilled. It is the characteristic most ideal
    // to have the puzzle solver try to find, because it remains ambiguous the longest.
    static string get_last_autofill_value();
    // Given a list of new streets, each containing a different proposal of for what to add to the various houses,
    // attempt to combine the proposals with existing list of possible streets, generating a new list of possible
    // streets. This is the heart of the whole program.
    //
    // char1 and char2 are characteristics to mark as "already used", *after* the new set of possible streets has
    // been made.
    static void make_combos(StreetList &new_streets, const string& char1, const string& char2);
    // Generate a set of proposals, given two characteristics located togther, then combine them with existing
    // possible streets. Basically, we try each characteristic pair in each of the five houses.
    static void add_new_characteristics(const string& char1, const string& char2);
    // Given two characteristics that are neighbors, generate a set of proposals and combine them with existing
    // possible streets. If dir == 1, the second neighbor is to the right of the first. If dir == 0, the left-right
    // order isn't known.
    // For dir == 1, there will be four proposals. For dir == 0, there will be eight.
    static void add_neighbor_pair(const string& char1, const string& char2, int dir);
    // Generates a single proposed house at given address and combines it with existing possible streets.
    static void add_address(int address, const string& the_char);

private:
    House houses[TOTAL_HOUSES];

    static StreetList possible_streets; // all the possible streets that are known at this stage of the game
    static set<int> values_present[TOTAL_CATEGORIES]; // characteristics that have been applied by this point in game
    static int last_autofill_value;
    static int last_autofill_cat;
};