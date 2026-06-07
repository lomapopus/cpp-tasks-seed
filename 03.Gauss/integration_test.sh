#!/bin/bash

set -euo pipefail

status=true
expected="$(mktemp)"
actual="$(mktemp)"

cleanup() {
  rm -f "$expected" "$actual"
}
trap cleanup EXIT

cat >"$expected" <<'EOF'
B
-1.000000
2.000000
EOF

./gauss AB.csv >"$actual"

if cmp -s "$expected" "$actual"; then
  echo Integration test ok!
else
  echo Integration test failed! >&2
  status=false
fi

$status
