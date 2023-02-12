#pragma once

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <iostream>
#include <vector>

using VectorUnI = std::vector<unsigned int>;

class IHasher
{
public:
	virtual VectorUnI Hash(const char* buffer, unsigned int buffer_length) = 0;
};

class CRC32Hasher: public IHasher
{
public:
	virtual VectorUnI Hash(const char* buffer, unsigned int buffer_length);
};

class MD5Hasher: public IHasher
{
public:
	virtual VectorUnI Hash(const char* buffer, unsigned int buffer_length);
};

class SHA1Hasher: public IHasher
{
public:
	virtual VectorUnI Hash(const char* buffer, unsigned int buffer_length);
};
