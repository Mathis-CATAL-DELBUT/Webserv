#include <fstream>
#include <sstream>
#include <iostream>

int main()
{
    std::ifstream ifs("../html/image/ale-sain.jpg", std::ios::binary);
    std::stringstream iss;
    iss << ifs.rdbuf();
    std::string content = iss.str();
    std::cout << content << std::endl;
}