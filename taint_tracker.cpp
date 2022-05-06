#include <bits/stdc++.h>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
/*
	Global Variables for storing taint addresses and registers 
	Used hash maps to map addresses
*/

std::unordered_set<int> TAINTED_ADDRESS;
std::unordered_set<std::string>	TAINTED_REGISTERS, SOURCES, SINKS;
std::map<std::string, std::vector<std::string> > BASIC_BLOCKS;
std::stack<int> STACK;

void read_sources_sinks() {
	std::ifstream sources("sources.txt");
	std::ifstream sinks("sinks.txt");
	std::string token;
	while (sources >> token) {
		SOURCES.insert(token);
	}
	while (sinks >> token) {
		SINKS.insert(token);
	}
}

void generate_raw_assembly() {
	// Creates raw_asm.txt
	system("objdump --disassemble ./sample > raw_asm.txt");
}

std::string remove_comments(std::string line) {
	std::string ret;
	for (char ch : line) {
		if (ch == '#')
			break;
		ret.push_back(ch);
	}
	return ret;
}

bool is_basic_block(std::string line) {
	std::reverse(line.begin(), line.end());
	std::stringstream parse_line(line);
	std::string token;
	parse_line >> token;
	if (token.substr(0, 2) == ":>") {
		return true;
	}
	return false;
}

std::string extract_block_name(std::string line) {
	std::stringstream parser(line);
	std::string ret;
	parser >> ret >> ret;
	ret.pop_back();
	return ret;
}

std::string get_instructions(std::string line) {
	std::reverse(line.begin(), line.end());
	std::string ret;
	std::stringstream parser(line);
	std::string token;
	while (parser >> token) {
		if (token.size() <= 2 && token != "ej") {
			break;
		}
		std::reverse(token.begin(), token.end());
		ret = token + " " + ret;
	}
	return ret;
}

void clean_raw_asm() {
	std::ifstream raw_asm("raw_asm.txt");
	std::string line;
	std::string cur_basic_block = "@";
	while (getline(raw_asm, line)) {
		line = remove_comments(line);
		if (is_basic_block(line)) {
			cur_basic_block = extract_block_name(line);
		} else {
			std::string instructions = get_instructions(line);
			BASIC_BLOCKS[cur_basic_block].push_back(instructions);
		}
	}

	// for(auto it : BASIC_BLOCKS) {
	// 	// Checked ok
	// 	if (SOURCES.count(it.first)) {
	// 		std::cerr << "Sensitive Sink " << it.first << std:: endl;
	// 	} else if (SINKS.count(it.first)) {
	// 		std::cerr << "Sensitive Source " << it.first << std::endl;
	// 	} 
	// 	std::cerr << "\n____________\n" << it.first << std::endl;
	// 	for (auto x : it.second) {
	// 		std::cerr << x << std::endl;
	// 	}
	// 	std::cerr << "\n____________\n";
	// }


	std::cerr << "********" << std::endl;
	// raw_asm.close();
}

bool is_termination(std::string instruction) {
	return instruction == "hlt" || instruction == "ret" || instruction == "leave";
}
bool no_op(std::string instruction) {
	return instruction == "endbr64" || instruction.substr(0, 3) == "nop";
}

void taint_solver(std::string block, int line_num) {
	// std::cerr << BASIC_BLOCKS[block][line_num] << std::endl;
	// OK till here
	if (line_num > BASIC_BLOCKS.size()) {
		return;
	}
	std::string instruction = BASIC_BLOCKS[block][line_num];
	if (instruction.size() == 0) {
		taint_solver(block, line_num + 1);
	}
	if (is_termination(instruction)) {
		return;
	}
	std::stringstream instruction_parser(instruction);
	std::string operation;
	// std::instruction_parser >> operation;
}

/*
	Registers to track rdi, rsi, rdx, rcx, r8, r9
	Return value in eax
*/

void generate_graph()
{
	std::cerr << "\nStart Generating Graph\n" << std::endl;
	int n = BASIC_BLOCKS.size();
	bool adj[n][n];
	std::ofstream outFile("basic_block.txt");

	outFile << n << std::endl;

	for (int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			adj[i][j] = false;
		}
	}
	

	std::map<std::string,int> index_of_func;
	int idx = 0;
	for (auto it:BASIC_BLOCKS)
	{
		index_of_func[it.first] = idx;
		outFile << it.first << std::endl;
		idx++;
	}
	
	for (auto &it:BASIC_BLOCKS)
	{
		// std::cerr << "hi" << std::endl;
		idx = index_of_func[it.first];

		for(auto &str:it.second)
		{
			int func_start = str.find('<');

			if(func_start == -1)
				continue;

			int func_end = str.find('+',func_start);

			std::string func = str.substr(func_start,func_end-func_start);
			func += '>';
			// std::cerr << str << std::endl;
			// std::cerr << it.first << " " << func << " " << func_start << " " << func_end << std::endl;
			adj[idx][index_of_func[func]] = true;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(adj[i][j])
				outFile << 1;
			else
				outFile << 0;
			
			outFile << " ";
		}

		outFile << std::endl;
	}
	
	
	outFile.close();
}

int main(int argc, char* argv[]) {
	// Command to run
	// g++ taint_tracker.cpp -o taint_tracker; ./taint_tracker ./sample
	if (argc < 2) {
		printf("Enter path to binary code");
	}
	read_sources_sinks();
	generate_raw_assembly();
	clean_raw_asm();
	taint_solver("<main>", 0);
	generate_graph();
}