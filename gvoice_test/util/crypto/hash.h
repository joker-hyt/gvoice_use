#pragma once

inline unsigned int hash_value(int v)
{
    return static_cast<unsigned int>(v);
}

inline unsigned int hash_value(unsigned int v)
{
    return static_cast<unsigned int>(v);
}

template <class T>
inline void hash_combine(unsigned int& seed, T const& v)

{
	seed ^= hash_value(v)
		+ 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <class It>
inline unsigned int hash_range(It first, It last)
{	
	unsigned int seed = 0;

	for(; first != last; ++first)
	{
		hash_combine(seed, *first);
	}

	return seed;
}

template <class It>
inline unsigned int hash_range(unsigned int& seed, It first, It last)
{
	for(; first != last; ++first)
	{
		hash_combine(seed, *first);
	}

	return seed;
}
