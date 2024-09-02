#!/bin/python3

import sys
import matplotlib.pyplot as plt

class QuantilePlotter:
    def __init__(self, real_quantile, eazy_quantile):
        self.real_quantile = real_quantile
        self.eazy_quantile = eazy_quantile
        

    def plot(self):
        plt.figure(figsize=(10, 6))
        plt.plot(self.real_quantile, label='Real Quantile')
        plt.plot(self.eazy_quantile, label='Estimate Quantile')

        plt.title('Comparison of Real and EZQ Estimate Quantiles')
        plt.xlabel('N')
        plt.ylabel('Quantile Value')
        plt.legend()
        plt.grid(True)
        plt.show()

def main():
    if len(sys.argv) < 3:
        print("Usage: ./"+ sys.argv[0] +" [real_quantile_array] [eazy_quantile_array]")
        sys.exit(1)
    # parsing input
    real_quantile = list(map(float, sys.argv[1].split()))
    eazy_quantile = list(map(float, sys.argv[2].split()))

    # controllo lunghezza
    if len(real_quantile) != len(eazy_quantile):
        print("Error: Both arrays must have the same length.")
        sys.exit(1)

    # plot
    plotter = QuantilePlotter(real_quantile, eazy_quantile)
    plotter.plot()

if __name__ == "__main__":
    main()
