#
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "hash.h"

struct HashBlock	// Single hash block
{
	std::vector<unsigned int> data;
	bool operator==(const HashBlock& other) const;
	bool operator!=(const HashBlock& other) const;
};

class CompareFileHash
{
public:
	bool in_result = false;	
	const size_t block_count;					

	CompareFileHash(std::filesystem::path path, uintmax_t size, uintmax_t hash_blocksize, IHasher* hasher) :
		path(path), 
		file_size(size), 
		block_size(hash_blocksize), 
		hasher(hasher),
		block_count((size + hash_blocksize - 1) / hash_blocksize) {};

	bool operator==(const CompareFileHash& other) const;	
	size_t get_hash_data_size() const;				
	uintmax_t get_file_size() const;				
	std::filesystem::path get_path() const;						
	void open_handle();								
	void close_handle();							
	std::unique_ptr<char[]> get_next_block();		
	void calc_next_hash();							
	HashBlock calc_hash_block(size_t addr);			
	HashBlock get_hash_block(size_t addr);				
	bool equal_hash_file(CompareFileHash& other);	
private:
	std::filesystem::path path;
	uintmax_t file_size;	
	std::vector<HashBlock> hash_data;				
	uintmax_t block_size;
	IHasher* hasher;
	std::unique_ptr<std::ifstream> file_handle;
};
