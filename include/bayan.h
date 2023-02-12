#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/unordered_set.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "file.h"
#include "hash.h"

namespace fs = boost::filesystem;
/*! 
 *  @brief Класс поиска дубликатов
*/
class SearchDuplicate
{
public:
	SearchDuplicate(std::vector<std::string> include_dirs, std::vector<std::string> exclude_dirs, std::vector<std::string> vec_file_mask,
					size_t block_size, size_t scan_depth, uintmax_t min_file_size, std::string hasher);
	std::unordered_map<std::string, std::vector<std::string>> map_find_duplicates();
	void set_include_dirs(std::vector<std::string> dirs);
	void set_exclude_dirs(std::vector<std::string> dirs);
private:
  /*! 
   *  @brief Каталоги для сканнирования
   */
	std::vector<fs::path> include_dirs;
	/**
	 * @brief Каталоги исключения
	 * 
	 */
	std::vector<fs::path> exclude_dirs;
	/**
	 * @brief Маски сканирования
	 * 
	 */
	std::vector<boost::regex> vec_reg_filemasks;
	/**
	 * @brief величина хэш-блока для сравнения
	 * 
	 */
	size_t block_size;
	/**
	 * @brief Максимальная глубина сканирования
	 * 
	 */
	size_t scan_depth;
	/**
	 * @brief Минимальный размер файла для сканирования
	 * 
	 */
	uintmax_t min_file_size;
	/**
	 * @brief Информация по отсканированным файлам
	 * 
	 */
	std::vector<File> files;
	/**
	 * @brief Информация для исключения дубликатов
	 * 
	 */
	std::unordered_set<std::string> files_canonical;
	/**
	 * @brief Класс для кэширования файлов
	 * 
	 */
	IHasher* hasher;
	/**
	 * @brief Установка для обьекта класса
	 */
	void set_hasher(std::string hash_str);
	/**
	 * @brief Проверка на исключенные пути
	 */
	bool excluded_path(fs::path p);
	/**
	 * @brief Проверка по маске
	 */
	bool filename_with_filemask(const std::string& filename);
	/**
	 * @brief Добавлением файла для сканирования
	 */
	void add_file(fs::path p);
	/**
	 * @brief Добавлением файла для сканирования с учетом маски и исключен
	 */
	void add_filtered_file(fs::path p);
	/**
	 * @brief Рекурсированное сканирование
	 * 
	 */
	void scan_path(fs::path p, size_t depth);
	/**
	 * @brief Запуск сканирования
	 * 
	 */
	void search();
};