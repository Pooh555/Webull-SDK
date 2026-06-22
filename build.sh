#!/usr/bin/env bash

# ==============================================================================
# Strict Mode:
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error.
# -o pipefail: The return value of a pipeline is the status of the last command to exit with a non-zero status.
# ==============================================================================
set -euo pipefail

# --- Configuration & Defaults ---
BUILD_TYPE="Debug"
CLEAN_BUILD=0
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# --- Logging Utilities ---
GREEN=$(tput setaf 2 || echo "")
BLUE=$(tput setaf 4 || echo "")
RED=$(tput setaf 1 || echo "")
YELLOW=$(tput setaf 3 || echo "")
BOLD=$(tput bold || echo "")
RESET=$(tput sgr0 || echo "")

log_info()    { echo -e "${BLUE}${BOLD}[INFO]${RESET} $1"; }
log_success() { echo -e "${GREEN}${BOLD}[SUCCESS]${RESET} $1"; }
log_warn()    { echo -e "${YELLOW}${BOLD}[WARN]${RESET} $1"; }
log_error()   { echo -e "${RED}${BOLD}[ERROR]${RESET} $1"; >&2; }

# --- Error Handler ---
trap 'exit_code=$?; if [ $exit_code -ne 0 ]; then log_error "Build failed with exit code $exit_code."; fi; exit $exit_code' EXIT

# --- Help Menu ---
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -b, --build-type <type>   Set build type (Debug, Release, RelWithDebInfo). Default: Debug"
    echo "  -c, --clean               Remove the existing build directory before building"
    echo "  -h, --help                Show this help menu"
    echo ""
}

# --- Argument Parsing ---
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -b|--build-type) BUILD_TYPE="$2"; shift ;;
        -c|--clean) CLEAN_BUILD=1 ;;
        -h|--help) show_help; exit 0 ;;
        *) log_error "Unknown parameter passed: $1"; show_help; exit 1 ;;
    esac
    shift
done

BUILD_DIR="${PROJECT_ROOT}/out/build/${BUILD_TYPE,,}-ninja"

# --- Pre-flight Checks ---
log_info "Checking dependencies..."

for cmd in cmake ninja; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        log_error "Required command '$cmd' is not installed. Please install it and try again."
        exit 1
    fi
done

# --- Execution ---
cd "${PROJECT_ROOT}"

if [[ ${CLEAN_BUILD} -eq 1 ]]; then
    log_info "Clean flag detected. Removing build directory: ${BUILD_DIR}"
    rm -rf "${BUILD_DIR}"
fi

log_info "Configuring project for ${BOLD}${BUILD_TYPE}${RESET}..."

cmake -B "${BUILD_DIR}" -G Ninja \
      -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

log_success "CMake configuration completed."

if command -v nproc >/dev/null 2>&1; then
    CORES=$(nproc)
else
    CORES=4
fi

log_info "Building Webull-SDK and Examples using ${CORES} parallel jobs..."

cmake --build "${BUILD_DIR}" --parallel "${CORES}"

log_success "Build completed successfully!"

# --- Post-Build Output ---
echo ""
log_info "Artifact Locations:"
echo "  - SDK Library:   ${BUILD_DIR}/libWebull-SDK.a"
echo "  - Demo Binary:   ${PROJECT_ROOT}/examples/bin/Webull-SDK-Demo"
echo ""