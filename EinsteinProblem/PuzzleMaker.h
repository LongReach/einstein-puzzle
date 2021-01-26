#pragma once
#include <deque>
#include "PuzzleSolver.h"
#include "House.h"

class PuzzleMaker {
public:
	PuzzleMaker();
	void make_model_street();
	void make_puzzle();

private:
	void reset_usage_count();
	bool can_use_value(string& val);
	void increase_usage(string& val);
	bool make_and_run_rules(vector<string>& ret_rules_list, int limiter);
	string make_rule();
	void make_pairs_rule(string* ret_char1, string* ret_char2);
	void make_neighbors_rule(string* ret_char1, string* ret_char2, int* ret_dir);
	void make_address_rule(int *ret_addr, string* ret_char);

	void populate_rules_cache(vector<string>& good_rules, int num_skip);

	Street model_street;
	map<string, int> usage_count;
	int total_rules;
	int total_address_rules;

	deque<string> rules_cache;
};