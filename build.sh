#!/bin/bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "            Webull Trading Bot            "
echo "=========================================="

STRAY_CLASS="./app/src/main/java/webull/trading/bot/App.class"

if [ -f "$STRAY_CLASS" ]; then
    echo "[Cleanup] Removing stray manual .class file from source directory..."
    rm "$STRAY_CLASS"
fi

if [ ! -x "./gradlew" ]; then
    echo "[Fix] Adding execution permission to gradlew..."
    chmod +x ./gradlew
fi

echo "[Build & Run] Launching Gradle..."

./gradlew :app:run