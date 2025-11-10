#!/bin/bash
# Automated environment setup script for OmNomNum development
# This script checks for and installs required tools:
# 1. lemon (parser generator from SQLite)
# 2. re2c (lexer generator)
# 3. google benchmark (performance testing framework)
#
# Usage:
#   bash scripts/setup_environment.sh        # Install only
#   source scripts/setup_environment.sh      # Install + add to PATH

# Detect if being sourced or executed
(return 0 2>/dev/null) && SOURCED=1 || SOURCED=0

# Only exit on error if not sourced (to avoid killing the user's shell)
if [ "$SOURCED" -eq 0 ]; then
    set -e
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
TOOLS_DIR="$PROJECT_ROOT/tools"
TOOLS_BIN="$TOOLS_DIR/bin"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    if [ "$SOURCED" -eq 1 ]; then
        return 1
    else
        exit 1
    fi
}

section() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

# Check if command exists
command_exists() {
    command -v "$1" &> /dev/null
}

# Check and install lemon (from SQLite)
check_install_lemon() {
    section "Checking lemon (Parser Generator)"

    # Check if lemon exists in PATH or local tools
    if command_exists lemon; then
        info "lemon is already installed: $(which lemon)"
        lemon -? 2>&1 | head -5
        return 0
    elif [ -f "$TOOLS_BIN/lemon" ]; then
        info "lemon found in local tools: $TOOLS_BIN/lemon"
        export PATH="$TOOLS_BIN:$PATH"
        lemon -? 2>&1 | head -5
        return 0
    fi

    warn "lemon not found. Installing from SQLite source..."

    # Create tools directory
    mkdir -p "$TOOLS_BIN"

    # Build and install lemon
    info "Cloning SQLite repository..."
    cd /tmp
    rm -rf sqlite-mirror 2>/dev/null || true
    git clone --depth 1 https://github.com/mackyle/sqlite.git sqlite-mirror

    info "Building lemon..."
    cd sqlite-mirror/tool
    cc -o lemon lemon.c

    info "Installing lemon to $TOOLS_BIN..."
    cp lemon "$TOOLS_BIN/"
    cp lempar.c "$TOOLS_BIN/"

    # Add to PATH for this session
    export PATH="$TOOLS_BIN:$PATH"

    # Verify installation
    if [ ! -f "$TOOLS_BIN/lemon" ]; then
        error "lemon installation failed"
    fi

    info "lemon installed successfully!"
    lemon -? 2>&1 | head -5

    # Cleanup
    cd /tmp
    rm -rf sqlite-mirror
}

# Check and install re2c (lexer generator)
check_install_re2c() {
    section "Checking re2c (Lexer Generator)"

    # Check if re2c exists in PATH or local tools
    if command_exists re2c; then
        info "re2c is already installed: $(which re2c)"
        re2c --version
        return 0
    elif [ -f "$TOOLS_BIN/re2c" ]; then
        info "re2c found in local tools: $TOOLS_BIN/re2c"
        export PATH="$TOOLS_BIN:$PATH"
        re2c --version
        return 0
    fi

    warn "re2c not found. Installing from source..."

    # Check for cmake
    if ! command_exists cmake; then
        error "cmake is required to build re2c. Please install cmake first."
    fi

    # Create tools directory
    mkdir -p "$TOOLS_BIN"

    # Build and install re2c
    info "Cloning re2c repository..."
    cd /tmp
    rm -rf re2c 2>/dev/null || true
    git clone --depth 1 https://github.com/skvadrik/re2c.git

    info "Building re2c..."
    cd re2c
    rm -rf build 2>/dev/null || true
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$TOOLS_DIR"
    make -j4

    info "Installing re2c to $TOOLS_BIN..."
    make install

    # Add to PATH for this session
    export PATH="$TOOLS_BIN:$PATH"

    # Verify installation
    if [ ! -f "$TOOLS_BIN/re2c" ]; then
        error "re2c installation failed"
    fi

    info "re2c installed successfully!"
    re2c --version

    # Cleanup
    cd /tmp
    rm -rf re2c
}

