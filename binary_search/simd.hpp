#ifndef SIMD_HPP_
#define SIMD_HPP_

#include <immintrin.h>


uint64_t simd_avx2(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
#ifdef __AVX2__
	const size_t group_size=8;
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		__m256i v_keys = _mm256_loadu_si256((__m256i*)&keys[i]);
		//const uint32_t *first1=array, *first2=array, *first3=array, *first4=array;
		__m256i v_first = _mm256_setzero_si256();
		//size_t count = array_size;
		__m256i v_count = _mm256_set1_epi32(array_size);

		//while(size_t step = count / 2){
		__m256i v_step = _mm256_srli_epi32(v_count, 1); // step = count/2
		while(!_mm256_testz_si256(v_step, v_step)){
			//const uint32_t *it1 = first1 + step; 
			__m256i v_idx = _mm256_add_epi32(v_first, v_step);
			//if(*it1 < keys[i+0]) first1 = it1;
			__m256i v_values = _mm256_i32gather_epi32((const int*)array, v_idx, 4);
			__m256i v_mask = _mm256_cmpgt_epi32(v_keys, v_values);
			v_first = (__m256i)_mm256_blendv_ps((__m256)v_first, (__m256)v_idx, (__m256)v_mask);
			//count -= step;
			v_count = _mm256_sub_epi32(v_count, v_step);

			v_step = _mm256_srli_epi32(v_count, 1); // step = count/2
		}
		//if(count==1){
		if(!_mm256_testz_si256(v_count, v_count)){
			//if(*first1 < keys[i+0]) first1++;
			__m256i v_values = _mm256_i32gather_epi32((const int*)array, v_first, 4);
			__m256i v_mask = _mm256_cmpgt_epi32(v_keys, v_values);
			v_mask = _mm256_srli_epi32(v_mask, 31);
			v_first = _mm256_add_epi32(v_first, v_mask);
		}
		//if(*first1 == keys[i+0]) sum += keys[i+0];
		v_first = _mm256_i32gather_epi32((const int*)array, v_first, 4);
		v_first = _mm256_cmpeq_epi32(v_first, v_keys);
		v_keys = _mm256_and_si256(v_keys, v_first);
		int32_t arr_keys[group_size];
		_mm256_storeu_si256((__m256i*)arr_keys, v_keys);
		for(size_t g=0; g<group_size; ++g){
			sum += arr_keys[g];
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
#else
	puts("AVX2 not supported");
	return 0;
#endif
}

uint64_t simd_avx2_interleaved2(const int32_t *array, size_t array_size, const int32_t *keys, size_t keys_size){
#ifdef __AVX2__
	const size_t group_size=16;
	uint64_t sum=0, i=0;
	size_t size = (keys_size / group_size) * group_size;
	for(; i<size; i+=group_size){
		__m256i v_keys1 = _mm256_loadu_si256((__m256i*)&keys[i]);
		__m256i v_keys2 = _mm256_loadu_si256((__m256i*)&keys[i+8]);
		//const uint32_t *first1=array, *first2=array, *first3=array, *first4=array;
		__m256i v_first1 = _mm256_setzero_si256();
		__m256i v_first2 = _mm256_setzero_si256();
		//size_t count = array_size;
		__m256i v_count1 = _mm256_set1_epi32(array_size);
		__m256i v_count2 = _mm256_set1_epi32(array_size);

		//while(size_t step = count / 2){
		__m256i v_step1 = _mm256_srli_epi32(v_count1, 1); // step = count/2
		__m256i v_step2 = _mm256_srli_epi32(v_count2, 1);
		while(!_mm256_testz_si256(v_step1, v_step2)){
			//const uint32_t *it1 = first1 + step; 
			__m256i v_idx1 = _mm256_add_epi32(v_first1, v_step1);
			__m256i v_idx2 = _mm256_add_epi32(v_first2, v_step2);
			//if(*it1 < keys[i+0]) first1 = it1;
			__m256i v_values1 = _mm256_i32gather_epi32((const int*)array, v_idx1, 4);
			__m256i v_values2 = _mm256_i32gather_epi32((const int*)array, v_idx2, 4);
			__m256i v_mask1 = _mm256_cmpgt_epi32(v_keys1, v_values1);
			__m256i v_mask2 = _mm256_cmpgt_epi32(v_keys2, v_values2);
			v_first1 = (__m256i)_mm256_blendv_ps((__m256)v_first1, (__m256)v_idx1, (__m256)v_mask1);
			v_first2 = (__m256i)_mm256_blendv_ps((__m256)v_first2, (__m256)v_idx2, (__m256)v_mask2);
			//count -= step;
			v_count1 = _mm256_sub_epi32(v_count1, v_step1);
			v_count2 = _mm256_sub_epi32(v_count2, v_step2);

			v_step1 = _mm256_srli_epi32(v_count1, 1); // step = count/2
			v_step2 = _mm256_srli_epi32(v_count2, 1);
		}
		//if(count==1){
		if(!_mm256_testz_si256(v_count1, v_count1)){
			//if(*first1 < keys[i+0]) first1++;
			__m256i v_values1 = _mm256_i32gather_epi32((const int*)array, v_first1, 4);
			__m256i v_mask1 = _mm256_cmpgt_epi32(v_keys1, v_values1);
			v_mask1 = _mm256_srli_epi32(v_mask1, 31);
			v_first1 = _mm256_add_epi32(v_first1, v_mask1);
		}
		if(!_mm256_testz_si256(v_count2, v_count2)){
			//if(*first1 < keys[i+0]) first1++;
			__m256i v_values2 = _mm256_i32gather_epi32((const int*)array, v_first2, 4);
			__m256i v_mask2 = _mm256_cmpgt_epi32(v_keys2, v_values2);
			v_mask2 = _mm256_srli_epi32(v_mask2, 31);
			v_first2 = _mm256_add_epi32(v_first2, v_mask2);
		}
		//if(*first1 == keys[i+0]) sum += keys[i+0];
		v_first1 = _mm256_i32gather_epi32((const int*)array, v_first1, 4);
		v_first2 = _mm256_i32gather_epi32((const int*)array, v_first2, 4);
		v_first1 = _mm256_cmpeq_epi32(v_first1, v_keys1);
		v_first2 = _mm256_cmpeq_epi32(v_first2, v_keys2);
		v_keys1 = _mm256_and_si256(v_keys1, v_first1);
		v_keys2 = _mm256_and_si256(v_keys2, v_first2);
		int32_t arr_keys[group_size];
		_mm256_storeu_si256((__m256i*)arr_keys, v_keys1);
		_mm256_storeu_si256((__m256i*)(arr_keys+8), v_keys2);
		for(size_t g=0; g<group_size; ++g){
			sum += arr_keys[g];
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
#else
	puts("AVX2 not supported");
	return 0;
#endif
}

#endif
