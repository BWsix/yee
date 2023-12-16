#!/bin/bash

set -ex

clang -Wall -Wextra -ggdb -o yee yee.c -lm
