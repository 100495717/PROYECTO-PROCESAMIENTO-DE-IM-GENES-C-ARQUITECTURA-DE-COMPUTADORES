#!/bin/bash


compress_lake_small() {
    echo "Ejecutando test compress lake-small"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small-comprimido.ppm"
    local expected="$2../ex/compress/lake-small.cppm"
    local operation="compress"
    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución en milisegundos
    start_time=$(date +%s%3N)
    if $comando; then
        end_time=$(date +%s%3N)
        elapsed_time=$((end_time - start_time))
        echo "Comando ejecutado con éxito en ${elapsed_time} ms"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi

    # Funciones para leer dimensiones y valor máximo de color
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

    # Verificar dimensiones
    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de compress FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de compress PASSED: Las dimensiones son correctas"
    fi

    # Verificar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")
    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de compress FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    else
        echo "Test de compress PASSED: El valor máximo de color coincide"
    fi
}

compress_lake_small "$PWD/"

compress_deer_small() {
    echo "Ejecutando test compress deer-small"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small-comprimido.ppm"
    local expected="$2../ex/compress/deer-small.cppm"
    local operation="compress"
    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución en milisegundos
    start_time=$(date +%s%3N)
    if $comando; then
        end_time=$(date +%s%3N)
        elapsed_time=$((end_time - start_time))
        echo "Comando ejecutado con éxito en ${elapsed_time} ms"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi

    # Funciones para leer dimensiones y valor máximo de color
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

    # Verificar dimensiones
    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de compress FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de compress PASSED: Las dimensiones son correctas"
    fi

    # Verificar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")
    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de compress FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    else
        echo "Test de compress PASSED: El valor máximo de color coincide"
    fi
}

compress_deer_small "$PWD/"
