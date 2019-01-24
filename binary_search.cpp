#include <cstdio>
#include <chrono>
#include <algorithm>

#include "utils/gendata.hpp"

#include "binary_search/single.hpp"
#include "binary_search/plain_interleaving.hpp"
#include "binary_search/group_prefetching.hpp"


typedef uint64_t(*func_t)(const uint32_t*,size_t,const uint32_t*,size_t);

static void probe(const char *desc, func_t fn, const GenData &gd){
	auto start = std::chrono::high_resolution_clock::now();

	uint64_t sum = fn(gd.keys, gd.number_of_elements, gd.data, 1024*1024);
	//uint64_t sum = fn(gd.keys, gd.number_of_elements, gd.data, gd.number_of_elements);

	auto end = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	printf("%20s: %4lu us; sum: %lu\n", desc, time, sum);
}

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr, "requires import file as first argument\n");
		return -1;
	}

	GenData gd(argv[1]);

	// for now use data column as search keys which is not the best
	//TODO: get search keys which are not all hitting

	probe("std::binary_search", stl_binary_search, gd);
	probe("std::lower_bound", stl_lower_bound, gd);
	probe("single", single, gd);
	probe("single2", single2, gd);
	probe("plain_interleaving4", plain_interleaving4, gd);
	probe("plain_interleaving8", plain_interleaving8, gd);
	probe("plain_interleaving<4", plain_interleaving<4>, gd);
	probe("plain_interleaving<8", plain_interleaving<8>, gd);
	probe("group_prefetching<4>", group_prefetching<4>, gd);
	probe("group_prefetching<8>", group_prefetching<8>, gd);

	return 0;
}
