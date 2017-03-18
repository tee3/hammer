#if !defined(h_6a5596c3_c168_4cc4_b600_a5f897e32e89)
#define h_6a5596c3_c168_4cc4_b600_a5f897e32e89

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

namespace hammer {
struct hammer_walker_context;
class engine;
class parser : public boost::noncopyable
{
public:
  parser(engine* e);
  bool parse(const char* file_name);
  bool parse(const boost::filesystem::path& file_name);
  bool parse_raw_script(const std::string& script_body,
                        const std::string& script_name);
  void walk(hammer_walker_context* ctx);
  ~parser();

private:
  struct impl_t;
  impl_t* impl_;

  void reset();
};
}

#endif // h_6a5596c3_c168_4cc4_b600_a5f897e32e89
