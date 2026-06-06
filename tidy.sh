#!/bin/bash

CHECKS='-*,clang-analyzer-*,performance-*,bugprone-*'
FILES=$(git ls-files '*.cpp' '*.cxx' '*.cc' | tr '\n' ' ')
TIDY_ARGS=(--extra-arg=-std=c++17)

if pkg-config --exists gtest 2>/dev/null; then
  while read -r flag; do
    [[ -n "$flag" ]] && TIDY_ARGS+=(--extra-arg="$flag")
  done < <(pkg-config --cflags gtest | tr ' ' '\n')
fi

if [[ -z "$FILES" ]]; then
  echo "No source files to analyze."
  exit 0
fi

set -eo pipefail

for f in $FILES; do
  echo "Running clang-tidy on $f"
  clang-tidy -p . -checks="$CHECKS" "$f" "${TIDY_ARGS[@]}" --extra-arg="-isystem${PWD}/../external/eigen" --extra-arg="-isystem${PWD}/../external/lazycsv/include" --quiet 2>&1
done
