#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <clocale>
#include <chrono>
#include <algorithm>

#include "utils/gendata.hpp"

#include "binary_search/single.hpp"
#include "binary_search/plain_interleaving.hpp"
#include "binary_search/group_prefetching.hpp"

#ifdef __AVX2__
#include "binary_search/simd.hpp"
#endif


typedef uint64_t(*func_t)(const int32_t*,size_t,const int32_t*,size_t);


struct functions_t{
	const char *name;
	func_t fptr;
};
static const functions_t impls[] = {
	{"std::binary_search", stl_binary_search},
	{"std::lower_bound", stl_lower_bound},
	{"single", single},
	{"single2", single2},
#ifdef __AVX2__
	{"simd_avx2", simd_avx2},
	{"simd_avx2_interleaved2", simd_avx2_interleaved2},
#endif
	{"plain_interleaving4", plain_interleaving4},
	{"plain_interleaving8", plain_interleaving8},
	{"plain_interleaving<4>", plain_interleaving<4>},
	{"plain_interleaving<8>", plain_interleaving<8>},
	{"plain_interleaving<10>", plain_interleaving<10>},
	{"plain_interleaving<12>", plain_interleaving<12>},
	{"plain_interleaving<14>", plain_interleaving<14>},
	{"plain_interleaving<16>", plain_interleaving<16>},
	{"plain_interleaving<18>", plain_interleaving<18>},
	{"plain_interleaving<20>", plain_interleaving<20>},
	{"group_prefetching<4>", group_prefetching<4>},
	{"group_prefetching<8>", group_prefetching<8>},
	{"group_prefetching<10>", group_prefetching<10>},
	{"group_prefetching<12>", group_prefetching<12>},
	{"group_prefetching<14>", group_prefetching<14>},
	{"group_prefetching<16>", group_prefetching<16>},
	{"group_prefetching<18>", group_prefetching<18>},
	{"group_prefetching<20>", group_prefetching<20>},
};
static constexpr size_t impl_size = sizeof(impls)/sizeof(impls[0]);


static void probe(const char *desc, func_t fn, const GenData &gd, size_t keys, size_t repetitions){
	if(keys > gd.number_of_elements) keys = gd.number_of_elements;

	while(repetitions--){
		auto start = std::chrono::high_resolution_clock::now();

		uint64_t sum = fn(gd.keys, gd.number_of_elements, gd.data, keys);

		auto end = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		printf("%23s: %'10lu us; sum: %lu\n", desc, time, sum);
	}
}

static void print_usage(){
	puts(
R"(optional arguments:
	-i name          name of input file, default: newdata.dat
	-f name          include named function in run, see below, default: run all
	-k number        number of keys to search for, default: size of array
	-r number        number of repetitions, default: 1)"
	);

	puts("\nfunctions to run:");
	for(size_t i=0; i<impl_size; ++i){
		printf("%s\n", impls[i].name);
	}
}


int main(int argc, char **argv){
	// set locale for this program to whatever is set system-wide
	// otherwise digit separators with printf would not work
	setlocale(LC_NUMERIC, "");

	const char *fname = "newdata.dat";
	size_t runs[impl_size];
	size_t runs_size=0;
	size_t keys=0;
	size_t repetitions=1;
	bool run_all=true;

	int opt;
	while((opt=getopt(argc, argv, "i:f:k:r:h")) != -1){
		switch(opt){
			case 'i':
				fname = optarg;
				break;
			case 'f': {
				run_all = false;
				size_t i;
				for(i=0; i<impl_size; ++i){
					if(strncmp(optarg, impls[i].name, strlen(impls[i].name)+1) == 0){
						runs[runs_size++] = i;
						break;
					}
				}
				if(i == impl_size){
					printf("cannot find function %s\n", optarg);
					return -1;
				}
				break;
			}
			case 'k':
				keys = strtoull(optarg, nullptr, 10); //FIXME: error handling
				break;
			case 'r':
				repetitions = strtoull(optarg, nullptr, 10); //FIXME: error handling
				break;
			case 'h':
				print_usage();
				return 0;

			default:
				// abort when unknown option, getopt() already prints message
				return -1;
		}
	}

	GenData gd(fname);
	if(keys == 0) keys = gd.number_of_elements;

	// for now use data column as search keys which is not the best
	//TODO: get search keys which are not all hitting

	if(run_all){
		for(size_t i=0; i<impl_size; ++i){
			probe(impls[i].name, impls[i].fptr, gd, keys, repetitions);
		}
	}else{
		for(size_t i=0; i<runs_size; ++i){
			const auto &impl = impls[runs[i]];
			probe(impl.name, impl.fptr, gd, keys,repetitions);
		}
	}
	return 0;
}
