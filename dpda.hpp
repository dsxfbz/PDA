#ifndef DPDA_HPP
#define DPDA_HPP
rule::rule() {
	m_start_state = '\0';
	m_symbol = '\0';
	m_stack_symbol = '\0';
	
	m_next_state = '\0';
	m_push = false;
	m_pop = false;
	m_next_stack_symbol = '\0';
	
	m_state = nullptr;
	m_curr_symbol = nullptr;
	m_stack = nullptr;
}
rule::rule(const char & start_state, const char & symbol, const char & stack_symbol, const char & next_state, const bool & pop, const bool & push, const char & new_stack_symbol, char * state, char * curr_symbol, std::stack<char> * stack) {
	m_start_state = start_state;
	m_symbol = symbol;
	m_stack_symbol = stack_symbol;
	
	m_next_state = next_state;
	m_push = push;
	m_pop = pop;
	m_next_stack_symbol = new_stack_symbol;
	
	set_target(state, curr_symbol, stack);
}
rule::rule(const rule & source) {
	m_start_state = source.m_start_state;
	m_symbol = source.m_symbol;
	m_stack_symbol = source.m_stack_symbol;
	
	m_next_state = source.m_next_state;
	m_push = source.m_push;
	m_pop = source.m_pop;
	m_next_stack_symbol = source.m_next_stack_symbol;
	
	set_target(source.m_state, source.m_curr_symbol, source.m_stack);
}
rule rule::operator = (const rule & source) {
	m_start_state = source.m_start_state;
	m_symbol = source.m_symbol;
	m_stack_symbol = source.m_stack_symbol;
	
	m_next_state = source.m_next_state;
	m_push = source.m_push;
	m_pop = source.m_pop;
	m_next_stack_symbol = source.m_next_stack_symbol;
	
	set_target(source.m_state, source.m_curr_symbol, source.m_stack);
	
	return *this;
}

void rule::set_target(char * state, char * symbol, std::stack<char> * stack) {
	m_state = state;
	m_curr_symbol = symbol;
	m_stack = stack;
}

bool rule::followed() {
	if ((m_start_state == '*') || (*m_state == m_start_state)) {
		if ((m_symbol == '*') || (*m_curr_symbol == m_symbol)) {
			if (m_stack_symbol != '*') {
				if (!m_stack->empty()) {
					if (m_stack->top() == m_stack_symbol) {
						*m_state = m_next_state;
						if (m_pop) {
							m_stack->pop();
						}
						if (m_push) {
							m_stack->push(m_next_stack_symbol);
						}
						return true;
					}
				}
			}
			else {
				*m_state = m_next_state;
				if (m_pop && !m_stack->empty()) {
					m_stack->pop();
				}
				if (m_push) {
					m_stack->push(m_next_stack_symbol);
				}
				return true;
			}
		}
	}
	return false;
}

void rule::print() {
	std::cout<<m_start_state<<m_symbol<<m_stack_symbol<<m_next_state<<m_pop<<m_push<<m_next_stack_symbol<<",";
}


dpda::dpda() {
	m_curr_state = '\0';
	m_symbol = '\0';
	m_stack = std::stack<char>();
	
	m_num_starts = 0;
	m_start_states = nullptr;
	
	m_num_rules = 0;
	m_rule = nullptr;
	
	m_num_ends = 0;
	m_end_states = nullptr;
}
dpda::dpda(const std::string & filename) {
	m_curr_state = '\0';
	m_symbol = '\0';
	m_stack = std::stack<char>();
	
	m_num_starts = 0;
	m_start_states = nullptr;
	
	m_num_rules = 0;
	m_rule = nullptr;
	
	m_num_ends = 0;
	m_end_states = nullptr;
	
	get_data(filename);
}
dpda::dpda(const dpda & source) {
	m_curr_state = source.m_curr_state;
	m_symbol = source.m_symbol;
	m_stack = source.m_stack;
	
	m_num_starts = source.m_num_starts;
	m_start_states = new char[m_num_starts];
	for (int i=0; i<m_num_starts; i++) {
		m_start_states[i] = source.m_start_states[i];
	}
	
	m_num_rules = source.m_num_rules;
	m_rule = new rule[m_num_rules];
	for (int i=0; i<m_num_rules; i++) {
		m_rule[i] = source.m_rule[i];
	}
	
	m_num_ends = 0;
	m_end_states = new char[m_num_ends];
	for (int i=0; i<m_num_ends; i++) {
		m_end_states[i] = source.m_end_states[i];
	}
}

