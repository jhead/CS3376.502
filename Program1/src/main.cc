## Author: Justin Head
## jxh122430@utdallas.edu
## SE3376.502

#include <iostream>
#include "other.h"

int main(int argc, char* argv[])
{
    std::cout << "Entered main()" << std::endl;
    std::cout << "argc: " << (argc - 1) << std::endl;

    if (argc > 1) {
        for (int i = 1; i < argc; i++)
        {
            std::cout << argv[i] << std::endl;
        }
    }

    proc1();

    return 0;
}
