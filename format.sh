#!/bin/bash

shopt -s globstar
clang-format -i main/**/*.c components/**/*.{h,c}
