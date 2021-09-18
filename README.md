# cpu-usage

Copyright (c) 2021 Christophe BLAESS <christophe.blaess@logilin.fr>

## License

This program is free software; you can redistribute it and/or modify it
under the terms and conditions of the GNU General Public License,
version 2  of the License, or (at your option) any later version.

## Concept

`cpu-usage` is a shell script that monitors the CPU consumption of the threads
of a process.


The script runs with Bash or compatible shells (it uses arrays of variables).

## Command line

Simply run `cpu-usage` followed by the PID of the process you want to monitor.

It displays the (approximated) percent of CPU used by each threads in user mode
and kernel mode.
 
The script runs continuously until the process terminates or you hit
`CONTROL-C`.

```console
[cpu-usage]$ ./cpu-usage 301028
301028: 0/0, 301029: 99/0, 301030: 15/0, 301031: 63/36, 301032: 0/0, 
301028: 0/0, 301029: 100/0, 301030: 13/0, 301031: 58/41, 301032: 0/0, 301101: 100/0, 
301028: 0/0, 301029: 100/0, 301030: 13/0, 301031: 63/36, 301032: 0/0, 301101: 100/0, 
301028: 0/0, 301029: 99/0, 301030: 15/0, 301031: 66/32, 301032: 0/0, 
^C
```

When the monitoring above starts, the process runs with 5 threads: `301028` is
the main thread, `30129` to `301032` have been created by him.

By default, `cpu-usage` displays its statistics every two seconds.
The `-n` option could change this timing.


On the second line we see that the process started a new thread `301101`, that
disappears a few second after.

The threads have distincts behaviors:

- `301028` is sleeping and does not use any user nor kernel CPU tick.
- `301029` is a *cpuhog* that loops as fast as possible. Its user space CPU consumption is always near to 100%. Its kernel consumption is null.
- `301030` is a semi *cpuhog* that loops for a while and sleeps the rest of its time. Its CPU consumption in user space is near 15%.
- `301031` is a user and kernel *cpuhog* that does a lot of syscalls. Its CPU consumption is about 60% user space and 40% kernel space.
- `301032` is a sleeping thread that does not consume CPU cycles.
- `301101` is a temporary *cpuhog* that does a few loops then terminates. 


## Test

To test the script, the `test-process` program is available in the `test/`
subdirectory.

First, build it with:

```console
[cpu-usage]$ cd test/

[test]$ make
cc -Wall -pthread  -pthread  test-process.c   -o test-process
```

Then, run:

```console
[test]$ ./test-process 
PID = 301792
```

Run the `cpu-usage` in a second terminal, with the PID `301792` as argument.

The threads will be distributed among the CPU cores. Several threads can reach
100% CPU simultaneously:

```console
[cpu-usage]$ ./cpu-usage 301792
301792: 0/0, 301793: 100/0, 301794: 14/0, 301795: 57/42, 301796: 0/0, 301799: 100/0, 
301792: 0/0, 301793: 100/0, 301794: 13/0, 301795: 61/38, 301796: 0/0, 
301792: 0/0, 301793: 100/0, 301794: 13/0, 301795: 58/41, 301796: 0/0, 301929: 99/0, 
```

But we can also pin them on the same CPU:

```console
[test]$ taskset -c 0 ./test-process 
PID = 308106
```

In this case, the sum of the CPU times on a line will be near to 100%:

```console
[cpu-usage]$ ./cpu-usage 308106
308106: 0/0, 308107: 28/0, 308108: 13/0, 308109: 16/13, 308110: 0/0, 308112: 28/0, 
308106: 0/0, 308107: 28/0, 308108: 13/0, 308109: 20/8, 308110: 0/0, 
308106: 0/0, 308107: 28/0, 308108: 13/0, 308109: 18/10, 308110: 0/0, 308242: 28/0, 
308106: 0/0, 308107: 29/0, 308108: 13/0, 308109: 15/13, 308110: 0/0, 
```

