#include <algorithm>
#include <random>
#include <vector>
#include <cstdio>

#include "utils/gendata.hpp"


static void print_usage(){
	puts(
R"(optional arguments:
	-o name          name of output file, default: newdata.dat
	-i number        seed to initialized random numbers, default: 42
	-s number        number of keys and data to generate, default: 256 * 2^20
	-l               enable logging of keys to text file keys.log)"
	);
}


int main(int argc, char **argv){
	const char *fname = "newdata.dat";
	size_t size = 256ULL*1024*1024;
	uint32_t seed = 42;
	bool logging = false;

	int opt;
	while((opt=getopt(argc, argv, "o:i:s:lh")) != -1){
		switch(opt){
			case 'o':
				fname = optarg;
				break;
			case 'i':
				seed = atoi(optarg); //FIXME: error handling
				break;
			case 's':
				size = strtoul(optarg, nullptr, 10); //FIXME: error handling
				break;
			case 'l':
				logging = true;
				break;
			case 'h':
				print_usage();
				return 0;

			default:
				// abort when unknown option, getopt() already prints message
				return -1;
		}
	}
	printf("seed: %u; size: %lu\n", seed, size);

	std::mt19937 gen;
	gen.seed(seed); // same seed everytime

	std::vector<int32_t> keys(size);
	for(auto &ele : keys){
		ele = gen();
	}
	std::sort(keys.begin(), keys.end());
	auto last = std::unique(keys.begin(), keys.end());
	keys.erase(last, keys.end());
	const size_t real_size = keys.size();
	printf("size without duplicates: %lu\n", real_size);

	if(logging){
		FILE *fd_log = fopen("keys.log", "w");
		for(const auto &ele : keys){
			fprintf(fd_log, "%u\n", ele);
		}
		fclose(fd_log);
	}

	// use keys as data too, but randomly shuffled
	std::vector<int32_t> data;
	data.reserve(real_size);
	std::copy(keys.begin(), keys.end(), std::back_inserter(data));
	std::shuffle(data.begin(), data.end(), gen);

	GenData::writeData(fname, real_size, keys.data(), data.data());

	return 0;
}
