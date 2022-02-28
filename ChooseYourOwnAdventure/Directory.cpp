#include "Directory.hpp"

Directory::Directory(const char * inputName): storyPageNum(0){
    dictPath.append(inputName);// "./story1"
    DIR * dir = opendir(dictPath.c_str());
    struct dirent *tempFile;
    while ((tempFile = readdir(dir)) != NULL){
        std::string tempStr(tempFile->d_name);
        dictContent.push_back(tempStr);
    }
    closedir(dir);
    //initialize sorted dictPageNum
    std::vector<int> dictPageNum;
    for (size_t i = 0; i < dictContent.size(); ++i) {
        size_t len = dictContent[i].size();
        //if only if the file format is page????.txt, parse the ???? into the int
        if (dictContent[i].compare(0, 4, "page") == 0 && dictContent[i].compare(len - 4, 4, ".txt") == 0) {
            size_t start = dictContent[i].find('e') + 1;
            size_t end = dictContent[i].find('.');
            size_t sz = end - start;
            std::string num = dictContent[i].substr(start, sz);
            int pageNum = std::stoi(num);
            dictPageNum.push_back(pageNum);
        }
    }
    std::sort(dictPageNum.begin(), dictPageNum.end());
    for (std::vector<int>::iterator it = dictPageNum.begin(); it !=dictPageNum.end(); ++it) {
        if (storyPageNum == (unsigned)(*it - 1)){
            storyPageNum++;
        }
    }
    checkDict();
    fillStory();
}

