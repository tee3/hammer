#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <hammer/core/location.h>
#include <hammer/core/toolset.h>

namespace hammer {

class engine;

class toolset_manager {
   public:
      // add toolset to manager and register toolset::use_rule() in rule_manager
      void add_toolset(engine& e,
                       std::unique_ptr<toolset> t);
      void init_toolset(engine& e,
                        const std::string& toolset_name,
                        const std::string& toolset_version,
                        const location_t* toolset_home = NULL) const;
      void autoconfigure(engine& e) const;
      ~toolset_manager();

   private:
      typedef std::unordered_map<std::string, std::unique_ptr<toolset>> toolsets_t;

      toolsets_t toolsets_;
};

}
