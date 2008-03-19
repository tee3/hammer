#include "stdafx.h"
#include "feature_set.h"
#include "feature_registry.h"
#include "feature.h"

using namespace std;

namespace hammer{

   feature_set::feature_set(feature_registry* fr) : fr_(fr)
   {
   }

   void feature_set::insert(const char* name, const char* value)
   {
      features_.push_back(fr_->create_feature(name, value));
   }

   void feature_set::insert(feature* f)
   {
      features_.push_back(f);
   }

/*
   const feature* feature_set::get(const char* name_) const
   {
      const feature* f = find(name_);
      if (!f)
         throw runtime_error("feature '" + string(name_) + "not founded");
      
      return f;
   }
*/

   const feature* feature_set::find(const char* name_, const char* value) const
   {
      string name(name_);
      for(features_t::const_iterator i = features_.begin(), last = features_.end(); i != last; ++i)
         if ((**i).def()->name() == name && (**i).value() == value)
            return *i;

      return 0;
   }

}