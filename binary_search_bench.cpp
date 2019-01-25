#include "utils/benchmark.hpp"

#include "binary_search/single.hpp"
#include "binary_search/plain_interleaving.hpp"
#include "binary_search/group_prefetching.hpp"


BENCHMARK_TEMPLATE(BM_benchmark, stl_binary_search);
BENCHMARK_TEMPLATE(BM_benchmark, stl_lower_bound);
BENCHMARK_TEMPLATE(BM_benchmark, single);
BENCHMARK_TEMPLATE(BM_benchmark, single2);

BENCHMARK_TEMPLATE(BM_benchmark, plain_interleaving4);
BENCHMARK_TEMPLATE(BM_benchmark, plain_interleaving8);
BENCHMARK_TEMPLATE(BM_benchmark, plain_interleaving<4>);
BENCHMARK_TEMPLATE(BM_benchmark, plain_interleaving<8>);

BENCHMARK_TEMPLATE(BM_benchmark, group_prefetching<4>);
BENCHMARK_TEMPLATE(BM_benchmark, group_prefetching<8>);

BENCHMARK_MAIN();
