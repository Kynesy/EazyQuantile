#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef enum { MAX_MIN, AVERAGE } Mode;

/**
 * @brief Struttura per il calcolo delle stime basate su quantili.
 */
typedef struct {
    double quantile;           /**< Quantile utilizzato per il calcolo */
    double estimate_value;     /**< Valore stimato corrente */
    double sum_value;          /**< Somma dei valori osservati */
    double max_value;          /**< Valore massimo osservato */
    double min_value;          /**< Valore minimo osservato */
    int observed_num;          /**< Numero totale di valori osservati */
    int counter_low;           /**< Contatore dei valori inferiori alla stima */
    int counter_high;          /**< Contatore dei valori superiori alla stima */
    double threshold;          /**< Soglia per il cambio di valore stimato */
    double lamb;               /**< Parametro di aggiornamento per la stima */
    double avg;                /**< Media dei valori osservati */
    double toggle_threshold;   /**< Soglia per il cambio della modalità di calcolo */
    Mode mode;                 /**< Modalità di calcolo (MAX_MIN o AVERAGE) */
} EZQ;

/**
 * @brief Crea e inizializza una struttura EZQ.
 *
 * @param quantile Il quantile da utilizzare per il calcolo.
 * @return Puntatore alla struttura EZQ inizializzata.
 */
EZQ* create_EZQ(double quantile) {
    EZQ *ezq = (EZQ *)malloc(sizeof(EZQ));
    if (ezq == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    ezq->quantile = quantile;
    ezq->estimate_value = 0;
    ezq->sum_value = 0;
    ezq->max_value = -1;
    ezq->min_value = DBL_MAX;
    ezq->observed_num = 0;
    ezq->counter_low = 0;
    ezq->counter_high = 0;
    ezq->threshold = 0;
    ezq->lamb = 0;
    ezq->avg = 0;
    ezq->toggle_threshold = 0.7;
    ezq->mode = (quantile > ezq->toggle_threshold) ? MAX_MIN : AVERAGE;
    return ezq;
}

/**
 * @brief Aggiorna la struttura EZQ con un nuovo valore e calcola la nuova stima.
 *
 * @param ezq Puntatore alla struttura EZQ da aggiornare.
 * @param stream_data Nuovo valore da includere nella stima.
 * @return La nuova stima calcolata.
 */
double update(EZQ *ezq, double stream_data) {
    ezq->observed_num += 1;
    
    if (ezq->observed_num <= 1) {
        ezq->estimate_value = stream_data;
        return ezq->estimate_value;
    }

    ezq->threshold = ezq->observed_num * ezq->quantile;

    // Colleziona statistiche
    if (stream_data < ezq->min_value) {
        ezq->min_value = stream_data;
    } else if (stream_data > ezq->max_value) {
        ezq->max_value = stream_data;
    }
    ezq->sum_value += stream_data;

    // Toggle Mode
    if (ezq->mode == AVERAGE) {
        ezq->lamb = ((ezq->sum_value / (ezq->observed_num * 1.0)) / (ezq->observed_num - 1)) * 2;
    } else if (ezq->mode == MAX_MIN) {
        ezq->lamb = (ezq->max_value - ezq->min_value) / (ezq->observed_num - 1);
    } else {
        fprintf(stderr, "No such mode!\n");
        exit(1);
    }

    // Update
    if (stream_data <= ezq->estimate_value) {
        if (ezq->counter_low + 1 > ezq->threshold) {
            ezq->estimate_value -= ezq->lamb;
            ezq->counter_high += 1;
        } else {
            ezq->counter_low += 1;
        }
    } else if (stream_data > ezq->estimate_value) {
        if (ezq->counter_high + 1 > ezq->observed_num - ezq->threshold) {
            ezq->estimate_value += ezq->lamb;
            ezq->counter_low += 1;
        } else {
            ezq->counter_high += 1;
        }
    }

    return ezq->estimate_value;
}

/**
 * @brief Libera la memoria allocata per una struttura EZQ.
 *
 * @param ezq Puntatore alla struttura EZQ da liberare.
 */
void destroy_EZQ(EZQ *ezq) {
    free(ezq);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <quantile>\n", argv[0]);
        return 1;
    }
    double quantile = atof(argv[1]);
    double value;

    EZQ *ezq = create_EZQ(quantile);

    while (scanf("%lf", &value) == 1) {
        double estimate = update(ezq, value);
        printf("%f\n", estimate);
    }
    destroy_EZQ(ezq);

    return 0;
}
