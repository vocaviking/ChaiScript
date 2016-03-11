// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2016, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_DYNAMIC_OBJECT_HPP_
#define CHAISCRIPT_DYNAMIC_OBJECT_HPP_

#include <map>
#include <string>
#include <utility>

#include "boxed_value.hpp"

namespace chaiscript {
class Type_Conversions;
namespace dispatch {
class Proxy_Function_Base;
}  // namespace dispatch
}  // namespace chaiscript

namespace chaiscript
{
  namespace dispatch
  {
    struct option_explicit_set : std::runtime_error {
      option_explicit_set(const std::string &t_param_name)
        : std::runtime_error("option explicit set and parameter '" + t_param_name + "' does not exist")
      {

      }

      option_explicit_set(const option_explicit_set &) = default;

      virtual ~option_explicit_set() noexcept = default;
    };

    class Dynamic_Object
    {
      public:
        Dynamic_Object(std::string t_type_name)
          : m_type_name(std::move(t_type_name)), m_option_explicit(false)
        {
        }

        Dynamic_Object() : m_type_name(""), m_option_explicit(false)
        {
        }

        bool is_explicit() const
        {
          return m_option_explicit;
        }

        void set_explicit(const bool t_explicit)
        {
          m_option_explicit = t_explicit;
        }

        std::string get_type_name() const
        {
          return m_type_name;
        }

        const Boxed_Value &operator[](const std::string &t_attr_name) const
        {
          return get_attr(t_attr_name);
        }

        Boxed_Value &operator[](const std::string &t_attr_name)
        {
          return get_attr(t_attr_name);
        }

        const Boxed_Value &get_attr(const std::string &t_attr_name) const
        {
          auto a = m_attrs.find(t_attr_name);

          if (a != m_attrs.end()) {
            return a->second;
          } else {
            throw std::range_error("Attr not found '" + t_attr_name + "' and cannot be added to const obj");
          }
        }

        Boxed_Value &get_attr(const std::string &t_attr_name)
        {
          return m_attrs[t_attr_name];
        }

        Boxed_Value &method_missing(const std::string &t_method_name)
        {
          if (m_option_explicit && m_attrs.find(t_method_name) == m_attrs.end()) {
            throw option_explicit_set(t_method_name);
          }

          return get_attr(t_method_name);
        }

        const Boxed_Value &method_missing(const std::string &t_method_name) const
        {
          if (m_option_explicit && m_attrs.find(t_method_name) == m_attrs.end()) {
            throw option_explicit_set(t_method_name);
          }

          return get_attr(t_method_name);
        }


        std::map<std::string, Boxed_Value> get_attrs() const
        {
          return m_attrs;
        }

      private:
        std::string m_type_name;
        bool m_option_explicit;

        std::map<std::string, Boxed_Value> m_attrs;
    };

  }
}
#endif

