#if !defined(h_9b0699bd_cc04_4e19_874b_45c95b909551)
#define h_9b0699bd_cc04_4e19_874b_45c95b909551

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "build_node.h"

namespace hammer
{
   class type;
   class feature_set;
   class basic_target;
   class pstring;
   class engine;
   class main_target;

   class generator
   {
      public:
         struct consumable_type
         {
            consumable_type(const type& t, unsigned int f, const feature_set* fs = 0) : type_(&t), flags_(f), features_(fs) {}   
            
            const type* type_;
            unsigned int flags_;
            const feature_set* features_;
         };

         struct produced_type
         {
            produced_type(const type& t, bool need_tag = false) : type_(&t), need_tag_(need_tag) {}

            const type* type_;
            bool need_tag_;
         };

         typedef std::vector<consumable_type> consumable_types_t;
         typedef std::vector<produced_type> producable_types_t;

         generator(engine& e,
                   const std::string& name,
                   const consumable_types_t& source_types,
                   const producable_types_t& target_types,
                   bool composite,
                   const feature_set* c = 0);
         virtual ~generator();

         hammer::engine& engine() { return *engine_; }
         hammer::engine& engine() const { return *engine_; }
         const std::string& name() const { return name_; }
         const consumable_types_t& consumable_types() const { return source_types_; }
         const producable_types_t& producable_types() const { return target_types_; }
         const feature_set* constraints() const { return constraints_; }
 
         virtual std::vector<boost::intrusive_ptr<build_node> >
            construct(const type& target_type, 
                      const feature_set& props,
                      const std::vector<boost::intrusive_ptr<build_node> >& sources,
                      const basic_target* t,
                      const pstring* composite_target_name,
                      const main_target& owner) const;

         bool is_consumable(const type& t) const;
         bool is_composite() const { return composite_; }
         bool include_composite_generators() const { return include_composite_generators_; } 
         void include_composite_generators(bool v) { include_composite_generators_ = v; }

         template <typename T>
         void action(std::auto_ptr<T>& a) { action_ = a; }
         const build_action* action() const { return action_.get(); } 

      private:
         hammer::engine* engine_;
         const std::string name_;
         consumable_types_t source_types_;
         producable_types_t target_types_;
         bool composite_;
         const feature_set* constraints_; // == null if no constraints specified
         std::auto_ptr<build_action> action_;
         bool include_composite_generators_; // include composite generators while searching for sources indirect transformations
   };

   void remove_dups(std::vector<boost::intrusive_ptr<build_node> >& nodes);
}

#endif //h_9b0699bd_cc04_4e19_874b_45c95b909551