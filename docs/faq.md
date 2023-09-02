# FAQ

## Why use myvas::cmdout?

The common C++ code to execute a shell command and get its output is:
```
auto tmpfile = 
auto cmd = "some-shell-command > output-file";
auto exit_code = std::system(cmd);
auto result read_as_string("output-file")
` +  is exactlly ineffective.
