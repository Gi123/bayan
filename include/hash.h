#pragma once

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <iostream>
#include <vector>

using digest = std::vector<unsigned int>;
/*! 
 *  @brief Алгоритмы хеширования
 */
class IHasher
{
public:
	virtual digest Hash(const char* buffer, unsigned int buffer_length) = 0;
};
/*! 
 *  @brief Алгоритмы хеширования CRC32
 */
class CRC32Hasher: public IHasher
{
public:
	virtual digest Hash(const char* buffer, unsigned int buffer_length);
};
/*! 
 *  @brief Алгоритмы хеширования MD5
 */
class MD5Hasher: public IHasher
{
public:
	virtual digest Hash(const char* buffer, unsigned int buffer_length);
};
/*! 
 *  @brief Алгоритмы хеширования SHA1
 */
class SHA1Hasher: public IHasher
{
public:
	virtual digest Hash(const char* buffer, unsigned int buffer_length);
};
