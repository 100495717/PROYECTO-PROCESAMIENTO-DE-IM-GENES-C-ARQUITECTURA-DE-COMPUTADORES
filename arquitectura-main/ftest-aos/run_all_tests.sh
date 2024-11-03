#!/bin/bash

dir=$(dirname $0)
mkdir -p "$dir/output"

source $dir/test_utils.sh
source $dir/args_test.sh
source $dir/info_test.sh
source $dir/maxlevel_test.sh
source $dir/resize_test.sh

failed_tests=()

run_test args_0
run_test args_1
run_test args_2

run_test info_normal
run_test info_too_many_args
run_test info_invalid_input

run_test maxlevel_normal
run_test maxlevel_not_enough_args
run_test maxlevel_too_many_args
run_test maxlevel_negativo
run_test maxlevel_string
run_test maxlevel_demasiado_grande

run_test resize_normal
run_test resize_invalid_width
run_test resize_invalid_height
run_test resize_missing_file

rm -rf "$dir/output"

summary