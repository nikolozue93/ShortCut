#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::cout << "Enter the name of the text file: ";
    std::string fileName;
    std::cin >> fileName;

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return 1;
    }

    std::cout << "Contents of the file:" << std::endl;

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::cout << line << std::endl;
    }

    inputFile.close();

    return 0;
}
