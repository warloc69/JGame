
#pragma once

#include <vector>
#include <hash_map>
#include <sstream>

#define SAFE_RELEASE(x)		{ if(x) { (x)->Release(); (x) = NULL; } }
#define SAFE_RELEASE_P(x)	{ if(x) { (x)->Release(); delete (x); (x) = NULL; } }

#define SAFE_DELETE_P(x)	{ if(x) { delete (x); (x) = NULL; } }
#define SAFE_DELETE_ARRAY(x){ if(x) { delete [] (x); (x) = NULL; } }

#define SAFE_DELETE_LIST(x) { if(x) { (x)->clear(); delete (x); (x) = NULL; } }

#define HR_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }

template <class T> 
std::wstring toWString(const T& t)
{
	std::wostringstream oss;
	oss.clear();
	oss << t;
	return oss.str();
};

template <class T> 
std::string toString(const T& t)
{
	std::ostringstream oss;
	oss.clear();
	oss << t;
	return oss.str();
};

template <typename T>
std::vector<T> ARRAY_TO_VECTOR(T* v, size_t N)
{
	std::vector<T> result;
	for(size_t i = 0; i < N; i++)
		result.push_back(v[i]);
	return result;
};

template <class T>
void SAFE_DELETE_P_VECTOR(std::vector<T*> *vec)
{
	std::vector<T*>::iterator i = vec->begin();
	for(; i != vec->end(); i++)
		SAFE_DELETE_P(*i);
};

template <class T>
void SAFE_RELEASE_VECTOR(std::vector<T*> *vec)
{
	if(vec->size() == 0)
		return;

	std::vector<T*>::iterator i = vec->begin();
	for(; i != vec->end(); i++)
		SAFE_RELEASE(*i);
};

template <class T>
void SAFE_RELEASE_P_VECTOR(std::vector<T*> *vec)
{
	if(vec->size() == 0)
		return;

	std::vector<T*>::iterator i = vec->begin();
	for(; i != vec->end(); i++)
		SAFE_RELEASE_P(*i);
};

template <typename K, class V>
void SAFE_DELETE_HASH_MAP_V(std::hash_map<K,V*> *map)
{
	std::hash_map<K,V*>::iterator i = map->begin();
	for(; i != map->end(); i++)
		SAFE_DELETE_P(i->second);
	map->clear();
};

template <typename K, class V>
void SAFE_RELEASE_HASH_MAP_V(std::hash_map<K,V*> *map)
{
	std::hash_map<K,V*>::iterator i = map->begin();
	for(; i != map->end(); i++)
		SAFE_RELEASE(i->second);
	map->clear();
};

template <class K, typename V>
void SAFE_RELEASE_HASH_MAP_K(std::hash_map<K*,V> *map)
{
	if(map->size() == 0)
		return;

	std::hash_map<K*,V>::iterator i = map->begin();
	for(; i != map->end(); i++)
		SAFE_RELEASE(i->first);
};