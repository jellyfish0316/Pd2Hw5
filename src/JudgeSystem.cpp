#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <climits>
#include <thread>
#include "JudgeSystem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

void clearWindow() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    return;
}
JudgeSystem::JudgeSystem(std::string userPath, std::string problemPath, std::string msgPath, std::string version) {
    this->USER_DATA_PATH = userPath;
    this->PROBLEM_DATA_PATH = problemPath;
    this->LOGIN_MSG_PATH = msgPath;
    this->VERSION = version;
    this->status = "NOT READY";
}
void JudgeSystem::effectLoading(std::string content) {
    static const char* spinner[] = {"|", "/", "-", "\\"};
    for (int i = 0, j = 0; j < 20; j++) { 
        std::cout << "\033[33m" << content << "\033[0m" << spinner[i] << "\r";
        std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        i = ( i + 1 ) % 4;
    }
    return;
}
void JudgeSystem::loadData() {

    // Step1: Call AccountSystem::init with user data path
    AccountSystem::init(JudgeSystem::USER_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading user data...");
    std::cout << yellow("Status - Loading user data...") << green("OK!\n");

    // Step2: Call ProblemSystem::init with problem data path
    ProblemSystem::init(JudgeSystem::PROBLEM_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading problem data...");
    std::cout << yellow("Status - Loading problem data...") << green("OK!\n");

    // Step3: welcome the user
    try {
        std::ifstream inputFile("./msg/login.txt");
        if( !inputFile ) {
            throw std::runtime_error("Error: File does not exist - ./msg/login.tx");
        }

        std::string line;
        std::cout << "\033[36m";
        while (std::getline(inputFile, line)) {
            std::cout << line;
            std::cout << "\n";
        }
        inputFile.close(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return;
}
int JudgeSystem::mainPage() {
    if(JudgeSystem::status == "NOT READY"){
        JudgeSystem::loadData();
        JudgeSystem::status = "USER LOGIN";
        return 0;
    }
    else if(JudgeSystem::status == "USER LOGIN"){
        std::pair<bool, std::string> user_info = AccountSystem::login();
        if(user_info.first == true){
            status = "READY";
        }
        return 0;
    }


    std::string temp;
    std::getline(std::cin, temp);

    while(true){
        MainPage::mainpagePrint();
        
        int op = MainPage::operationCheck();
        if(op == 1){
            std::cout << AccountSystem::getuserLogin() << std::endl;
            return 0;
        }
        if(op == 2){
            std::cout << JudgeSystem::getVersion() << std::endl;
            return 0;
        }
        if(op == 3){
            std::vector<Problem>* plist = ProblemSystem::list_problem();
            for(int i = 0; i < plist->size(); i++){
                std::cout << "Problem " << (char)(65 + i) << ": " << (*plist)[i].getTitle() << std::endl;
            }
            return 0;
        }
        if(op == 5){
            ProblemSystem::submit_code();
            return 0;
        }
        if(op == 6){
            ProblemSystem::newproblem_set(PROBLEM_DATA_PATH);
            return 0;
        }
        if(op == 7){
            return 1;

        }
        if(op == -1){
            std::cout << "No such operation." << std::endl;
            return 0;
        }
        
    }
    clearWindow();


    

}