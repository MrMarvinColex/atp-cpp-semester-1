#include <iostream>
#include <string>
size_t countDigitsBeforeDot (std::string s){
	size_t i = 0;
	while (s[i] != '.' && i < s.size()){
		++i;
	}
	return i;

}
signed main () {
	std::string s1;
	std::string s2;
	std::cin >> s1 >> s2;
	bool flag = false;
	if (s1[0] == '-' && s2[0] != '-'){
		std::cout << "<\n";
		return 0;
	}
	if (s1[0] != '-' && s2[0] == '-'){
		std::cout << ">\n";
		return 0;
	}
	if (s1[0] == '-' && s2[0] == '-')
		flag = true;
	char output = '=';
	if (countDigitsBeforeDot(s1) < countDigitsBeforeDot(s2))
		output = '<';
	if (countDigitsBeforeDot(s1) > countDigitsBeforeDot(s2))
		output = '>';
	if (countDigitsBeforeDot(s1) == countDigitsBeforeDot(s2)){
		size_t i = 0;
		while (s1[i] == s2[i] && i < std::min(s1.size(), s2.size())) {
			++i;
		}
		if (i == std::min(s1.size(), s2.size())){
			output = '<';
			if (i == s1.size() && i < s2.size()) {
				bool f = true;
				++i;
				while (i < s2.size()){
					if (s2[i] != '0'){
						break;
					}
					++i;
				}
				if (i == s2.size())
				output = '=';
			}
			if (i == s2.size() && i < s1.size())
				output = '>';
			if (i == s1.size() && i == s2.size())
				output = '=';
		} else {
			if (s1[i] > s2[i])
				output = '>';
			else
				output = '<';
		}
	}
	if (flag){
		if (output == '<')
			output = '>';
		if (output == '>')
			output = '<';
	}
	std::cout << output << '\n';
}
