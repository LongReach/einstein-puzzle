#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "House.h"
#include "PuzzleMaker.h"

PuzzleMaker::PuzzleMaker() {
	srand((int) time(NULL));
	total_rules = 0;
	total_address_rules = 0;
}

void PuzzleMaker::make_model_street() {
	for (int cat = 0; cat < TOTAL_CATEGORIES; cat++) {
		for (int i = 0; i < TOTAL_HOUSES; i++) {
			string val = House::get_characteristic_string(cat, i);
			// We have a characteristic. Now, select a random house to put it in.
			int rand_addr = rand() % TOTAL_CATEGORIES;
			while (true) {
				// Find a house that doesn't already have characteristic from that category
				string current_val = model_street.get_characteristic(rand_addr, cat);
				if (current_val == "*") {
					model_street.set_characteristic(rand_addr, val);
					break;
				}
				rand_addr++;
				if (rand_addr >= TOTAL_CATEGORIES) rand_addr = 0;
			}
		}
	}

	model_street.print_info();
}

void PuzzleMaker::make_puzzle() {
	unsigned int best_solution_len = 25;
	vector<string> best_rules;
	vector<string> rules_list;
	// We do a bunch of iterations. After each one, we note the best sequence of rules found
	// so far. The next iteration uses the best sequences as a starting point, randomly varying
	// a few steps. Once we have a best sequence found so far, we never allow a longer sequence
	// than that.
	for (int i = 0; i < 150; i++) {
		rules_list.clear();
		populate_rules_cache(best_rules, 3);
		bool success = make_and_run_rules(rules_list, best_solution_len);
		string outcome_str = (success) ? to_string(rules_list.size()) : "";
		cout << "Attempt " << i << " " << outcome_str << endl;
		if (success && rules_list.size() < best_solution_len) {
			best_solution_len = rules_list.size();
			best_rules = rules_list;
		}
	}

	PuzzleSolver solver;
	solver.clear_steps();
	solver.add_steps(best_rules);
	while (solver.run_next_step());
	Street::print_street_list();

	vector<string> unused_vals;
	Street::get_unused_values(unused_vals);
	// Choose a random value to be the thing to be found
	string target = (unused_vals.size() == 0) ? "*" : unused_vals[rand() % unused_vals.size()];
	string last_rule = "single '" + target + "'";
	best_rules.push_back(last_rule);

	cout << "Best rules, size=" << best_rules.size() << endl;
	for (vector<string>::iterator it = best_rules.begin(); it != best_rules.end(); it++) {
		cout << *it << endl;
	}
}

bool PuzzleMaker::make_and_run_rules(vector<string> &ret_rules_list, int limiter) {
	total_rules = 0;
	total_address_rules = 0;

	PuzzleSolver solver;
	solver.clear_steps();
	reset_usage_count();

	// We keep adding rules and running them until we either have a valid rules sequence
	// or have gone too many steps.
	bool success = false;
	while (total_rules < limiter) {
		string new_rule = make_rule();
		solver.add_step(new_rule);
		ret_rules_list.push_back(new_rule);

		solver.run_next_step();
		//cout << "Streets count is: " << Street::get_possible_streets_count() << endl;

		if (Street::have_used_enough_values() && Street::get_possible_streets_count() == 1) {
			success = true;
			break;
		}
	}
	//Street::print_street_list();
	cout << "Done making puzzle! Total rules: " << total_rules << endl << endl;
#if 0
	model_street.print_info();
	cout << "Rules:" << endl;
	for (vector<string>::iterator it = rules_list.begin(); it != rules_list.end(); it++) {
		cout << *it << endl;
	}
#endif
	return success;
}

