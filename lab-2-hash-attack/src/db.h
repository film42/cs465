#ifndef HASH_ATTACK_DB
#define HASH_ATTACK_DB

#include <unordered_map>
#include <vector>
#include <string>

class Database {
  public:

    static Database * get_instance() {
      if( !m_instance ) {
        m_instance = new Database();
      }

      return m_instance;
    }

    bool original_exists( std::string original ) {
      return m_original_cache.count( original ) > 0;
    }

    void erase() {
      m_database.clear();
      m_original_cache.clear();
    }

    void insert( std::string original, std::string hash ) {

      if( m_database.count( hash ) <= 0 ) {
        m_database[hash] = std::vector<std::string>();
      }

      m_original_cache[original] = true;
      m_database[hash].push_back( original );
    }

    bool collision_for_hash( std::string hash ) {

      if( m_database.count(hash) > 0 ) {
        return m_database[hash].size() > 1;
      }

      return false;
    }

    int collision_count() {
      int acc = 0;

      for( auto pair : m_database ) {

        if( pair.second.size() > 1 ) {
          ++acc;
        }

      }

      return acc;
    }

  private:
    std::unordered_map<std::string, std::vector<std::string> > m_database;
    std::unordered_map<std::string, bool > m_original_cache;

    static Database * m_instance;
};

#endif