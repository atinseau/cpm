#include "cpm.hpp"

char *p = get_current_dir_name();

string& current_path()
{
	static string pwd = p;
	if (p)
	{
		free(p);
		p = NULL;
	}
	return pwd;
}