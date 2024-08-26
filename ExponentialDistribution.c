#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LAMBDA 1.0  // Valore di lambda per la distribuzione esponenziale

int main(int argc, char *argv[]) {
    // Inizializza il generatore di numeri casuali
    srand(time(NULL));

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <array_length>\n", argv[0]);
        return 1;
    }
    int ARRAY_LENGTH = atoi(argv[1]);

    // Genera e stampa dati stream con distribuzione esponenziale
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        double u = (double)rand() / RAND_MAX;  // Genera un numero casuale tra 0 e 1
        double value = -log(1 - u) / LAMBDA;  // Calcola il valore secondo la distribuzione esponenziale
        fprintf(stdout, "%f\n", value);  // Aggiungere un newline per separare i valori
    }

    return 0;
}
