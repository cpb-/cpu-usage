# cpu-usage

This small shell script monitors the CPU usage of the different threads of a process.

It displays continuously the (approximated) percent of CPU used by the thread in user mode and kernel mode.

The script runs with Bash or compatible shells (it uses arrays of variables).

It is currently under development and this documentation is incomplete.


## Test

You can use the `test-process` program in the `test/` sub-directory: it's a process that
starts several threads with distinct behaviors.


