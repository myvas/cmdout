# myvas::cmdout
[![Build Status](https://app.travis-ci.com/myvas/cmdout.svg?branch=main)](https://app.travis-ci.com/myvas/cmdout)
[![Build status](https://ci.appveyor.com/api/projects/status/pudfd8l1ni8jkj2w/branch/main?svg=true)](https://ci.appveyor.com/project/MyvasAdmin/cmdout/branch/main)
[![Release](https://img.shields.io/github/release/myvas/cmdout.svg)](https://github.com/myvas/cmdout/releases)

The cmdout library provides facilities for executing shell commands and get their output before time runs out.

## Getting Started
### Import and link the library via CMake
```
include(FetchContent)
FetchContent_Declare(cmdout
	URL https://github.com/myvas/cmdout/archive/refs/tags/0.2.0.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(cmdout)

target_link_libraries(<your_target> PRIVATE cmdout)
```
### Include the header file
```
#include <cmdout/cmdout.h>
```

### Examples
C++17 on GNU/Linux Debian 11 (bullseye)
```
#include <cmdout/cmdout_ext.h>
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

## Tests
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

## For developers
### FAQ 1. Could NOT find Doxygen (missing: dot) (found version "1.9.5")
We should install the doxygen and graphviz on the build/test/debug machine.
(CMakeLists.txt Line: 119-123)
```
apt install doxygen
apt install graphviz
```

### FAQ 2. How to install/upgrade cmake 3.27.0?
We should install the cmake 3.27.0 (or above) on the build/test/debug machine. [Here](https://askubuntu.com/questions/829310/how-to-upgrade-cmake-in-ubuntu) are steps on Debian:

1.Check current version and location.
```
cmake --version
which cmake
```

2.Download the installation script.
```
wget -b -c --show-progress  https://github.com/Kitware/CMake/releases/download/v3.27.0/cmake-3.27.0-linux-x86_64.sh
```

3.Run the script to unzip files.
```
sudo mv cmake-3.27.0-linux-x86_64.sh /opt
cd /opt
sudo bash cmake-3.27.0-linux-x86_64.sh
```

4.Replace old symbolic links.
```
sudo rm /usr/local/bin/ccmake
sudo rm /usr/local/bin/cmake
sudo rm /usr/local/bin/cmake-gui
sudo rm /usr/local/bin/cpack
sudo rm /usr/local/bin/ctest
sudo ln /opt/cmake-3.27.0-linux-x86_64/bin/* /usr/local/bin
```

## Todo list
- By now, it works on Linux; but it should work on more platforms.
