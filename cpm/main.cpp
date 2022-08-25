#include "cpm.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	
	try
	{
		Module::envp = envp;
		App::modules = get_modules(CORE_PATH);

		for (auto& module: App::modules)
		{
			cout << "Module: " << module->name << endl;
			cout << "Header: " << module->header << endl;
			cout << "Files: " << endl;
			for (auto& file: module->files)
				cout << "\t" << file << endl;
			
			module->compile();
		}
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}
	App::free();
	return (0);
}