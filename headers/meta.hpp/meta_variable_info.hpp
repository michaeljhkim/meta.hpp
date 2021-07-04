/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "meta_fwd.hpp"
#include "meta_value.hpp"

#include "meta_data_info.hpp"

namespace meta_hpp::variable_detail
{
    template < typename Variable >
    struct variable_traits;

    template < typename T >
    struct variable_traits<T*> {
        static constexpr bool is_const = false;
        using value_type = T;
    };

    template < typename T >
    struct variable_traits<const T*>
         : variable_traits<T*> {
        static constexpr bool is_const = true;
    };

    template < auto Variable  >
    value getter() {
        using vt = variable_traits<std::remove_reference_t<decltype(Variable)>>;
        using value_type = typename vt::value_type;

        value_type typed_value = *Variable;

        return value{std::move(typed_value)};
    }

    template < auto Variable  >
    void setter(value value) {
        using vt = variable_traits<std::remove_reference_t<decltype(Variable)>>;
        using value_type = typename vt::value_type;

        if constexpr ( !vt::is_const ) {
            *Variable = value.cast<value_type>();
        } else {
            throw std::logic_error("an attempt to change a constant variable");
        }
    }
}

namespace meta_hpp
{
    class variable_info {
    public:
        variable_info() = delete;

        variable_info(variable_info&&) = default;
        variable_info(const variable_info&) = default;

        variable_info& operator=(variable_info&&) = default;
        variable_info& operator=(const variable_info&) = default;
    public:
        const family_id& fid() const noexcept {
            return fid_;
        }

        const std::string& id() const noexcept {
            return id_;
        }

        value get() const {
            return getter_();
        }

        template < typename Value >
        void set(Value&& value) const {
            return setter_(std::forward<Value>(value));
        }

        template < typename F >
        void each_data(F&& f) const {
            for ( auto&& id_info : datas_ ) {
                std::invoke(f, id_info.second);
            }
        }

        template < typename F >
        void visit(F&& f) const {
            each_data(f);
        }

        std::optional<data_info> get_data(std::string_view id) const {
            return detail::find_opt(datas_, id);
        }

        void merge(const variable_info& other) {
            if ( fid() != other.fid() ) {
                throw std::logic_error("variable_info::merge failed");
            }
            detail::merge_with(datas_, other.datas_, &data_info::merge);
        }
    private:
        template < auto Variable >
        friend class variable_;

        template < typename VariableType, VariableType Variable >
        variable_info(detail::auto_arg_t<Variable>, std::string id)
        : fid_{get_family_id<VariableType>()}
        , id_{std::move(id)}
        , getter_{&variable_detail::getter<Variable>}
        , setter_{&variable_detail::setter<Variable>} {}
    private:
        family_id fid_;
        std::string id_;
        value(*getter_)();
        void(*setter_)(value);
        std::map<std::string, data_info, std::less<>> datas_;
    };
}
