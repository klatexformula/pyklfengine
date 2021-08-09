cmake_minimum_required(VERSION 3.9)

project(pyklfengine)

find_package(pybind11 2.4 REQUIRED)

# we'll include <klfengine/implementation> ourselves somewhere
set(KLFENGINE_SEPARATE_IMPLEMENTATION TRUE)

find_package(klfengine REQUIRED)

pybind11_add_module(pyklfengine
  cxx/pyklfengine.cxx
  cxx/pyklf_basestuff.cxx
  cxx/pyklf_engines.cxx
  )

set_target_properties(pyklfengine PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQURIED on
  )

target_link_libraries(pyklfengine
  PRIVATE klfengine::klfengine
  )
