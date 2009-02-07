#include "stdafx.h"
#include <hammer/core/project_generators/msvc_project.h>
#include <hammer/core/main_target.h>
#include <hammer/core/meta_target.h>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/types.h>
#include <hammer/core/type.h>
#include <hammer/core/feature.h>
#include <hammer/core/feature_def.h>
#include <hammer/core/feature_registry.h>
#include "../fs_helpers.h"
#include <hammer/core/pch_main_target.h>
#include <hammer/core/build_environment.h>
#include <hammer/core/fs_argument_writer.h>
#include <hammer/core/free_feature_arg_writer.h>
#include <hammer/core/source_argument_writer.h>

using namespace std;
   
namespace hammer{ namespace project_generators{

namespace
{
   class fake_environment : public build_environment
   {
      public:
         fake_environment(const location_t& cur_dir) : current_dir_(cur_dir) {}

         virtual bool run_shell_commands(const std::vector<std::string>& cmds) const { return true; }
         virtual bool run_shell_commands(std::string& captured_output, const std::vector<std::string>& cmds) const { return true; }
         virtual const location_t& current_directory() const { return current_dir_; }
         virtual void create_directories(const location_t& dir_to_create) const {};
         virtual void remove(const location_t& p) const {};
         virtual void copy(const location_t& source, const location_t& destination) const {};
         virtual bool write_tag_file(const std::string& filename, const std::string& content) const { return true; }
         virtual std::auto_ptr<std::ostream> create_output_file(const char* filename, std::ios_base::_Openmode mode) const 
         { 
            return std::auto_ptr<std::ostream>(new ostringstream);
         }
      
      private:
         location_t current_dir_;
   };

   class pch_argument_writer : public argument_writer
   {
      public:
         pch_argument_writer(const std::string& name) : argument_writer(name) {}
         virtual pch_argument_writer* clone() const { return new pch_argument_writer(*this); }

