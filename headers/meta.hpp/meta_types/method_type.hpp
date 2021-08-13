/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_types_fwd.hpp"

namespace meta_hpp
{
    enum class method_flags : unsigned {
        is_const = 1 << 0,
        is_noexcept = 1 << 1,
        is_volatile = 1 << 2,
        is_lvalue_qualified = 1 << 3,
        is_rvalue_qualified = 1 << 4,
    };

    ENUM_HPP_OPERATORS_DECL(method_flags)

    class method_type final : public type_base {
    public:
        method_type() = default;

        method_type(method_type&&) = default;
        method_type& operator=(method_type&&) = default;

        method_type(const method_type&) = default;
        method_type& operator=(const method_type&) = default;

        template < typename T >
        explicit method_type(typename_arg_t<T>);

        std::size_t arity() const noexcept;
        any_type class_type() const noexcept;
        any_type return_type() const noexcept;
        any_type argument_type(std::size_t i) const noexcept;
        const std::vector<any_type>& argument_types() const noexcept;

        bitflags<method_flags> flags() const noexcept;
        bool is_const() const noexcept;
        bool is_noexcept() const noexcept;
        bool is_volatile() const noexcept;
        bool is_lvalue_qualified() const noexcept;
        bool is_rvalue_qualified() const noexcept;
    private:
        struct state;
        std::shared_ptr<state> state_;
    };
}

namespace meta_hpp::detail
{
    template < typename T >
    struct method_pointer_traits;

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...)> {
        static constexpr std::size_t arity{sizeof...(Args)};

        using class_type = C;
        using return_type = R;
        using qualified_type = C;
        using argument_types = std::tuple<Args...>;

        static any_type make_class_type() {
            return type_db::get<class_type>();
        }

        static any_type make_return_type() {
            return type_db::get<return_type>();
        }

        static std::vector<any_type> make_argument_types() {
            return type_db::multi_get<argument_types>();
        }

        static bitflags<method_flags> make_flags() noexcept {
            return bitflags<method_flags>{};
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_noexcept;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const | method_flags::is_noexcept;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) &> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) & noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_noexcept | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const &> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const & noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const | method_flags::is_noexcept | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) &&> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) && noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_noexcept | method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const &&> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const | method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) const && noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_const | method_flags::is_noexcept | method_flags::is_rvalue_qualified;
        }
    };

    //

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_noexcept;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const | method_flags::is_noexcept;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile &> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile & noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_noexcept | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const &> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const & noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const | method_flags::is_noexcept | method_flags::is_lvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile &&> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile && noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_noexcept | method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const &&> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const | method_flags::is_rvalue_qualified;
        }
    };

    template < typename R, typename C, typename... Args >
    struct method_pointer_traits<R(C::*)(Args...) volatile const && noexcept> : method_pointer_traits<R(C::*)(Args...)> {
        using qualified_type = const C&&;
        static bitflags<method_flags> make_flags() noexcept {
            return method_flags::is_volatile | method_flags::is_const | method_flags::is_noexcept | method_flags::is_rvalue_qualified;
        }
    };
}

namespace meta_hpp
{
    struct method_type::state final {
        const std::size_t arity;
        const any_type class_type;
        const any_type return_type;
        const std::vector<any_type> argument_types;
        const bitflags<method_flags> flags;
    };

    template < typename T >
    method_type::method_type(typename_arg_t<T>)
    : type_base{typename_arg<struct member_type_tag, T>}
    , state_{std::make_shared<state>(state{
        detail::method_pointer_traits<T>::arity,
        detail::method_pointer_traits<T>::make_class_type(),
        detail::method_pointer_traits<T>::make_return_type(),
        detail::method_pointer_traits<T>::make_argument_types(),
        detail::method_pointer_traits<T>::make_flags(),
    })} {
        static_assert(std::is_member_function_pointer_v<T>);
    }

    inline std::size_t method_type::arity() const noexcept {
        return state_->arity;
    }

    inline any_type method_type::class_type() const noexcept {
        return state_->class_type;
    }

    inline any_type method_type::return_type() const noexcept {
        return state_->return_type;
    }

    inline any_type method_type::argument_type(std::size_t i) const noexcept {
        return i < state_->argument_types.size()
            ? state_->argument_types[i]
            : any_type{};
    }

    inline const std::vector<any_type>& method_type::argument_types() const noexcept {
        return state_->argument_types;
    }

    inline bitflags<method_flags> method_type::flags() const noexcept {
        return state_->flags;
    }

    inline bool method_type::is_const() const noexcept {
        return state_->flags.has(method_flags::is_const);
    }

    inline bool method_type::is_noexcept() const noexcept {
        return state_->flags.has(method_flags::is_noexcept);
    }

    inline bool method_type::is_volatile() const noexcept {
        return state_->flags.has(method_flags::is_volatile);
    }

    inline bool method_type::is_lvalue_qualified() const noexcept {
        return state_->flags.has(method_flags::is_lvalue_qualified);
    }

    inline bool method_type::is_rvalue_qualified() const noexcept {
        return state_->flags.has(method_flags::is_rvalue_qualified);
    }
}
