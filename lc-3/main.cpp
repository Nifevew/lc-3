#include "LC_3.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string path_to_file;

    if (argc == 2)
    {
        path_to_file = argv[1];
    }
    else
    {
        std::cout << "Enter a path to program file : ";
        std::cin >> path_to_file;
        system("cls");
    }

    try
    {
        LC_3 lc_3(path_to_file);

        lc_3.run();
    }
    catch (std::runtime_error& re)
    {
        std::cerr << re.what() << std::endl;
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    
    return 0;
}