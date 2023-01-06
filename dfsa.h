#ifndef DFSA_H
#define DFSA_H

#include <iostream>
#include <fstream>
#include <string>
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
		rule(const char & start_state, const char & symbol, const char & next_state);
		rule(const rule & source);
		rule operator = (const rule & source);
		
		//Returns True if this rule is followed, False otherwise. If and only if this rule is followed, state is set to m_next_state.
		bool followed(char & state, const char & symbol);
		
		void print();
	private:
		char m_start_state;
		char m_symbol;
		char m_next_state;
};

class dfsa {
	public:
		dfsa();
		dfsa(const std::string & filename);
		dfsa(const dfsa & source);
		
		dfsa operator = (const dfsa & source);
		
		~dfsa();
		
		void get_data(const std::string & filename);
		
		bool process(const std::string & input);
		
		void download(const std::string & filename);
		
		void print();
		
	private:
		char m_curr_state;
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

#include "dfsa.hpp"

#endif
