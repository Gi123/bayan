#include "../include/file.h"

#include <fstream>

namespace fs = boost::filesystem;

bool HashBlock::operator==(const HashBlock& other) const {
	return data == other.data;
}
bool HashBlock::operator!=(const HashBlock& other) const {
	return !(*this == other);
}
/*!
*  @brief const comparsion for unordered_set
*/
bool File::operator==(const File& other) const {
	return fs::equivalent(path, other.path);
}
/*!
*  @brief getter for size of calculated hash
*/
size_t File::get_hash_data_Size() const {
	return hash_data.size();
}
/*!
*  @brief getter for filesize
*/
uintmax_t File::get_file_size() const {
	return filesize;
}
/*!
*  @brief getter for path
*/
fs::path File::get_path() const {
	return path;
}
/*!
*  @brief Открыть файл и перейти в место последнего невычесленного блока
*/
void File::open_handle() {
	if(!file_handle) {
		file_handle = std::make_unique<std::ifstream>(get_path().string());
		file_handle.get()->seekg(hash_data.size() * blocksize);
	}
}
/*!
*  @brief закрыть файл
*/
void File::close_handle() {
	if(file_handle != nullptr) {
		file_handle->close();
		delete file_handle.release();
	}
}

/*!
*  @brief Получить следующий блок из файла
*/
std::unique_ptr<char[]> File::get_next_block() {
	open_handle();
	auto buffer = std::make_unique<char[]>(blocksize);
	file_handle->read(buffer.get(), blocksize);
	return buffer;
}

/*!
*  @brief Вычислить следующий блок хеша.
*/
void File::calc_next_hash() {
	hash_data.emplace_back(HashBlock{hasher->Hash(get_next_block().get(), blocksize)});
}

/*!
*  @brief Вычислить хеш до блока N включительно.
*/
HashBlock File::calc_hash_block(size_t addr) {
	while(hash_data.size() <= addr) {
		calc_next_hash();
	}
	return hash_data.back();
}

/*!
*  @brief get N-th HashBlock
*/
HashBlock File::get_hash_block(size_t addr) {
	if(addr >= block_count)
		throw;
	if(addr < hash_data.size())
		return hash_data[addr];
	else
		return calc_hash_block(addr);
}

/*!
*  @brief Compare by hash with other File
*/
bool File::equal_hash_file(File& other) {
	if(this->get_file_size() != other.get_file_size())
		return false;
	for(size_t i = 0; i < block_count; ++i) {
		if(get_hash_block(i) != other.get_hash_block(i)) {
			close_handle();
			other.close_handle();
			return false;
		}
	}
	close_handle();
	other.close_handle();
	return true;
}

/*!
 *  @brief hash opertor for unordered_map
*/
namespace std
{
	template <>
	struct hash<File>
	{
		size_t operator()(const File& obj) const {
			return hash<string>()(obj.get_path().string());
		}
	};
} // namespace std