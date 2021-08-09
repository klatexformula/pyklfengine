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




void pyklf_def_engines(py::module & m)
{

  //
  // ******* klfengine::klfimplpkg_engine *******
  //
  
  { // klfimplpkg_engine module -- only quick compilation function for now
    auto klfimplpkg_engine_m = m.def_submodule("klfimplpkg_engine", "klfimplpkg engine (...)");

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
  }

  //
  // ******* klfengine::latextoimage_engine *******
  //
  
  { // latextoimage_engine module -- only quick compilation function for now
    auto latextoimage_engine_m = m.def_submodule("latextoimage_engine", "latextoimage engine (...)");

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
  }

}
