#if !defined(h_85f86c30_04b2_4778_838d_12e9b607ed32)
#define h_85f86c30_04b2_4778_838d_12e9b607ed32

#include <hammer/core/pstring.h>

namespace hammer{

class subfeature_def;

class subfeature
{
   public:
      subfeature(const subfeature_def& def,
                 const pstring& value);
      ~subfeature();

      const subfeature_def& definition() const { return *definition_; }
      const std::string& name() const;
      const pstring& value() const { return value_; }


   private:
      const subfeature_def* definition_;
      const pstring value_;
};

}

#endif
