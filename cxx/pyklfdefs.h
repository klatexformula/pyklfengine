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

#pragma once


#define KLFENGINE_HANDLE_WARNING pyklf_callback_warn
#define KLFENGINE_HANDLE_ERROR pyklf_callback_error

#include <klfengine/klfengine>

#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using namespace pybind11::literals;





template<typename ClassType, typename MemberType>
void set_if_in_kwargs(ClassType & x, MemberType ClassType::* member, std::string member_name,
                      py::kwargs & kwargs)
{
  auto member_name_s = py::str(member_name);
  if (kwargs.attr("__contains__")(member_name_s).cast<bool>()) {
    x.*member = kwargs.attr("pop")(member_name_s).cast<MemberType>();
  }
}



// Use the interface that these objects provide to get populated via
// nlohmann::json objects.  Using python's json module is a terrible hack.
template<typename ObjType>
void pydict_get_to_obj(ObjType & obj, const py::dict & pyd)
{
  auto json_module = py::module::import("json");
  nlohmann::json j =
    nlohmann::json::parse( json_module.attr("dumps")(pyd).cast<std::string>() );
  j.get_to(obj);
}
inline
py::dict pydict_from_valuedict(const klfengine::value::dict & dic)
{
  auto json_module = py::module::import("json");
  nlohmann::json j;
  j = dic;
  return json_module.attr("loads")( j.dump() );
}



// // type casters in case we use mpark/variant
// #ifdef KLFENGINE_USE_MPARK_VARIANT
// // cf. https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#c-17-library-containers
// namespace pybind11 { namespace detail {
// template <typename... Ts>
// struct type_caster<mpark::variant<Ts...>> : variant_caster<mpark::variant<Ts...>> {};
// // Specifies the function used to visit the variant -- `apply_visitor` instead of `visit`
// template <>
// struct visit_helper<mpark::variant> {
//   template <typename... Args>
//   static auto call(Args &&...args) -> decltype(mpark::apply_visitor(args...)) {
//     return mpark::apply_visitor(args...);
//   }
// };
// }} // namespace pybind11::detail
// #endif



// py::object value_to_pyobj(const klfengine::value & value);
// klfengine::value pyobj_to_value(const py::object & obj);
//
//
// py::dict valuedict_to_pydict(const klfengine::value::dict & d)
// {
//   py::dict pyd{};
//   for (const auto & item : d) {
//     pyd[item.first] = py::cast(item.second);
//   }
//   return pyd;
// }
// klfengine::value::dict pydict_to_valuedict(const py::dict & pyd)
// {
//   klfengine::value::dict d;
//   for (auto item : pyd) {
//     d[item.first] = item.second.cast<klfengine::value>();
//   }
//   return d;
// }
// py::list valuearray_to_pylist(const klfengine::value::array & d)
// {
//   py::list pyd{};
//   for (const auto & item : d) {
//     pyd.append(value_to_pyobj(item));
//   }
//   return pyd;
// }
// template<typename PyListOrTuple>
// klfengine::value::array pylist_to_valuearray(const PyListOrTuple & pyd)
// {
//   klfengine::value::array d;
//   for (auto item : pyd) {
//     d.push_back(pyobj_to_value(item));
//   }
//   return d;
// }
//
// struct value_to_pyobj_transformer
// {
//   py::object operator()(const klfengine::value::array & a)
//   {
//     return valuearray_to_pylist(a);
//   }
//   py::object operator()(const klfengine::value::dict & d)
//   {
//     return valuedict_to_pylist(d);
//   }
//   template<typename SimpleType,
//            typename SimpleTypeNoRef = typename std::remove_reference<SimpleType>::type,
//            typename std::enable_if<
//              !(std::is_same<SimpleTypeNoRef, value::array>::value ||
//                std::is_same<SimpleTypeNoRef, value::dict>::value),
//            bool>::type = true>
//   py::object operator()(SimpleType x) {
//     return py::cast(x);
//   }
// };
//
// py::object value_to_pyobj(const klfengine::value & value)
// {
//   return value.transform(value_to_pyobj_transformer{});
// }
// klfengine::value pyobj_to_value(py::object obj)
// {
//   if ( py::isinstance<py::list>(obj) ) {
//     return klfengine::value{pylist_to_valuearray(py::list{obj})};
//   }
//   if ( py::isinstance<py::tuple>(obj) ) {
//     return klfengine::value{pylist_to_valuearray(py::tuple{obj})};
//   }
//   if ( py::isinstance<py::dict>(obj) ) {
//     return klfengine::value{pydict_to_valuedict(py::dict{obj})};
//   }
//   return 
// }
