/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_infos_fwd.hpp"

#include "ctor_info.hpp"
#include "data_info.hpp"
#include "enum_info.hpp"
#include "function_info.hpp"
#include "member_info.hpp"
#include "method_info.hpp"

namespace meta_hpp
{
    class class_info final {
    public:
        class_info() = default;

        void merge(const class_info& other);
        explicit operator bool() const noexcept;

        const std::string& name() const noexcept;
        const class_type& type() const noexcept;
    public:
        template < typename F >
        void visit(F&& f) const;

        template < typename F >
        void each_class(F&& f) const;

        template < typename F >
        void each_ctor(F&& f) const;

        template < typename F >
        void each_data(F&& f) const;

        template < typename F >
        void each_enum(F&& f) const;

        template < typename F >
        void each_function(F&& f) const;

        template < typename F >
        void each_member(F&& f) const;

        template < typename F >
        void each_method(F&& f) const;
    private:
        template < typename Class > friend class class_;

        template < typename Class >
        explicit class_info(typename_arg_t<Class>, std::string name);
    private:
        struct state;
        std::shared_ptr<state> state_;
    };
}

namespace meta_hpp
{
    struct class_info::state final {
        std::string name;
        class_type type;
        class_info_map classes;
        ctor_info_map ctors;
        data_info_map datas;
        enum_info_map enums;
        function_info_map functions;
        member_info_map members;
        method_info_map methods;
    };
}

namespace meta_hpp
{
    inline void class_info::merge(const class_info& other) {
        (void)other;
        ///TODO: implme
    }

    inline class_info::operator bool() const noexcept {
        return !!state_;
    }

    inline const std::string& class_info::name() const noexcept {
        return state_->name;
    }

    inline const class_type& class_info::type() const noexcept {
        return state_->type;
    }
}

namespace meta_hpp
{
    template < typename F >
    void class_info::visit(F&& f) const {
        each_class(f);
        each_ctor(f);
        each_data(f);
        each_enum(f);
        each_function(f);
        each_member(f);
        each_method(f);
    }

    template < typename F >
    void class_info::each_class(F&& f) const {
        for ( auto&& name_info : state_->classes ) {
            std::invoke(f, name_info.second);
        }
    }

    template < typename F >
    void class_info::each_ctor(F&& f) const {
        for ( auto&& id_info : state_->ctors ) {
            std::invoke(f, id_info.second);
        }
    }

    template < typename F >
    void class_info::each_data(F&& f) const {
        for ( auto&& name_info : state_->datas ) {
            std::invoke(f, name_info.second);
        }
    }

    template < typename F >
    void class_info::each_enum(F&& f) const {
        for ( auto&& name_info : state_->enums ) {
            std::invoke(f, name_info.second);
        }
    }

    template < typename F >
    void class_info::each_function(F&& f) const {
        for ( auto&& name_info : state_->functions ) {
            std::invoke(f, name_info.second);
        }
    }

    template < typename F >
    void class_info::each_member(F&& f) const {
        for ( auto&& name_info : state_->members ) {
            std::invoke(f, name_info.second);
        }
    }

    template < typename F >
    void class_info::each_method(F&& f) const {
        for ( auto&& name_info : state_->methods ) {
            std::invoke(f, name_info.second);
        }
    }
}

namespace meta_hpp
{
    template < typename Class >
    inline class_info::class_info(typename_arg_t<Class>, std::string name)
    : state_{std::make_shared<state>(state{
        std::move(name),
        class_type{typename_arg<Class>},
        {}, {}, {}, {}, {}, {}, {}
    })} {}
}
