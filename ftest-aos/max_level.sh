#!/bin/bash

max_level_deer-small-255(){
    echo "Ejecutando test maxlevel a deer small a 255: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small255.ppm"
    local expected="$2../exp/maxlevel/deer-small-255.ppm"
    local operation="maxlevel"
    local n=255
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

   read_ppm_dimensions(){
        local file=$1
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
   }
     
   read_ppm_maxcolorvalue(){
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies(){
        local file=$1
         local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Dimensiones incorrectas"
        return 1
    else
         echo "Las dimensiones son correctas"
    fi
     # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "El número total de píxeles es correcto"
    fi
}


max_level_deer-small-65535(){
    echo "Ejecutando test maxlevel deer small a 65535: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small255.ppm"
    local expected="$2../exp/maxlevel/deer-small-65535.ppm"
    local operation="maxlevel"
    local n=65535
    local comando="$ejecutable $input $output $operation $n"

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

   read_ppm_dimensions(){
        local file=$1
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
   }
     
   read_ppm_maxcolorvalue(){
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies(){
        local file=$1
         local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Dimensiones incorrectas"
        return 1
    else
         echo "Las dimensiones son correctas"
    fi
     # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "El número total de píxeles es correcto"
    fi
}


max_level_lake-small-255(){
    echo "Ejecutando test maxlevel lake small a 255: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small255.ppm"
    local expected="$2../exp/maxlevel/lake-small-255.ppm"
    local operation="maxlevel"
    local n=255
    local comando="$ejecutable $input $output $operation $n"

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

   read_ppm_dimensions(){
        local file=$1
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
   }
     
   read_ppm_maxcolorvalue(){
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies(){
        local file=$1
         local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Dimensiones incorrectas"
        return 1
    else
         echo "Las dimensiones son correctas"
    fi
     # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "El número total de píxeles es correcto"
    fi
}



max_level_lake-small-65535(){
    echo "Ejecutando test maxlevel lake small a 65535: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small65535.ppm"
    local expected="$2../exp/maxlevel/lake-small-65535.ppm"
    local operation="maxlevel"
    local n=65535
    local comando="$ejecutable $input $output $operation $n"

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

   read_ppm_dimensions(){
        local file=$1
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
   }
     
   read_ppm_maxcolorvalue(){
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies(){
        local file=$1
         local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Dimensiones incorrectas"
        return 1
    else
         echo "Las dimensiones son correctas"
    fi
     # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "El número total de píxeles es correcto"
    fi
}



max_level_lake-small-65535vs255(){
    echo "Ejecutando test maxlevel lake small a 65535: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small65535.ppm"
    local expected="$2../exp/maxlevel/lake-small-255.ppm"
    local operation="maxlevel"
    local n=65535
    local comando="$ejecutable $input $output $operation $n"

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

   read_ppm_dimensions(){
        local file=$1
        local width=$(head -n 2 $file | tail -n 1 | cut -d " " -f 1)
        local height=$(head -n 2 $file | tail -n 1 | cut -d " " -f 2)
        echo "$width $height"
   }
     
   read_ppm_maxcolorvalue(){
        local file=$1
        local maxcolorvalue=$(head -n 3 $file | tail -n 1)
        echo "$maxcolorvalue"
    }

    read_ppm_frequencies(){
        local file=$1
         local frequencies=$(tail -n +4 "$file" | tr -d '\0' | sort | uniq -c | sort -nr)
        echo "$frequencies"
    }

    local dim_output=$(read_ppm_dimensions $output)
    local dim_expected=$(read_ppm_dimensions $expected)
    if [ "$dim_output" != "$dim_expected" ]; then
        echo "Dimensiones incorrectas"
        return 1
    else
         echo "Las dimensiones son correctas"
    fi
     # Comparar maxcolorvalue
    local maxcolorvalue_output=$(read_ppm_maxcolorvalue "$output")
    local maxcolorvalue_expected=$(read_ppm_maxcolorvalue "$expected")

    if [ "$maxcolorvalue_output" != "$maxcolorvalue_expected" ]; then
        echo "El valor máximo de color no coincide: $maxcolorvalue_output vs $maxcolorvalue_expected. Test exitoso (compara 65535 con 255)"
        return 1
    fi

    # Comparar número total de píxeles
    local total_pixels_output=$(tail -n +4 "$output" | wc -c)
    local total_pixels_expected=$(tail -n +4 "$expected" | wc -c)

    if [ "$total_pixels_output" != "$total_pixels_expected" ]; then
        echo "El número total de píxeles no coincide: $total_pixels_output vs $total_pixels_expected"
        return 1
    else
        echo "El número total de píxeles es correcto"
    fi
}





max_level_deer-small-255 "$@"
max_level_deer-small-65535 "$@"
max_level_lake-small-255 "$@"
max_level_lake-small-65535 "$@"
max_level_lake-small-65535vs255 "$@"