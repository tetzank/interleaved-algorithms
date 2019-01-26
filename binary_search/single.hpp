#ifndef SINGLE_HPP_
#define SINGLE_HPP_

// doing a single binary one after the other

#include <algorithm>


uint64_t stl_binary_search(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
	uint64_t sum=0;
	const int32_t *array_end  = array + array_size;
	for(const int32_t *ptr=keys, *end=keys+keys_size; ptr!=end; ptr++){
		bool found = std::binary_search(array, array_end, *ptr);
		if(found){
			sum += *ptr;
		}
	}
	return sum;
}

uint64_t stl_lower_bound(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
	uint64_t sum=0;
	const int32_t *array_end  = array + array_size;
	for(const int32_t *ptr=keys, *end=keys+keys_size; ptr!=end; ptr++){
		auto it = std::lower_bound(array, array_end, *ptr);
		if(*it == *ptr){
			sum += *ptr;
		}
	}
	return sum;
}

static bool binsearch_single(const int32_t *first, const int32_t *last, int32_t key){
	size_t count = last - first;
	while(count > 0){
		size_t step = count / 2;
		const int32_t *it = first + step; 
		if(*it < key){
			first = it + 1;
			count = count - step - 1; 
		}else{
			count = step;
		}
	}
    return *first == key;
}
uint64_t single(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
	uint64_t sum=0;
	const int32_t *array_end  = array + array_size;
	for(const int32_t *ptr=keys, *end=keys+keys_size; ptr!=end; ptr++){
		bool found = binsearch_single(array, array_end, *ptr);
		if(found){
			sum += *ptr;
		}
	}
	return sum;
}

static bool binsearch_single2(const int32_t *first, const int32_t *last, int32_t key){
	size_t count = last - first;
	while(size_t step=count/2){
		const int32_t *it = first + step; 
		if(*it < key){
			first = it;
		}
		count -= step;
	}
	if((count==1) && (*first < key)) first++;
    return *first == key;
}
uint64_t single2(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
	uint64_t sum=0;
	const int32_t *array_end  = array + array_size;
	for(const int32_t *ptr=keys, *end=keys+keys_size; ptr!=end; ptr++){
		bool found = binsearch_single2(array, array_end, *ptr);
		if(found){
			sum += *ptr;
		}
	}
	return sum;
}


#endif
