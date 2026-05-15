#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${ROOT}/build"
SERVER_BIN="${BUILD_DIR}/reqrep_server"
CLIENT_BIN="${BUILD_DIR}/reqrep_client"

echo "==> 配置并编译..."
cmake -S "${ROOT}" -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" -j"$(nproc)"

echo "==> 运行 REQ/REP 示例..."
"${SERVER_BIN}" &
SERVER_PID=$!
cleanup() {
  kill "${SERVER_PID}" 2>/dev/null || true
  wait "${SERVER_PID}" 2>/dev/null || true
}
trap cleanup EXIT

sleep 0.2
"${CLIENT_BIN}"

echo "==> 完成"