      protected:   
         virtual void write_impl(std::ostream& output, const build_node& node, const build_environment& environment) const
         {
            const feature_set& build_request = node.build_request();
            feature_set::const_iterator pch_iter = build_request.find("pch");
            if (pch_iter == build_request.end() || (**pch_iter).value() == "off")
               return;
          
            const pch_main_target* pch_target = static_cast<const pch_main_target*>((**pch_iter).get_generated_data().target_);
            location_t pch_header(pch_target->pch_header().name().to_string());
            output << pch_header.leaf();
         }
   };
}

static const string configuration_option_format_string(
   "         CharacterSet=\"$(charset)\">\n");

static const string compiller_option_format_string(
   "            Optimization=\"$(optimization)\"\n"
   "            InlineFunctionExpansion=\"$(inlining)\"\n"
   "            AdditionalIncludeDirectories=\"$(includes)\"\n"
   "            DebugInformationFormat=\"$(debug_format)\"\n"
   "            WarningLevel=\"$(warning_level)\"\n"
   "            PreprocessorDefinitions=\"$(defines)\"\n"
   "            ExceptionHandling=\"$(exception_handling)\"\n"
   "            RuntimeLibrary=\"$(runtime)\"\n"
   "            RuntimeTypeInfo=\"$(rtti)\"\n"
   "            AdditionalOptions=\"$(cxxflags)$(cflags)\"\n"
   "            UsePrecompiledHeader=\"$(pch_type)\"\n"
   "            PrecompiledHeaderThrough=\"$(pch_header)\"\n");

static const string linker_option_format_string(
   "            AdditionalDependencies=\"$(additional_libraries)\"\n"
   "            AdditionalLibraryDirectories=\"$(additional_libraries_dirs)\"\n"
   "            GenerateDebugInformation=\"$(debug_info)\"\n");

msvc_project::msvc_project(engine& e, 
                           const location_t& output_dir, 
                           const boost::guid& uid) 
   : 
    engine_(&e), 
    uid_(uid),
    output_dir_(output_dir),
    project_output_dir_(output_dir_),
    searched_lib_(engine_->get_type_registry().get(types::SEARCHED_LIB)),
    obj_type_(engine_->get_type_registry().get(types::OBJ)),
    pch_type_(engine_->get_type_registry().get(types::PCH)),
    configuration_options_(configuration_option_format_string),
    compiller_options_(compiller_option_format_string),
    linker_options_(linker_option_format_string)
{
   output_dir_.normalize();
   // configuration options
   boost::shared_ptr<fs_argument_writer> charset(new fs_argument_writer("charset", e.feature_registry()));
   charset->add("<character-set>unicode", "1").
            add("<character-set>multi-byte", "2").
            add("<character-set>unknown", "0");
   configuration_options_ += charset;

   // compiller options
   boost::shared_ptr<fs_argument_writer> optimization(new fs_argument_writer("optimization", e.feature_registry()));
   optimization->add("<optimization>off", "0").
                 add("<optimization>speed", "2").
                 add("<optimization>space", "1");
   
   compiller_options_ += optimization;

   boost::shared_ptr<fs_argument_writer> inlining(new fs_argument_writer("inlining", e.feature_registry()));
   inlining->add("<inlining>off", "0").
             add("<inlining>on", "1").
             add("<inlining>full", "2");
   compiller_options_ += inlining;

   boost::shared_ptr<free_feature_arg_writer> includes(new free_feature_arg_writer("includes", e.feature_registry().get_def("include"), string(), string(), ";"));
   compiller_options_ += includes;

   boost::shared_ptr<fs_argument_writer> debug_format(new fs_argument_writer("debug_format", e.feature_registry()));
   debug_format->add("<debug-symbols>on/<profiling>off", "4").
                 add("<debug-symbols>on/<profiling>on",  "3").
                 add("<debug-symbols>off",  "0");
   compiller_options_ += debug_format;

   boost::shared_ptr<fs_argument_writer> warning_level(new fs_argument_writer("warning_level", e.feature_registry()));
   warning_level->add("<warnings>on", "3").
                  add("<warnings>off", "0").
                  add("<warnings>all", "4");
   compiller_options_ += warning_level;

   boost::shared_ptr<free_feature_arg_writer> defines(new free_feature_arg_writer("defines", e.feature_registry().get_def("define"), string(), string(), ";"));
   compiller_options_ += defines;

   boost::shared_ptr<fs_argument_writer> exception_handling(new fs_argument_writer("exception_handling", e.feature_registry()));
   exception_handling->add("<exception-handling>off", "0").
                       add("<exception-handling>on/<asynch-exceptions>off", "1").
                       add("<asynch-exceptions>on", "2");
   compiller_options_ += exception_handling;

   boost::shared_ptr<fs_argument_writer> runtime(new fs_argument_writer("runtime", e.feature_registry()));
   runtime->add("<runtime-link>static/<runtime-debugging>off", "0").
            add("<runtime-link>static/<runtime-debugging>on", "1").
            add("<runtime-link>shared/<runtime-debugging>off", "2").
            add("<runtime-link>shared/<runtime-debugging>on", "3");
   compiller_options_ += runtime;

   boost::shared_ptr<fs_argument_writer> rtti(new fs_argument_writer("rtti", e.feature_registry()));
   rtti->add("<rtti>on", "true").
         add("<rtti>off", "false");
   compiller_options_ += rtti;

   boost::shared_ptr<free_feature_arg_writer> cxxflags(new free_feature_arg_writer("cxxflags", e.feature_registry().get_def("cxxflags")));
   compiller_options_ += cxxflags;

   boost::shared_ptr<free_feature_arg_writer> cflags(new free_feature_arg_writer("cflags", e.feature_registry().get_def("cflags")));
   compiller_options_ += cflags;

   boost::shared_ptr<pch_argument_writer> pch_header(new pch_argument_writer("pch_header"));
   compiller_options_ += pch_header;

   boost::shared_ptr<fs_argument_writer> pch_type(new fs_argument_writer("pch_type", e.feature_registry()));
   pch_type->add("<pch>use", "2").
             add("<pch>create", "1").
             add("<pch>off", "0");
   compiller_options_ += pch_type;

   // linker options
   boost::shared_ptr<source_argument_writer> additional_libraries(
       new source_argument_writer("additional_libraries", 
                                  e.get_type_registry().get(types::SEARCHED_LIB)));
   linker_options_ += additional_libraries;

   boost::shared_ptr<free_feature_arg_writer> additional_libraries_dirs(
       new free_feature_arg_writer("additional_libraries_dirs", 
                                   e.feature_registry().get_def("search"),
                                   string(), string(), ";"));
   linker_options_ += additional_libraries_dirs;

   boost::shared_ptr<fs_argument_writer> link_debug_info(new fs_argument_writer("debug_info", engine_->feature_registry()));
   link_debug_info->add("<debug-symbols>on", "true").
                    add("<debug-symbols>off", "false");
   linker_options_ += link_debug_info;
}

static std::string make_variant_name(const feature_set& fs)
{
   const feature& f = fs.get("variant");
   return f.value().to_string();
}

void msvc_project::add_variant(boost::intrusive_ptr<const build_node> node)
{
   assert(!node->products_.empty());
   std::auto_ptr<variant> v(new variant);
   variant* naked_variant = v.get();
   const basic_target* t = node->products_[0];
   v->properties_ = &t->properties();
   v->node_ = node;
   v->target_ = node->products_[0]->mtarget();
   v->name_ = make_variant_name(t->properties());
   v->owner_ = this;
   variants_.push_back(v);
   if (id_.empty())
   {
      meta_target_ = naked_variant->target_->meta_target();
      project_output_dir_ = output_dir() / name().to_string();
      project_output_dir_.normalize();
      id_ = project_output_dir().string();
      full_project_name_ = project_output_dir() / (name().to_string() + ".vcproj");
      meta_target_relative_to_output_ = relative_path(meta_target_->location(), project_output_dir());
      meta_target_relative_to_output_.normalize();
   }
}

void msvc_project::fill_filters()
{
   filter_t::types_t source_types;
   source_types.push_back(&engine_->get_type_registry().get(types::CPP));
   source_types.push_back(&engine_->get_type_registry().get(types::C));
   files_.push_back(filter_t(source_types, "Source Files", "{4FC737F1-C7A5-4376-A066-2A32D752A2FF}"));
   filter_t::types_t header_types;
   header_types.push_back(&engine_->get_type_registry().get(types::H));
   files_.push_back(filter_t(header_types, "Header Files", "{93995380-89BD-4b04-88EB-625FBE52EBFB}"));
}

const pstring& msvc_project::name() const
{
   return variants_.front().target_->meta_target()->name();
}

void msvc_project::write_header(ostream& s) const
{
   s << "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\n"
        "<VisualStudioProject\n"
        "   ProjectType=\"Visual C++\"\n"
        "   Version=\"8,00\"\n"
        "   Name=\"" << name() <<"\"\n"
        "   ProjectGUID=\"" << boost::guid::showbraces << uid_ << "\"\n"
        "   RootNamespace=\"" << name() << "\"\n"
        "   Keyword=\"Win32Proj\">\n"
        "	<Platforms>\n"
        "       <Platform Name=\"Win32\"/>\n"
        "   </Platforms>\n"
        "   <ToolFiles/>\n";
}

static void write_bottom(std::ostream& s)
{
   s << "</VisualStudioProject>";
}

configuration_types::value msvc_project::resolve_configuration_type(const variant& v) const
{
   const type& exe_type = engine_->get_type_registry().get(types::EXE);
   const type& static_lib_type = engine_->get_type_registry().get(types::STATIC_LIB);
   const type& shared_lib_type = engine_->get_type_registry().get(types::SHARED_LIB);
   const type& header_lib_type = engine_->get_type_registry().get(types::HEADER_LIB);
   if (v.target_->type().equal_or_derived_from(exe_type))
      return configuration_types::exe;
   else
      if (v.target_->type().equal_or_derived_from(static_lib_type))
         return configuration_types::static_lib;
      else
         if (v.target_->type().equal_or_derived_from(shared_lib_type))
            return configuration_types::shared_lib;
         else
            if (v.target_->type().equal_or_derived_from(header_lib_type))
               return configuration_types::utility;
            else
               throw std::runtime_error("[msvc_project] Can't resolve configurations type '" + v.target_->type().tag().name() + "'.");
} 

static void write_compiler_options(std::ostream& s, 
                                   const cmdline_builder& formater, 
                                   const build_node& node,
                                   const build_environment& environment)
{
   s << "         <Tool\n"
      "            Name=\"VCCLCompilerTool\"\n";
   formater.write(s, node, environment);
   s << "         />\n";
}

void msvc_project::write_configurations(std::ostream& s) const
{
   s << "   <Configurations>\n";
   
   for(variants_t::iterator i = variants_.begin(), last = variants_.end(); i != last; ++i)
   {
      configuration_types::value cfg_type = resolve_configuration_type(*i);
      s << "      <Configuration\n"
           "         Name=\"" << i->name_ << "|Win32\"\n"
           "         OutputDirectory=\"$(SolutionDir)$(ConfigurationName)\"\n"
           "         IntermediateDirectory=\"$(ConfigurationName)\"\n"
           "         ConfigurationType=\"" << cfg_type << "\"\n";

      configuration_options_.write(s, *i->node_, fake_environment(project_output_dir()));

      write_compiler_options(s, compiller_options_, *i->node_, fake_environment(project_output_dir()));

      switch(cfg_type)
      {
         case configuration_types::exe:
         case configuration_types::shared_lib:
         {
            s << "         <Tool\n"
                 "            Name=\"VCLinkerTool\"\n";
            linker_options_.write(s, *i->node_, fake_environment(project_output_dir()));
            s << "         />\n";
            break;
         }
      }

      s << "      </Configuration>\n";
   }

   s << "   </Configurations>\n"; 
}

static feature_set* compute_file_conf_properties(const basic_target& target, const msvc_project::variant& v)
{
   feature_set* result = v.target_->meta_target()->project()->engine()->feature_registry().make_set();
   for(feature_set::const_iterator i = target.properties().begin(), last = target.properties().end(); i != last; ++i)
   {
      feature_set::const_iterator f = v.properties_->find(**i);
      if (f == v.properties_->end())
         result->join(*i);
   }

   return result;
}

void msvc_project::file_configuration::write(write_context& ctx, const variant& v) const
{
   if (!v.properties_->contains(target_->properties()))
   {
      ctx.output_ << "              <FileConfiguration\n"
                  << "                   Name=\"" << v.name_ << "\">\n";
      write_compiler_options(ctx.output_, ctx.compiller_options_, *node_, ctx.environment_);
      ctx.output_ << "              </FileConfiguration>\n";
   }
}

void msvc_project::file_with_cfgs_t::write(write_context& ctx, const std::string& path_prefix) const
{
   location_t file_name(location_t(path_prefix) / file_name_.to_string());
   file_name.normalize();
   ctx.output_ << "         <File\n"
                  "            RelativePath=\"" << file_name.native_file_string() << "\">\n";

   for(file_config_t::const_iterator i = file_config.begin(), last = file_config.end(); i != last; ++i)
      if (*i->first->properties_ != i->second.target_->properties() &&
          &i->second.target_->type() != &ctx.h_type_) // discard any differences for H targets. Done for PCH. May be FIXME:
      {
         i->second.write(ctx, *i->first);
      }

   ctx.output_ << "         </File>\n";
}

struct less_target 
{
   bool operator ()(const basic_target* lhs, const basic_target* rhs)
   {
      location_t lhs_id = lhs->location() / lhs->name().to_string();
      location_t rhs_id = rhs->location() / rhs->name().to_string();
      lhs_id.normalize();
      rhs_id.normalize();

      return lhs_id < rhs_id;
   }
};

void msvc_project::filter_t::write(write_context& ctx, const std::string& path_prefix) const
{
   ctx.output_ << "         <Filter\n"
                  "            Name=\"" << name << "\"\n"
                  "            Filter=\"cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx\"\n"
                  "            UniqueIdentifier=\"" << uid << "\">\n";

   // FIXME: this trick used only for test to stabilize order of sources in project file
   typedef std::map<const basic_target*, boost::reference_wrapper<const file_with_cfgs_t>, less_target> stabilized_t;
   stabilized_t stabilized(files_.begin(), files_.end());

   for(stabilized_t::const_iterator i = stabilized.begin(), last = stabilized.end(); i != last; ++i)
      i->second.get().write(ctx, path_prefix);

   ctx.output_ << "         </Filter>\n";
}

void msvc_project::write_files(write_context& ctx) const
{
   ctx.output_ << "      <Files>\n";
   string path_prefix = meta_target_relative_to_output_.native_file_string();
   for(files_t::const_iterator i = files_.begin(), last = files_.end(); i != last; ++i)
      i->write(ctx, path_prefix);

   ctx.output_ << "      </Files>\n";
}

void msvc_project::generate()
{
   if (variants_.empty())
      throw runtime_error("Can't generate empty msvc project");

   fill_filters();
   gether_files();
}

void msvc_project::write() const
{
   if (variants_.empty())
      throw runtime_error("Can't write empty msvc project");

   create_directories(full_project_name_.branch_path());
   boost::filesystem::ofstream f(full_project_name_, std::ios_base::trunc);
   write_header(f);
   write_configurations(f);

   fake_environment environment(project_output_dir());
   write_context ctx(f, engine_->get_type_registry().get(types::H), environment, compiller_options_);
   write_files(ctx);

   write_bottom(f);
}

bool msvc_project::filter_t::accept(const type* t) const
{
   for(types_t::const_iterator i = types_.begin(), last = types_.end(); i != last; ++i)
   {
      if (*i == t)
         return true;
   }

   return false;
}

void msvc_project::filter_t::insert(const boost::intrusive_ptr<build_node>& node, 
                                    const basic_target* t, 
                                    const variant& v)
{
   file_with_cfgs_t& fwc = files_[t];
   fwc.file_name_ = t->name();
   file_configuration& fc = fwc.file_config[&v];
   fc.exclude_from_build = false;
   fc.target_ = t;
   fc.node_ = node;
}

void msvc_project::insert_into_files(const boost::intrusive_ptr<build_node>& node, 
                                     const basic_target* t, 
                                     const variant& v)
{
   const type* tp = &t->type();
   for(files_t::iterator fi = files_.begin(), flast = files_.end(); fi != flast; ++fi)
   {
      if (fi->accept(tp))
      {
         fi->insert(node, t, v);
         return;
      }
   }
}

void msvc_project::gether_files_impl(const build_node& node, variant& v)
{
   typedef build_node::sources_t::const_iterator iter;
   for(iter mi = node.sources_.begin(), mlast = node.sources_.end(); mi != mlast; ++mi)
   {
      if (mi->source_target_->mtarget()->meta_target() == meta_target_ ||
          mi->source_target_->mtarget()->type().equal_or_derived_from(obj_type_) ||
          mi->source_target_->mtarget()->type().equal_or_derived_from(pch_type_))
      {
         insert_into_files(mi->source_node_, mi->source_target_, v);
         if (mi->source_node_)
            gether_files_impl(*mi->source_node_, v);
      }
      else
      {
         if (mi->source_target_->mtarget()->type().equal_or_derived_from(searched_lib_))
         { // this target is searched lib product
            const pstring& file_name = mi->source_target_->name();
            location_t searched_file(mi->source_target_->location().empty() 
                                       ? file_name.to_string() 
                                       : relative_path(mi->source_target_->location(), project_output_dir_) / file_name.to_string());
            searched_file.normalize();
         }
         else
            dependencies_.push_back(mi->source_target_->mtarget());
      }
   }
}

void msvc_project::gether_files()
{
   for(variants_t::iterator i = variants_.begin(), last = variants_.end(); i != last; ++i)
      gether_files_impl(*i->node_, *i);

   std::sort(dependencies_.begin(), dependencies_.end());
   dependencies_.erase(std::unique(dependencies_.begin(), dependencies_.end()), dependencies_.end());
}

bool msvc_project::has_variant(const main_target* v) const
{
   for(variants_t::const_iterator i = variants_.begin(), last = variants_.end(); i != last; ++i)
   {
      if (i->target_ == v)
         return true;
   }

   return false;
}

}}