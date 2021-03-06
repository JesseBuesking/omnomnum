# OmNomNum

Implements most of the logic from [numerizer](https://github.com/jduff/numerizer) in c using [re2c](http://re2c.org/) and [lemon](http://www.hwaci.com/sw/lemon/).

## Differences between Numerizer and OmNomNum

- OmNomNum preserves whitespace, Numerizer does not.
- OmNomNum supports an optional `parse_second` parameter, Numerizer does not.
- OmNomNum can support most currency formats, Numerizer does not.
- Numerizer supports fractions, OmNomNum does not (yet?).

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

- [ ] Consider supporting scientific and other notations as well.
- [ ] Try to support fractions.
    - Not sure if it's possible to have both fractional support and multi-number
      (e.g. "one two three four") support.
- [ ] Try to make grammar unambiguous.
- [ ] Test for thread safety.
    - Unnecessary for my current needs since I'm embedding in ruby and it has a
      global lock.

## Notes

### Multiple numbers

Multiple numbers back-to-back causes this grammar to be ambiguous (see the rule
`numbers ::= numbers number.`). I believe it's not possible to resolve this. For
example, take "one two" and "one thousand one hundred". Let's assume that you're
on "two" in the first example, and the second "one" in the second example. It's
not possible for the  parser to use any reduce rules in the second example
because it doesn't yet know if there's more data available to match another
rule -- the "hundred" -- or if it's safe to reduce like in the first example. I
may be wrong, but this is my current understanding of why it's ambiguous.
