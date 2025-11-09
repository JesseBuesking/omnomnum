# OmNomNum Scripts

Automation scripts for maintaining the OmNomNum codebase.

## regenerate_gperf.sh

Automated tool to regenerate the perfect hash function for denominator word lookup.

### What it does

1. **Checks dependencies**: Verifies gperf and re2c are installed (installs gperf if missing)
2. **Generates perfect hash**: Runs gperf with Preset B parameters on `denom_words.gperf`
3. **Transforms output**: Adds `DENOM_` prefixes and adjusts function names for integration
4. **Updates source files**: Replaces the gperf section in `scanner.re`
5. **Regenerates scanner**: Runs re2c to update `scanner.c`
6. **Updates references**: Updates `denom_preset_b.c` reference file
7. **Verifies**: Builds the project and runs test cases

### Usage

```bash
# Run from project root
./scripts/regenerate_gperf.sh
```

The script will automatically:
- Install gperf if missing (via apt/yum/dnf/brew)
- Create a backup of `scanner.re` as `scanner.re.bak`
- Save temporary files in `.tmp/` directory
- Verify the build works and all tests pass

### When to use

Run this script whenever you modify `denom_words.gperf`:
- Adding new denominator words (e.g., "halves", "thirds")
- Changing denominator values
- Updating the dictionary structure

### Files modified

- `scanner.re` - Updated gperf section (lines 35-268)
- `scanner.c` - Regenerated from scanner.re
- `denom_preset_b.c` - Reference file for comparison

### Gperf parameters (Preset B)

```bash
gperf -C -l -c -t -m 100 -k '1,2,3,$' denom_words.gperf
```

- `-C`: Use C language output
- `-l`: Compare strings with string comparison
- `-c`: Include string contents in table
- `-t`: Include string pool data structure
- `-m 100`: Maximum number of iterations (for optimization)
- `-k '1,2,3,$'`: Use 1st, 2nd, 3rd, and last character for hash

### Testing

The script includes built-in test cases:
- "three halves" → 3/2
- "two halves" → 2/2
- "two and a half" → 5/2
- "three and two halves" → 8/2
- "one half" → 1/2

### Troubleshooting

**Q: Script fails with "gperf not found"**
A: The script will attempt to install gperf automatically. If that fails, install manually:
```bash
# Debian/Ubuntu
sudo apt-get install gperf

# RHEL/CentOS/Fedora
sudo yum install gperf  # or dnf

# macOS
brew install gperf
```

**Q: Build fails after regeneration**
A: Check `/tmp/build.log` for compiler errors. Restore backup with:
```bash
mv scanner.re.bak scanner.re
```

**Q: Tests fail**
A: Verify `denom_words.gperf` is correctly formatted. Each line should be:
```
word, numeric_value
```

## Future additions

Potential automation scripts to add:
- Regenerate cardinal number lookup table (`card_small_fast.gperf`)
- Run full benchmark suite and compare results
- Update version numbers and build metadata
