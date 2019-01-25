#include <algorithm>
#include <random>
#include <vector>
#include <cstdio>

#include "utils/gendata.hpp"


int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr, "requires file name as first argument\n");
		return -1;
	}

	std::mt19937 gen;
	gen.seed(42);

	const size_t size = 4ULL*1024*1024*1024;
	std::vector<uint32_t> keys(size);
	for(auto &ele : keys){
		ele = gen();
	}
	std::sort(keys.begin(), keys.end());
	auto last = std::unique(keys.begin(), keys.end());
	keys.erase(last, keys.end());
	const size_t real_size = keys.size();
	printf("size without duplicates: %lu\n", real_size);

#ifndef NDEBUG
	FILE *fd_log = fopen("keys.log", "w");
	for(const auto &ele : keys){
		fprintf(fd, "%u\n", ele);
	}
	fclose(fd);
#endif

	// use keys as data too, but randomly shuffled
	std::vector<uint32_t> data;
	data.reserve(real_size);
	std::copy(keys.begin(), keys.end(), std::back_inserter(data));
	std::shuffle(data.begin(), data.end(), gen);

	GenData::writeData(argv[1], real_size, keys.data(), data.data());

	return 0;
}
