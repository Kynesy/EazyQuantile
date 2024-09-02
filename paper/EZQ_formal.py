import math
from enum import Enum
import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt


class Mode(Enum):
    MAX_MIN = 1
    AVERAGE = 2


class EZQ:
    def __init__(self, quantile):
        self.quantile = quantile
        self.estimate_value = 0
        self.sum_value = 0
        self.max_value = -1
        self.min_value = math.inf
        self.observed_num = 0
        self.counter_low = 0
        self.counter_high = 0
        self.threshold = 0
        self.lamb = 0
        self.avg = 0
        self.toggle_threshold = 0.7
        self.mode = Mode.MAX_MIN if quantile > self.toggle_threshold else Mode.AVERAGE

        self.stored_stream_data_list = []  # only for plot

    # only for plot
    def real_quantile(self):
        return np.percentile(self.stored_stream_data_list, self.quantile * 100, method="linear")

    def update(self, stream_data):
        self.observed_num += 1
        self.stored_stream_data_list.append(stream_data)

        # First stream node
        if self.observed_num <= 1:
            self.estimate_value = stream_data
            return self.estimate_value

        self.threshold = self.observed_num * self.quantile

        # Collect statistics
        if stream_data < self.min_value:
            self.min_value = stream_data
        elif stream_data > self.max_value:
            self.max_value = stream_data
        self.sum_value += stream_data

        # Toggle Mode
        if self.mode == Mode.AVERAGE:
            self.lamb = ((self.sum_value / (self.observed_num * 1.0)) / (self.observed_num - 1)) * 2
        elif self.mode == Mode.MAX_MIN:
            self.lamb = (self.max_value - self.min_value) / (self.observed_num)
        else:
            raise Exception("No such mode! ")

        # Update
        if stream_data <= self.estimate_value:
            if self.counter_low + 1 > self.threshold:
                self.estimate_value -= self.lamb
                self.counter_high += 1
            else:
                self.counter_low += 1
        elif stream_data > self.estimate_value:
            if self.counter_high + 1 > self.observed_num - self.threshold:
                self.estimate_value += self.lamb
                self.counter_low += 1
            else:
                self.counter_high += 1

        return self.estimate_value


# Example: normal distribution X~N(0.5,0.125)
if __name__ == "__main__":
    array_length = 4096
    quantile = 0.9
    lower, upper = 0, 1
    mu, sigma = 0.5, 0.125

    # X represents the normal distribution with maximum and minimum constraints
    X = stats.truncnorm(
        (lower - mu) / sigma, (upper - mu) / sigma, loc=mu, scale=sigma)  # Random numbers with interval restrictions
    # Generate stream data
    stream_data = []
    aa = X.rvs(array_length)
    for i in aa:
        stream_data.append(i)

    # Update
    real_quantile_list = []
    estimate_quantile_list = []
    ezq = EZQ(quantile=quantile)
    for i in stream_data:
        estimate_quantile_list.append(ezq.update(i))
        real_quantile_list.append(ezq.real_quantile())

    plt.figure()
    plt.plot(real_quantile_list, label="Real value")
    plt.plot(estimate_quantile_list, label="Est value")
    plt.xlabel("N", size=14)
    plt.ylabel("Est Value & Real Value", size=14)
    plt.legend()
    plt.show()
