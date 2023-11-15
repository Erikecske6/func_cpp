#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

std::vector<std::string> getCamelCaseWords(const std::string &line) {
    auto words = std::vector<std::string>{};

    // bWord always points to the beginning of a word
    // eWord always points to the position after the word
    auto bWord = std::find_if(std::cbegin(line), std::cend(line), std::isalnum);
    while (bWord != std::cend(line)) {

        // the next char after the end of a valid word is
        // a non-digit-and-not-letter or upper case letter.
        auto eWord = std::find_if(std::next(bWord), std::cend(line), [](char c) {
            return !std::isalnum(c) || std::isupper(c);
        });

        words.push_back(std::string(bWord, eWord));  // old fashioned way
                                                     // a temporary string is created

        //words.emplace_back(bWord, eWord);  // C++11 way; in C++11 containers have
                                             // besides the push* methods emplace*
                                             // methods, thats are faster

        bWord = std::find_if(eWord, std::cend(line), std::isalnum);
    }
    return words;
}

int main() {

    std::ifstream ifs{ "camelCase.txt" };
    if (!ifs) {
        std::cerr << "Cannot open file. Exiting...\n";
        return -1;
    }

    std::string line;  // store the lines here
    int i = 0;         // line counter

    // read the content of the file line by line
    while (std::getline(ifs, line)) {
        std::cout << (++i) << ":\n";  // people start counting at 1

        // process each line and pritn the result
        auto words = getCamelCaseWords(line);
        for (const auto &w : words) {
            std::cout << w << "\n";
        }

        std::cout << "---\n";
    }

    return 0;
}
