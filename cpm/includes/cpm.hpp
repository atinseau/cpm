#ifndef CPM_HPP
#define CPM_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <memory>

using namespace std;

#define MODULE_FILE "module.json"
#define CORE_PATH "core_modules"
#define EXEC_PATH "/usr/bin/"
#define CC "clang++"
#define CC_PATH EXEC_PATH CC
#define AR "llvm-ar-10"
#define AR_PATH EXEC_PATH AR

# define CLEAR "\e[0m"
# define RED "\e[1;31m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;36m"
# define PURPLE "\e[1;35m"
# define GREEN "\e[1;32m"

# define LOG(color, output, msg) output << color << msg << CLEAR

# define ERROR(msg) LOG(RED, std::cerr, msg)
# define WARNING(msg) LOG(YELLOW, std::cerr, msg)
# define INFO(msg) LOG(BLUE, std::cout, msg)
# define DEBUG(msg) LOG(PURPLE, std::cout, msg)
# define SUCCESS(msg) LOG(GREEN, std::cout, msg) 

struct Module;

struct App
{
	static vector<Module *> modules;
	static void free();
};

struct Module
{
	string name;
	string path;
	string header;
	vector<string> files;
	static Module *create(struct dirent *module_ent);
	static char **envp;

	void compile();

private:
	std::string with_path(const std::string &file);
	void compile_file(std::string& file);
	void linking();
};

std::string &current_path();
vector<Module *> get_modules(const string &path);

#endif