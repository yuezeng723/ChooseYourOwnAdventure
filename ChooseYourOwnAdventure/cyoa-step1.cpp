#include "Page.hpp"

int main(int argc, const char ** argv){
    if (argc != 2){
        std::cerr << "Command:invalid command input!" << std::endl;
        exit(EXIT_FAILURE);
    }
    Page thisPage(argv[1]);
    thisPage.printPage();
    return EXIT_SUCCESS;
}