string PuzzleMaker::make_rule() {
	string rule;
	if (rules_cache.size() > 0) {
		rule = rules_cache.front();
		rules_cache.pop_front();
		total_rules++;

		int first_space_idx = rule.find(' ');
		string command = rule.substr(0, first_space_idx);
		if (command == "address") total_address_rules++;

		return rule;
	}

	int which_rule = 0;
	for (int n = 0; n < 15; n++) {
		// Neighbors rules occur 1/3 of the time
		// Address rules occur 1/5 of the time
		// Pair rules are the remainder
		int dice_roll = rand() % 30;
		which_rule = 0;
		if (dice_roll < 10) which_rule = 1;
		else if (dice_roll < 16) which_rule = 2;
		if (which_rule == 2 && total_address_rules > 3) {
			// we can only make so many address rules
			continue;
		}
		else {
			break;
		}
	}

	if (which_rule == 0) {
		// Make a pair rule
		// Note the logic about not using a particular value too much
		string val1, val2;
		for (int n = 0; n < 15; n++) {
			make_pairs_rule(&val1, &val2);
			if (can_use_value(val1) && can_use_value(val2)) break;
		}
		increase_usage(val1);
		increase_usage(val2);
		rule = rule + "pair '" + val1 + "' '" + val2 + "'";
	}
	else if (which_rule == 1) {
		// make a neighbors rule
		string val1, val2;
		int dir;
		for (int n = 0; n < 15; n++) {
			make_neighbors_rule(&val1, &val2, &dir);
			if (can_use_value(val1) && can_use_value(val2)) break;
		}
		increase_usage(val1);
		increase_usage(val2);
		string dir_str = (dir == 1) ? " 1" : "";
		rule = rule + "neighbor '" + val1 + "' '" + val2 + "'" + dir_str;
	}
	else if (which_rule == 2) {
		// make an address rule
		int addr;
		string val1;
		for (int n = 0; n < 15; n++) {
			make_address_rule(&addr, &val1);
			if (can_use_value(val1)) break;
		}
		increase_usage(val1);
		rule = rule + "address " + to_string(addr + 1) + " '" + val1 + "'";
		total_address_rules++;
	}
	//cout << "Rule is: " << rule << endl;
	total_rules++;
	return rule;
}

void PuzzleMaker::reset_usage_count() {
	usage_count.clear();
	for (int j = 0; j < TOTAL_CATEGORIES; j++) {
		for (int i = 0; i < TOTAL_CATEGORIES; i++) {
			string val = House::get_characteristic_string(j, i);
			usage_count[val] = 0;
		}
	}
}

bool PuzzleMaker::can_use_value(string& val) {
	return usage_count[val] < 2;
}

void PuzzleMaker::increase_usage(string& val) {
	usage_count[val] = usage_count[val] + 1;
}

void PuzzleMaker::make_pairs_rule(string* ret_char1, string* ret_char2) {
	// make a pairs rule
	int cat1 = rand() % TOTAL_CATEGORIES;
	int cat2 = -1;
	while (cat2 == -1) {
		cat2 = rand() % TOTAL_CATEGORIES;
		if (cat2 == cat1) cat2 = -1;
	}
	// choose a random house
	int rand_addr = rand() % TOTAL_HOUSES;
	*ret_char1 = model_street.get_characteristic(rand_addr, cat1);
	*ret_char2 = model_street.get_characteristic(rand_addr, cat2);
}

void PuzzleMaker::make_neighbors_rule(string* ret_char1, string* ret_char2, int* ret_dir) {
	// make a neighbors rule
	int cat1 = rand() % TOTAL_CATEGORIES;
	int cat2 = rand() % TOTAL_CATEGORIES;
	int shown_dir = rand() % 2;
	int actual_dir = 1;
	if (shown_dir == 0) {
		actual_dir = (rand() % 2) * 2 - 1;
	}
	int first_addr = (rand() % (TOTAL_HOUSES - 1)) + (actual_dir == -1) ? 1 : 0;
	int second_addr = first_addr + actual_dir;
	*ret_char1 = model_street.get_characteristic(first_addr, cat1);
	*ret_char2 = model_street.get_characteristic(second_addr, cat2);
	*ret_dir = shown_dir;
}

void PuzzleMaker::make_address_rule(int* ret_addr, string* ret_char) {
	int cat1 = rand() % TOTAL_CATEGORIES;
	int rand_addr = rand() % TOTAL_HOUSES;
	*ret_char = model_street.get_characteristic(rand_addr, cat1);
	*ret_addr = rand_addr;
}

void PuzzleMaker::populate_rules_cache(vector<string>& good_rules, int num_skip) {
	if (good_rules.size() == 0) {
		rules_cache.clear();
		return;
	}
	set<int> idxs_to_remove; // these indices from the vector won't go into cache
	for (int i = 0; i < num_skip; i++) {
		idxs_to_remove.insert(rand() % good_rules.size());
	}
	for (unsigned int i = 0; i < good_rules.size(); i++) {
		if (idxs_to_remove.find(i) == idxs_to_remove.end()) {
			rules_cache.push_back(good_rules[i]);
		}
	}
}

void PuzzleMaker::print_rules(vector<string>& good_rules, bool english_format) {

}

string PuzzleMaker::get_rule_in_english(string& rule) {
	return "";
}
