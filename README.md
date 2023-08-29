# cmdout
The `cmdout` library provides facilities for executing shell commands and get their output before time runs out.

## Todo list
- By now, it works on Linux; but it should work on more platforms.

## Getting Started
### Import and link the library via CMake
```
FetchContent_Declare(
  cmdout
  GIT_REPOSITORY https://github.com/myvas/cmdout.git
  GIT_TAG        0.1.4
)
FetchContent_MakeAvailable(cmdout)

target_link_libraries(<your_target> PRIVATE cmdout)
```
### Include the header file
```
#include <cmdout.hpp>
```

### Demo
C++17 on GNU/Linux Debian 11 (bullseye)
```
#include <cmdout_ext.hpp>
#include <iostream>

int main() {
  auto result20 = myvas::cmdout().exec();
  std::cout << result20 << std::endl;

  auto result22 = myvas::cmdout("ls not-exist 2>&1").exec();
  std::cout << result22 << std::endl;

  auto result42 = myvas::cmdout("ls / -l", std::chrono::milliseconds(4)).exec();
  std::cout << result42 << std::endl;
}
```
