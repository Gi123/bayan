#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include "../include/bayan.h"
namespace std
{
	std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec) {
		for(const auto& item : vec) {
			os << item << " ";
		}
		return os;
	}
}

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
	try {
		po::options_description desc{"Options"};
		desc.add_options()
			("help,h", "Help screen")
			("include_dir,I", po::value<std::vector<std::string>>()->multitoken(), "Include directories to scan")
			("exclude_dir,E", po::value<std::vector<std::string>>()->multitoken()->default_value(std::vector<std::string>{}), "Exclude directories to exclude")
			("depth,D", po::value<size_t>()->default_value(2), "Depth to scan")
			("min_file_size,S", po::value<uintmax_t>()->default_value(1), "Minimum file size")
			("hash,H", po::value<std::string>()->default_value("crc32"), "A hashing algorithm crc32/md5/sha1")
			("file_masks", po::value<std::vector<std::string>>()->multitoken()->default_value(std::vector<std::string>{}), "Mask for files to scans")
			("block_size", po::value<size_t>()->default_value(512), "Block size in bytes")
            ("unreg", "Unrecognized options");

		po::variables_map vm;
		po::store(parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(vm.count("help"))
			std::cout << desc << '\n';
		else if(vm.count("include_dir")) {
			SearchDuplicate dup(vm["include_dir"].as<std::vector<std::string> >(),
								vm["exclude_dir"].as<std::vector<std::string> >(),
								vm["file_masks"].as<std::vector<std::string> >(),
								vm["block_size"].as<size_t>(),
								vm["depth"].as<size_t>() + 1,
								vm["min_file_size"].as<uintmax_t>(),
								vm["hash"].as<std::string>()
			);
			auto dups = dup.map_find_duplicates();
			for(const auto& i : dups) {
				std::cout << i.first << '\n';
				for(const auto& j : i.second) {
					std::cout << j << '\n';
				}
				std::cout << std::endl;
			}

		}
	}
	catch(const po::error& ex) {
		std::cerr << ex.what() << '\n';
	}
}