#include <cassert>
#include <regex>
#include <stdlib.h>
#include <iostream>
#include "Street.h"
#include "PuzzleSolver.h"

using namespace std;

PuzzleSolver::PuzzleSolver() {
	current_step = 0;
	total_steps = 0;
	verbose = false;
}

void PuzzleSolver::reset() {
	current_step = 0;
	Street::reset();
}

void PuzzleSolver::clear_steps() {
	reset();
	rules.clear();
}

void PuzzleSolver::add_steps(string steps[]) {
	for (int i = 0;; i++) {
		if (steps[i].find("done") != string::npos) {
			total_steps = i;
			break;
		}
		rules.push_back(steps[i]);
	}
}

void PuzzleSolver::add_steps(vector<string>& steps) {
	for (vector<string>::iterator it = steps.begin(); it != steps.end(); it++) {
		add_step(*it);
	}
}

void PuzzleSolver::add_step(string &rule) {
	rules.push_back(rule);
	total_steps++;
}

bool PuzzleSolver::run_next_step() {
	if (current_step >= total_steps) {
		return false;
	}
	string rule = rules[current_step];
	if (verbose) {
		cout << "Executing rule: " << rule << endl;
	}
	execute_rule(rule);
	if (verbose) {
		cout << "Existing combos: " << Street::get_possible_streets_count()
			<< ", completable streets: " << Street::completable_streets_exist()
			<< endl;
		if (Street::get_possible_streets_count() == 1 && Street::completable_streets_exist()) {
			Street::print_street_list();
		}
	}
	current_step++;
	return true;
}

bool PuzzleSolver::parse_rule(const string &rule, string& ret_command, string& ret_char1, string& ret_char2, int& ret_num) {
	// Some info about regex:
	// https://www.softwaretestinghelp.com/regex-in-cpp/
	// https://www.geeksforgeeks.org/smatch-regex-regular-expressions-in-c/
	// https://cs.lmu.edu/~ray/notes/regex/

	int first_space_idx = rule.find(' ');
	assert(first_space_idx != string::npos);
	string command = rule.substr(0, first_space_idx);
	ret_command = command;
	string parms = rule.substr(first_space_idx);
	if (command == "pair") {
		regex re(" *'([a-zA-Z ]+)' '([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3);
			ret_char1 = match.str(1);
			ret_char2 = match.str(2);
			return true;
		}
		else {
			cout << "No match with: " << rule << endl;
			return false;
		}
	}
	else if (command == "neighbor") {
		regex re(" *'([a-zA-Z ]+)' '([a-zA-Z ]+)' *([0-1]*)");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3 || match.size() == 4);
			ret_char1 = match.str(1);
			ret_char2 = match.str(2);
			ret_num = 0;
			if (match.size() == 4) {
				ret_num = atoi(match.str(3).c_str());
			}
			return true;
		}
		else {
			cout << "No match with: " << rule << endl;
			return false;
		}
	}
	else if (command == "address") {
		regex re(" *([1-5]*) '([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3);
			ret_num = atoi(match.str(1).c_str());
			ret_char1 = match.str(2);
			return true;
		}
		else {
			cout << "No match with: " << rule << endl;
			return false;
		}
	}
	else if (command == "single") {
		regex re(" *'([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 2);
			ret_char1 = match.str(1);
			return true;
		}
		else {
			cout << "No match with: " << rule << endl;
			return false;
		}
	}
	return false;
}

void PuzzleSolver::execute_rule(const string& rule) {

	string command;
	string char1, char2;
	int num;
	bool success_parse = parse_rule(rule, command, char1, char2, num);
	assert(success_parse);

	if (command == "pair") {
		do_pairs_rule(char1, char2);
	}
	else if (command == "neighbor") {
		do_neighbors_rule(char1, char2, num);
	}
	else if (command == "address") {
		do_address_rule(num, char1);
	}
	else if (command == "single") {
		do_single_rule(char1);
	}
	else {
		assert(false);
	}
}

void PuzzleSolver::do_pairs_rule(const string& char1, const string& char2) {
	Street::add_new_characteristics(char1, char2);
}

void PuzzleSolver::do_neighbors_rule(const string& char1, const string& char2, int dir) {
	Street::add_neighbor_pair(char1, char2, dir);
}

void PuzzleSolver::do_address_rule(int address, const string& the_char) {
	Street::add_address(address, the_char);
}

void PuzzleSolver::do_single_rule(const string& the_char) {
	Street::add_new_characteristics(the_char, the_char);
}
