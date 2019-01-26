#ifndef GROUP_PREFETCHING_HPP_
#define GROUP_PREFETCHING_HPP_

// process batch of searches at once, split search in 2 stages and interleave stages of searches in batch

template<size_t group_size>
uint64_t group_prefetching(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		const int32_t *arr_first[group_size];
		for(size_t g=0; g<group_size; ++g){
			arr_first[g] = array;
		}
		size_t count = array_size;
		while(size_t step = count / 2){
			// prefetch stage
			for(size_t g=0; g<group_size; ++g){
				const int32_t *it = arr_first[g] + step;
				__builtin_prefetch(it, 0, 0);
			}
			// load stage
			for(size_t g=0; g<group_size; ++g){
				const int32_t *it = arr_first[g] + step;
				if(*it < keys[i + g]){
					arr_first[g] = it;
				}
			}
			count -= step;
		}
		for(uint64_t g=0; g<group_size; ++g){
			if((count==1) && (*arr_first[g] < keys[i + g])) arr_first[g]++;
			if(*arr_first[g] == keys[i + g]){
				sum += keys[i + g];
			}
		}
	}
	// tail handling
	const int32_t *array_end  = array + array_size;
	for(; i<keys_size; ++i){
		if(binsearch_single2(array, array_end, keys[i])){
			sum += keys[i];
		}
	}
	return sum;
}

#endif
