
#include "pyklf_basestuff.h"

void pyklf_callback_error(const std::string & where, const std::string & what)
{
  auto pylogging = py::module::import("logging");
  pylogging.attr("getLogger")(where).attr("error")(what);
}
void pyklf_callback_warn(const std::string & where, const std::string & what)
{
  auto pylogging = py::module::import("logging");
  pylogging.attr("getLogger")(where).attr("warning")(what);
}


void pyklf_def_base_stuff(py::module & m)
{
  //
  // ******* klfengine basic data types *******
  //

  py::class_<klfengine::length>(m, "length")
    .def(py::init(
             [](double value, std::string unit) {
               return std::unique_ptr<klfengine::length>{
                 new klfengine::length{value, std::move(unit)}
               };
             }),
         "value"_a = 0.0,
         "unit"_a = std::string{"pt"}
    )
    .def(py::init(
             [](const std::string & texlength) {
               return std::unique_ptr<klfengine::length>{
                 new klfengine::length{texlength}
               };
             })
    )
    .def(py::init<const klfengine::length&>())
    .def_readwrite("value", &klfengine::length::value)
    .def_readwrite("unit", &klfengine::length::unit)
    .def("__repr__",
         [](const klfengine::length & l) {
           std::string repr_length_as_str{
             py::repr(py::str{l.to_string()}).cast<std::string>()
           };
           return "pyklfengine.length(" + repr_length_as_str + ")";
         })
    ;

  py::class_<klfengine::color>(m, "color")
    .def(py::init(
             [](uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
               return std::unique_ptr<klfengine::color>{ new klfengine::color{
                   r, g, b, a
                 } };
             }),
         "red"_a = 0,
         "green"_a = 0,
         "blue"_a = 0,
         "alpha"_a = 255
        )
    .def(py::init<const klfengine::color&>())
    .def_readwrite("red", &klfengine::color::red)
    .def_readwrite("green", &klfengine::color::green)
    .def_readwrite("blue", &klfengine::color::blue)
    .def_readwrite("alpha", &klfengine::color::alpha)
    .def("__repr__",
         [](const klfengine::color & c) {
           nlohmann::json j;
           j = c;
           return "pyklfengine.color(*" + j.dump() + ")";
         })
    ;

  py::class_<klfengine::margins>(m, "margins")
    .def(py::init(
             [](py::dict d) {
               auto mm = std::unique_ptr<klfengine::margins>{ new klfengine::margins{} };
               pydict_get_to_obj(*mm, d);
               return mm;
             }
             ))
    .def(py::init(
             [](klfengine::length top, klfengine::length right,
                klfengine::length bottom, klfengine::length left) {
               return std::unique_ptr<klfengine::margins>{ new klfengine::margins{
                   top, right, bottom, left
                 } };
             }),
         "top"_a = klfengine::length{},
         "right"_a = klfengine::length{},
         "bottom"_a = klfengine::length{},
         "left"_a = klfengine::length{}
        )
    .def(py::init(
             [](double top, double right, double bottom, double left) {
               return std::unique_ptr<klfengine::margins>{ new klfengine::margins{
                   top, right, bottom, left
                 } };
             }),
         "top"_a = klfengine::length{},
         "right"_a = klfengine::length{},
         "bottom"_a = klfengine::length{},
         "left"_a = klfengine::length{}
        )
    .def(py::init<const klfengine::margins&>())
    .def_readwrite("top", &klfengine::margins::top)
    .def_readwrite("right", &klfengine::margins::right)
    .def_readwrite("bottom", &klfengine::margins::bottom)
    .def_readwrite("left", &klfengine::margins::left)
    .def("__repr__",
         [](const klfengine::margins & m) {
           nlohmann::json j;
           j = m;
           return "pyklfengine.margins(*" + j.dump() + ")";
         })
    ;

  //
  // ******* klfengine::format_spec *******
  //

  py::class_<klfengine::format_spec>(m, "format_spec")
    .def(py::init(
             [](std::string format, py::dict parameters_py) {
               klfengine::value::dict parameters;
               pydict_get_to_obj(parameters, parameters_py);
               return std::unique_ptr<klfengine::format_spec>{ new klfengine::format_spec{
                   format, parameters
                 } };
             }),
         "format"_a = std::string{},
         "parameters"_a = py::dict{}
        )
    .def(py::init<const klfengine::format_spec&>())
    .def_readwrite("format", &klfengine::format_spec::format)
    .def_property(
        "parameters",
        [](klfengine::format_spec & obj) {
          return pydict_from_valuedict(obj.parameters);
        },
        [](klfengine::format_spec & obj, py::dict dic) {
          pydict_get_to_obj(obj.parameters, dic);
        }
        )
    .def("__repr__",
         [](const klfengine::format_spec & m) {
           nlohmann::json j;
           j = m.parameters;
           return "pyklfengine.format_spec(format=" + m.format + ", parameters="
             + j.dump() + ")";
         })
    ;

  //
  // ******* klfengine::format_description *******
  //

  py::class_<klfengine::format_description>(m, "format_description")
    .def(py::init(
             [](klfengine::format_spec format_spec, std::string title, std::string desc) {
               return std::unique_ptr<klfengine::format_description>{
                 new klfengine::format_description{
                   std::move(format_spec), std::move(title), std::move(desc)
                 }
               };
             }),
         "format_spec"_a = klfengine::format_spec{},
         "title"_a = std::string{},
         "description"_a = std::string{}
        )
    .def(py::init<const klfengine::format_description&>())
    .def_readwrite("format_spec", &klfengine::format_description::format_spec)
    .def_readwrite("title", &klfengine::format_description::title)
    .def_readwrite("description", &klfengine::format_description::description)
    .def("__repr__",
         [](const klfengine::format_description & m) {
           nlohmann::json jfs;
           jfs = m.format_spec;
           nlohmann::json jtitle;
           jtitle = m.title;
           nlohmann::json jdesc;
           jdesc = m.description;
           return "pyklfengine.format_description(format_spec=" + jfs.dump()
             + ", title=" + jtitle.dump() + ", description=" + jdesc.dump() + ")";
         })
    ;

  //
  // ******* klfengine::format_description *******
  //
  py::class_<klfengine::format_provider>(m, "format_provider")
    .def("canonical_format", &klfengine::format_provider::canonical_format)
    .def("canonical_format_or_empty", &klfengine::format_provider::canonical_format_or_empty)
    .def("available_formats", &klfengine::format_provider::available_formats)
    .def("has_format",
         static_cast<bool (klfengine::format_provider::*)(std::string)>(
           &klfengine::format_provider::has_format
         )
    )
    .def("has_format",
         static_cast<bool (klfengine::format_provider::*)(const klfengine::format_spec &)>(
           &klfengine::format_provider::has_format
         )
    )
    .def("find_format", [](klfengine::format_provider & f,
                           const std::vector<klfengine::format_spec> & formats) {
      return f.find_format(formats);
    })
    ;

  //
  // ******* klfengine::input *******
  //

  py::class_<klfengine::input>(m, "input")
    .def(py::init(
             [](py::kwargs kwargs) {
               auto inptr = std::unique_ptr<klfengine::input>{new klfengine::input{}};

               klfengine::input & in = *inptr;

               set_if_in_kwargs(in, &klfengine::input::latex, "latex", kwargs);
               set_if_in_kwargs(in, &klfengine::input::math_mode, "math_mode", kwargs);
               set_if_in_kwargs(in, &klfengine::input::preamble, "preamble", kwargs);
               set_if_in_kwargs(in, &klfengine::input::latex_engine, "latex_engine", kwargs);
               set_if_in_kwargs(in, &klfengine::input::font_size, "font_size", kwargs);
               set_if_in_kwargs(in, &klfengine::input::fg_color, "fg_color", kwargs);
               set_if_in_kwargs(in, &klfengine::input::bg_color, "bg_color", kwargs);
               set_if_in_kwargs(in, &klfengine::input::margins, "margins", kwargs);
               set_if_in_kwargs(in, &klfengine::input::dpi, "dpi", kwargs);
               set_if_in_kwargs(in, &klfengine::input::scale, "scale", kwargs);
               set_if_in_kwargs(in, &klfengine::input::outline_fonts, "outline_fonts", kwargs);
               set_if_in_kwargs(in, &klfengine::input::parameters, "parameters", kwargs);

               if (py::len(kwargs) > 0) {
                 throw py::value_error("Invalid keyword argument for pyklfengine.input: "
                                       + py::repr(kwargs).cast<std::string>());
               }

               return inptr;
             }))
    .def(py::init<const klfengine::input&>())
    .def_readwrite("latex", &klfengine::input::latex)
    .def_readwrite("math_mode", &klfengine::input::math_mode)
    .def_readwrite("preamble", &klfengine::input::preamble)
    .def_readwrite("latex_engine", &klfengine::input::latex_engine)
    .def_readwrite("font_size", &klfengine::input::font_size)
    .def_readwrite("fg_color", &klfengine::input::fg_color)
    .def_readwrite("bg_color", &klfengine::input::bg_color)
    .def_readwrite("margins", &klfengine::input::margins)
    .def_readwrite("dpi", &klfengine::input::dpi)
    .def_readwrite("scale", &klfengine::input::scale)
    .def_readwrite("outline_fonts", &klfengine::input::outline_fonts)
    .def_property(
        "parameters",
        [](klfengine::input & obj) {
          return pydict_from_valuedict(obj.parameters);
        },
        [](klfengine::input & obj, py::dict dic) {
          pydict_get_to_obj(obj.parameters, dic);
        }
        )
    .def("to_json", [](klfengine::input & obj) {
          nlohmann::json j;
          j = obj;
          return j.dump();
        })
    .def("__repr__",
         [](const klfengine::input & c) {
           nlohmann::json j;
           j = c;
           return "pyklfengine.input(*" + j.dump() + ")";
         })
    ;



  //
  // ******* klfengine::settings *******
  //
  py::class_<klfengine::settings>(m, "settings")
    .def(py::init(
             [](py::kwargs kwargs) {
               auto settptr = std::unique_ptr<klfengine::settings>{new klfengine::settings{}};

               klfengine::settings & sett = *settptr;

               set_if_in_kwargs(sett, &klfengine::settings::temporary_directory,
                                "temporary_directory", kwargs);
               set_if_in_kwargs(sett, &klfengine::settings::texbin_directory,
                                "texbin_directory", kwargs);
               set_if_in_kwargs(sett, &klfengine::settings::gs_method,
                                "gs_method", kwargs);
               set_if_in_kwargs(sett, &klfengine::settings::gs_executable_path,
                                "gs_executable_path", kwargs);
               set_if_in_kwargs(sett, &klfengine::settings::gs_libgs_path,
                                "gs_libgs_path", kwargs);
               set_if_in_kwargs(sett, &klfengine::settings::subprocess_add_environment,
                                "subprocess_add_environment", kwargs);

               if (py::len(kwargs) > 0) {
                 throw py::value_error("Invalid keyword argument for pyklfengine.settings: "
                                       + py::repr(kwargs).cast<std::string>());
               }

               return settptr;
             }))
    .def(py::init<const klfengine::settings&>())
    .def_readwrite("temporary_directory", &klfengine::settings::temporary_directory)
    .def_readwrite("texbin_directory", &klfengine::settings::texbin_directory)
    .def_readwrite("gs_method", &klfengine::settings::gs_method)
    .def_readwrite("gs_executable_path", &klfengine::settings::gs_executable_path)
    .def_readwrite("gs_libgs_path", &klfengine::settings::gs_libgs_path)
    .def_readwrite("subprocess_add_environment",
                   &klfengine::settings::subprocess_add_environment)
    .def("get_tex_executable_path",
         [](const klfengine::settings & obj, const std::string & exe_name) {
           return obj.get_tex_executable_path(exe_name);
         }
        )
    .def_static(
        "detect_settings",
        [](const std::vector<std::string> & extra_paths) {
          return klfengine::settings::detect_settings(extra_paths);
        },
        "extra_paths"_a = std::vector<std::string>{}
    )
    .def_static(
        "get_wildcard_search_paths",
        [](const std::vector<std::string> & extra_paths) {
          return klfengine::settings::get_wildcard_search_paths(extra_paths);
        },
        "extra_paths"_a = std::vector<std::string>{}
    )
    .def("to_json", [](klfengine::settings & obj) {
          nlohmann::json j;
          j = obj;
          return j.dump();
        })
    .def("__repr__",
         [](const klfengine::settings & c) {
           nlohmann::json j;
           j = c;
           return "pyklfengine.settings(*" + j.dump() + ")";
         })
    ;

}
