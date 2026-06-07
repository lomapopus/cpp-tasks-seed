#!/bin/bash

CHECKS='-*,clang-analyzer-*,performance-*,bugprone-*'
FILES=$(git ls-files '*.cpp' '*.cxx' '*.cc' | tr '\n' ' ')
read -r -a GTEST_CFLAGS <<<"$(pkgconf --cflags gtest 2>/dev/null || true)"

if [[ -z "$FILES" ]]; then
  echo "No source files to analyze."
  exit 0
fi

set -eo pipefail

for f in $FILES; do
  echo "Running clang-tidy on $f"
  args=(
    -p .
    -checks="$CHECKS"
    "$f"
    --extra-arg=-std=c++17 \
    --extra-arg="-isystem${PWD}/../external/eigen" \
    --extra-arg="-isystem${PWD}/../external/lazycsv/include" \
    --quiet
  )
  for flag in "${GTEST_CFLAGS[@]}"; do
    args+=(--extra-arg="$flag")
  done
  clang-tidy "${args[@]}" 2>&1
done
