#ifndef BENCHMARK_HPP_
#define BENCHMARK_HPP_

#include <benchmark/benchmark.h>

#include "gendata.hpp"

template<uint64_t(*fn)(const uint32_t*,size_t,const uint32_t*,size_t)>
static void BM_benchmark(benchmark::State &state){
	GenData gd("newdata.dat"); //FIXME: hardcoded file name
	//for(auto _ : state){
	while(state.KeepRunning()){
		benchmark::DoNotOptimize(
			fn(gd.keys, gd.number_of_elements, gd.data, 1024)
		);
	}
}


#endif
