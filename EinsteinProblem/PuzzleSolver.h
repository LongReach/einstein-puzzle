#pragma once
#include <vector>
#include "House.h"

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
	// Runs the next step that hasn't been run yet. Returns false if no more steps left to run.
	bool run_next_step();

private:
	void parse_rule(string& rule);
	void do_pairs_rule(string& char1, string& char2);
	void do_neighbors_rule(string& char1, string& char2, int dir);
	void do_address_rule(int address, string& the_char);
	void do_single_rule(string& the_char);

	vector<string> rules;
	int current_step;
	int total_steps;
};