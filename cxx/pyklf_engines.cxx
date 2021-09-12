/* This file is part of the pyklfengine library, which is distributed under the
 * terms of the MIT license.
 *
 *     https://github.com/klatexformula/pyklfengine
 *
 * The MIT License (MIT)
 *
 * Copyright 2021 Philippe Faist
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "pyklf_engines.h"


template<typename EngineClass>
py::bytes run_compile_to(klfengine::input input, klfengine::format_spec fmt,
                         py::object settings_obj)
{
  klfengine::settings sett;
  if (!settings_obj.is_none()) {
    sett = settings_obj.cast<klfengine::settings>();
  } else {
    sett = klfengine::settings::detect_settings();
  }
  
  EngineClass e;

  e.set_settings(sett);

  // { using json = nlohmann::json;
  //   json j; j = sett;
  //   fprintf(stderr, "DEBUG: using settings = %s\n", j.dump().c_str()); }

  auto r = e.run(input);
  r->compile();

  auto data = r->get_data(fmt);

  return py::bytes{(const char*)data.data(), data.size()};
}



// // see https://pybind11.readthedocs.io/en/stable/advanced/classes.html#combining-virtual-functions-and-inheritance
// template<typename KlfengineBase = klfengine::engine>
// class PyKlfEngineEngineTrampoline : public KlfengineBase
// {
// public:
//   using KlfengineBase::engine;
//
//   virtual void adjust_for_new_settings(klfengine::settings &settings)
//     override
//   {
//     PYBIND11_OVERRIDE(void, klfengine::engine, adjust_for_new_settings, settings);
//   }
//
//   virtual klfengine::engine_run_implementation *
//   impl_create_engine_run_implementation(klfengine::input input_, klfengine::settings settings_)
//     override
//   {
//     PYBIND11_OVERRIDE(
//       klfengine::engine_run_implementation *,
//       KlfengineBase,
//       adjust_for_new_settings,
//       input_, settings_
//     );
//   }
// };



void pyklf_def_engines(py::module & m)
{

  //
  // ******* klfengine::engine_run_implementation *******
  //
  py::class_<klfengine::engine_run_implementation, klfengine::format_provider>(
      m, "engine_run_implementation"
    )
    .def("input", &klfengine::engine_run_implementation::input)
    .def("settings", &klfengine::engine_run_implementation::settings)
    //.def("compile", &klfengine::engine_run_implementation::compile)
    ;

  //
  // ******* klfengine::run *******
  //
  py::class_<klfengine::run>(m, "run")
    //.def(py::init<std::unique_ptr<klfengine::engine_run_implementation> >())
    .def("compile", &klfengine::run::compile)
    .def("compiled", &klfengine::run::compiled)
    .def("has_format",
         static_cast<bool (klfengine::run::*)(std::string)>(
           &klfengine::run::has_format
         )
    )
    .def("has_format",
         static_cast<bool (klfengine::run::*)(const klfengine::format_spec&)>(
           &klfengine::run::has_format
         )
    )
    .def("available_formats", &klfengine::run::available_formats)
    .def("canonical_format", &klfengine::run::canonical_format)
    .def("canonical_format_or_empty", &klfengine::run::canonical_format_or_empty)
    .def("find_format",
         [](klfengine::run & r, const std::vector<klfengine::format_spec> & formats)
         {
           return r.find_format(formats);
         })
    .def("get_data",
         [](klfengine::run & r, const klfengine::format_spec & format)
         {
           const auto & data_cref = r.get_data_cref(format);
           return py::bytes{ (const char*)data_cref.data(), data_cref.size() };
         }
         )
    ;

  //
  // ******* klfengine::engine *******
  //
  py::class_<klfengine::engine>(m, "engine") //, PyKlfEngineTrampoline<> >(m, "engine")
    //.def(py::init<const std::string& >()) // pure virtual functions
    .def("name", &klfengine::engine::name)
    .def("set_settings", &klfengine::engine::set_settings)
    .def("settings", &klfengine::engine::settings)
    .def("run", &klfengine::engine::run)
    ;
  
  

  //
  // ******* klfengine::klfimplpkg_engine *******
  //
  
  { // klfimplpkg_engine module
    auto klfimplpkg_engine_m =
      m.def_submodule("klfimplpkg_engine", "klfimplpkg engine (...)");

    klfimplpkg_engine_m.def(
        "compile_to",
        [](klfengine::input input, klfengine::format_spec fmt, py::object settings_obj) {
          return run_compile_to<klfengine::klfimplpkg_engine::engine>(
              std::move(input), std::move(fmt), std::move(settings_obj)
          );
        },
        "input"_a,
        "format"_a,
        "settings"_a = py::none{}
    );

    py::class_<klfengine::klfimplpkg_engine::engine, klfengine::engine>(
        klfimplpkg_engine_m,
        "engine"
      )
      .def(py::init<>())
      ;

  }

  //
  // ******* klfengine::latextoimage_engine *******
  //
  
  { // latextoimage_engine module
    auto latextoimage_engine_m =
      m.def_submodule("latextoimage_engine", "latextoimage engine (...)");

    latextoimage_engine_m.def(
        "compile_to",
        [](klfengine::input input, klfengine::format_spec fmt, py::object settings_obj) {
          return run_compile_to<klfengine::latextoimage_engine::engine>(
              std::move(input), std::move(fmt), std::move(settings_obj)
          );
        },
        "input"_a,
        "format"_a,
        "settings"_a = py::none{}
    );


    py::class_<klfengine::latextoimage_engine::engine, klfengine::engine>(
        latextoimage_engine_m,
        "engine"
      )
      .def(py::init<>())
      ;
  }

}
