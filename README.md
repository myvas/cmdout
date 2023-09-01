# myvas::cmdout
The `myvas::cmdout` library provides facilities for executing shell commands and get their output before time runs out.

## Todo list
- By now, it works on Linux; but it should work on more platforms.

## Getting Started
### Import and link the library via CMake
```
include(FetchContent)
FetchContent_Declare(cmdout
  URL https://github.com/myvas/cmdout/archive/refs/tags/0.1.11.tar.gz
)
FetchContent_MakeAvailable(cmdout)

target_link_libraries(<your_target> PRIVATE myvas::cmdout)
```
### Include the header file
```
#include <cmdout.hpp>
```

### Examples
C++17 on GNU/Linux Debian 11 (bullseye)
```
#include <cmdout_ext.hpp>
#include <cassert>
//...
int main() {
	// function myvas::system()
	{
		auto result = myvas::system("exit 0");
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system("exit 1");
		std::cout << result << std::endl;
		assert(result.status() == EXIT_FAILURE);
	}
	{
		auto result = myvas::system("ls not-exist 2>&1");
		std::cout << result << std::endl;
		assert(result.status() == ENOENT);
	}
	{
		auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(0)).exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system_timeout_ms("ls / -l", 1);
		std::cout << result << std::endl;
		assert(result.status() == ETIME);
	}
	{
		auto result = myvas::system_timeout_ms("ls / -l", 9);
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}

	// class myvas::cmdout
	{
		auto result = myvas::cmdout().exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::cmdout("ls not-exist 2>&1").exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::cmdout("ls / -l", std::chrono::milliseconds(1)).exec();
		std::cout << result << std::endl;
		assert(result.status() == ETIME);
	}
	{
		auto result = myvas::cmdout("ls / -l", std::chrono::milliseconds(9)).exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
}
```

## tests
```
All tests passed (33 assertions in 3 test cases)
```

## Benchmarks
```
std::system("exit 0"):          runs 10000 times, elapsed 5366836922 ns, result: 1863.29 x/s
myvas::system("exit 0"):        runs 10000 times, elapsed 5872007506 ns, result: 1702.48 x/s
myvas::cmdout("exit 0").exec(): runs 10000 times, elapsed 6787948119 ns, result: 1473.21 x/s
```
