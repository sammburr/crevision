#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<random>
#include<limits>

std::vector<std::string> get_lines(const char* filename) {
	std::ifstream file(filename);
	std::string line;

	std::vector<std::string> lines;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else {
		std::cerr << "⚠️  Unable to open file! " << filename << std::endl;
	}

	return lines;
}

std::string join(std::vector<std::string> s, std::string delimiter) {

	std::string res;

	for (std::string token : s) {
		res = res + token + delimiter;
	}

	if (res.length() == 0)
		return "";

	res.pop_back();
	return res;

}

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
}

int main() {

	char* path = getenv("HOME");
	if (!path) {
		std::cout << "⚠️  Could not find home directory!" << std::endl;
		return -1;
	}
	
	strcat(path, "/.crevision/topics.csv");

	std::vector<std::string> lines = get_lines(path);

	// Shuffle the lines after header (first line stays in place)
	std::random_device rd;	
	auto rng = std::default_random_engine {rd()};
	std::shuffle(++std::begin(lines), std::end(lines), rng); 

	// Copy vector before erasing header for parsing
	std::vector<std::string> lines_cpy = lines;
	lines.erase(lines.begin());

	int lowest_score = std::numeric_limits<int>::max();

	for (std::string line : lines) {
		std::vector<std::string> values = split(line, ",");
		int score = std::stoi(values[2]);
		if (score < lowest_score)
			lowest_score = score;	
	}

		
	size_t i = 0;
	for (std::string line : lines) {
		std::vector<std::string> values = split(line, ",");
		int score = std::stoi(values[2]);
		if (score == lowest_score) {
			std::cout << "\n";
			std::cout << "📄 Module: \e[32;1m" << values[0] << std::endl; 
			std::cout << "\e[32;0m✍️   Topic: \e[32;1m" << values[1] << std::endl; 
			std::cout << "\e[32;0m💯  Score: \e[32;1m" << values[2] << std::endl; 
			std::cout << "\e[32;0m\n";
			score ++;
			values[2] = std::to_string(score);
			lines[i] = join(values, ",");
			break;
		}
		i++;
	}

	std::ofstream file(path);
	if (file.is_open()) {
		// Rewrite the header
		file << lines_cpy[0] << "\n";
		for (std::string line : lines) {
			file << line << "\n";
		}
		file.close();
	}
	else {
		std::cout << "⚠️  Could not open file to write!\n";
	}


	return 0;
}
