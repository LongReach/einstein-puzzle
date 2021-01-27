#pragma once
#include "House.h"

/*
* A Street represents five houses in order from left to right. No characteristic is present more than once on a
* street. Some characteristics might not be assigned yet.
*/
class Street;
typedef vector<Street*> StreetList;

class Street {
public:
    Street();
    // Assigns a characteristic to house at the given address.
    void set_characteristic(int addr, const string& characteristic);
    const string& get_characteristic(int addr, int cat_idx);
    // Combines this street with the other, if possible, returns new Street object. If not possible, returns NULL.
    Street* combine(Street* other_street, set<int> values_present[]);
    // Prints out pretty-formatted info about contents of houses on this street
    void print_info();

private:
    House houses_[TOTAL_HOUSES];
};

/*
* A StreetGroup is a collection of possible streets, ones that haven't yet been discarded as possible solutions to
* the puzzle. As new characteristics are tried in different houses, some possible streets will be discarded. Obviously,
* there are combinations that don't work, such as two different people in the same house.
* 
* The class keeps track of characteristics that have already been assigned, so they won't be used again.
*/
class StreetGroup {
public:
    StreetGroup();
    void reset();

    int get_possible_streets_count();
    // Given a list of streets, delete all the Street objects
    void erase_street_list(StreetList& the_list);
    // Given a list of streets, print them all.
    // If no list given, use the static possible_streets list. If quiet = true, print less info.
    void print_street_list(StreetList* the_list = NULL, bool quiet = false);
    bool completable_streets_exist();
    // If 4/5 characteristics in a category have been assigned, the 5th one can be autofilled. The last autofilled
    // value is the one that comes from the last category to be autofilled. It is the characteristic most ideal
    // to have the puzzle solver try to find, because it remains ambiguous the longest.
    string get_last_autofill_value();
    // Given a list of new streets, each containing a different proposal of for what to add to the various houses,
    // attempt to combine the proposals with existing list of possible streets, generating a new list of possible
    // streets. This is the heart of the whole program.
    //
    // char1 and char2 are characteristics to mark as "already used", *after* the new set of possible streets has
    // been made.
    void make_combos(StreetList& new_streets, const string& char1, const string& char2);
    // Generate a set of proposals, given two characteristics located togther, then combine them with existing
    // possible streets. Basically, we try each characteristic pair in each of the five houses.
    void add_new_characteristics(const string& char1, const string& char2);
    // Given two characteristics that are neighbors, generate a set of proposals and combine them with existing
    // possible streets. If dir == 1, the second neighbor is to the right of the first. If dir == 0, the left-right
    // order isn't known.
    // For dir == 1, there will be four proposals. For dir == 0, there will be eight.
    void add_neighbor_pair(const string& char1, const string& char2, int dir);
    // Generates a single proposed house at given address and combines it with existing possible streets.
    void add_address(int address, const string& the_char);

private:

    StreetList possible_streets_; // all the possible streets that are known at this stage of the game
    set<int> values_present_[TOTAL_CATEGORIES]; // characteristics that have been applied by this point in game
    int last_autofill_value_;
    int last_autofill_cat_;
};