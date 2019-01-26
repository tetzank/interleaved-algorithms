#ifndef GENDATA_HPP_
#define GENDATA_HPP_

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


class GenData{
private:
	char *mapped_addr; // address returned by mmap()
	uint64_t fsize;

public:
	uint64_t number_of_elements;
	int32_t *keys;
	int32_t *data;

	GenData(const char *fname){
		int fd = open(fname, O_RDONLY);
		if(fd == -1){
			perror("failed to open file for import");
			exit(EXIT_FAILURE);
		}
		// get file size
		struct stat s;
		if(fstat(fd, &s) == -1){
			perror("failed to fstat file");
			exit(EXIT_FAILURE);
		}
		fsize = s.st_size;
		// map file into memory
		mapped_addr = reinterpret_cast<char*>(mmap(nullptr, fsize, PROT_READ, MAP_PRIVATE, fd, 0));
		if(mapped_addr == MAP_FAILED){
			perror("failed to fstat file");
			exit(EXIT_FAILURE);
		}
		// read header
		char *addr = mapped_addr;
		uint64_t magic = *reinterpret_cast<uint64_t*>(addr);
		if(magic != *(uint64_t*)"GENDATA"){
			fprintf(stderr, "wrong file type\n");
			exit(EXIT_FAILURE);
		}
		addr += sizeof(uint64_t);
		number_of_elements = *reinterpret_cast<uint64_t*>(addr);
		addr += sizeof(uint64_t);
		keys = reinterpret_cast<int32_t*>(addr);
		addr += number_of_elements * sizeof(int32_t);
		data = reinterpret_cast<int32_t*>(addr);

		// close file, undo open(), file still open because of mmap()
		close(fd);
	}
	~GenData(){
		munmap(mapped_addr, fsize);
	}

	static void writeData(const char *fname, size_t elements, int32_t *keys, int32_t *data){
		FILE *fd = fopen(fname, "wb");
		if(!fd){
			perror("failed to open file for writing");
			exit(EXIT_FAILURE);
		}
		// header
		fwrite("GENDATA", 8, 1, fd); // magic number to signal file type
		fwrite(&elements, sizeof(size_t), 1, fd); // number of elements
		//TODO: maybe padding to start with full cache-line
		// keys
		fwrite(keys, sizeof(int32_t), elements, fd);
		// data
		fwrite(data, sizeof(int32_t), elements, fd);
		fclose(fd);
	}
};

#endif
