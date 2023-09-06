# FAQ

## Why use cmdout?

Here is a common C++17 code to execute a shell command and get its output:

### example
file: examples/example_std_system_out.cpp
```
int main() {
{
	auto cmd = "ls not-exist";
	auto result = system_stdout(cmd);
	std::cout << "status: " << std::get<0>(result) << std::endl;
	std::cout << "out: \n" << std::get<1>(result) << std::endl;
}{
	auto cmd = "ls ~ -C";
	auto result = system_stdout(cmd);
	std::cout << "status: " << std::get<0>(result) << std::endl;
	std::cout << "out: \n" << std::get<1>(result) << std::endl;
}}
```

### function std_system_out()
file: src/experimental/std_system_out.cpp
```
#include <cstdio>
#include <string>
#include <fstream>
#include <tuple>

std::tuple<int, std::string> std_system_out(const std::string& cmd)
{
	// warning: the use of `tmpnam' is dangerous, better use `mkstemp'
	auto tempfilepath = std::tmpnam(nullptr);

	// execute a shell command, and log the stdout and stderr to a temp file
	auto cmd_stdout = cmd + std::string(" >> ") + tempfilepath + std::string(" 2>&1");
	auto exit_code = std::system(cmd_stdout.c_str());
	auto status = WEXITSTATUS(exit_code);

	// read the temp file
	std::ifstream stdout_file(tempfilepath);
	if (!stdout_file.is_open()) {
		status = EXIT_FAILURE;
	}
	auto out = std::string((std::istreambuf_iterator<char>(stdout_file)), std::istreambuf_iterator<char>());

	// cleanup the temp file
	std::remove(tempfilepath);

	return std::make_tuple(status, out);
}
```
It works!
But it is neither smart nor effective. Furthermore, it lacks asynchorization or cancellation mechanism to avoid block in a long time.

So we must improve it! Just use myvas::cmdout!
