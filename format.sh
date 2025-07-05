#!/bin/bash

cmake-format -i CMakeLists.txt

find ./src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} -style Chromium \;
