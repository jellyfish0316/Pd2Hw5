#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include "AccountSystem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

User* AccountSystem::search(std::string name) {
    for(auto& user : AccountSystem::user_list){
        if(user.getUsername() == name){
            return &user;
        }
    }
    return nullptr;
}
void AccountSystem::init(std::string USER_DATA_PATH) {

    AccountSystem::USER_DATA_PATH = USER_DATA_PATH;

    try{
        std::ifstream file(USER_DATA_PATH);

        if( !file ) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        std::string line, username, password;
        while(std::getline(file, line)){
            int commapos;
            commapos = line.find(',');

            username = line.substr(0, commapos);
            password = line.substr(commapos + 1);

            AccountSystem::adding_user(username, password);
        }

        file.close();
    }

    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

}
void AccountSystem::sign_up() {
    std::string usrname, pswd1, pswd2;
    std::cout << "Welcome! Please enter your username: ";
    std::cin >> usrname;
    while(true){
        std::cout << "Please enter your password: ";
        std::cin >> pswd1;
        std::cout << "Please enter your password again: ";
        std::cin >> pswd2;
        if(pswd1 == pswd2){
            AccountSystem::adding_user(usrname, pswd1);
            return;
        }
        else{
            std::cout << "Two passwords are not the same, please try again." << std::endl;
            continue;
        }
    }
    

}
std::pair<bool, std::string> AccountSystem::login() {
    

    std::string inputusrnme;
    
    while(true){
        std::cout << "User Name (Enter -1 to sign up): ";
        std::cin >> inputusrnme;
        if(inputusrnme == "-1"){
            AccountSystem::sign_up();
            return std::make_pair(false, "");
        }
        else{
            if(User* userptr = AccountSystem::search(inputusrnme)){
                std::string inputpswd;
                std::cout << "Welcome aboard, " << inputusrnme << std::endl;
                std::cout << "Please enter your password: ";
                std::cin  >> inputpswd;
                
                if(userptr->getPassword() == inputpswd){
                    std::cout << "Login Success!!! Welcome aboard. " << std::endl;
                    AccountSystem::login_user = userptr->getUsername();
                    return std::make_pair(true, userptr->getUsername());
                }
                else{
                    for(int i = 0; i < 2; i++){
                        std::cout << "Password incorrect... please try again";
                        std::cin  >> inputpswd;
                        if(userptr->getPassword() == inputpswd){
                            std::cout << "Login Success!!! Welcome aboard. " << std::endl;
                            AccountSystem::login_user = userptr->getUsername();
                            return std::make_pair(true, userptr->getUsername());
                        }
                    }
                    std::cout << "Too many attempts... you're a failure, your mom must be disappointed..." << std::endl;
                    continue;
                }
            }
            else{
                std::cout << "User does not exist!" << std::endl;
                continue;
            }
        }
    }
    
}
void AccountSystem::adding_user(std::string username, std::string password) {
    User new_user(username, password);
    user_list.push_back(new_user);
    AccountSystem::userdataUpdate();
    return;
 
}
std::string AccountSystem::getuserLogin() {
    return AccountSystem::login_user;
}
void AccountSystem::userdataUpdate() {
    
    try{
        std::ofstream outputfile;

        outputfile.open(AccountSystem::USER_DATA_PATH);
        if(!outputfile) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        for(auto& user : AccountSystem::user_list){
            outputfile << user.getUsername() << ',' << user.getPassword() << std::endl;
        }

        outputfile.close();

    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
    

}