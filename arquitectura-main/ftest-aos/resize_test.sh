#!/bin/bash

resize_normal() {
    local INPUT_FILE="$2/input/deer-small.ppm"
    local OUTPUT_FILE="$2/output/deer-small-resize100x200.ppm"
    local OPERATION="resize"
    local WIDTH="100"
    local HEIGHT="200"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $WIDTH $HEIGHT"

    local EXPECTED_FILE="$2/expected/deer-small-resize100x200.ppm"

    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}



resize_invalid_width() {
    local INPUT_FILE="$2/input/deer-small.ppm"
    local OUTPUT_FILE="$2/output/deer-small-resize-invalid.ppm"
    local OPERATION="resize"
    local WIDTH="-100"
    local HEIGHT="200"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $WIDTH $HEIGHT"

    local EXPECTED_ERROR="Error: Invalid resize width: -100"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

resize_invalid_height() {
    local INPUT_FILE="$2/input/deer-small.ppm"
    local OUTPUT_FILE="$2/output/deer-small-resize-invalid.ppm"
    local OPERATION="resize"
    local WIDTH="100"
    local HEIGHT="-200"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $WIDTH $HEIGHT"

    local EXPECTED_ERROR="Error: Invalid resize height: -200"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

resize_missing_file() {
    local INPUT_FILE="$2/input/nonexistent.ppm"
    local OUTPUT_FILE="$2/output/nonexistent-resize.ppm"
    local OPERATION="resize"
    local WIDTH="100"
    local HEIGHT="200"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $WIDTH $HEIGHT"

    local EXPECTED_ERROR="Error opening file: $2/input/nonexistent.ppm"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}