# Check and install Google Benchmark
check_install_benchmark() {
    section "Checking Google Benchmark"

    # Check if benchmark is already installed (system-wide or local)
    local benchmark_found=false
    if [ -f "/usr/local/lib/libbenchmark.a" ] || [ -f "/usr/local/lib/libbenchmark.so" ]; then
        info "Google Benchmark appears to be installed in /usr/local"
        benchmark_found=true
    elif [ -f "$TOOLS_DIR/lib/libbenchmark.a" ] || [ -f "$TOOLS_DIR/lib/libbenchmark.so" ]; then
        info "Google Benchmark found in local tools: $TOOLS_DIR/lib"
        benchmark_found=true
    fi

    if [ "$benchmark_found" = true ]; then
        if [ -f "/usr/local/lib/pkgconfig/benchmark.pc" ]; then
            pkg-config --modversion benchmark 2>/dev/null && info "Version: $(pkg-config --modversion benchmark)"
        elif [ -f "$TOOLS_DIR/lib/pkgconfig/benchmark.pc" ]; then
            PKG_CONFIG_PATH="$TOOLS_DIR/lib/pkgconfig:$PKG_CONFIG_PATH" pkg-config --modversion benchmark 2>/dev/null && info "Version: $(PKG_CONFIG_PATH="$TOOLS_DIR/lib/pkgconfig:$PKG_CONFIG_PATH" pkg-config --modversion benchmark)"
        fi
        return 0
    fi

    warn "Google Benchmark not found. Installing from source..."

    # Check for cmake
    if ! command_exists cmake; then
        error "cmake is required to build Google Benchmark. Please install cmake first."
    fi

    # Create tools directory
    mkdir -p "$TOOLS_DIR"

    # Build and install Google Benchmark
    info "Cloning Google Benchmark repository..."
    cd /tmp
    rm -rf benchmark 2>/dev/null || true
    git clone --depth 1 https://github.com/google/benchmark.git

    info "Building Google Benchmark with LTO enabled..."
    cd benchmark
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true -DBENCHMARK_ENABLE_TESTING=OFF -DCMAKE_INSTALL_PREFIX="$TOOLS_DIR"
    make -j4

    info "Installing Google Benchmark to $TOOLS_DIR..."
    make install

    # Update PKG_CONFIG_PATH for this session
    export PKG_CONFIG_PATH="$TOOLS_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"

    # Verify installation
    if [ ! -f "$TOOLS_DIR/lib/libbenchmark.a" ] && [ ! -f "$TOOLS_DIR/lib/libbenchmark.so" ]; then
        error "Google Benchmark installation failed"
    fi

    info "Google Benchmark installed successfully!"

    # Cleanup
    cd /tmp
    rm -rf benchmark
}

# Verify all installations
verify_installations() {
    section "Verifying All Installations"

    local failed=0

    # Ensure tools/bin is in PATH
    export PATH="$TOOLS_BIN:$PATH"
    export PKG_CONFIG_PATH="$TOOLS_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"

    # Check lemon
    if command_exists lemon; then
        echo -e "  ${GREEN}✓${NC} lemon: $(which lemon)"
    elif [ -f "$TOOLS_BIN/lemon" ]; then
        echo -e "  ${GREEN}✓${NC} lemon: $TOOLS_BIN/lemon"
    else
        echo -e "  ${RED}✗${NC} lemon: NOT FOUND"
        failed=$((failed + 1))
    fi

    # Check re2c
    if command_exists re2c; then
        echo -e "  ${GREEN}✓${NC} re2c: $(which re2c)"
    elif [ -f "$TOOLS_BIN/re2c" ]; then
        echo -e "  ${GREEN}✓${NC} re2c: $TOOLS_BIN/re2c"
    else
        echo -e "  ${RED}✗${NC} re2c: NOT FOUND"
        failed=$((failed + 1))
    fi

    # Check Google Benchmark
    if [ -f "/usr/local/lib/libbenchmark.a" ] || [ -f "/usr/local/lib/libbenchmark.so" ]; then
        echo -e "  ${GREEN}✓${NC} Google Benchmark: /usr/local/lib/libbenchmark.*"
    elif [ -f "$TOOLS_DIR/lib/libbenchmark.a" ] || [ -f "$TOOLS_DIR/lib/libbenchmark.so" ]; then
        echo -e "  ${GREEN}✓${NC} Google Benchmark: $TOOLS_DIR/lib/libbenchmark.*"
    else
        echo -e "  ${RED}✗${NC} Google Benchmark: NOT FOUND"
        failed=$((failed + 1))
    fi

    echo ""

    if [ $failed -eq 0 ]; then
        info "All tools verified successfully!"
        echo ""

        # Create env.sh helper script
        create_env_script

        info "Tools installed to: $TOOLS_DIR"
        echo ""

        # Handle environment setup based on whether script was sourced or executed
        if [ "$SOURCED" -eq 1 ]; then
            # Script was sourced - automatically add to PATH for current shell
            export PATH="$TOOLS_BIN:$PATH"
            export PKG_CONFIG_PATH="$TOOLS_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"

            info "Tools added to PATH for current shell session!"
            echo ""
            info "To add permanently, add this to your ~/.bashrc or ~/.zshrc:"
            echo "    source $PROJECT_ROOT/tools/env.sh"
        else
            # Script was executed - provide instructions
            info "To use the tools in your current shell, run:"
            echo "    source tools/env.sh"
            echo ""
            echo "Or run the setup script with source to install + add to PATH:"
            echo "    source scripts/setup_environment.sh"
            echo ""
            info "To add permanently, the script can update your shell config."
            echo ""

            # Offer to add to shell config
            add_to_shell_config
        fi

        echo ""
        info "You can now build OmNomNum with:"
        echo "    make"
        echo ""
        info "To regenerate parser/scanner files:"
        echo "    make regen"
        echo ""
        info "To run benchmarks:"
        echo "    make benchmark"
        echo ""
        return 0
    else
        error "$failed tool(s) failed verification"
    fi
}

