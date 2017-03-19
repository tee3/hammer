#if !defined(h_47f09334_e5bd_48ac_a256_1b8d0efd8506)
#define h_47f09334_e5bd_48ac_a256_1b8d0efd8506

#include "requirements_decl.h"
#include <string>

namespace hammer {
class project_requirements_decl
{
public:
  project_requirements_decl(std::string name, const requirements_decl& r);
  const std::string& name() const { return name_; }
  const requirements_decl& requirements() const { return requirements_; }
  requirements_decl& requirements() { return requirements_; }

private:
  std::string name_;
  requirements_decl requirements_;
};
}

#endif // h_47f09334_e5bd_48ac_a256_1b8d0efd8506
