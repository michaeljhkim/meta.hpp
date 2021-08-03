/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "meta_fwd.hpp"
#include "meta_infos.hpp"

#include "meta_registry/class_.hpp"
#include "meta_registry/data_.hpp"
#include "meta_registry/enum_.hpp"
#include "meta_registry/evalue_.hpp"
#include "meta_registry/function_.hpp"
#include "meta_registry/member_.hpp"
#include "meta_registry/method_.hpp"
#include "meta_registry/namespace_.hpp"

namespace meta_hpp
{
    class registry final {
    public:
        registry() = default;

        class_info get_class_by_name(std::string_view name) const noexcept;
        data_info get_data_by_name(std::string_view name) const noexcept;
        enum_info get_enum_by_name(std::string_view name) const noexcept;
        evalue_info get_evalue_by_name(std::string_view name) const noexcept;
        function_info get_function_by_name(std::string_view name) const noexcept;
        member_info get_member_by_name(std::string_view name) const noexcept;
        method_info get_method_by_name(std::string_view name) const noexcept;
        namespace_info get_namespace_by_name(std::string_view name) const noexcept;
    public:
        template < typename... Internals >
        registry& operator()(Internals&&... internals);
    private:
        template < typename Class >
        void add_(const std::string& prefix, const class_<Class>& internal);
        void add_(const std::string& prefix, const data_& internal);
        template < typename Enum >
        void add_(const std::string& prefix, const enum_<Enum>& internal);
        template < typename Function >
        void add_(const std::string& prefix, const function_<Function>& internal);
        template < typename Member >
        void add_(const std::string& prefix, const member_<Member>& internal);
        template < typename Method >
        void add_(const std::string& prefix, const method_<Method>& internal);
        void add_(const std::string& prefix, const namespace_& internal);
        void add_(const std::string& prefix, ...) = delete;
    private:
        struct add_info_f;
        void add_info_(const std::string& prefix, const class_info& info);
        void add_info_(const std::string& prefix, const ctor_info& info);
        void add_info_(const std::string& prefix, const data_info& info);
        void add_info_(const std::string& prefix, const enum_info& info);
        void add_info_(const std::string& prefix, const evalue_info& info);
        void add_info_(const std::string& prefix, const function_info& info);
        void add_info_(const std::string& prefix, const member_info& info);
        void add_info_(const std::string& prefix, const method_info& info);
        void add_info_(const std::string& prefix, const namespace_info& info);
        void add_info_(const std::string& prefix, ...) = delete;
    private:
        class_info_map classes_;
        data_info_map datas_;
        enum_info_map enums_;
        evalue_info_map evalues_;
        function_info_map functions_;
        member_info_map members_;
        method_info_map methods_;
        namespace_info_map namespaces_;
    };
}

namespace meta_hpp
{
    inline class_info registry::get_class_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(classes_, name);
    }

    inline data_info registry::get_data_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(datas_, name);
    }

    inline enum_info registry::get_enum_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(enums_, name);
    }

    inline evalue_info registry::get_evalue_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(evalues_, name);
    }

    inline function_info registry::get_function_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(functions_, name);
    }

    inline member_info registry::get_member_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(members_, name);
    }

    inline method_info registry::get_method_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(methods_, name);
    }

    inline namespace_info registry::get_namespace_by_name(std::string_view name) const noexcept {
        return detail::find_or_default(namespaces_, name);
    }
}

namespace meta_hpp
{
    template < typename... Internals >
    inline registry& registry::operator()(Internals&&... internals) {
        (add_(std::string{}, std::forward<Internals>(internals)), ...);
        return *this;
    }
}

namespace meta_hpp
{
    template < typename Class >
    inline void registry::add_(const std::string& prefix, const class_<Class>& internal) {
        add_info_(prefix, internal.make_info());
    }

    inline void registry::add_(const std::string& prefix, const data_& internal) {
        add_info_(prefix, internal.make_info());
    }

    template < typename Enum >
    inline void registry::add_(const std::string& prefix, const enum_<Enum>& internal) {
        add_info_(prefix, internal.make_info());
    }

    template < typename Function >
    inline void registry::add_(const std::string& prefix, const function_<Function>& internal) {
        add_info_(prefix, internal.make_info());
    }

    template < typename Member >
    inline void registry::add_(const std::string& prefix, const member_<Member>& internal) {
        add_info_(prefix, internal.make_info());
    }

    template < typename Method >
    inline void registry::add_(const std::string& prefix, const method_<Method>& internal) {
        add_info_(prefix, internal.make_info());
    }

    inline void registry::add_(const std::string& prefix, const namespace_& internal) {
        add_info_(prefix, internal.make_info());
    }
}

namespace meta_hpp
{
    struct registry::add_info_f final {
        template < typename Info >
        void operator()(registry& self, const std::string& prefix, Info&& info) const {
            self.add_info_(prefix, std::forward<Info>(info));
        }
    };

    inline void registry::add_info_(const std::string& prefix, const class_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(classes_, name, info, &class_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const ctor_info& info) {
        (void)prefix;
        (void)info;
    }

    inline void registry::add_info_(const std::string& prefix, const data_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(datas_, name, info, &data_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const enum_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(enums_, name, info, &enum_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const evalue_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(evalues_, name, info, &evalue_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const function_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(functions_, name, info, &function_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const member_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(members_, name, info, &member_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const method_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(methods_, name, info, &method_info::merge);
    }

    inline void registry::add_info_(const std::string& prefix, const namespace_info& info) {
        std::string name = prefix.empty() ? info.name() : prefix + "::" + info.name();
        info.visit(curry(add_info_f{}, std::ref(*this), name));
        detail::merge_with(namespaces_, name, info, &namespace_info::merge);
    }
}
