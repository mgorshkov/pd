#!/bin/bash

find include samples src unit_tests -type f -iname *.cpp -o -iname *.hpp | xargs clang-format -i
