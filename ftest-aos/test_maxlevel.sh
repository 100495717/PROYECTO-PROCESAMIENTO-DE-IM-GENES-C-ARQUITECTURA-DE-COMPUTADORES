#!/bin/bash

# Variables para los archivos de entrada y salida
maxlevel_deer-small-255(){
    ejecutable="../build/imtool-aos/imtool-aos"
    input_file="../img_in/deer-small.ppm"
    expected_output="../img_ex/maxlevel/deer-small-255.ppm"
    output_file="../img_out/maxlevel/deer-small-255-out.ppm"
    operacion="maxlevel"

    # Valor de maxlevel a probar
    valor=255

    # Construir el comando
    comando="$ejecutable $input_file $output_file $operacion $valor"

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
    output_max_color_value=$(grep -m 1 "255" "$output_file")
    expected_max_color_value=$(grep -m 1 "255" "$expected_output")
    if [ "$output_max_color_value" == "$expected_max_color_value" ]; then
        echo "max_color_value comparison: PASSED"
    else
        echo "max_color_value comparison: FAILED"
    fi
}
maxlevel_deer-small-255()

maxlevel_deer-small-65535(){
    ejecutable="../build/imtool-aos/imtool-aos"
    input_file="../img_in/maxlevel/deer-small.ppm"
    expected_output="../img_ex/maxlevel/deer-small-65535.ppm"
    output_file="../img_out/maxlevel/deer-small-65535-out.ppm"
    operacion="maxlevel"

    # Valor de maxlevel a probar
    valor=65535

    # Construir el comando
    comando="$ejecutable $input_file $output_file $operacion $valor"

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
    output_max_color_value=$(grep -m 1 "65535" "$output_file")
    expected_max_color_value=$(grep -m 1 "65535" "$expected_output")
    if [ "$output_max_color_value" == "$expected_max_color_value" ]; then
        echo "max_color_value comparison: PASSED"
    else
        echo "max_color_value comparison: FAILED"
    fi
}
maxlevel_deer-small-65535()