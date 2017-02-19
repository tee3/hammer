#if !defined(h_ab5e0642_d07a_4fde_9332_972f2306020d)
#define h_ab5e0642_d07a_4fde_9332_972f2306020d

#include <memory>
#include "build_environment.h"

namespace hammer{

class build_environment_impl : public build_environment
{
   public:
      build_environment_impl(const location_t& cur_dir,
                             const bool print_shell_commands = false);
      virtual ~build_environment_impl();

      virtual bool run_shell_commands(const std::vector<std::string>& cmds, const location_t& working_dir) const;
      virtual bool run_shell_commands(std::string& captured_output, const std::vector<std::string>& cmds, const location_t& working_dir) const;
      virtual bool run_shell_commands(std::ostream& captured_output_stream,
                                      const std::vector<std::string>& cmds,
                                      const location_t& working_dir) const;
      virtual bool run_shell_commands(std::ostream& captured_output_stream,
                                      std::ostream& captured_error_stream,
                                      const std::vector<std::string>& cmds,
                                      const location_t& working_dir) const;
      virtual const location_t& current_directory() const;
      virtual void create_directories(const location_t& dir_to_create) const;
      virtual void remove(const location_t& p) const;
      virtual void remove_file_by_pattern(const location_t& dir, const std::string& pattern) const;
      virtual void copy(const location_t& full_source_path, const location_t& full_destination_path) const;
      virtual bool write_tag_file(const std::string& filename, const std::string& content) const;
      virtual std::unique_ptr<std::ostream> create_output_file(const char* filename, std::ios_base::openmode mode) const;
      virtual location_t working_directory(const basic_target& t) const;
      virtual std::ostream& output_stream() const;
      virtual std::ostream& error_stream() const;
      virtual const location_t* cache_directory() const;

   private:
      location_t current_directory_;
      const bool print_shell_commands_;
      location_t cache_directory_;
      std::unique_ptr<std::ostream*> output_stream_;
      std::unique_ptr<std::ostream*> error_stream_;

      // FIXME: need to find better name
      void dump_shell_command(std::ostream& s, const location_t& full_content_file_name) const;
      bool run_shell_commands(std::ostream* captured_output_stream,
                              std::ostream* captured_error_stream,
                              const std::vector<std::string>& cmds,
                              const location_t& working_dir) const;
};

}

#endif //h_ab5e0642_d07a_4fde_9332_972f2306020d
