#pragma once

#include <boost/regex.hpp>
#include <boost/unordered_set.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "compare.h"
#include "hash.h"

class FindDuplicate
{
public:
	FindDuplicate(std::vector<std::string> dirs,
				  std::vector<std::string> skip_dirs,
				  std::vector<std::string> vec_file_mask,
				  size_t block_size, 
				  size_t scan_depth, 
				  uintmax_t min_file_size, 
				  std::string hasher);
	std::unordered_map<std::string, std::vector<std::string>> map_find_duplicates();
	void set_dirs(std::vector<std::string> _dirs);
	void set_skip_dirs(std::vector<std::string> _dirs);
private:
  
	std::vector<std::filesystem::path> dirs;				
	std::vector<std::filesystem::path> skip_dirs;		
	std::vector<boost::regex> vec_regex_filemasks;	
	size_t block_size;						
	size_t scan_depth;						
	uintmax_t min_file_size;				
	std::vector<CompareFileHash> info_scan_files;		
	std::unordered_set<std::string> files_canonical;	
	IHasher* hasher;						
	void set_hasher(std::string hash_str);	
	bool excluded_path(std::filesystem::path p);			
	bool filename_with_filemask(const std::string& filename);	
	void add_file(std::filesystem::path p);				
	void add_filtered_file(std::filesystem::path p);		
	void scan_path(std::filesystem::path p, size_t depth);	
	void search();							
};
