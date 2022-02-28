#include "Page.hpp"

Page::Page(const char *inputFile): ending(notEND), currPage(1), fileName(inputFile){
    currPage = parsePageName();
    std::ifstream file;
    file.open(fileName);
    if (file.is_open()){
        choiceInfo temp;
        std::string curr;
        while (std::getline(file, curr)){
            if (curr[0] == '#'){
                break;
            }
            else if (std::isdigit(curr[0])){
                temp = parseChoice(curr);
                choices.push_back(temp);
            }
            else if (std::isalpha(curr[0])){
                parseEnding(curr);
            }
            else {
                std::cerr << "File: wrong content format-"<< fileName << std::endl;
                file.close();
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        std::cerr << "File: can not open the file-"<< fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();
}

choiceInfo Page::parseChoice(std::string & curr){
    choiceInfo ans;
    size_t colon = curr.find(':');
    std::string num = curr.substr(0, colon);
    ans.toPage = std::stoul(num);
    ans.info = curr.substr(colon + 1);
    return ans;
}

void Page::parseEnding(std::string & curr){
    if (curr.compare(0, 3, "WIN") == 0){
        ending = -2;
    }
    if (curr.compare(0, 4, "LOSE") == 0){
        ending = -1;
    }
}

void Page::printPageText(){
    std::ifstream file;
    file.open(fileName);
    if (file.is_open()){
        std::string curr;
        size_t jumpLine;
        if (ending == notEND){
            jumpLine = choices.size() + 1;
        }
        if (ending == WIN || ending == LOSE){
            jumpLine = 2;
        }
        size_t lineCounter = 0;
        while (std::getline(file, curr)){
            lineCounter++;
            if (lineCounter > jumpLine){
                std::cout << curr << std::endl;
            }
        }
        std::cout << '\n';
    }
    else {
        std::cerr << "File: can not open the file-"<< fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();
}

void Page::printPageChoices(){
    if (ending == notEND) {
        std::cout << "What would you like to do?" << '\n' << std::endl;
        for (size_t i = 0; i < choices.size(); ++i) {
            std::cout << " " << (i + 1) << ". ";
            std::cout << choices[i].info << '\n';
        }
    }
}

void Page::printPageEnding() {
    if (ending == WIN) {
        std::cout << "Congratulations! You have won. Hooray!" << std::endl;
    }
    if (ending == LOSE) {
        std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
    }
}

void Page::printPage(){
    printPageText();
    printPageChoices();
    printPageEnding();
}

std::map<unsigned , size_t> Page::getChoices(){
    std::map<unsigned , size_t> map;
    if (ending == notEND) {
        for (size_t i = 0; i < choices.size(); ++i) {
            map.insert(std::pair<unsigned, size_t>(choices[i].toPage, i+1));
        }
    }
    return map;
}

int Page::getEnding(){
    return ending;
}

unsigned Page::getCurrPage(){
    return currPage;
}

unsigned Page::getChoiceNumber(){
    return choices.size();
}

