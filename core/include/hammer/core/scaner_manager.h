#if !defined(h_d7ff7f5f_e931_49f6_9c2f_e8bc7f3b0fde)
#define h_d7ff7f5f_e931_49f6_9c2f_e8bc7f3b0fde

#include <memory>
#include <boost/noncopyable.hpp>
#include <vector>

namespace hammer
{
   class target_type;
   class scanner;

   class scanner_manager : public boost::noncopyable
   {
      public:
         scanner_manager();
         void register_scanner(const target_type& t, const std::shared_ptr<scanner>& scanner);
         const scanner* find(const target_type& t) const;
         const scanner& get(const target_type& t) const;

      private:
         struct impl_t;
         std::shared_ptr<impl_t> impl_;
   };
}

#endif //h_d7ff7f5f_e931_49f6_9c2f_e8bc7f3b0fde
