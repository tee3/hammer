#pragma once
#include <hammer/core/location.h>

namespace hammer {

class hashed_location : private location_t {
   public:
      hashed_location(const hashed_location& l)
         : location_t(l.location()),
           hash_(boost::hash_value(l.location().string()))
      {}

      hashed_location(const location_t& l)
         : location_t(l),
           hash_(boost::hash_value(l.string()))
      {}

      hashed_location(const std::string& l)
         : location_t(l),
         hash_(boost::hash_value(l))
      {}

      hashed_location() {}
      const location_t& location() const { return static_cast<const location_t&>(*this); }
      std::size_t hash() const { return hash_; }
      bool operator == (const location_t& rhs) const { return string() == rhs.string(); }
      bool operator == (const hashed_location& rhs) const { return hash_ == rhs.hash_ && string() == rhs.string(); }

   private:
      std::size_t hash_;
};

inline std::size_t hash_value(const hashed_location& hl) {
   return hl.hash();
}

}
