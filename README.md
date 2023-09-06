# myvas::cmdout
The cmdout library provides facilities for executing shell commands and get their output before time runs out.

## Todo list
- By now, it works on Linux; but it should work on more platforms.

## Getting Started
### Import and link the library via CMake
```
include(FetchContent)
FetchContent_Declare(cmdout
	URL https://github.com/myvas/cmdout/archive/refs/tags/0.1.22.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(cmdout)

target_link_libraries(<your_target> PRIVATE cmdout)
```
### Include the header file
```
#include <cmdout/cmdout.hpp>
```

### Examples
C++17 on GNU/Linux Debian 11 (bullseye)
```
#include <cmdout/cmdout_ext.hpp>
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
./benchmarks/benchmarks
2023-09-06T13:07:51+08:00
Running ./benchmarks
Run on (4 X 2893.41 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 4096 KiB (x1)
Load Average: 1.81, 1.24, 0.94
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_std_system        1587179 ns        81671 ns         8562
BM_std_system_out    1751184 ns       147665 ns         4749
BM_myvas_system      1639949 ns       100367 ns         7086
BM_myvas_cmdout      1760044 ns        56635 ns        12510
```
