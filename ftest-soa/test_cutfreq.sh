#!/bin/bash

cut_freq_lakelarge100k() {
    echo "Ejecutando test lakelarge a 100k: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../img_in/lake-large.ppm"
    local output="$2../out/lake-large-100.ppm"
    local expected="$2../img_ex/cutfreq/lake-large-100K.ppm"
    local operation="cutfreq"
    local n=100000
    local comando="$ejecutable $input $output $operation $n"

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
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
    }

    read_ppm_maxcolorvalue() {
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies() {
        local file=$1
        local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de cutfreq FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de cutfreq PASSED: Las dimensiones son correctas"
    fi

    # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de cutfreq FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "Test de cutfreq FAILED: El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "Test de cutfreq PASSED: El número total de píxeles es correcto"
    fi
}

# Llamada a la función para ejecutar la prueba
cut_freq_lakelarge100k


cut_freq_lakelarge162k() {
    echo "Ejecutando test lakelarge a 162k: $1"
    local ejecutable="$2../build/imtool-soa/imtool-soa"
    local input="$2../img_in/lake-large.ppm"
    local output="$2../out/lake-large-100.ppm"
    local expected="$2../img_ex/cutfreq/lake-large-100K.ppm"
    local operation="cutfreq"
    local n=162000
    local comando="$ejecutable $input $output $operation $n"

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
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
    }

    read_ppm_maxcolorvalue() {
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies() {
        local file=$1
        local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Test de cutfreq FAILED: Dimensiones incorrectas"
        return 1
    else
        echo "Test de cutfreq PASSED: Las dimensiones son correctas"
    fi

    # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "Test de cutfreq FAILED: El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "Test de cutfreq FAILED: El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "Test de cutfreq PASSED: El número total de píxeles es correcto"
    fi
}

# Llamada a la función para ejecutar la prueba
cut_freq_lakelarge162k