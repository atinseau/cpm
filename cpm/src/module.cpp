#include "cpm.hpp"

char **Module::envp = NULL;

Module *Module::create(struct dirent *module_ent)
{

	Module *m = new Module();
	DIR *dir;
	struct dirent *ent;

	m->path = current_path() + "/" + CORE_PATH + "/" + module_ent->d_name;
	dir = opendir(m->path.c_str());

	if (!dir)
	{
		cout << "Error: Could not open directory " << m->path << endl;
		delete m;
		return NULL;
	}

	while ((ent = readdir(dir)))
	{
		if (ent->d_type == DT_REG && !strcmp(ent->d_name, MODULE_FILE))
		{
			ifstream file(m->path + "/" + ent->d_name);
			Json::Reader reader;
			Json::Value value;
			if (!file.is_open())
			{
				cout << "Error: Could not open file " << m->path + "/" + ent->d_name << endl;
				delete m;
				return NULL;
			}
			reader.parse(file, value);
			m->name = module_ent->d_name;
			m->header = value["header"].asString();
			for (auto &f : value["files"])
				m->files.push_back(f.asString());
			file.close();
		}
	}
	closedir(dir);
	return m;
}

std::string Module::with_path(const std::string &file)
{
	return path + "/" + file;
}

char *const *vec_to_array(const vector<string> &vec)
{
	char **argv = new char *[vec.size() + 1];
	for (size_t i = 0; i < vec.size(); i++)
		argv[i] = strdup(vec[i].c_str());
	argv[vec.size()] = NULL;

	return argv;
}

void Module::compile_file(std::string &file)
{
	vector<string> args = {
		CC,
		"-std=c++11",
		"-Wall",
		"-Wextra",
		"-Werror",
		"-c",
		"-I" + with_path(header),
		"-o" + with_path(file) + ".o",
		with_path(file)};
	char *const *argv = vec_to_array(args);
	App::free();
	execve(CC_PATH, argv, Module::envp);
	exit(EXIT_SUCCESS);
}

void Module::linking()
{
	pid_t pid;

	if ((pid = fork()) == -1)
		throw runtime_error("Error: Could not fork process");
	if (pid == 0)
	{
		vector<string> args = {
			AR,
			"rc",
			with_path(name) + ".a"
		};
		for (auto &file : files)
			args.push_back(with_path(file) + ".o");
		char *const *argv = vec_to_array(args);
		App::free();
		execve(AR_PATH, argv, Module::envp);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(pid, NULL, 0);
	INFO("\nModule \"" << name << "\" is ready to use" << endl);
}

void Module::compile()
{
	DEBUG("Compiling module: " << name << endl);
	for (size_t i = 0; i < files.size(); i++)
	{
		pid_t pid;

		if ((pid = fork()) == -1)
			throw runtime_error("Error: Could not fork process");
		if (pid == 0)
			compile_file(files[i]);
		else
			waitpid(pid, NULL, 0);
		SUCCESS("✅ " << files[i] << " compiled " << (i < files.size() - 1 ? ", " : ""));
	}
	linking();
	cout << endl;
}

vector<Module *> get_modules(const string &path)
{
	vector<Module *> modules;
	DIR *dir = opendir((current_path() + "/" + path).c_str());
	struct dirent *ent;

	if (!dir)
		throw runtime_error("Could not open directory: " + path);

	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
		{
			Module *m = Module::create(ent);
			if (m)
				modules.push_back(m);
		}
	}
	closedir(dir);
	return modules;
}
