#!/bin/bash

resize_deer-large100(){
    echo "Ejecutando test resize a deer large a 100x100: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-large.ppm"
    local output="$2../out/deer-large-resize100.ppm"
    local expected="$2../exp/resize/deer-large-100.ppm"
    local operation="resize"
    local n1=100
    local n2=100
    local comando="$ejecutable $input $output $operation $n1 $n2"

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

resize_deer-large1000(){
    echo "Ejecutando test resize a deer large a 1000x1000: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-large.ppm"
    local output="$2../out/deer-large-resize1000.ppm"
    local expected="$2../exp/resize/deer-large-1000.ppm"
    local operation="resize"
    local n1=1000
    local n2=1000
    local comando="$ejecutable $input $output $operation $n1 $n2"

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

resize_deer-small100(){
    echo "Ejecutando test resize a deer small a 100x100: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small-resize100.ppm"
    local expected="$2../exp/resize/deer-small-100.ppm"
    local operation="resize"
    local n1=100
    local n2=100
    local comando="$ejecutable $input $output $operation $n1 $n2"

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

resize_deer-small1000(){
    echo "Ejecutando test resize a deer small a 1000x1000: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/deer-small.ppm"
    local output="$2../out/deer-small-resize1000.ppm"
    local expected="$2../exp/resize/deer-small-1000.ppm"
    local operation="resize"
    local n1=1000
    local n2=1000
    local comando="$ejecutable $input $output $operation $n1 $n2"

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


resize_lake-large100(){

    echo "Ejecutando test resize a lake large a 100x100: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-large.ppm"
    local output="$2../out/lake.large-resize100.ppm"
    local expected="$2../exp/resize/lake-large-100.ppm"
    local operation="resize"
    local n1=100
    local n2=100
    local comando="$ejecutable $input $output $operation $n1 $n2"

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

resize_lake-large1000(){
    echo "Ejecutando test resize a lake large a 1000x1000: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-large.ppm"
    local output="$2../out/lake.large-resize1000.ppm"
    local expected="$2../exp/resize/lake-large-1000.ppm"
    local operation="resize"
    local n1=1000
    local n2=1000
    local comando="$ejecutable $input $output $operation $n1 $n2"

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


resize_lake-small100(){
    echo "Ejecutando test resize a lake small a 100x100: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small-resize100.ppm"
    local expected="$2../exp/resize/lake-small-100.ppm"
    local operation="resize"
    local n1=100
    local n2=100
    local comando="$ejecutable $input $output $operation $n1 $n2"

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

resize_lake-small1000(){
    echo "Ejecutando test resize a lake small a 1000x1000: $1"
    local ejecutable="$2../build/imtool-aos/imtool-aos"
    local input="$2../in/lake-small.ppm"
    local output="$2../out/lake-small-resize1000.ppm"
    local expected="$2../exp/resize/lake-small-1000.ppm"
    local operation="resize"
    local n1=1000
    local n2=1000
    local comando="$ejecutable $input $output $operation $n1 $n2"

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



resize_deer-large100 "$@"
resize_deer-large1000 "$@"
resize_deer-small100 "$@"
resize_deer-small1000 "$@"
resize_lake-large100 "$@"
resize_lake-large1000 "$@"
resize_lake-small100 "$@"
resize_lake-small1000 "$@"
