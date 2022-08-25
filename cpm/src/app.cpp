#include "cpm.hpp"


vector<Module*> App::modules = vector<Module*>();

void App::free()
{
	for (auto& module: modules)
		delete module;
}