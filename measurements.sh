#!/bin/bash

STREAM_LENGTH=4096
NUM_RUNS=100         
QUANTILES=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99)  
DISTRIBUTIONS=("NormalDistribution" "UniformDistribution" "ExponentialDistribution" "ZipfianDistribution") 

declare -A total_relative_error

for dist in "${DISTRIBUTIONS[@]}"; do
    for q in "${QUANTILES[@]}"; do
        total_relative_error["$dist,$q"]=0
    done
done

for dist in "${DISTRIBUTIONS[@]}"; do
    echo "Running tests for distribution: $dist" >&2
    
    for ((run=1; run<=NUM_RUNS; run++))
    do
        stream_data="$("./$dist" $STREAM_LENGTH)"

        for q in "${QUANTILES[@]}"
        do
            # Calcola il quantile reale ed estimato
            real_quantile=$(echo "$stream_data" | ./RealQuantile $q | tail -n 1)
            eazy_quantile=$(echo "$stream_data" | ./EazyQuantile $q | tail -n 1)

            # Calcola l'errore relativo per questo quantile
            relative_error=$(echo "scale=10; ($eazy_quantile - $real_quantile) / $real_quantile" | bc -l)
            relative_error=$(echo "scale=10; if($relative_error < 0) $relative_error * -1 else $relative_error" | bc -l)

            # Accumula l'errore relativo
            total_relative_error["$dist,$q"]=$(echo "${total_relative_error["$dist,$q"]} + $relative_error" | bc -l)
        done
    done
done

declare -A error_dict

# Dati per il plot
for dist in "${DISTRIBUTIONS[@]}"; do
    error_list=()
    for q in "${QUANTILES[@]}"; do
        average_relative_error=$(echo "${total_relative_error["$dist,$q"]} / $NUM_RUNS" | bc -l)
        error_list+=("$average_relative_error")
    done
    error_dict["$dist"]=$(IFS=,; echo "${error_list[*]}")
done

quantiles_str=$(IFS=,; echo "${QUANTILES[*]}")
normal_errors_str=${error_dict[NormalDistribution]}
uniform_errors_str=${error_dict[UniformDistribution]}
exponential_errors_str=${error_dict[ExponentialDistribution]}
zipfian_errors_str=${error_dict[ZipfianDistribution]}

./plot_measurements.py "$quantiles_str" "$normal_errors_str" "$uniform_errors_str" "$exponential_errors_str" "$zipfian_errors_str" > /dev/null 2>&1
