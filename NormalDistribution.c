#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

# define M_PI 3.14159265358979323846
#define LOWER 0
#define UPPER 1
#define MU 0.5
#define SIGMA 0.125

// Funzione per generare numeri casuali con distribuzione normale (Box-Muller Transform)
double rand_normal(double mean, double stddev) {
    double u1 = ((double)rand() / RAND_MAX);
    double u2 = ((double)rand() / RAND_MAX);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return z0 * stddev + mean;
}

// Funzione per generare numeri casuali con distribuzione normale troncata
double truncnorm(double lower, double upper, double mean, double stddev) {
    double x;
    do {
        x = rand_normal(mean, stddev);
    } while (x < lower || x > upper);
    return x;
}

int main(int argc, char *argv[]) {
    // Inizializza il generatore di numeri casuali
    srand(time(NULL));

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <array_length>\n", argv[0]);
        return 1;
    }
    int ARRAY_LENGTH = atoi(argv[1]);


    // Genera e stampa dati stream con distribuzione normale troncata
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        double value = truncnorm(LOWER, UPPER, MU, SIGMA);
        fprintf(stdout, "%f\n", value);  // Aggiungere un newline per separare i valori
    }

    return 0;
}
