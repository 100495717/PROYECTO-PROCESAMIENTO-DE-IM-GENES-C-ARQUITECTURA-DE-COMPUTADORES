#!/bin/bash

# Variables para los archivos de entrada y salida
maxlevel_deer_small_255(){
    echo "Ejecutando test maxlevel deer-samll 255: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../img_in/deer-small.ppm"
    local expected_output="$2../img_ex/maxlevel/deer-small-255.ppm"
    local output_file="$2../out/deer-small-max-255-out.ppm"
    local operacion="maxlevel"

    # Valor de maxlevel a probar
    local valor=255

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $valor"

    echo "Probando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_ms=$((elapsed_time / 1000000))
        echo "Comando ejecutado con éxito en $elapsed_time_ms ms"
    else
        echo "Error en la ejecución del comando"
        exit 1
    fi

    # Comparar max_color_value
    local output_max_color_value=$(grep -m 1 "255" "$output_file")
    local expected_max_color_value=$(grep -m 1 "255" "$expected_output")
    if [ "$output_max_color_value" == "$expected_max_color_value" ]; then
        echo "Test de maxlevel: PASSED"
    else
        echo "Test de maxlevel: FAILED"
    fi
}
maxlevel_deer_small_255

maxlevel_deer_small_65535(){
    echo "Ejecutando test maxlevel deer-samll 65536: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../img_in/deer-small.ppm"
    local expected_output="$2../img_ex/maxlevel/deer-small-65535.ppm"
    local output_file="$2../out/deer-small-max-65535-out.ppm"
    local operacion="maxlevel"

    # Valor de maxlevel a probar
    local valor=65535

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $valor"

    echo "Probando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_ms=$((elapsed_time / 1000000))
        echo "Comando ejecutado con éxito en $elapsed_time_ms ms"
    else
        echo "Error en la ejecución del comando"
        exit 1
    fi

    # Comparar max_color_value
    local output_max_color_value=$(grep -m 1 "65535" "$output_file")
    local expected_max_color_value=$(grep -m 1 "65535" "$expected_output")
    if [ "$output_max_color_value" == "$expected_max_color_value" ]; then
        echo "Test de maxlevel: PASSED"
    else
        echo "Test de maxlevel: FAILED"
    fi
}
maxlevel_deer_small_65535