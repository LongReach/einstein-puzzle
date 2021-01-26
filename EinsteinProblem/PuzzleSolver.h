#pragma once
#include <vector>
#include "House.h"

/*
* This class solves the Einstein Puzzle by processing rules one at a time until the rules are exhausted.
* It uses the Street class.
* 
* A rule (e.g. "The owner of the green house drinks coffee.") is represented with a string in the form:
* COMMAND [ADDR] [CHARACTERISTIC1] [CHARACTERISTIC2] [DIR]
* Valid commands are: "pair", "neighbor", "address", "single", or "done"
* addr: 1 - 5
* characteristic: 'Characteristic Name'
* dir: 0 - 1
* 
* Thus: "pair 'green' 'coffee'
* or: "neighbor 'green' 'white' 1"
* 
*/
class PuzzleSolver {
public:
	PuzzleSolver();

	// Resets the puzzle to step 0. Any rules in memory remain there.
	void reset();
	// Resets the puzzle and clears rules from memory.
	void clear_steps();
	// Puts an entire sequence of rules into memory. 
	// Each rule is a string in the form of: "COMMAND [ADDR] [CHARACTERISTIC1] [CHARACTERISTIC2] [DIR]"
	// Valid commands are: "pair", "neighbor", "address", "single", or "done"
	void add_steps(string steps[]);
	void add_steps(vector<string>& steps);
	void add_step(string& rule);
	// Runs the next step that hasn't been run yet. Returns false if no more steps left to run.
	bool run_next_step();

	// Returns false if parse failed
	static bool parse_rule(string &rule, string &ret_command, string& ret_char1, string& ret_char2, int& ret_num);

private:
	// Parse and execute a rule.
	void execute_rule(string& rule);
	// Functions for executing rule of particular type.
	void do_pairs_rule(string& char1, string& char2);
	void do_neighbors_rule(string& char1, string& char2, int dir);
	void do_address_rule(int address, string& the_char);
	void do_single_rule(string& the_char);

	vector<string> rules;
	int current_step;
	int total_steps;
};