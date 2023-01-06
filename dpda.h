#ifndef DPDA_H
#define DPDA_H

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
//#include <deque>

//U must be a pointer to an array of type T, and len must be the array's size.
template <class T, class U>
bool in_arr(const T & elem, const U & arr, const int & len) {
	U ptr = arr;
	for (int i=0; i<len; i++) {
		if (*ptr == elem) {
			return true;
		}
		ptr++;
	}
	return false;
}

class rule {
	public:
		rule();
		rule(const char & start_state, const char & symbol, const char & stack_symbol, const char & next_state, const bool & pop, const bool & push, const char & new_stack_symbol, char * state, char * curr_symbol, std::stack<char> * stack);
		rule(const rule & source);
		rule operator = (const rule & source);
		
		//Sets rule to read data from the passed state, symbol, and stack
		void set_target(char * state, char * symbol, std::stack<char> * stack);
		
		//Returns True if this rule is followed, False otherwise. If and only if this rule is followed, state is set to m_next_state.
		bool followed();
		
		void print();
	private:
		char m_start_state;
		char m_symbol;
		char m_stack_symbol;
		
		char m_next_state;
		bool m_push;
		bool m_pop;
		char m_next_stack_symbol;
		
		char * m_state;
		char * m_curr_symbol;
		std::stack<char> * m_stack;
};

class dpda {
	public:
		dpda();
		dpda(const std::string & filename);
		dpda(const dpda & source);
		
		dpda operator = (const dpda & source);
		
		~dpda();
		
		void get_data(const std::string & filename);
		
		bool process(const std::string & input);
		
		void download(const std::string & filename);
		
		void print();
		
	private:
		char m_curr_state;
		char m_symbol;
		std::stack<char> m_stack;
		int m_num_starts;
		char * m_start_states;
		int m_num_rules;
		rule * m_rule;
		int m_num_ends;
		char * m_end_states;
		
		bool step(const char & symbol);
		
		bool valid();
		
		void parse_starts(const std::string & line);
		void parse_rules(const std::string & line);
		void parse_ends(const std::string & line);
};

#include "dpda.hpp"

#endif
