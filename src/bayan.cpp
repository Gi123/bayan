#include "../include/bayan.h"

#include <fstream>

namespace fs = boost::filesystem;

std::vector<fs::path> vec_str_to_path(const std::vector<std::string>& other) {
	std::vector<fs::path> result;
	result.reserve(other.size());
	for(const auto& i : other) {
		result.emplace_back(fs::path(i));
	}
	return result;
}

void SearchDuplicate::set_hasher(std::string hash_str) {
	if(hash_str == "md5") {
		hasher = new MD5Hasher();
	}
	else if(hash_str == "sha1") {
		hasher = new SHA1Hasher();
	}
	else {
		hasher = new CRC32Hasher();
	}
}

bool SearchDuplicate::excluded_path(fs::path p) {
	for(const auto& i : exclude_dirs) {
		if(fs::equivalent(p, i)) {
			return true;
		}
	}
	return false;
}

bool SearchDuplicate::filename_with_filemask(const std::string& filename) {
	if(vec_reg_filemasks.size() == 0) {
		return true;
	}
	for(const auto& i : vec_reg_filemasks) {
		if(boost::regex_match(filename, i)) {
			return true;
		}
	}
	return false;
}

void SearchDuplicate::add_file(fs::path p) {
	if(fs::exists(p) && fs::is_regular_file(p)) {
		if(files_canonical.find(fs::weakly_canonical(p).string()) == files_canonical.end()) {
			files_canonical.insert(fs::weakly_canonical(p).string());
			files.emplace_back(File(p.string(), fs::file_size(p), block_size, hasher));
		};
	}
}

void SearchDuplicate::add_filtered_file(fs::path p) {
	if(filename_with_filemask(p.filename().string())) {
		add_file(p);
	}
}

void SearchDuplicate::scan_path(fs::path p, size_t depth) {
	if(fs::exists(p) && !excluded_path(p)) {
		if(fs::is_regular_file(p)) {
			if(fs::file_size(p) >= min_file_size) {
				add_filtered_file(p);
			}
		}
		else if(fs::is_directory(p) && depth > 0) {
			for(const fs::directory_entry& x : fs::directory_iterator(p)) {
				scan_path(x.path(), depth - 1);
			}
		}
	}
}

SearchDuplicate::SearchDuplicate(std::vector<std::string> include_dirs, std::vector<std::string> exclude_dirs, std::vector<std::string> vec_file_mask,
								 size_t block_size, size_t scan_depth, uintmax_t min_file_size, std::string hasher)
	: include_dirs(vec_str_to_path(include_dirs)), exclude_dirs(vec_str_to_path(exclude_dirs)), block_size(block_size),
	scan_depth(scan_depth), min_file_size(min_file_size) {
	set_hasher(hasher);
	for(const auto& i : vec_file_mask) {
		vec_reg_filemasks.emplace_back(boost::regex(i));
	}
}

std::unordered_map<std::string, std::vector<std::string>> SearchDuplicate::map_find_duplicates() {
	search();
	std::unordered_map<std::string, std::vector<std::string>> result;

	if(files.size() < 2) {
		return result;
	}
	
	for(auto first = std::begin(files); first != end(files); ++first) {
		if(!first->in_result) {
			for(auto second = std::next(first); second != end(files); ++second) {
				if(!second->in_result && first->equal_hash_file(*second)) {
					result[first->get_path().string()].push_back(second->get_path().string());
					second->in_result = true;
				}
			}
		}
	}
	return result;
}

void SearchDuplicate::set_include_dirs(std::vector<std::string> dirs) {
	include_dirs = vec_str_to_path(dirs);
}

void SearchDuplicate::set_exclude_dirs(std::vector<std::string> dirs) {
	exclude_dirs = vec_str_to_path(dirs);
}

void SearchDuplicate::search() {
	for(const auto& i : include_dirs) {
		scan_path(i, scan_depth);
	}
}