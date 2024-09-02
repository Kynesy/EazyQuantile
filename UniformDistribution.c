#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <array_length>\n", argv[0]);
        return 1;
    }
    int ARRAY_LENGTH = atoi(argv[1]);

    // Genera e stampa dati stream con distribuzione uniforme tra 0 e 1
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        double value = (double)rand() / RAND_MAX;
        fprintf(stdout, "%f\n", value); 
    }

    return 0;
}
