#pragma once
#include <set>
#include <string>

using namespace std;

const int TOTAL_HOUSES = 5;
const int TOTAL_CATEGORIES = 6;

typedef map<string, int> CatMap;
class House;
typedef map<string, House*> HouseMap;

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
    bool set_characteristic(string& the_char);
    int get_address();
    bool match_address(int address);
    bool can_merge(House* other);
    // Merges two houses together. The other's contents are copied into this one, other is marked as no longer valid.
    void merge(House* other);
    // Returns true if this House object has been merged into another. We keep the objects around so stale pointers
    // can be updated
    bool is_defunct(); 
    House *resolve_pointer();
    int get_id();
    void print_info();

    static House* get_house(string& characteristic);
    // Returns the house object that contains the specific characteristic. If none found, make new House object.
    static House* make_or_get_house(string& characteristic);
    // Adds a characteristic and its category number to tracking, for easy lookup
    static void add_characteristic_and_category(string& characteristic, int cat);
    static bool at_last_characteristic(int cat);
    static string get_last_characteristic(int cat);
    static void print_unused_characteristics();
    
private:
    string values[TOTAL_CATEGORIES]; // contains the assigned characteristic in each category or "*", if none
    House* merged_into; // pointer to the House object that this one was merged into
    int id; // For debugging

public:
    static vector<House*> houses; // All House instances that are created are tracked here
private:
    static HouseMap characteristic_to_house_map; // maps a characteristic to a House object
    static CatMap characteristic_to_category_map; // maps a characteristic to a category number
    static set<string> unused_characteristics[TOTAL_CATEGORIES];
    static int id_counter; // For debugging purposes

};

class NeighborData {
public:
    NeighborData(string &char1, string &char2, int d);
    bool attempt_resolve();
    void print_info();

    static vector<NeighborData*> pairs;

    string char_name[2];
    int dir; // 0 or 1
};