dpda dpda::operator = (const dpda & source) {
	m_curr_state = source.m_curr_state;
	m_symbol = source.m_symbol;
	m_stack = source.m_stack;
	
	m_num_starts = source.m_num_starts;
	m_start_states = new char[m_num_starts];
	for (int i=0; i<m_num_starts; i++) {
		m_start_states[i] = source.m_start_states[i];
	}
	
	m_num_rules = source.m_num_rules;
	m_rule = new rule[m_num_rules];
	for (int i=0; i<m_num_rules; i++) {
		m_rule[i] = source.m_rule[i];
	}
	
	m_num_ends = 0;
	m_end_states = new char[m_num_ends];
	for (int i=0; i<m_num_ends; i++) {
		m_end_states[i] = source.m_end_states[i];
	}
	
	return *this;
}

dpda::~dpda() {
	delete[] m_start_states;
	delete[] m_rule;
	delete[] m_end_states;
}

void dpda::get_data(const std::string & filename) {
	std::string curr_line;
	
	std::ifstream data;
	data.open(filename, std::ios::in);
	
	while (std::getline(data, curr_line)) {
		switch (int(curr_line[0])) {
			case int('S'):
				parse_starts(curr_line+"\0");
				break;
			case int('R'):
				parse_rules(curr_line+"\0");
				break;
			case int('E'):
				parse_ends(curr_line+"\0");
				break;
			default:
				std::cout<<"ERROR: Invalid line \""<<curr_line<<"\"\n";
		}
	}
	
	data.close();
}

bool dpda::process(const std::string & input) {
	m_curr_state = m_start_states[0];
	
	for (int i=0; i<input.size(); i++) {
		step(input[i]);
	}
	
	if (valid()) {
		return true;
	}
	return false;
}

void dpda::print() {
	std::cout<<"S ";
	for (int i=0; i<m_num_starts; i++) {
		std::cout<<m_start_states[i];
	}
	std::cout<<"\n";
	
	std::cout<<"E ";
	for (int i=0; i<m_num_ends; i++) {
		std::cout<<m_end_states[i];
	}
	std::cout<<"\n";
	
	std::cout<<"R ";
	for (int i=0; i<m_num_rules; i++) {
		//std::cout<<m_rule_starts[i];
		//std::cout<<m_rule_symbols[i];
		//std::cout<<m_rule_ends[i];
		m_rule[i].print();
		//std::cout<<",";
	}
	std::cout<<"\n";
}


bool dpda::step(const char & symbol) {
	rule * curr = &m_rule[0];
	m_symbol = symbol;
	
	for (int i=0; i<m_num_rules; i++) {
		//if (curr->followed(m_curr_state, symbol)) {///////////////
		if (curr->followed()) {
			return true;
		}
		curr++;
	}
	return false;
}

bool dpda::valid() {
	return in_arr(m_curr_state, m_end_states, m_num_ends);
}

void dpda::parse_starts(const std::string & line) {
	delete[] m_start_states;
	m_num_starts = 0;
	
	for (int i=2; line[i]!='\0'; i++)
		m_num_starts++;
	m_start_states = new char[m_num_starts];
	
	for (int i=0; i<m_num_starts; i++)
		m_start_states[i] = line[i+2];
	
	return;
}
void dpda::parse_rules(const std::string & line) {
	delete[] m_rule;
	m_num_rules = 0;
	char start;
	char symb;
	char stack_symb;
	char next;
	bool pop;
	bool push;
	char next_stack;
	
	
	for (int i=2; line[i]!='\0'; i++)
		if (line[i] == ',')
			m_num_rules++;
	m_rule = new rule[m_num_rules];
	
	int indx = 0;
	for (int i=2; line[i]!='\0'; i++) {
		if (line[i]==',') {//Add rule at end
			m_rule[indx] = rule(start, symb, stack_symb, next, pop, push, next_stack, &m_curr_state, &m_symbol, &m_stack);
			indx++;
		}
		else {
			//std::cout<<"  line["<<i<<"]->"<<(i-2-indx)%3<<": "<<line[i]<<"\n";
			switch ((i-2-indx)%7) {
				//Start state
				case 0:
					start = line[i];
					break;
				//Symbol
				case 1:
					symb = line[i];
					break;
				//Stack Symbol
				case 2:
					stack_symb = line[i];
					break;
				//Next state
				case 3:
					next = line[i];
					break;
				//Pop stack
				case 4:
					pop = (line[i] == '1');
					break;
				//Push stack
				case 5:
					push = (line[i] == '1');
					break;
				//New stack symbol
				case 6:
					next_stack = line[i];
					break;
			}
		}
	}
	
	return;
}
void dpda::parse_ends(const std::string & line) {
	delete[] m_end_states;
	m_num_ends = 0;
	
	for (int i=2; line[i]!='\0'; i++)
		m_num_ends++;
	m_end_states = new char[m_num_ends];
	
	for (int i=0; i<m_num_ends; i++)
		m_end_states[i] = line[i+2];
	
	return;
}

#endif
