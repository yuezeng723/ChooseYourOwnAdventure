#include "Directory.hpp"

int main(int argc, const char ** argv) {
    if (argc != 2){
        std::cerr << "Command: invalid command input!" << std::endl;
        exit(EXIT_FAILURE);
    }
    Directory currDict(argv[1]);
    try {
        currDict.checkMatrix();
        currDict.printWinPath();
    }
    catch (const myException & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

