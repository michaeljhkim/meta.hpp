/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../meta_base.hpp"
#include "../meta_types.hpp"

namespace meta_hpp
{
    inline any_type::any_type(data_ptr data)
    : data_{data} {}

    inline bool any_type::is_valid() const noexcept {
        return data_ != nullptr;
    }

    inline any_type::operator bool() const noexcept {
        return is_valid();
    }

    inline type_id any_type::get_id() const noexcept {
        return data_->id;
    }

    inline type_kind any_type::get_kind() const noexcept {
        return data_->kind;
    }

    inline const metadata_map& any_type::get_metadata() const noexcept {
        return data_->metadata;
    }

    inline any_type::any_type(const array_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const class_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const constructor_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const destructor_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const enum_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const function_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const member_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const method_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const nullptr_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const number_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const pointer_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const reference_type& other) noexcept
    : data_{detail::type_access(other)} {}

    inline any_type::any_type(const void_type& other) noexcept
    : data_{detail::type_access(other)} {}

    template < detail::type_family Type >
    bool any_type::is() const noexcept {
        if constexpr ( std::is_same_v<Type, any_type> ) {
            return data_ != nullptr;
        } else {
            return data_ != nullptr && data_->kind == Type::kind;
        }
    }

    template < detail::type_family Type >
    Type any_type::as() const noexcept {
        return is<Type>() ? Type{static_cast<typename Type::data_ptr>(data_)} : Type{};
    }

    inline bool any_type::is_array() const noexcept {
        return is<array_type>();
    }

    inline bool any_type::is_class() const noexcept {
        return is<class_type>();
    }

    inline bool any_type::is_constructor() const noexcept {
        return is<constructor_type>();
    }

    inline bool any_type::is_destructor() const noexcept {
        return is<destructor_type>();
    }

    inline bool any_type::is_enum() const noexcept {
        return is<enum_type>();
    }

    inline bool any_type::is_function() const noexcept {
        return is<function_type>();
    }

    inline bool any_type::is_member() const noexcept {
        return is<member_type>();
    }

    inline bool any_type::is_method() const noexcept {
        return is<method_type>();
    }

    inline bool any_type::is_nullptr() const noexcept {
        return is<nullptr_type>();
    }

    inline bool any_type::is_number() const noexcept {
        return is<number_type>();
    }

    inline bool any_type::is_pointer() const noexcept {
        return is<pointer_type>();
    }

    inline bool any_type::is_reference() const noexcept {
        return is<reference_type>();
    }

    inline bool any_type::is_void() const noexcept {
        return is<void_type>();
    }

    inline array_type any_type::as_array() const noexcept {
        return as<array_type>();
    }

    inline class_type any_type::as_class() const noexcept {
        return as<class_type>();
    }

    inline constructor_type any_type::as_constructor() const noexcept {
        return as<constructor_type>();
    }

    inline destructor_type any_type::as_destructor() const noexcept {
        return as<destructor_type>();
    }

    inline enum_type any_type::as_enum() const noexcept {
        return as<enum_type>();
    }

    inline function_type any_type::as_function() const noexcept {
        return as<function_type>();
    }

    inline member_type any_type::as_member() const noexcept {
        return as<member_type>();
    }

    inline method_type any_type::as_method() const noexcept {
        return as<method_type>();
    }

    inline nullptr_type any_type::as_nullptr() const noexcept {
        return as<nullptr_type>();
    }

    inline number_type any_type::as_number() const noexcept {
        return as<number_type>();
    }

    inline pointer_type any_type::as_pointer() const noexcept {
        return as<pointer_type>();
    }

    inline reference_type any_type::as_reference() const noexcept {
        return as<reference_type>();
    }

    inline void_type any_type::as_void() const noexcept {
        return as<void_type>();
    }
}
