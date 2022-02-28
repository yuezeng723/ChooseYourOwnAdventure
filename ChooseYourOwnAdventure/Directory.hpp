#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__
#include "Page.hpp"
#include <dirent.h>
#include <algorithm>
#include <set>
#include <sstream>
#include <stack>
#include <queue>
#include <climits>


/**
 * @class Directory
 * @brief Directory class holds:
 * dictPath: the full directory path name;
 * dict Content: it holds each file names in it;
 * storyPageNum: it holds the max number of the continues valid page;
 * Story: it holds the adjacency matrix which shows the directed graph of page relationship;
 * winPages: it holds the page number of win pages;
 * losePages: it holds the page number of lose pages;
 */
class Directory{
private:
    std::string dictPath;//  ./story1
    std::vector<std::string> dictContent;//. .. page1.txt page2.txt page3.txt page5.txt
    size_t storyPageNum;
    std::vector<std::vector<std::pair<bool, int> > > Story;
    std::set<unsigned> winPages;
    std::set<unsigned > losePages;
public:
    /**
     * @brief explicit constructor take the input directory path name
     * @param inputName a const pointer points to the input directory path name
     */
    explicit Directory(const char * inputName);

    /**
     * @brief check whether the directory contains the page1.txt, if not report error
     */
    void checkDict();

    /**
     * @brief receive the input page number and form a string page name
     * @param num the input page number, for example: 3
     * @return the string page name, for examole: page3.txt
     */
    std::string makePageName(size_t num);

    /**
     * @brief read the valid pages in the directory, parse their info and build an adjacency matrix;
     * aims to store the edge information in the matrix, the page number can be calculated from the matrix index
     */
    void fillStory();
    
    /**
     * @brief check if there is page(except page1.txt) not referenced by other pages;
     * check if there is no win pages or lose pages;
     * if there is a column(except the 0th column) with all pairs{0,0}, then the page(column+1) is not referenced;
     * if the losePage set and winPage set are empty, there is no win or lose page;
     */
    void checkMatrix();

    /**
     * @brief a function start the game, print the story content;
     * User will interact in command window with this game;
     */
    void playGame();

    /**
     * @brief use DFS algorithm search all the possible path between "from" node to "to" node;
     * @param from the page number which is the start point of the path;
     * @param to the page number which is the end point of the path;
     * @return all possible path between the "from" node to "to" node;
     */
    std::vector<std::vector<unsigned> > DFS(unsigned from, unsigned to);

    /**
     * @brief print the page number and its shortest path depth start from page 1 and to itself
     * print in the format: page 1:0
     */
    void printDepth();

    /**
     * @brief search the adjacency matrix with the input connected "from" page and "to" page;
     * return the corresponding choice number of the "to page" in the "from page"
     * @param row the fromPage number
     * @param column the toPage number
     * @return the choice number of the toPage in fromPage content
     */
    unsigned getRefNum(size_t row, size_t column);

    /**
     * @brief pint all the possible win path in the story. 
     */
    void printWinPath();

};
#endif
