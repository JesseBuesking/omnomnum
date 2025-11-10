#!/bin/bash
# Session start hook for OmNomNum development
# This script runs automatically when a Claude Code session starts
# It ensures development tools (lemon, re2c, google benchmark) are available

# Source the environment setup script to make tools available
# This will install tools if needed and add them to PATH
source scripts/setup_environment.sh
