#ifndef __PAGE_H__
#define __PAGE_H__
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include "Exceptions.hpp"

#define WIN -2
#define LOSE -1
#define notEND -3

typedef struct choiceInfo_tag{
    unsigned toPage;
    std::string info;
    choiceInfo_tag():toPage(1){}
}choiceInfo;

/**
 * @class Page
 * @brief Page class holds the ending of this page(WIN or LOSE or notEnd);
 * it holds the page number of current page;
 * it holds the choices description and its toPage number;
 * it holds the pointer to input file name.
 */
class Page{
private:
    int ending;//WIN or LOSE or notEND
    unsigned currPage;//current page number
    std::vector<choiceInfo> choices;
    const char * fileName;

    unsigned parsePageName(){
        unsigned ans;
        std::string str(fileName);
        size_t start = str.find("page") + 4;
        size_t end = str.find(".");
        size_t size = end - start;
        std::string rel = str.substr(start, size);
        ans = std::stoul(rel);
        return ans;
    }

public:
    /**
     * @brief constructor taking the input file name as parameter
     * @param inputFile the input file name
     */
    explicit Page(const char * inputFile);

    /**
     * @brief parse the read line into choice information
     * @param curr read line
     * @return a struct which stores the "to Page" number and its corresponding choice
     */
    choiceInfo parseChoice(std::string & curr);

    /**
     * @brief parse the read line and store its WIN or LOSE or neither information
     * @param curr read line
     */
    void parseEnding(std::string & curr);

    /**
     * @brief print the text in a page, the text is the content after the # line, and before the choices
     */
    void printPageText();

    /**
     * @brief print the content after the text in one page, with the choice number and brief description of each choice
     */
    void printPageChoices();

   /**
   * @brief print the ending of page, Win or Lose and with corresponding descriptions
   */
    void printPageEnding();

    /**
     * @brief print the whole page with the order of text->choices or ending info
     */
    void printPage();

    /**
     * @brief store the choice's toPage number as the key and its corresponding choice number as value in map
     * @return a map hold the toPage number and choice number
     */
    std::map<unsigned , size_t> getChoices();

    /**
     * @brief get the ending of the Page, WIN or LOSE or notEND
     * @return number of the ending filed
     */
    int getEnding();

    /**
     * @brief get this page number
     * @return an unsigned int of the current page number
     */
    unsigned getCurrPage();

    /**
     * @brief get the total number of choices are provided in this page
     * @return the total number of choices
     */
    unsigned getChoiceNumber();


};

#endif
