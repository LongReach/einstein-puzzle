#pragma once
#include <string>

using namespace std;

const int TOTAL_HOUSES = 5;
const int TOTAL_CATEGORIES = 6;

typedef map<string, int> CatMap;
class House;
typedef map<string, House*> HouseMap;

class House {
public:
    House();
    bool set_characteristic(string& the_char);
    void merge(House* other);
    bool get_remove_flag();
    void print_info();

    static House* make_or_get_house(string& characteristic);
    static void add_characteristic_and_category(string& characteristic, int cat);
    
    static vector<House*> houses;
    static HouseMap characteristic_to_house_map;
    static CatMap characteristic_to_category_map;

private:
    string values[TOTAL_CATEGORIES];
    bool remove_flag;
};

