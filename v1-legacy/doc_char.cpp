#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

int main()
{
    std::cout << "Enter the name of the text file: ";
    std::string fileName;
    std::cin >> fileName;

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cout << "Error opening file: " << fileName << std::endl;
        return 1;
    }

    std::cout << "Contents of the file:" << std::endl;

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::cout << line << std::endl;
    }

    inputFile.close();

    std::map<char, int> charCount;
    for (char c : line)
    {
        if (std::isalnum(c))
        {
            charCount[c]++;
        }
    }

    std::cout << "\nCharacter counts:\n";
    for (const auto &[character, count] : charCount)
    {
        std::cout << character << ": " << count << "\n";
    }

    std::cout << "\nOrdered by frequency:\n";
    std::vector<std::pair<char, int>> charCountVector(charCount.begin(), charCount.end());
    std::sort(charCountVector.begin(), charCountVector.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    for (const auto &[character, count] : charCountVector)
    {
        std::cout << character << ": " << count << "\n";
    }

    return 0;
}
