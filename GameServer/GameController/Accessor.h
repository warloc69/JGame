
#pragma once

#include <hash_map>

template <typename K, typename C> class Accessor
{
	public:
		Accessor()
		{
			m_values.clear();
		}

		~Accessor()
		{
			SAFE_DELETE_HASH_MAP_V(&m_values);
		}

		const std::hash_map<K,C*> getMap()
		{
			return m_values;
		}

		C* getValue(K k)
		{
			std::hash_map<K,C*>::iterator itr = m_values.begin();
			for(; itr != m_values.end(); itr++)
			{
				if(itr->first == k)
					return itr->second;
			}

			return NULL;
		}

		void add(C* o);

		void remove(C* o)
		{
			for(std::hash_map<K,C*>::iterator itr = m_values.begin(); itr != m_values.end(); itr++)
			{
				if(itr->second != o)
					continue;
				SAFE_DELETE_P(itr->second);
				m_values.erase(itr);
				break;
			}
		}

		void remove(K k)
		{
			for(std::hash_map<K,C*>::iterator itr = m_values.begin(); itr != m_values.end(); itr++)
			{
				if(itr->first != k)
					continue;
				SAFE_DELETE_P(itr->second);
				m_values.erase(itr);
				break;
			}
		}

	private:
		std::hash_map<K,C*> m_values;

		K generateKey();
};