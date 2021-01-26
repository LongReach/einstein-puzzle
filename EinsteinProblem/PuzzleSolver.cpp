#include <cassert>
#include <regex>
#include <stdlib.h>
#include <iostream>
#include "PuzzleSolver.h"

using namespace std;

PuzzleSolver::PuzzleSolver() {
	current_step = 0;
	total_steps = 0;
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

void PuzzleSolver::add_step(string &rule) {
	rules.push_back(rule);
	total_steps++;
}

bool PuzzleSolver::run_next_step() {
	if (current_step >= total_steps) {
		return false;
	}
	string rule = rules[current_step];
	parse_rule(rule);
	current_step++;
	return true;
}

void PuzzleSolver::parse_rule(string& rule) {

	// Some info about regex:
	// https://www.softwaretestinghelp.com/regex-in-cpp/
	// https://www.geeksforgeeks.org/smatch-regex-regular-expressions-in-c/
	// https://cs.lmu.edu/~ray/notes/regex/

	int first_space_idx = rule.find(' ');
	assert(first_space_idx != string::npos);
	string command = rule.substr(0, first_space_idx);
	string parms = rule.substr(first_space_idx);
	if (command == "pair") {
		regex re(" *'([a-zA-Z ]+)' '([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3);
			string char1 = match.str(1);
			string char2 = match.str(2);
			//cout << "Connecting " << char1 << " to " << char2 << endl;
			do_pairs_rule(char1, char2);
		}
		else {
			cout << "No match with: " << rule << endl;
			assert(false);
		}
	}
	else if (command == "neighbor") {
		regex re(" *'([a-zA-Z ]+)' '([a-zA-Z ]+)' *([0-1]*)");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3 || match.size() == 4);
			string char1 = match.str(1);
			string char2 = match.str(2);
			int dir = 0;
			if (match.size() == 4) {
				dir = atoi(match.str(3).c_str());
			}
			do_neighbors_rule(char1, char2, dir);
			//cout << "Neighbors " << char1 << " to " << char2 << " " << dir << endl;
		}
		else {
			cout << "No match with: " << rule << endl;
			assert(false);
		}
	}
	else if (command == "address") {
		regex re(" *([1-5]*) '([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 3);
			int addr = atoi(match.str(1).c_str());
			string char1 = match.str(2);
			//cout << "Address of " << char1 << " to " << addr << endl;
			do_address_rule(addr, char1);
		}
		else {
			cout << "No match with: " << rule << endl;
			assert(false);
		}
	}
	else if (command == "single") {
		regex re(" *'([a-zA-Z ]+)'");
		smatch match;
		if (regex_search(parms, match, re) == true) {
			assert(match.size() == 2);
			string char1 = match.str(1);
			//cout << "Single " << char1 << endl;
			do_single_rule(char1);
		}
		else {
			cout << "No match with: " << rule << endl;
			assert(false);
		}
	}
	else {
		assert(false);
	}
}

void PuzzleSolver::do_pairs_rule(string& char1, string& char2) {
	Street::add_new_characteristics(char1, char2);
}

void PuzzleSolver::do_neighbors_rule(string& char1, string& char2, int dir) {
	Street::add_neighbor_pair(char1, char2, dir);
}

void PuzzleSolver::do_address_rule(int address, string& the_char) {
	Street::add_address(address, the_char);
}

void PuzzleSolver::do_single_rule(string& the_char) {
	Street::add_new_characteristics(the_char, the_char);
}
