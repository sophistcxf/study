perf script | FlameGraph-master/stackcollapse-perf.pl > perf.folded
FlameGraph-master/flamegraph.pl perf.folded > perf.svg
