#pragma once
#include <deque>
#include "PuzzleSolver.h"
#include "House.h"

/*
* This class generates an Einstein Puzzle. A complete puzzle consists of a set of rules
* that, when processed and solved, yield a single solution.
* 
* The heuristic works by generating random rule sequences that are limited to some maximum
* number of steps. Many randomly-generated rule sequences just won't work. When the heuristic
* finds one that does, it saves a copy of that sequence and then repeatly iterates on it,
* making small random modifications in hope of arriving at more efficient solution. This is
* a little bit comparable to how genetic algorithms work and a little bit comparable to 
* gradient-descent in machine learning. 
*/
class PuzzleMaker {
public:
	PuzzleMaker();
	// Makes a randomly-populated street of houses. This is needed to generate the puzzle.
	void make_model_street();
	// The core function. Continuously tries different rule sets until a valid sequence of rules
	// is generated.
	void make_puzzle();

private:
	// We don't want to use any characteristic more than twice. The usage count dictionary tracks that.
	void reset_usage_count();
	// Returns true if a characteristic has been used no more than twice in rule set.
	bool can_use_value(string& val);
	void increase_usage(string& val);
	// Makes a rule sequence at semi-random. If the rule sequence is valid (one complete answer) and contains
	// no more than the number of steps specified by 'limiter', returns true. 'ret_rules_list' receives
	// the rule sequence generated.
	bool make_and_run_rules(vector<string>& ret_rules_list, int limiter);
	// Semi-randomly makes a new rule. If there are rules in the rules cache, use the one at the top of the
	// the queue. If not, randomly make one. This facilitates the reuse of successful rules.
	string make_rule();
	void make_pairs_rule(string* ret_char1, string* ret_char2);
	void make_neighbors_rule(string* ret_char1, string* ret_char2, int* ret_dir);
	void make_address_rule(int *ret_addr, string* ret_char);

	// Creates the rules cache as described for make_rule(). See comments in make_puzzle() definition.
	// 'num_skip' specifies how many rules in the sequence to randomly omit from being cached.
	void populate_rules_cache(vector<string>& good_rules, int num_skip);

	Street model_street;
	// Count of how often a particular characteristic has been used in generating latest sequence of rules.
	map<string, int> usage_count;
	int total_rules;
	int total_address_rules;

	deque<string> rules_cache;
};