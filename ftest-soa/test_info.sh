#!/bin/bash

info_sabatini() {
    echo "Ejecutando test info de sabtatini: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../img_in/sabatini.ppm"
    local output_file="$2../out/sabatini_info_out.ppm"
    local operacion="info"

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion"

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

    # Extraer las dimensiones de ambas imágenes, de la propuesta y la creada
    local input_info=$(sed -n '1p 2p 3p' "$input_file")
    local output_info=$(sed -n '1p 2p 3p' "$output_file")

    # Comparar dimensiones
    if [ "$input_info" = "$output_info" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
info_sabatini