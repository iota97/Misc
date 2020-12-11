#include <iostream>
#include <fstream>
#include <string>

int main (int argc, char *argv[]) {
	std::ifstream file("input_2020.txt");
	std::string str;
	
	int counter1 = 0;
	int counter2 = 0;
	const char* key[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	const char* col[] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
	std::string val[7];
	bool found[7]{};
	while (std::getline(file, str)) {
		if (str.empty()) {
			bool ok = true;
			for (int i = 0; i < sizeof(found)/sizeof(bool); ++i) {
				if (!found[i]) {
					ok = false;
				}
				found[i] = false;
			}
			if (!ok) continue;
			counter1++;

			if (std::stoi(val[0]) < 1920 ||  std::stoi(val[0]) > 2002) 
				continue;
			if (std::stoi(val[1]) < 2010 ||  std::stoi(val[1]) > 2020) 
				continue;
			if (std::stoi(val[2]) < 2020 ||  std::stoi(val[2]) > 2030) 
				continue;
			if (val[3].find("cm") != std::string::npos && (std::stoi(val[3]) < 150 ||  std::stoi(val[3]) > 193))
				continue;
			if (val[3].find("in") != std::string::npos && (std::stoi(val[3]) < 59 ||  std::stoi(val[3]) > 76))
				continue;
			if (val[3].find("cm") == std::string::npos && val[3].find("in") == std::string::npos)
				continue;
			if (val[4][0] != '#' || val[4].size() != 7)
				continue;
			for (int i = 1; i < 7; ++i) {
				bool d = val[4][i] >= '0' && val[4][i] <= '9';
				bool l = val[4][i] >= 'a' && val[4][i] <= 'f';
				if (!d && !l) {
					ok = false;
					break;
				}
			}
			if (!ok) continue;
			ok = false;
			for (int i = 0; i < 7; ++i) {
				if (val[5] == col[i]) {
					ok = true;
					break;
				}
			}
			if (!ok) continue;
			if (val[6].size() != 9) continue;
			counter2++;
		} else {
			for (int i = 0; i < sizeof(found)/sizeof(bool); ++i) {
				size_t pos = str.find(key[i]);
				if (pos != std::string::npos) {
					found[i] = true;
					val[i] = str.substr(pos+4, str.find(" ", pos+4)-pos-4);
				}
			}
		}
	}
	
	std::cout << "1 Star: " << counter1 << " , 2 Star: " << counter2 << std::endl;

	return 0;
}
