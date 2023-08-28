# cmdout
To execute a shell command and get its output, including stdout, stderr, status (exit code).

There are 3 overloads:
```
cmdout system(const char *cmd);
cmdout system(const char *cmd, unsigned timeout_ms);
cmdout system(const char *cmd, std::chrono::milliseconds timeout_ms);
```
- All of them will return a result or a timeout error, both are wrapped in a `cmdout` object.
- Each of them will wait for the result to become available until specified timeout has elapsed.
- The default value of timeout is **10 seconds**, if the `timeout_ms` argument is 0 or unspecified.

### Todo list
- By now, it works well on Linux; but it should work on more platforms.

## Getting Started
### CMake file
```
FetchContent_Declare(
  cmdout
  GIT_REPOSITORY https://github.com/myvas/cmdout.git
  GIT_TAG        0.0.1 # release-0.0.1
)
FetchContent_MakeAvailable(cmdout)

#target_link_libraries(<your_target> PRIVATE cmdout)
```
### Include the header file
```
#include <cmdout.hpp>
```

## Demo
### C++17 on GNU/Linux Debian 11 (bullseye)
```
#include <cmdout_ext.hpp>
#include <iostream>

int main() {
  auto result22 = myvas::system("ls not-exist 2>&1");
  std::cout << result22 << std::endl;

  auto result42 = myvas::system("ls / -l", std::chrono::milliseconds(4));
  std::cout << result42 << std::endl;
}
```
