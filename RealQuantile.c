#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double *data;
    int length;
} StreamData;

StreamData* create_StreamData() {
    StreamData *streamData = (StreamData *)malloc(sizeof(StreamData));
    streamData->data = NULL;
    streamData->length = 0;
    return streamData;
}

void add_value(StreamData *streamData, double value) {
    streamData->data = realloc(streamData->data, (streamData->length + 1) * sizeof(double));
    if (streamData->data == NULL) {
        fprintf(stderr, "Errore di allocazione memoria!\n");
        exit(EXIT_FAILURE);
    }
    streamData->data[streamData->length] = value;
    streamData->length++;
}

void destroy_StreamData(StreamData *streamData) {
    free(streamData->data);
    free(streamData);
}

// Funzione di confronto per qsort
int compare(const void *a, const void *b) {
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

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
