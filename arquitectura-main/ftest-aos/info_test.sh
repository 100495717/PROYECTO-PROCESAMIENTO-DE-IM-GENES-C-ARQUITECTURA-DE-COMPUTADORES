#!/bin/bash

info_normal() {
    local INPUT_FILE="$2/input/deer-small.ppm"
    local OUTPUT_FILE="out"
    local OPERATION="info"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION"

    local EXPECTED_OUTPUT="$(cat << EOF
Input: $2/input/deer-small.ppm
Output: out
Operation: info
Image size: 267x200
Max level: 255
EOF
)"

    test_output "$1" "$COMMAND" "$EXPECTED_OUTPUT"
}

info_too_many_args() {
    local INPUT_FILE="photo.ppm"
    local OUTPUT_FILE="out.ppm"
    local OPERATION="info"
    local EXTRA="100"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $EXTRA"

    local EXPECTED_ERROR="Error: Invalid extra arguments for info: 100"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

info_invalid_input() {
    local INPUT_FILE="$2/input/deer-small-invalid.ppm"
    local OUTPUT_FILE="out"
    local OPERATION="info"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION"

    local EXPECTED_ERROR="Invalid magic number"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

