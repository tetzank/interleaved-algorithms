#ifndef PLAIN_INTERLEAVING_HPP_
#define PLAIN_INTERLEAVING_HPP_

// process batch of search at once, interleaving each operation in search one after the other

uint64_t plain_interleaving4(const uint32_t *array, size_t array_size, const uint32_t *keys, size_t keys_size){
	const size_t group_size=4;
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		const uint32_t *first1=array, *first2=array, *first3=array, *first4=array;
		size_t count = array_size;
		while(size_t step = count / 2){
			const uint32_t *it1 = first1 + step; 
			const uint32_t *it2 = first2 + step; 
			const uint32_t *it3 = first3 + step; 
			const uint32_t *it4 = first4 + step; 
			if(*it1 < keys[i+0]) first1 = it1;
			if(*it2 < keys[i+1]) first2 = it2;
			if(*it3 < keys[i+2]) first3 = it3;
			if(*it4 < keys[i+3]) first4 = it4;
			count -= step;
		}
		if(count==1){
			if(*first1 < keys[i+0]) first1++;
			if(*first2 < keys[i+1]) first2++;
			if(*first3 < keys[i+2]) first3++;
			if(*first4 < keys[i+3]) first4++;
		}
		if(*first1 == keys[i+0]) sum += keys[i+0];
		if(*first2 == keys[i+1]) sum += keys[i+1];
		if(*first3 == keys[i+2]) sum += keys[i+2];
		if(*first4 == keys[i+3]) sum += keys[i+3];
	}
	// tail handling
	const uint32_t *array_end  = array + array_size;
	for(; i<keys_size; ++i){
		if(binsearch_single2(array, array_end, keys[i])){
			sum += keys[i];
		}
	}
	return sum;
}
uint64_t plain_interleaving8(const uint32_t *array, size_t array_size, const uint32_t *keys, size_t keys_size){
	const size_t group_size=8;
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		const uint32_t *first1=array, *first2=array, *first3=array, *first4=array,
			*first5=array, *first6=array, *first7=array, *first8=array;
		size_t count = array_size;
		while(size_t step = count / 2){
			const uint32_t *it1 = first1 + step; 
			const uint32_t *it2 = first2 + step; 
			const uint32_t *it3 = first3 + step; 
			const uint32_t *it4 = first4 + step; 
			const uint32_t *it5 = first5 + step; 
			const uint32_t *it6 = first6 + step; 
			const uint32_t *it7 = first7 + step; 
			const uint32_t *it8 = first8 + step; 
			if(*it1 < keys[i+0]) first1 = it1;
			if(*it2 < keys[i+1]) first2 = it2;
			if(*it3 < keys[i+2]) first3 = it3;
			if(*it4 < keys[i+3]) first4 = it4;
			if(*it5 < keys[i+4]) first5 = it5;
			if(*it6 < keys[i+5]) first6 = it6;
			if(*it7 < keys[i+6]) first7 = it7;
			if(*it8 < keys[i+7]) first8 = it8;
			count -= step;
		}
		if(count==1){
			if(*first1 < keys[i+0]) first1++;
			if(*first2 < keys[i+1]) first2++;
			if(*first3 < keys[i+2]) first3++;
			if(*first4 < keys[i+3]) first4++;
			if(*first5 < keys[i+4]) first5++;
			if(*first6 < keys[i+5]) first6++;
			if(*first7 < keys[i+6]) first7++;
			if(*first8 < keys[i+7]) first8++;
		}
		if(*first1 == keys[i+0]) sum += keys[i+0];
		if(*first2 == keys[i+1]) sum += keys[i+1];
		if(*first3 == keys[i+2]) sum += keys[i+2];
		if(*first4 == keys[i+3]) sum += keys[i+3];
		if(*first5 == keys[i+4]) sum += keys[i+4];
		if(*first6 == keys[i+5]) sum += keys[i+5];
		if(*first7 == keys[i+6]) sum += keys[i+6];
		if(*first8 == keys[i+7]) sum += keys[i+7];
	}
	// tail handling
	const uint32_t *array_end  = array + array_size;
	for(; i<keys_size; ++i){
		if(binsearch_single2(array, array_end, keys[i])){
			sum += keys[i];
		}
	}
	return sum;
}

template<size_t group_size>
uint64_t plain_interleaving(const uint32_t *array, size_t array_size, const uint32_t *keys, size_t keys_size){
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		const uint32_t *arr_first[group_size];
		for(size_t g=0; g<group_size; ++g){
			arr_first[g] = array;
		}
		size_t count = array_size;
		while(size_t step = count / 2){
			for(size_t g=0; g<group_size; ++g){
				const uint32_t *it = arr_first[g] + step;
				if(*it < keys[i + g]) arr_first[g] = it;
			}
			count -= step;
		}
		if(count==1){
			for(size_t g=0; g<group_size; ++g){
				if(*arr_first[g] < keys[i+g]) arr_first[g]++;
			}
		}
		for(size_t g=0; g<group_size; ++g){
			if(*arr_first[g] == keys[i+g]) sum += keys[i+g];
		}
	}
	// tail handling
	const uint32_t *array_end  = array + array_size;
	for(; i<keys_size; ++i){
		if(binsearch_single2(array, array_end, keys[i])){
			sum += keys[i];
		}
	}
	return sum;
}

#endif
