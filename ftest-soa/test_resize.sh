#!/bin/sh

resize_deer_small_100() {
    echo "Ejecutando test resize deer-small 100x100: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/deer-small.ppm"
    local expected_output="$2../ex/resize/deer-small-100.ppm"
    local output_file="$2../out/deer-small-100-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=100
    local new_height=100

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_deer_small_100

resize_deer_small_1000(){
    echo "Ejecutando test resize deer-small 1000x1000: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/deer-small.ppm"
    local expected_output="$2../ex/resize/deer-small-1000.ppm"
    local output_file="$2../out/deer-small-1000-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=1000
    local new_height=1000

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_deer_small_1000

resize_deer_large_100(){
    echo "Ejecutando test resize deer-large 100x100: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/deer-large.ppm"
    local expected_output="$2../ex/resize/deer-large-100.ppm"
    local output_file="$2../out/deer-large-100-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=100
    local new_height=100

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_deer_large_100

resize_deer_large_1000(){
    echo "Ejecutando test resize deer-large 1000x1000: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/deer-large.ppm"
    local expected_output="$2../ex/resize/deer-large-1000.ppm"
    local output_file="$2../out/deer-large-1000-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=1000
    local new_height=1000

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_deer_large_1000


resize_lake_small_100(){
    echo "Ejecutando test resize lake-small 100x100: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/lake-small.ppm"
    local expected_output="$2../ex/resize/lake-small-100.ppm"
    local output_file="$2../out/lake-small-100-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=100
    local new_height=100

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_lake_small_100

resize_lake_small_1000(){
    echo "Ejecutando test resize lake-small 1000x1000: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/lake-small.ppm"
    local expected_output="$2../ex/resize/lake-small-1000.ppm"
    local output_file="$2../out/lake-small-1000-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=1000
    local new_height=1000

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_lake_small_1000

resize_lake_large_100(){
    echo "Ejecutando test resize lake-large 100x100: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/lake-large.ppm"
    local expected_output="$2../ex/resize/lake-large-100.ppm"
    local output_file="$2../out/lake-large-100-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=100
    local new_height=100

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")

    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_lake_large_100

resize_lake_large_1000(){
    echo "Ejecutando test resize lake-large 1000x1000: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input_file="$2../in/lake-large.ppm"
    local expected_output="$2../ex/resize/lake-large-1000.ppm"
    local output_file="$2../out/lake-large-1000-out.ppm"
    local operacion="resize"

    # Valores de resize a probar
    local new_width=1000
    local new_height=1000

    # Construir el comando
    local comando="$ejecutable $input_file $output_file $operacion $new_width $new_height"

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

    # Extraer las dimensiones de ambas imágenes
    local output_dimensions=$(sed -n '2p' "$output_file")
    local expected_dimensions=$(sed -n '2p' "$expected_output")


    # Comparar dimensiones
    if [ "$output_dimensions" = "$expected_dimensions" ]; then
        echo "Test de resize: PASSED (dimensiones coinciden)"
    else
        echo "Test de resize: FAILED (dimensiones no coinciden)"
    fi
}

# Llamada a la función para ejecutar la prueba
resize_lake_large_1000