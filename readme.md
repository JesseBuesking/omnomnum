# OmNomNum

Implements most of the logic from [numerizer](https://github.com/jduff/numerizer) in c using [re2c](http://re2c.org/) and [lemon](http://www.hwaci.com/sw/lemon/).

## Prereqs

To run the tests:

- You need to make and install [yaml-cpp](https://github.com/jbeder/yaml-cpp).
    - $ `cmake -G "Unix Makefiles"`
    - $ `make`
    - $ `sudo make install`
- You need to make and install [googletest](https://github.com/google/googletest).

To run the benchmarks:

- You need to make and install [google benchmark](https://github.com/google/benchmark).
    - $ `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true`
    - $ `make`
    - $ `sudo make install`
    - before running, you should set your cpu governor to performance:
        - `sudo cpufreq-set -c \# -g performance`
        - you can switch back to `ondemand`
        - bonus: set up a function to change for all cores, e.g. `cpup performance`

## Building

1. Install lemon to ~/repositories/lemon or update the Makefile to point to it's location.
2. Install re2c
3. make

## TODOS:

- [x] Test for thread safety & make thread safe
    - Not needed for embedding in ruby since it has a global lock.
- [x] faster double to string
- [x] faster int to string
- [x] allow for null byte within the string
- [x] maybe try adding is_dbl and doing int to string by default
  - use dbl by default, but do long double -> long int cast, then printf
- [ ] Prevent parse error (falls back to reduce which works, but the grammar shouldn't be ambiguous).
- [ ] Make sure numbers containing commas (and periods) work as well.
- [ ] Clean up the Makefile.
- [ ] Reuse sds, but "grow"(shrink) to max size between if over DEFAULT_SIZE.
- [ ] Fractions.
- [ ] Clean up code. Better organization, cleaner files, etc.

Performance related:
- [ ] For faster parsing of strings with lots of numbers, sdscatprinf (sdscatprintf -> vsnprintf -> vfprintf -> \_\_printf_fp -> hack_digit) (faster dtoa implementation).
- [ ] For faster parsing of strings with fewer numbers omnomnum_scanner_start and Parse (probably grammar changes).
