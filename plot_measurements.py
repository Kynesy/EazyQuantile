#!/bin/python3

import matplotlib.pyplot as plt
import sys

# parsing dei dati
def parse_array(arg):
    try:
        return [float(e) for e in arg.split(',') if e.strip()]
    except ValueError:
        raise ValueError(f"Error parsing array from string: '{arg}'")

def main():
    if len(sys.argv) != 6:
        print("Usage: python plot_measurements.py <quantiles> <normal_errors> <uniform_errors> <exponential_errors> <zipfian_errors>")
        sys.exit(1)

    # leggiamo i dati
    quantiles_str = sys.argv[1]
    normal_errors_str = sys.argv[2]
    uniform_errors_str = sys.argv[3]
    exponential_errors_str = sys.argv[4]
    zipfian_errors_str = sys.argv[5]

    # usiamo il parsing scritto prima
    try:
        quantiles = parse_array(quantiles_str)
        normal_errors = parse_array(normal_errors_str)
        uniform_errors = parse_array(uniform_errors_str)
        exponential_errors = parse_array(exponential_errors_str)
        zipfian_errors = parse_array(zipfian_errors_str)
    except ValueError as e:
        print(e)
        sys.exit(1)

    # assicuriamoci che ci siano i dati per ogni quantile
    length = len(quantiles)
    if not (len(normal_errors) == len(uniform_errors) == len(exponential_errors) == len(zipfian_errors) == length):
        print("Error: All error arrays must be of the same length as quantiles.")
        sys.exit(1)

    # aggiungiamo un piccola costante per prevenire errore in caso l'errore sia zero
    epsilon = 1e-10
    normal_errors = [error + epsilon for error in normal_errors]
    uniform_errors = [error + epsilon for error in uniform_errors]
    exponential_errors = [error + epsilon for error in exponential_errors]
    zipfian_errors = [error + epsilon for error in zipfian_errors]

    # plot
    plt.figure(figsize=(10, 6))

    plt.plot(quantiles, uniform_errors, label='Uniform Distribution', marker='o')
    plt.plot(quantiles, normal_errors, label='Normal Distribution', marker='o')
    plt.plot(quantiles, exponential_errors, label='Exponential Distribution', marker='o')
    plt.plot(quantiles, zipfian_errors, label='Zipfian Distribution', marker='o')

    plt.yscale('log') # scala logaritmica
    plt.xlabel('Quantiles', size=14)
    plt.ylabel('Average Relative Error (ARE)', size=14)
    plt.title('Average Relative Error (ARE) by Quantile and Distribution', size=16)
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    
    # impostiamo i quantili sull'asse x
    plt.xticks(ticks=quantiles, labels=[f'{q:.2f}' for q in quantiles])
    plt.yticks([1, 0.1, 0.01, 0.001, 0.0001], ['10^0', '10^-1', '10^-2', '10^-3', '10^-4'])

    # salviamo il plot
    plt.savefig('error_plot.png')
    plt.show()

if __name__ == '__main__':
    main()
