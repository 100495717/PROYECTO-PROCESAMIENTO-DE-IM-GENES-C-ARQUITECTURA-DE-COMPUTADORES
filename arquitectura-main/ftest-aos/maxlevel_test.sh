#!/bin/bash

maxlevel_normal() { 
    local INPUT_FILE="$2/input/deer-small.ppm"
    local OUTPUT_FILE="$2/output/deer-small-maxlevel10.ppm"
    local OPERATION="maxlevel"
    local LEVEL="10"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $LEVEL"

    local EXPECTED_FILE="$2/expected/deer-small-maxlevel10.ppm"

    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}

maxlevel_not_enough_args() {
    local INPUT_FILE="$photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="maxlevel"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION"

    local EXPECTED_ERROR="Error: Invalid number of extra arguments for maxlevel: 0"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

maxlevel_too_many_args() {
    local INPUT_FILE="$photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="maxlevel"
    local LEVEL="100"
    local EXTRA="100"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $LEVEL $EXTRA"

    local EXPECTED_ERROR="Error: Invalid number of extra arguments for maxlevel: 2"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

maxlevel_negativo() {
    local INPUT_FILE="$photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="maxlevel"
    local LEVEL="-1"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $LEVEL"

    local EXPECTED_ERROR="Error: Invalid maxlevel: -1"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

 maxlevel_demasiado_grande() {
    local INPUT_FILE="$photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="maxlevel"
    local LEVEL="70000"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $LEVEL"

    local EXPECTED_ERROR="Error: Invalid maxlevel: 70000"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

maxlevel_string() {
    local INPUT_FILE="$photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="maxlevel"
    local LEVEL="copy"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $LEVEL"

    local EXPECTED_ERROR="Error: Invalid maxlevel: copy"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}