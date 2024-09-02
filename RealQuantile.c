#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Struttura che rappresenta un flusso di dati.
 *
 * Questa struttura contiene un array dinamico di dati e la lunghezza attuale dell'array.
 */
typedef struct {
    double *data;  /**< Puntatore all'array di dati. */
    int length;    /**< Lunghezza dell'array di dati. */
} StreamData;

/**
 * @brief Crea una nuova istanza di StreamData.
 *
 * Questa funzione alloca memoria per una nuova struttura StreamData e inizializza i campi
 * `data` e `length` a `NULL` e `0` rispettivamente.
 *
 * @return Puntatore alla nuova struttura StreamData.
 */
StreamData* create_StreamData() {
    StreamData *streamData = (StreamData *)malloc(sizeof(StreamData));
    streamData->data = NULL;
    streamData->length = 0;
    return streamData;
}

/**
 * @brief Aggiunge un valore alla struttura StreamData.
 *
 * Questa funzione ridimensiona dinamicamente l'array `data` per aggiungere un nuovo valore.
 * In caso di errore di allocazione della memoria, il programma termina.
 *
 * @param streamData Puntatore alla struttura StreamData a cui aggiungere il valore.
 * @param value Valore da aggiungere alla struttura.
 */
void add_value(StreamData *streamData, double value) {
    streamData->data = realloc(streamData->data, (streamData->length + 1) * sizeof(double));
    if (streamData->data == NULL) {
        fprintf(stderr, "Errore di allocazione memoria!\n");
        exit(EXIT_FAILURE);
    }
    streamData->data[streamData->length] = value;
    streamData->length++;
}

/**
 * @brief Libera la memoria associata a una struttura StreamData.
 *
 * Questa funzione libera la memoria allocata per l'array `data` e per la struttura StreamData stessa.
 *
 * @param streamData Puntatore alla struttura StreamData da distruggere.
 */
void destroy_StreamData(StreamData *streamData) {
    free(streamData->data);
    free(streamData);
}

/**
 * @brief Funzione di confronto per l'ordinamento dei dati.
 *
 * Questa funzione viene utilizzata da `qsort` per confrontare due valori `double`.
 *
 * @param a Puntatore al primo valore da confrontare.
 * @param b Puntatore al secondo valore da confrontare.
 * @return -1 se `a` è minore di `b`, 1 se `a` è maggiore di `b`, 0 se sono uguali.
 */
int compare(const void *a, const void *b) {
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

/**
 * @brief Calcola il quantile di un array di dati.
 *
 * Questa funzione ordina l'array `data`, quindi calcola il quantile specificato utilizzando
 * l'interpolazione lineare se necessario.
 *
 * @param data Array di dati su cui calcolare il quantile.
 * @param length Lunghezza dell'array di dati.
 * @param quantile Il quantile desiderato (valore tra 0 e 1).
 * @return Il valore del quantile calcolato.
 */
double calculate_quantile(double data[], int length, double quantile) {
    qsort(data, length, sizeof(double), compare); // ORDINAMENTO
    
    double index = quantile * (length - 1);
    int lower_index = (int)index;
    int upper_index = lower_index + 1;
    double fraction = index - lower_index;

    // Se l'indice è intero, restituisci il valore corrispondente
    if (fraction == 0) {
        return data[lower_index];
    } else {
        // Altrimenti interpolazione tra i due valori
        return data[lower_index] * (1 - fraction) + data[upper_index] * fraction;
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <quantile>\n", argv[0]);
        return 1;
    }
    double quantile = atof(argv[1]);

    double value;
    StreamData *data = create_StreamData();

    while (scanf("%lf", &value) == 1) {
        add_value(data, value);

        double realQuantile = calculate_quantile(data->data, data->length, quantile);
        fprintf(stdout, "%f\n", realQuantile);
    }
    destroy_StreamData(data);

    return 0;
}
