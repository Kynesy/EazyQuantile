all: EazyQuantile RealQuantile UniformDistribution NormalDistribution ExponentialDistribution ZipfianDistribution

RealQuantile: RealQuantile.c
	$(CC) RealQuantile.c -o RealQuantile -lm

EazyQuantile: EazyQuantile.c
	$(CC) EazyQuantile.c -o EazyQuantile -lm

UniformDistribution: UniformDistribution.c
	$(CC) UniformDistribution.c -o UniformDistribution -lm

NormalDistribution: NormalDistribution.c
	$(CC) NormalDistribution.c -o NormalDistribution -lm

ExponentialDistribution: ExponentialDistribution.c
	$(CC) ExponentialDistribution.c -o ExponentialDistribution -lm

ZipfianDistribution: ZipfianDistribution.c
	$(CC) ZipfianDistribution.c -o ZipfianDistribution -lm

clean:
	rm -f NormalDistribution EazyQuantile RealQuantile UniformDistribution ExponentialDistribution ZipfianDistribution