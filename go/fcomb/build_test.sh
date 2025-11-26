#!/usr/bin/env bash

ROOT1="one"

mkdir -p "$ROOT1"/{dir1,dir2/subdirA,dir3/subdirB/subsub}

echo "hello" > "$ROOT1/dir1/file1.txt"
echo "world" > "$ROOT1/dir1/file2.txt"
echo "hello" > "$ROOT1/dir1/file3.txt"
echo "world" > "$ROOT1/dir1/file4.txt"

echo "foo"   > "$ROOT1/dir2/subdirA/a.txt"
echo "bar"   > "$ROOT1/dir2/subdirA/b.txt"

echo "deep"  > "$ROOT1/dir3/subdirB/subsub/deep.txt"

ROOT2="two"

mkdir -p "$ROOT2"/{dir1,dir2/subdirA,dir3/subdirB/subsub}

echo "hello" > "$ROOT2/dir1/file1.txt"
echo "world" > "$ROOT2/dir1/file2.txt"
echo "world" > "$ROOT2/dir1/file3.txt"

echo "foo"   > "$ROOT2/dir2/subdirA/a.txt"
echo "bar"   > "$ROOT2/dir2/subdirA/b.txt"
echo "bar"   > "$ROOT2/dir2/subdirA/c.txt"

echo "deep"  > "$ROOT2/dir3/subdirB/subsub/deep.txt"