void Directory::checkDict(){
    bool containPageOne = false;
    for (size_t i = 0; i < dictContent.size() ; ++i) {
        if (dictContent[i] == "page1.txt"){
            containPageOne = true;
        }
    }
    if (containPageOne == false){
        std::cerr << "Directory: don't contain page1.txt!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string Directory::makePageName(size_t num){
    std::stringstream target;
    target << dictPath << "/page" << num << ".txt";
    return target.str();
}

void Directory::fillStory(){
    for (size_t i = 1; i < storyPageNum+1; ++i){
        //one row store one page info
        std::string fileName = makePageName(i);
        const char * file = fileName.c_str();
        Page temp(file);
        //std::cout<<temp.getCurrPage()<<" ";
        std::vector<std::pair<bool, int> > row;
        //when this page is not WIN page or LOSE page, fill the key with toPage and fill the value of choice number
        if (temp.getEnding() == notEND) {
            std::map<unsigned, size_t> tempMap = temp.getChoices();
            //if the key value larger is than the storyPageNum, it is beyond the bond
            for (std::map<unsigned, size_t>::iterator it = tempMap.begin(); it != tempMap.end(); ++it) {
                if (it->first > storyPageNum){
                    throw myException("Page: the toPage number is beyond the bound!");
                }
            }
            for (unsigned j = 1; j < storyPageNum +1; ++j) {
                //When page j matches key (toPage) in map, the corresponding option number on page J is stored in matrix
                if (tempMap.find(j)!= tempMap.end()){
                    row.push_back(std::pair<bool, int>(1, tempMap.find(j)->second));
                }
                else {
                    row.push_back(std::pair<bool, int>(0, 0));
                }
            }
        }
        else if (temp.getEnding() == WIN) {
            //if it is the win page, insert corresponding page number into set and fill the matrix with{0,0}
            winPages.insert(temp.getCurrPage());
            for (unsigned n = 1; n < storyPageNum + 1; ++n) {
                row.push_back(std::pair<bool, int>(0, 0));
            }
        }
        else if (temp.getEnding() == LOSE){
            //if it is the lose page, insert page number into set and fill the matrix with{0,0}
            losePages.insert(temp.getCurrPage());
            for (unsigned m = 1; m < storyPageNum + 1; ++m) {
                row.push_back(std::pair<bool, int>(0, 0));
            }
        }
        Story.push_back(row);
    }
}

void Directory::checkMatrix(){
    if (losePages.empty() || winPages.empty()){
        throw myException("Directory: don't contain WIN or LOSE page!");
    }
    for (unsigned i = 1; i < storyPageNum; ++i) {
        int count = 0;
        for (unsigned j = 0; j < storyPageNum; ++j) {
            if (Story[j][i].first){
                count++;
            }
        }
        if (count == 0){
            throw myException("Directory: there is the page lose reference!");
        }
    }
}

void Directory::playGame(){
    int ending = notEND;
    unsigned dest = 1;//store the toPage number
    while(ending == notEND){
        std::string fileName = makePageName(dest);
        Page tempPage(fileName.c_str());
        tempPage.printPage();
        if (tempPage.getEnding() == WIN){
            ending = WIN;
            break;
        }
        if (tempPage.getEnding() == LOSE){
            ending = LOSE;
            break;
        }
        unsigned num;//store input choice number
        std::cin >> num;
        while (std::cin.fail() || num > tempPage.getChoiceNumber()){
            std::cout << "That is not a valid choice, please try again" << std::endl;
            std::cin.clear();
            std::cin.sync();
            while (std::cin.get() != '\n'){
                continue;
            }
        }
        for (size_t i = 0; i < storyPageNum; ++i) {
            if ((unsigned)Story[dest-1][i].second == num){
                dest = i+1;
                break;
            }
        }
    }
}

std::vector<std::vector<unsigned> > Directory::DFS(unsigned from, unsigned to){
    std::stack<std::vector<unsigned> > todo;
    std::set<unsigned> visited;
    std::vector<unsigned> currentPath;
    std::vector<std::vector<unsigned> > allPath;
    unsigned currentNode = from;
    currentPath.push_back(from);
    todo.push(currentPath);
    while (!todo.empty()){
        currentPath = todo.top();
        visited.clear();
        for (size_t j = 0; j < currentPath.size()-1; ++j) {
            visited.insert(currentPath[j]);
        }
        todo.pop();
        currentNode = currentPath.back();
        if (currentNode == to){
            allPath.push_back(currentPath);
            continue;
        }
        if (visited.find(currentNode) == visited.end()){
            for (unsigned i = 0; i < storyPageNum; ++i){
                if (Story[currentNode - 1][i].first){
                    std::vector<unsigned> tempPath = currentPath;
                    tempPath.push_back(i+1);
                    todo.push(tempPath);
                }
            }
        }
    }
    return allPath;
}

void Directory::printDepth(){
    std::cout << "Page 1:0" << std::endl;
    for (size_t i = 1; i < storyPageNum; ++i) {
        size_t smallDepth = INT32_MAX;
        std::vector<std::vector<unsigned> > allPath = DFS(1, i+1);
        if (allPath.size() == 0){
            std::cout << "Page " << i+1 << " is not reachable" <<std::endl;
        }
        else {
            for (size_t j = 0; j < allPath.size(); ++j) {
                if (allPath[j].size() < smallDepth) {
                    smallDepth = allPath[j].size();
                }
            }
            smallDepth = smallDepth - 1;
            std::cout << "Page " << i + 1 << ":" << smallDepth << std::endl;
        }
    }
}

unsigned Directory::getRefNum(size_t row, size_t column){
    return Story[row-1][column-1].second;
}

void Directory::printWinPath(){
    int count = 0;
    for (std::set<unsigned>::iterator it = winPages.begin(); it != winPages.end(); ++it){
        unsigned winNum = *it;
        std::vector<std::vector<unsigned> > allWinPath = DFS(1, winNum);
        count = count +allWinPath.size();
        for (size_t i = 0; i < allWinPath.size(); ++i) {
            for (size_t j = 0; j < allWinPath[i].size()-1; ++j) {
                unsigned ref;
                std::cout << allWinPath[i][j] << "(";
                ref = getRefNum(allWinPath[i][j],allWinPath[i][j+1]);
                std::cout << ref << "),";
            }
            std::cout << allWinPath[i][allWinPath[i].size()-1] << "(win)" << std::endl;
        }
    }
    if (count == 0){
        std::cout << "This story is unwinnable!" << std::endl;
    }
}

