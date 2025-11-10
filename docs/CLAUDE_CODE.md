# Claude Code Development Guide

This guide explains how to work with OmNomNum in Claude Code, including critical differences from normal shell environments.

## Quick Start - Session Initialization

Use this prompt at the start of **every session**:

```
Base branch: jesse/decade-late-improvements

1. Fetch and rebase onto origin/jesse/decade-late-improvements
2. Verify build tools: source tools/env.sh && which lemon re2c && pkg-config --modversion benchmark
3. If any tools missing, run: source scripts/setup_environment.sh
4. Show current branch and git status

Note: Each Bash command runs in a new shell - always use "source tools/env.sh && <command>" for builds.
```

## Critical Concept: Shell Session Isolation

**Each Bash tool invocation in Claude Code creates a new shell process.**

This means:

### ❌ What DOESN'T Work

```bash
# Command 1
source tools/env.sh

# Command 2 (in NEW shell - tools not found!)
make
```

The `source` in Command 1 modifies the shell's environment, but Command 2 runs in a **different shell** that doesn't have those changes.

### ✅ What DOES Work

```bash
# Single command with && chaining
source tools/env.sh && make
```

The `&&` operator chains commands in the **same shell session**, so the environment persists.

## Common Patterns

### Building

```bash
# Standard build
source tools/env.sh && make -j4

# Clean build
source tools/env.sh && make clean && make -j4

# Full rebuild from source
source tools/env.sh && make clean && make regen && make -j4
```

### Testing

```bash
# Run all tests
source tools/env.sh && make test

# Run specific test
source tools/env.sh && cd test && ./test_omnomnum
```

### Benchmarking

```bash
# Fast benchmark (~9s)
source tools/env.sh && make benchmark-fast

# Standard benchmark (~32s)
source tools/env.sh && make benchmark

# Accurate benchmark (~64s)
source tools/env.sh && make benchmark-accurate
```

### Regenerating Parser/Scanner

```bash
# Regenerate from .yy and .re files
source tools/env.sh && make regen

# Then rebuild
source tools/env.sh && make -j4
```

## Environment Setup

### First Time (Once per Clone)

```bash
source scripts/setup_environment.sh
```

This installs to `./tools/`:
- **lemon** - Parser generator from SQLite
- **re2c** - Lexer generator
- **Google Benchmark** - Performance testing framework

It also creates `tools/env.sh` which sets:
- `PATH` to include `tools/bin/`
- `PKG_CONFIG_PATH` to include `tools/lib/pkgconfig/`

### Every Session

You don't need to re-run the setup script. Just source the environment:

```bash
source tools/env.sh && <your-command>
```

### Verification

Check tools are available:

```bash
source tools/env.sh && which lemon re2c && pkg-config --modversion benchmark
```

Expected output:
```
/home/user/omnomnum/tools/bin/lemon
/home/user/omnomnum/tools/bin/re2c
1.9.4
```

## Working with Git

### Always Use Base Branch

Your base branch is: **jesse/decade-late-improvements**

Start each session by rebasing:

```bash
git fetch origin jesse/decade-late-improvements
git rebase origin/jesse/decade-late-improvements
```

### Branch Naming Convention

Claude Code requires branches to:
- Start with `claude/`
- End with the session ID

Example: `claude/fix-parser-bug-011CUyMZVfKJx7D3Wax2pjoy`

## Troubleshooting

### "lemon: command not found"

**Problem:** Trying to run build commands without sourcing environment.

**Solution:** Use `source tools/env.sh && make` instead of just `make`.

### "Tools missing after setup script"

**Problem:** Ran setup in one command, tried to use tools in next command.

**Solution:** Setup created `tools/env.sh`. Use `source tools/env.sh && <command>` for builds.

### "Changes but git push fails"

**Problem:** Trying to push to wrong branch or branch doesn't match session ID.

**Solution:** Ensure branch starts with `claude/` and ends with matching session ID.

## Example Workflow

### 1. Start Session

```
Base: jesse/decade-late-improvements. Fetch/rebase, verify build tools available, show status.
```

### 2. Make Changes

Edit files using Read/Edit tools as needed.

### 3. Build and Test

```bash
source tools/env.sh && make clean && make -j4 && make test
```

### 4. Commit and Push

```bash
git add <files>
git commit -m "Description of changes"
git push -u origin <branch-name>
```

## Best Practices

1. **Always check tools first** - Verify lemon, re2c, benchmark at session start
2. **Chain with &&** - Use `source tools/env.sh && command` for all builds
3. **Rebase often** - Keep your branch up to date with jesse/decade-late-improvements
4. **Test before commit** - Run tests to verify changes work
5. **Clean builds** - When in doubt, `make clean && make regen && make`

## File Structure

```
omnomnum/
├── tools/                    # Local tool installations
│   ├── bin/                  # lemon, re2c binaries
│   ├── lib/                  # benchmark libraries
│   └── env.sh               # Environment setup script ⭐
├── scripts/
│   └── setup_environment.sh  # One-time tool installation
├── parser.yy                 # Parser grammar (source)
├── scanner.re                # Scanner specification (source)
├── parser.c                  # Generated from parser.yy
├── scanner.c                 # Generated from scanner.re
└── Makefile                  # Build system
```

## Session Start Template

Copy/paste this at the start of each Claude Code session:

```
Base: jesse/decade-late-improvements

Setup:
1. git fetch origin jesse/decade-late-improvements && git rebase origin/jesse/decade-late-improvements
2. source tools/env.sh && which lemon re2c && pkg-config --modversion benchmark
3. git status

Ready to work. Remember: use "source tools/env.sh && <command>" for all builds.
```
