/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../meta_base.hpp"
#include "../meta_types.hpp"

#include "../meta_traits/void_traits.hpp"

namespace meta_hpp::detail
{
    template < void_kind Void >
    struct void_tag {};

    template < void_kind Void >
    void_type_data_ptr void_type_data::get() {
        static void_type_data_ptr data = std::make_shared<void_type_data>(type_list<Void>{});
        return data;
    }

    template < void_kind Void >
    void_type_data::void_type_data(type_list<Void>)
    : type_data_base{type_id{type_list<void_tag<Void>>{}}, type_kind::void_}
    , flags{void_traits<Void>::make_flags()} {}
}

namespace meta_hpp
{
    inline void_type::void_type(detail::void_type_data_ptr data)
    : data_{std::move(data)} {}

    inline bool void_type::is_valid() const noexcept {
        return !!data_;
    }

    inline void_type::operator bool() const noexcept {
        return is_valid();
    }

    inline type_id void_type::get_id() const noexcept {
        return data_->id;
    }

    inline bitflags<void_flags> void_type::get_flags() const noexcept {
        return data_->flags;
    }
}
