#ifndef DFSA_HPP
#define DFSA_HPP
rule::rule() {
	m_start_state = '\0';
	m_symbol = '\0';
	m_next_state = '\0';
}
rule::rule(const char & start_state, const char & symbol, const char & next_state) {
	m_start_state = start_state;
	m_symbol = symbol;
	m_next_state = next_state;
}
rule::rule(const rule & source) {
	m_start_state = source.m_start_state;
	m_symbol = source.m_symbol;
	m_next_state = source.m_next_state;
}
rule rule::operator = (const rule & source) {
	m_start_state = source.m_start_state;
	m_symbol = source.m_symbol;
	m_next_state = source.m_next_state;
	
	return *this;
}

bool rule::followed(char & state, const char & symbol) {
	if (state == m_start_state) {
		if (symbol == m_symbol) {
			state = m_next_state;
			return true;
		}
	}
	return false;
}

void rule::print() {
	std::cout<<m_start_state<<m_symbol<<m_next_state<<",";
}


dfsa::dfsa() {
	m_curr_state = ' ';
	
	m_num_starts = 0;
	m_start_states = nullptr;
	
	m_num_rules = 0;
	m_rule = nullptr;
	
	m_num_ends = 0;
	m_end_states = nullptr;
}
dfsa::dfsa(const std::string & filename) {
	m_curr_state = ' ';
	
	m_num_starts = 0;
	m_start_states = nullptr;
	
	m_num_rules = 0;
	m_rule = nullptr;
	
	m_num_ends = 0;
	m_end_states = nullptr;
	
	get_data(filename);
}
dfsa::dfsa(const dfsa & source) {
	m_curr_state = source.m_curr_state;
	
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

dfsa dfsa::operator = (const dfsa & source) {
	m_curr_state = source.m_curr_state;
	
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

dfsa::~dfsa() {
	delete[] m_start_states;
	delete[] m_rule;
	delete[] m_end_states;
}

void dfsa::get_data(const std::string & filename) {
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

bool dfsa::process(const std::string & input) {
	m_curr_state = m_start_states[0];
	
	for (int i=0; i<input.size(); i++) {
		step(input[i]);
	}
	
	if (valid()) {
		return true;
	}
	return false;
}

void dfsa::print() {
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


bool dfsa::step(const char & symbol) {
	rule * curr = &m_rule[0];
	
	for (int i=0; i<m_num_rules; i++) {
		if (curr->followed(m_curr_state, symbol)) {///////////////
			return true;
		}
		curr++;
	}
	return false;
}

bool dfsa::valid() {
	return in_arr(m_curr_state, m_end_states, m_num_ends);
}

void dfsa::parse_starts(const std::string & line) {
	delete[] m_start_states;
	m_num_starts = 0;
	
	for (int i=2; line[i]!='\0'; i++)
		m_num_starts++;
	m_start_states = new char[m_num_starts];
	
	for (int i=0; i<m_num_starts; i++)
		m_start_states[i] = line[i+2];
	
	return;
}
void dfsa::parse_rules(const std::string & line) {
	delete[] m_rule;
	m_num_rules = 0;
	char start;
	char symb;
	char next;
	
	
	for (int i=2; line[i]!='\0'; i++)
		if (line[i] == ',')
			m_num_rules++;
	m_rule = new rule[m_num_rules];
	
	int indx = 0;
	for (int i=2; line[i]!='\0'; i++) {
		if (line[i]==',') {//Add rule at end
			m_rule[indx] = rule(start, symb, next);
			indx++;
		}
		else {
			//std::cout<<"  line["<<i<<"]->"<<(i-2-indx)%3<<": "<<line[i]<<"\n";
			switch ((i-2-indx)%3) {
				//Start state
				case 0:
					start = line[i];
					break;
				//Symbol
				case 1:
					symb = line[i];
					break;
				//End state
				case 2:
					next = line[i];
					break;
			}
		}
	}
	
	return;
}
void dfsa::parse_ends(const std::string & line) {
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
