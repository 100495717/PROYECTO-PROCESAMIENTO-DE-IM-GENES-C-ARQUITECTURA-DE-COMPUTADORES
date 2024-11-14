#!/bin/bash

compress_lake_small() {
    echo "Ejecutando test compress lake-small: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small-comprimido.ppm"
    local expected="$2../ex/compress/lake-small.cppm"
    local operation="compress"
    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_s=$((elapsed_time / 1000000000))
        echo "Comando ejecutado con éxito en $elapsed_time_s s"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi

    read_ppm_dimensions() {
        local file=$1
        local dimensions=$(head -n 1 $file | cut -d " " -f 2,3)
        echo "$dimensions"
    }

    read_ppm_maxcolorvalue() {
        local file=$1
        local maxcolorvalue=$(head -n 1 $file | cut -d " " -f 4)
        echo "$maxcolorvalue"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de compress FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de compress PASSED: Las dimensiones son correctas"
    fi

    # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de cutfreq FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi
}
compress_lake_small

compress_deer_small() {
    echo "Ejecutando test compress deer-small: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small-comprimido.ppm"
    local expected="$2../ex/compress/deer-small.cppm"
    local operation="compress"
    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_s=$((elapsed_time / 1000000000))
        echo "Comando ejecutado con éxito en $elapsed_time_s s"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi

    read_ppm_dimensions() {
        local file=$1
        local dimensions=$(head -n 1 $file | cut -d " " -f 2,3)
        echo "$dimensions"
    }

    read_ppm_maxcolorvalue() {
        local file=$1
        local maxcolorvalue=$(head -n 1 $file | cut -d " " -f 4)
        echo "$maxcolorvalue"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de compress FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de compress PASSED: Las dimensiones son correctas"
    fi

    # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de cutfreq FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi
}
compress_deer_small