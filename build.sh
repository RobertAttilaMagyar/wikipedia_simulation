#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# --- Color Definitions for Pretty Output ---
NC='\033[0m' # No Color
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'

# --- Defaults ---
BUILD_TYPE="Release"
BUILD_DIR="build-linux" # Prevents cache clashes with Windows host "build" folder
CLEAN_FIRST=false
RUN_TEST=false

# --- Help Menu ---
show_help() {
    echo -e "${BLUE}Wikipedia Network Simulator Build Automator${NC}"
    echo "Usage: ./build.sh [options]"
    echo ""
    echo "Options:"
    echo "  -d, --debug       Configure build in Debug mode (Default is Release)"
    echo "  -c, --clean       Completely wipe the build folder before compiling"
    echo "  -t, --test        Run the Python extension verification test after building"
    echo "  -h, --help        Show this help message"
    echo ""
}

# --- Parse Arguments ---
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN_FIRST=true
            shift
            ;;
        -t|--test)
            RUN_TEST=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_help
            exit 1
            ;;
    esac
done

# --- Execution Steps ---

# 1. Handle Cleaning
if [ "$CLEAN_FIRST" = true ]; then
    if [ -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}🧹 Cleaning: Removing old build folder '$BUILD_DIR'...${NC}"
        rm -rf "$BUILD_DIR"
    else
        echo -e "${YELLOW}ℹ️ Clean requested, but '$BUILD_DIR' does not exist yet. Skipping.${NC}"
    fi
fi

# 2. Configure Project
echo -e "${BLUE}⚙️ Configuring CMake (Build Type: ${BUILD_TYPE}, Directory: ${BUILD_DIR})...${NC}"
cmake -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# 3. Compile Project
echo -e "${BLUE}🏗️ Compiling targets...${NC}"
cmake --build "$BUILD_DIR" --parallel $(nproc)

echo -e "${GREEN}✅ Build successful!${NC}"
echo -e "   - C++ executable located in: ./${BUILD_DIR}/main"
echo -e "   - Python extension located in: ./python/wikipedia_module/"

# 4. Run Optional Verification Test
if [ "$RUN_TEST" = true ]; then
    echo -e "${BLUE}🐍 Running Python module verification...${NC}"
    if cd python && python3 -c "import wikipedia_module._core; print('🎉 Import successful!')" 2>/dev/null; then
        echo -e "${GREEN}🚀 Python extension is fully functional inside WSL/Docker!${NC}"
    else
        echo -e "${RED}❌ Python extension failed to import.${NC}"
        exit 1
    fi
fi