#!/bin/bash
compress_deer-small(){
    echo "Ejecutando test compress deer-small: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small.cppm"
    local expected="$2../exp/compress/deer-small.cppm"
    local operation="compress"

    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_ms=$((elapsed_time / 1000000))
        echo "Comando ejecutado con éxito en $elapsed_time_ms ms"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi
    # Función para leer la cabecera de una imagen PPM
    read_ppm_header() {
        local file=$1
        local header=$(head -n 1 "$file")
        echo "$header"
    }
    
     # Comparar cabecera
    local header_output=$(read_ppm_header "$output")
    local header_expected=$(read_ppm_header "$expected")

    if [ "$header_output" != "$header_expected" ]; then
        echo "La cabecera no coincide:"
        echo "Salida: $header_output"
        echo "Esperado: $header_expected"
        return 1
    else
        echo "La cabecera es correcta"
    fi

   
}
compress_lake-small(){
    echo "Ejecutando test compress lake-small: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small.cppm"
    local expected="$2../exp/compress/lake-small.cppm"
    local operation="compress"

    local comando="$ejecutable $input $output $operation"

    echo "Ejecutando comando: $comando"

    # Medir el tiempo de ejecución
    start_time=$(date +%s%N)
    if $comando; then
        end_time=$(date +%s%N)
        elapsed_time=$((end_time - start_time))
        elapsed_time_ms=$((elapsed_time / 1000000))
        echo "Comando ejecutado con éxito en $elapsed_time_ms ms"
    else
        echo "Error en la ejecución del comando"
        return 1
    fi
    # Función para leer la cabecera de una imagen PPM
    read_ppm_header() {
        local file=$1
        local header=$(head -n 1 "$file")
        echo "$header"
    }
     # Función para contar el número de líneas de un archivo
    count_lines() {
        local file=$1
        local lines=$(wc -l < "$file")
        echo "$lines"
    }
     # Comparar cabecera
    local header_output=$(read_ppm_header "$output")
    local header_expected=$(read_ppm_header "$expected")

    if [ "$header_output" != "$header_expected" ]; then
        echo "La cabecera no coincide:"
        echo "Salida: $header_output"
        echo "Esperado: $header_expected"
        return 1
    else
        echo "La cabecera es correcta"
    fi

}

compress_deer-small "$@"
#compress_lake-small "$@"