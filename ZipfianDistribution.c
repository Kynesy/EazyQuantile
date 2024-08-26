#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define N 100
#define S 1.1

// Calcola la funzione armonica generalizzata H_s per un dato valore di s
double harmonic_generalized(int n, double s) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / pow(i, s);
    }
    return sum;
}

// Funzione Zipf che genera numeri distribuiti secondo Zipf
int zipf(double s, int n) {
    static int first = 1;
    static double c = 0;
    static double *sum_probs;
    double z;
    int zipf_value;
    int i, low, high, mid;

    // Calcola la costante di normalizzazione alla prima chiamata
    if (first) {
        sum_probs = malloc((n + 1) * sizeof(*sum_probs));
        if (sum_probs == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        c = harmonic_generalized(n, s);

        sum_probs[0] = 0;
        for (i = 1; i <= n; i++) {
            sum_probs[i] = sum_probs[i - 1] + (1.0 / pow(i, s)) / c;
        }
        first = 0;
    }

    // Estrai un numero casuale uniforme (0 < z < 1)
    do {
        z = rand() / (RAND_MAX + 1.0);
    } while (z == 0 || z == 1);

    // Mappa z al valore usando la ricerca binaria
    low = 1;
    high = n;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (sum_probs[mid] >= z && sum_probs[mid - 1] < z) {
            zipf_value = mid;
            break;
        } else if (sum_probs[mid] >= z) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    assert(zipf_value >= 1 && zipf_value <= n);
    return zipf_value;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <array_length>\n", argv[0]);
        return 1;
    }

    int array_length = atoi(argv[1]);  // Numero di elementi da generare

    // Inizializza il generatore di numeri casuali
    srand(time(NULL));

    // Genera e stampa array di numeri distribuiti secondo Zipf
    for (int i = 0; i < array_length; i++) {
        int value = zipf(S, N);
        printf("%d\n", value);
    }

    return 0;
}
