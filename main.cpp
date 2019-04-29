#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <functional>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdexcept>
#include <string>



int const NOT_DEFINED_INT = -1;
std::string NOT_DEFINED_STRING = "";

std::string rootPath = NOT_DEFINED_STRING;

int inum = NOT_DEFINED_INT;
std::string name = NOT_DEFINED_STRING;
int sizeG = NOT_DEFINED_INT;
int sizeL = NOT_DEFINED_INT;
int sizeE = NOT_DEFINED_INT;
int nlinks = NOT_DEFINED_INT;
std::string exec = NOT_DEFINED_STRING;


void parseArgs(int arc, char *argv[]) {
    if (arc < 2) {
        throw std::runtime_error("Enter arguments");
    }
    rootPath = argv[1];
    for (int i = 2; i < arc - 1; i += 2) {
        if (strcmp(argv[i], "-inum") == 0) {
            if (inum != NOT_DEFINED_INT) {
                throw std::runtime_error("inum was given twice");
            }
            inum = std::stol(argv[i + 1]);
        } else if (strcmp(argv[i], "-name") == 0) {
            if (name != NOT_DEFINED_STRING) {
                throw std::runtime_error("name was given twice");
            }
            name = argv[i + 1];
        } else if (strcmp(argv[i] + 1, "size") == 0) {
            int val = std::stol(std::string(argv[i + 1]));
            if (argv[i][0] == '-') {
                if (sizeL != NOT_DEFINED_INT) {
                    throw std::runtime_error("-size was given twice");
                }
                sizeL = val;
            } else if (argv[i][0] == '=') {
                if (sizeE != NOT_DEFINED_INT) {
                    throw std::runtime_error("=size was given twice");
                }
                sizeE = val;
            } else if (argv[i][0] == '+') {
                if (sizeG != NOT_DEFINED_INT) {
                    throw std::runtime_error("+size was given twice");
                }
                sizeG = val;
            } else {
                throw std::runtime_error("Unknown: " + std::string(argv[i]));
            }
            i++;
        } else if (strcmp(argv[i], "-nlinks") == 0) {
            if (nlinks != NOT_DEFINED_INT) {
                throw std::runtime_error("nlinks was given twice");
            }
            nlinks = std::stol(argv[i + 1]);
        } else if (strcmp(argv[i], "-exec") == 0) {
            if (exec != NOT_DEFINED_STRING) {
                throw std::runtime_error("exec was given twice");
            }
            exec = argv[i + 1];
        } else {
            throw std::runtime_error("Unknown: " + std::string(argv[i]));
        }
    }
}

bool filter(dirent * cur, struct stat const & file){
    if (inum != NOT_DEFINED_INT && file.st_ino != inum){
        return false;
    }
    if (name != NOT_DEFINED_STRING && cur->d_name != name){
        return false;
    }
    if (sizeL != NOT_DEFINED_INT && file.st_size >= sizeL){
        return false;
    }
    if (sizeG != NOT_DEFINED_INT && file.st_size <= sizeL){
        return false;
    }
    if (sizeE != NOT_DEFINED_INT && file.st_size != sizeE){
        return false;
    }
    if (nlinks != NOT_DEFINED_INT && file.st_nlink != nlinks){
        return false;
    }

    return true;
}

void execute(std::vector<std::string> const & val){
    for (const std::string &arg : val){
        std::system((exec + " " + arg).data());
    }
}

void walk(std::string path, std::vector<std::string> &ans) {
    DIR *dir = opendir(path.data());
    dirent *cur;
    std::vector<std::string> toWalk;

    while ((cur = readdir(dir)) != nullptr) {
        struct stat file;
        int res = stat((path + '/' + cur->d_name).c_str(), &file);
        if (res == -1){
            std::cout << "can't proceed: " + path + '/' + cur->d_name;
        }
        if (S_ISDIR(file.st_mode)){
            if (std::strcmp(cur->d_name, ".") != 0 && std::strcmp(cur->d_name, "..") != 0) {
                toWalk.push_back(path + '/' + cur->d_name);
            }
        }
        else{
            if (filter(cur, file)){
                ans.push_back(path + '/' + cur->d_name);
            }
        }
    }

    closedir(dir);

    for (std::string & val: toWalk){
        walk(val, ans);
    }
}

int main(int argc, char *argv[]) {
    try {
        parseArgs(argc, argv);
        std::vector<std::string> ans;
        walk(rootPath, ans);
        if (exec != NOT_DEFINED_STRING){
            execute(ans);
        }
        else{
            for (std::string const & val : ans){
                std::cout << val << std::endl;
            }
        }
    }
    catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}