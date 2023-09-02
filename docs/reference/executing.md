# Executing Reference

<!--* toc_depth: 3 *-->

This page lists the facilities provided by cmdout for executing a shell command and get its output. To use them, add `#include <cmdout/cmdout.hpp>`.

## class myvas::cmdout

This library defines a class for store outputs.

### constructor

```
cmdout();
explicit cmdout(const std::string& cmd);
explicit cmdout(const std::string& cmd, int64_t timeout_ms);
explicit cmdout(const std::string& cmd, std::chrono::milliseconds timeout);
```

### status

* EXIT_SUCCESS - The shell command goes well and returns successfully.
* EXIT_FAILURE - The shell command executed and exit with 1.
* ETIME - The shell command does NOT return before time runs out.
* ENOENT - The shell command executed and exit with error "No such file or directory"
* ... - Please read the <stdlib.h> to get more possible values

### out

A string came from the `stdout`, i.e. the file descriptor 1 in POSIX system.

If you also want to get the `stderr`, append ` 2>&1` would be an answer in POSIX system.

## function myvas::system()

```
cmdout system(const std::string& cmd);
cmdout system_timeout_ms(const std::string& cmd, int64_t timeout_ms);
cmdout system_timeout(const std::string& cmd, std::chrono::milliseconds timeout);
```