# Create a sourceable env.sh script
create_env_script() {
    local env_file="$TOOLS_DIR/env.sh"

    cat > "$env_file" <<'EOF'
#!/bin/bash
# Source this file to add OmNomNum development tools to your PATH
# Usage: source tools/env.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

export PATH="$SCRIPT_DIR/bin:$PATH"
export PKG_CONFIG_PATH="$SCRIPT_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"

echo "OmNomNum development tools added to PATH"
echo "  lemon: $SCRIPT_DIR/bin/lemon"
echo "  re2c: $SCRIPT_DIR/bin/re2c"
echo "  Google Benchmark: $SCRIPT_DIR/lib/libbenchmark.*"
EOF

    chmod +x "$env_file"
    info "Created $env_file"
}

# Offer to add to shell config
add_to_shell_config() {
    # Detect shell config file
    local shell_config=""
    if [ -n "$BASH_VERSION" ]; then
        if [ -f "$HOME/.bashrc" ]; then
            shell_config="$HOME/.bashrc"
        elif [ -f "$HOME/.bash_profile" ]; then
            shell_config="$HOME/.bash_profile"
        fi
    elif [ -n "$ZSH_VERSION" ]; then
        shell_config="$HOME/.zshrc"
    fi

    if [ -z "$shell_config" ]; then
        warn "Could not detect shell config file"
        return
    fi

    # Check if already added
    if grep -q "source.*tools/env.sh" "$shell_config" 2>/dev/null; then
        info "Already added to $shell_config"
        return
    fi

    echo ""
    read -p "Add 'source tools/env.sh' to $shell_config? (y/N) " -n 1 -r
    echo ""

    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "" >> "$shell_config"
        echo "# OmNomNum development tools" >> "$shell_config"
        echo "if [ -f \"$PROJECT_ROOT/tools/env.sh\" ]; then" >> "$shell_config"
        echo "    source \"$PROJECT_ROOT/tools/env.sh\"" >> "$shell_config"
        echo "fi" >> "$shell_config"

        info "Added to $shell_config"
        echo ""
        info "Restart your shell or run: source $shell_config"
    else
        info "Skipped. You can manually add this line to your shell config:"
        echo "    source \"$PROJECT_ROOT/tools/env.sh\""
    fi
    echo ""
}

# Display CPU governor recommendation
cpu_governor_info() {
    section "Performance Optimization Tip"

    info "For best benchmark results, set CPU governor to 'performance' mode:"
    echo ""
    echo "  # Set for a specific core (replace # with core number):"
    echo "  sudo cpufreq-set -c # -g performance"
    echo ""
    echo "  # Switch back to default:"
    echo "  sudo cpufreq-set -c # -g ondemand"
    echo ""
    echo "  # Pro tip: Create a function to change all cores at once"
    echo "  # Add to ~/.bashrc:"
    echo "  cpup() { for i in {0..$(nproc --ignore=1)}; do sudo cpufreq-set -c \$i -g \$1; done; }"
    echo ""
}

# Main execution
main() {
    echo ""
    echo "========================================="
    echo "  OmNomNum Environment Setup"
    if [ "$SOURCED" -eq 1 ]; then
        echo "  (Sourced mode - will add to PATH)"
    fi
    echo "========================================="
    echo ""
    echo "This script will install the following tools:"
    echo "  1. lemon  - Parser generator (from SQLite)"
    echo "  2. re2c   - Lexer generator"
    echo "  3. Google Benchmark - Performance testing framework"
    echo ""
    echo "Tools will be installed to: $TOOLS_DIR"
    echo ""

    # Install each tool
    check_install_lemon
    check_install_re2c
    check_install_benchmark

    # Verify everything is installed
    if ! verify_installations; then
        # Verification failed - don't show success message
        return 1
    fi

    # Show CPU governor tip
    cpu_governor_info

    section "Setup Complete!"

    info "All development tools are ready!"
}

# Run main function
main "$@"
