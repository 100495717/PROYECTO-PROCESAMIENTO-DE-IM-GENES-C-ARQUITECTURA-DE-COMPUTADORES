#!/bin/bash

args_0() {
    local COMMAND="../imtool-aos/imtool-aos"

    local EXPECTED_ERROR="Error: Invalid number of arguments: 0"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

args_1() {
    local COMMAND="../imtool-aos/imtool-aos photo.ppm"

    local EXPECTED_ERROR="Error: Invalid number of arguments: 1"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

args_2() {
    local COMMAND="../imtool-aos/imtool-aos photo.ppm out.ppm"

    local EXPECTED_ERROR="Error: Invalid number of arguments: 2"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

args_invalid_operation() {
    local COMMAND="../imtool-aos/imtool-aos photo.ppm out.ppm copy"

    local EXPECTED_ERROR="Error: Invalid option: copy"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}
