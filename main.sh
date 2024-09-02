#!/bin/bash

STREAM_LENGHT=4096
QUANTILE=0.99

usage() {
    echo "Usage: $0 [-s] [-c] [-n] [-u] [-e] [-z]" 1>&2
    echo "  -s  Setup the project" 1>&2
    echo "  -c  Clean the project" 1>&2
    echo "  -n  Test with normal distribution" 1>&2
    echo "  -u  Test with uniform distribution" 1>&2
    echo "  -e  Test with exponential distribution" 1>&2
    echo "  -z  Test with Zipfian distribution" 1>&2
    exit 1
}


setup() {
    echo "Setting up the project..."
    make all
    pip install -r requirements.txt
    echo "Setup complete."
}

clean() {
    echo "Cleaning the project..."
    make clean
    pip uninstall -r requirements.txt
    echo "Clean complete."
}

testNormal() {
    echo "Testing with normal distribution..."
    stream_data="$(./NormalDistribution $STREAM_LENGHT)"

    real_quantile="$(echo "$stream_data" | ./RealQuantile $QUANTILE)"
    eazy_quantile="$(echo "$stream_data" | ./EazyQuantile $QUANTILE)"

    ./plot.py "$(echo "$real_quantile" | tr '\n' ' ')" "$(echo "$eazy_quantile" | tr '\n' ' ')"
    echo "Normal distribution test complete."
}

testUniform() {
    echo "Testing with uniform distribution..."
    stream_data="$(./UniformDistribution $STREAM_LENGHT)"

    real_quantile="$(echo "$stream_data" | ./RealQuantile $QUANTILE)"
    eazy_quantile="$(echo "$stream_data" | ./EazyQuantile $QUANTILE)"

    ./plot.py "$(echo "$real_quantile" | tr '\n' ' ')" "$(echo "$eazy_quantile" | tr '\n' ' ')"
    echo "Uniform distribution test complete."
}

testExponential() {
    echo "Testing with exponential distribution..."
    stream_data="$(./ExponentialDistribution $STREAM_LENGHT)"

    real_quantile="$(echo "$stream_data" | ./RealQuantile $QUANTILE)"
    eazy_quantile="$(echo "$stream_data" | ./EazyQuantile $QUANTILE)"

    ./plot.py "$(echo "$real_quantile" | tr '\n' ' ')" "$(echo "$eazy_quantile" | tr '\n' ' ')"
    echo "Exponential distribution test complete."
}

testZipfian() {
    echo "Testing with Zipfian distribution..."
    stream_data="$(./ZipfianDistribution $STREAM_LENGHT)"

    real_quantile="$(echo "$stream_data" | ./RealQuantile $QUANTILE)"
    eazy_quantile="$(echo "$stream_data" | ./EazyQuantile $QUANTILE)"

    ./plot.py "$(echo "$real_quantile" | tr '\n' ' ')" "$(echo "$eazy_quantile" | tr '\n' ' ')"
    echo "Zipfian distribution test complete."
}

while getopts ":sunezc" opt; do
    case $opt in
        s)
            setup
            ;;
        n)
            testNormal
            ;;
        u)
            testUniform
            ;;
        e)
            testExponential
            ;;
        z)
            testZipfian
            ;;
        c)
            clean
            ;;
        \?)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ $OPTIND -eq 1 ]; then
    usage
fi
