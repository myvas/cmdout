# Quickstart: Building with CMake

This tutorial aims to get you up and running with cmdout using CMake. If
you're using CMake for the first time or need a refresher, we recommend
the [CMake](https://cmake.org) tutorial as a starting point.

## Prerequisites

To complete this tutorial, you'll need:

*   A compatible operating system (e.g. Linux, macOS, Windows).
*   A compatible C++ compiler that supports at least C++17.
*   [CMake](https://cmake.org/) and a compatible build tool for building the
    project.
    *   Compatible build tools include
        [Make](https://www.gnu.org/software/make/),
        [Ninja](https://ninja-build.org/), and others - see
        [CMake Generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
        for more information.

See [Supported Platforms](platforms.md) for more information about platforms
compatible with cmdout.

If you don't already have CMake installed, see the
[CMake installation guide](https://cmake.org/install).

{: .callout .note}
Note: The terminal commands in this tutorial show a Unix shell prompt, but the
commands work on the Windows command line as well.

## Set up a project

CMake uses a file named `CMakeLists.txt` to configure the build system for a
project. You'll use this file to set up your project and declare a dependency on
cmdout.

First, create a directory for your project:

```
$ mkdir my_project && cd my_project
```

Next, you'll create the `CMakeLists.txt` file and declare a dependency on
cmdout. There are many ways to express dependencies in the CMake ecosystem;
in this quickstart, you'll use the
[`FetchContent` CMake module](https://cmake.org/cmake/help/latest/module/FetchContent.html).
To do this, in your project directory (`my_project`), create a file named
`CMakeLists.txt` with the following contents:

```cmake
cmake_minimum_required(VERSION 3.19)
project(my_project)

# cmdout requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(cmdout
  URL https://github.com/myvas/cmdout/archive/refs/tag/0.2.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(CMDOUT_FORCE_SHARED_CRT ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cmdout)
```

The above configuration declares a dependency on cmdout which is downloaded
from GitHub. In the above example, `0.2.0` is
the Git tag of the cmdout version to use; we recommend updating the
version number often to point to the latest version.

For more information about how to create `CMakeLists.txt` files, see the
[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html).

## Create and run a binary

With cmdout declared as a dependency, you can use cmdout code within
your own project.

As an example, create a file named `hello_test.cc` in your `my_project`
directory with the following contents:

```cpp
#include <cmdout/cmdout.hpp>

int main() {
    auto cmd = "ls ~ -C";
    auto result = myvas::system(cmd);
    
    std::cout << "status: << result.status() << std::endl;
    std::cout << "out: \n" << result.out() << std::endl;
}
```

The above sample includes the main cmdout header file
and demonstrates some basic senario.

To build the code, add the following to the end of your `CMakeLists.txt` file:

```cmake
add_executable(hello_test
  PRIVATE hello_test.cpp
)
target_link_libraries(hello_test
  PRIVATE myvas::cmdout
)
```

Now you can build and run your test:

<pre>
<strong>my_project$ cmake -S . -B build</strong>
-- The C compiler identification is GNU 10.2.1
-- The CXX compiler identification is GNU 10.2.1
...
-- Build files have been written to: .../my_project/build

<strong>my_project$ cmake --build build</strong>
</pre>

Congratulations! You've successfully built and run a test binary using
cmdout.

## Next steps

*   [Check out the Primer](primer.md) to start learning how to write simple
    tests.
*   [See the code samples](samples.md) for more examples showing how to use a
    variety of cmdout features.