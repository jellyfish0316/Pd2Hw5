#include <iostream>
#include <fstream>
#include <sstream>
#include "Problem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

Problem::Problem(std::string title, std::string input, std::string output, int magic_num) {
    this->problem_title = title;
    this->input_path = input;
    this->output_path = output;
    this->magic_number = magic_num;
}
void ProblemSystem::adding_problem(Problem new_problem) {
    this->problem_list.push_back(new_problem);
    return;
}

void ProblemSystem::init(std::string &PROBLEM_DATA_PATH) {
    // TODO: Loading problem data from PROBLEM_DATA_PATH
    std::ifstream file(PROBLEM_DATA_PATH);
    if( !file.is_open() ) {
        std::cerr << "Error\n";
        std::cerr << "Please check your csv file in " << PROBLEM_DATA_PATH << "\n";
        exit(1);
    }

    std::string readline;
    while( getline(file, readline) ) {
        std::string title, input_path, output_path, magic_num;
        // TODO: spilt by char ',' (Hints: stringstream)
        std::stringstream ss;
        ss << readline;

        getline(ss, title, ',');
        getline(ss, input_path, ',');
        getline(ss, output_path, ',');
        getline(ss, magic_num);


        Problem new_problem(title, input_path, output_path, stoi(magic_num));
        ProblemSystem::adding_problem(new_problem);
    }
    return;
}
std::vector<Problem>* ProblemSystem::list_problem() {
    // TODO: retrun the correct variable
    return &this->problem_list;

}
void ProblemSystem::newproblem_set(std::string PROBLEM_DATA_PATH) {
     // write code here
    std::string title, input_path, output_path, magic_num;

    std::cout << "Now, please input the input path(testcase): ";
    std::cin >> input_path;
    std::cout << "OK! your input path is " << input_path << std::endl;

    std::cout << "Now, please input the output path(testcase): ";
    std::cin >> output_path;
    std::cout << "OK! your output path is " << output_path << std::endl;

    std::cout << "Please input the magic number(for this problem): ";
    std::cin >> magic_num;

    std::cout << "Final! please input the problem name: ";
    std::getline(std::cin, title);
    std::getline(std::cin, title);
    std::cout << "OK! your problem name(title) is " << title << std::endl;

    Problem new_problem(title, input_path, output_path, stoi(magic_num));
    ProblemSystem::adding_problem(new_problem);
    
    try { // update problem data
        std::ofstream info_out(PROBLEM_DATA_PATH);
        if( !info_out ) {
            throw std::runtime_error("Error: File does not exist - " + PROBLEM_DATA_PATH);
        }

        for( Problem problem : problem_list ) {
            info_out << problem.getTitle() << "," << problem.getInput() << "," << problem.getOutput() << "," << problem.getNum() << "\n";
        }

        info_out.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
    return;
}

void ProblemSystem::submit_code(){
    vector<Problem> plist = this->problem_list;
    char id;
    Problem p;
    std::string code_name;

    std::cout << "Please input the problem ID: ";
    std::cin >> id;
    p = plist[(int)id - 65];

    std::cout << "Please input your code name: ";
    std::cin >> code_name;

    std::string command;

    command = "g++ " + code_name + " -o " + "testing_exec";
    if(system(command.c_str()) != 0) {
        std::cout << "Compile error!" << std::endl;
        return;
    }
    
    command = "./testing_exec < " + p.getInput() + "/input.txt";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cout << "execution failed!" << std::endl;
        return;
    }
    std::string outputfilepath = p.getOutput() + "/ans.txt";
    if(compare_files(pipe, outputfilepath.c_str())){
        std::cout << "Accepted" << std::endl;
        return;
    }
    pclose(pipe);
    std::cout << "Wrong answer" << std::endl;
    return;
 }

bool compare_files(FILE* f1, const char* filepath) {
    FILE* f2 = fopen(filepath, "r");
    if (!f2) return false;

    const int SIZE = 1024;
    char buf1[SIZE], buf2[SIZE];

    while (!feof(f1) && !feof(f2)) {
        if (!fgets(buf1, SIZE, f1) || !fgets(buf2, SIZE, f2)) break;
        if (strcmp(buf1, buf2) != 0) {
            fclose(f2);
            return false;
        }
    }

    bool f1_end = feof(f1);
    bool f2_end = feof(f2);
    fclose(f2);
    return f1_end && f2_end;
}

string get_timestamp_ms() {
    using namespace chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return to_string(ms); 
}


void ProblemSystem::rand_problems(){
    vector<Problem> plist = this->problem_list;
    vector<string> pstrlist;
    for (const auto& prob : plist) {
        pstrlist.push_back(prob.getTitle());
    }
    
    // 使用生日 + 日期為種子
    string birthday;
    cout << "請輸入你的生日（格式 YYYYMMDD）：";
    getline(cin, birthday);
    string timestamp = get_timestamp_ms();

    // 結合生日與時間戳
    string seed_str = birthday + timestamp;
    size_t seed = hash<string>{}(seed_str);  // 安全且隨機

    // 使用 mt19937 初始化
    mt19937 rng(seed);
    
    cout << "按 Enter 開始轉盤...";
    cin.get();

    int result = spin_wheel(pstrlist, rng);
    
    cout << magenta("Congrats! you just won problem ") << pstrlist[result] << magenta(" as a prize!") << endl;
    
}