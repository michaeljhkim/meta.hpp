/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../meta_base.hpp"
#include "../meta_binds.hpp"
#include "../meta_registry.hpp"

namespace meta_hpp
{
    inline scope_bind::scope_bind(const scope& scope, metadata_map metadata)
    : state_bind_base{scope, std::move(metadata)} {}

    template < detail::function_pointer_kind Function, typename... Opts >
    scope_bind& scope_bind::function_(std::string name, Function function_ptr, Opts&&... opts) {
        using opts_t = detail::type_list<std::remove_cvref_t<Opts>...>;
        using policy_t = detail::type_list_first_of_t<function_policy::is_family, function_policy::as_copy_t, opts_t>;

        static_assert( //
            detail::type_list_count_of_v<function_policy::is_family, opts_t> <= 1,
            "function policy may be specified only once"
        );

        // there is no 'use after move' here because `from opts` takes only relevant opts
        metadata_bind::values_t metadata = metadata_bind::from_opts(META_HPP_FWD(opts)...);
        arguments_bind::values_t arguments = arguments_bind::from_opts(META_HPP_FWD(opts)...);

        auto state = detail::function_state::make<policy_t>(std::move(name), function_ptr, std::move(metadata));

        META_HPP_ASSERT(                                //
            arguments.size() <= state->arguments.size() //
            && "provided arguments don't match function argument count"
        );

        for ( std::size_t i{}, e{std::min(arguments.size(), state->arguments.size())}; i < e; ++i ) {
            argument& arg = state->arguments[i];
            detail::state_access(arg)->name = std::move(arguments[i].get_name());
            detail::state_access(arg)->metadata = std::move(arguments[i].get_metadata());
        }

        detail::insert_or_assign(get_state().functions, function{std::move(state)});
        return *this;
    }

    template < typename Type >
    scope_bind& scope_bind::typedef_(std::string name) {
        get_state().typedefs.insert_or_assign(std::move(name), resolve_type<Type>());
        return *this;
    }

    template < detail::pointer_kind Pointer, typename... Opts >
    scope_bind& scope_bind::variable_(std::string name, Pointer variable_ptr, Opts&&... opts) {
        using opts_t = detail::type_list<std::remove_cvref_t<Opts>...>;
        using policy_t = detail::type_list_first_of_t<variable_policy::is_family, variable_policy::as_copy_t, opts_t>;

        static_assert( //
            detail::type_list_count_of_v<variable_policy::is_family, opts_t> <= 1,
            "variable policy may be specified only once"
        );

        metadata_bind::values_t metadata = metadata_bind::from_opts(META_HPP_FWD(opts)...);
        auto state = detail::variable_state::make<policy_t>(std::move(name), variable_ptr, std::move(metadata));
        detail::insert_or_assign(get_state().variables, variable{std::move(state)});
        return *this;
    }
}
