#!/bin/bash

STREAM_LENGTH=4096
NUM_RUNS=100         
QUANTILES=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99)  
DISTRIBUTIONS=("NormalDistribution" "UniformDistribution" "ExponentialDistribution" "ZipfianDistribution") 


# è un array che contiene gli errori relativi con inidici (distribuzione,quantile)
declare -A total_relative_error

# inizializzazione a zero
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
            # prendiamo solo l'ultimo valore, così vediamo se è arrivato a convergenza
            real_quantile=$(echo "$stream_data" | ./RealQuantile $q | tail -n 1)
            eazy_quantile=$(echo "$stream_data" | ./EazyQuantile $q | tail -n 1)

            # calcoliamo l'errore relativo secondo la formula
            relative_error=$(echo "scale=10; ($eazy_quantile - $real_quantile) / $real_quantile" | bc -l)
            relative_error=$(echo "scale=10; if($relative_error < 0) $relative_error * -1 else $relative_error" | bc -l)

            # aggiungiamo l'errore relativo
            total_relative_error["$dist,$q"]=$(echo "${total_relative_error["$dist,$q"]} + $relative_error" | bc -l)
        done
    done
done

# per semplicità trasformiamo i dati per il file di plot
declare -A error_dict

for dist in "${DISTRIBUTIONS[@]}"; do
    error_list=() # lista errori per tutti i quantili di una distribuzione
    for q in "${QUANTILES[@]}"; do
        # media dei valori relativi
        average_relative_error=$(echo "${total_relative_error["$dist,$q"]} / $NUM_RUNS" | bc -l)
        error_list+=("$average_relative_error")
    done
    error_dict["$dist"]=$(IFS=,; echo "${error_list[*]}") # aggiungiamo la lista della distribuzione ai dati completi
done

quantiles_str=$(IFS=,; echo "${QUANTILES[*]}")
normal_errors_str=${error_dict[NormalDistribution]}
uniform_errors_str=${error_dict[UniformDistribution]}
exponential_errors_str=${error_dict[ExponentialDistribution]}
zipfian_errors_str=${error_dict[ZipfianDistribution]}

#plot
./plot_measurements.py "$quantiles_str" "$normal_errors_str" "$uniform_errors_str" "$exponential_errors_str" "$zipfian_errors_str" > /dev/null 2>&1
