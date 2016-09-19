# Concurrent Graph

## Build
Run `make` from `src` directory.

## Test Data
Can be generated using `graph_gen` binary included:
```bash
$ cd data
$ ../bin/graph_gen 65500 9825000 | ../bin/graph_gen > g_65500_9825000
$ cd ../src
$ split ../data/g_65500_9825000 -n l/3 # should create xaa, xab, xac
```

## Notes
- Currently only supports Linux x86_64 platform.
- Needs a lot more testing.
- Increasing `NVERTEX` to over 65536 on my test machine results in an unresponsive system depending on system load.
- It is wise to [enable Linux sysrq key combinations](http://askubuntu.com/questions/4408/what-should-i-do-when-ubuntu-freezes/36717#36717) during testing. In case of a hang, I can kill the most expensive process (careful, could be chrome or any other important process) using `Alt` + `SysReq` + `F`.
- The problem described in last two points can exacerbate if debugging options like the following are enabled in `Makefile`:
```
DFLAGS = -fsanitize=address -g -D_GLIBCXX_DEBUG
```
Address Sanitizer, specifically, will spawn its own threads further affecting system load.

# Test Platform
## CPU
```
$ lscpu
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                4
On-line CPU(s) list:   0-3
Thread(s) per core:    1
Core(s) per socket:    4
Socket(s):             1
NUMA node(s):          1
Vendor ID:             AuthenticAMD
CPU family:            16
Model:                 4
Model name:            AMD Phenom(tm) II X4 955 Processor
Stepping:              2
CPU MHz:               800.000
CPU max MHz:           3200.0000
CPU min MHz:           800.0000
BogoMIPS:              6429.77
Virtualization:        AMD-V
L1d cache:             64K
L1i cache:             64K
L2 cache:              512K
L3 cache:              6144K
NUMA node0 CPU(s):     0-3
Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm 3dnowext 3dnow constant_tsc rep_good nopl nonstop_tsc extd_apicid pni monitor cx16 popcnt lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs skinit wdt hw_pstate vmmcall npt lbrv svm_lock nrip_save
```

## Memory
```
$ sudo lshw -class memory -short -sanitize
H/W path         Device      Class       Description
====================================================
/0/0                         memory      128KiB BIOS
/0/4/a                       memory      128KiB L1 cache
/0/4/c                       memory      512KiB L3 cache
/0/b                         memory      128KiB L1 cache
/0/29                        memory      4GiB System Memory
/0/29/0                      memory      2GiB DIMM 1066 MHz (0.9 ns)
/0/29/1                      memory      2GiB DIMM 1066 MHz (0.9 ns)
/0/29/2                      memory      DIMM 1066 MHz (0.9 ns) [empty]
/0/29/3                      memory      DIMM 1066 MHz (0.9 ns) [empty]
```

## OS
```
$ lsb_release -dcs
Ubuntu 16.04.1 LTS
xenial

$ cat /proc/version
Linux version 4.4.0-36-generic (buildd@lcy01-01) (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.2) ) #55-Ubuntu SMP Thu Aug 11 18:01:55 UTC 2016
```

## Compiler
```
$ clang++ --version
clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
