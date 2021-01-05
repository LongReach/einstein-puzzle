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
*/
class House {
public:
    House();
    // Assigns a characteristic to this house
    bool set_characteristic(string& the_char);
    // Sets the numerical address, 1 to 5, for this house
    void set_address(int address);
    // Gets the numerical address
    int get_address();
    // Merges two houses together. The other's contents are copied into this one, other is marked as no longer valid.
    void merge(House* other);
    // Tells this house its neighbor. dir = -1 if neighbor is to left, 1 if to right, 0 if side is not known
    void set_neighbor(House* other, int dir);
    // Returns true if this House object has been merged into another. We keep the objects around so stale pointers
    // can be updated
    bool is_defunct(); 
    void update_stale_neighbor_pointers();
    void print_info();

    // Returns the house object that contains the specific characteristic. If none found, make new House object.
    static House* make_or_get_house(string& characteristic);
    // Adds a characteristic and its category number to tracking, for easy lookup
    static void add_characteristic_and_category(string& characteristic, int cat);
    
    static vector<House*> houses; // All House instances that are created are tracked here
    static HouseMap characteristic_to_house_map; // maps a characteristic to a House object
    static CatMap characteristic_to_category_map; // maps a characteristic to a category number
    static int id_counter; // For debugging purposes

private:
    string values[TOTAL_CATEGORIES]; // contains the assigned characteristic in each category or "*", if none
    House* merged_into; // pointer to the House object that this one was merged into
    // Left neighbor and right neighbor, respectively (left = lower address). NULL if not set.
    // Note that House pointers can get out of data if two houses are merged, but that's what the merged_into
    // pointer helps with.
    House* ordered_neighbors[2]; 
    // Holds neighbors for which we don't know what side they're on. As soon as that can be resolved, they're
    // moved into ordered_neigbors
    set<House *> unordered_neighbors;
    int id; // For debugging
};

