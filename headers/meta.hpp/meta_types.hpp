/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/meta.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "meta_base.hpp"
#include "meta_traits.hpp"

namespace meta_hpp
{
    class type_id final {
    public:
        template < typename T >
        explicit type_id(type_list<T>) noexcept
        : id_{type_to_id<T>()} {}

        type_id(type_id&&) = default;
        type_id& operator=(type_id&&) = default;

        type_id(const type_id&) = default;
        type_id& operator=(const type_id&) = default;

        std::size_t get_hash() const noexcept {
            return std::hash<underlying_type>{}(id_);
        }

        friend bool operator<(type_id l, type_id r) noexcept {
            return l.id_ < r.id_;
        }

        friend bool operator==(type_id l, type_id r) noexcept {
            return l.id_ == r.id_;
        }

        friend bool operator!=(type_id l, type_id r) noexcept {
            return l.id_ != r.id_;
        }
    private:
        using underlying_type = std::uint32_t;
        underlying_type id_{};
    private:
        static underlying_type next() noexcept {
            static std::atomic<underlying_type> id{};
            return ++id;
        }

        template < typename T >
        static underlying_type type_to_id() noexcept {
            static const underlying_type id{next()};
            return id;
        }
    };
}

namespace meta_hpp
{
    template < typename T >
    concept types_family =
        std::same_as<T, any_type> ||
        std::same_as<T, array_type> ||
        std::same_as<T, class_type> ||
        std::same_as<T, ctor_type> ||
        std::same_as<T, enum_type> ||
        std::same_as<T, function_type> ||
        std::same_as<T, member_type> ||
        std::same_as<T, method_type> ||
        std::same_as<T, number_type> ||
        std::same_as<T, pointer_type> ||
        std::same_as<T, reference_type> ||
        std::same_as<T, void_type>;

    template < types_family T >
    bool operator<(type_id l, const T& r) noexcept {
        return static_cast<bool>(r) && l < r.get_id();
    }

    template < types_family T >
    bool operator<(const T& l, type_id r) noexcept {
        return !static_cast<bool>(l) || l.get_id() < r;
    }

    template < types_family T, types_family U >
    bool operator<(const T& l, const U& r) noexcept {
        if ( !static_cast<bool>(r) ) {
            return false;
        }

        if ( !static_cast<bool>(l) ) {
            return true;
        }

        return l.get_id() < r.get_id();
    }

    template < types_family T >
    bool operator==(type_id l, const T& r) noexcept {
        return static_cast<bool>(r) && l == r.get_id();
    }

    template < types_family T >
    bool operator==(const T& l, type_id r) noexcept {
        return static_cast<bool>(l) && l.get_id() == r;
    }

    template < types_family T, types_family U >
    bool operator==(const T& l, const U& r) noexcept {
        if ( static_cast<bool>(l) != static_cast<bool>(r) ) {
            return false;
        }

        if ( !static_cast<bool>(l) ) {
            return true;
        }

        return l.get_id() == r.get_id();
    }

    template < types_family T >
    bool operator!=(type_id l, const T& r) noexcept {
        return !(l == r);
    }

    template < types_family T >
    bool operator!=(const T& l, type_id r) noexcept {
        return !(l == r);
    }

    template < types_family T, types_family U >
    bool operator!=(const T& l, const U& r) noexcept {
        return !(l == r);
    }
}

namespace meta_hpp
{
    class any_type final {
    public:
        explicit any_type() = default;
        explicit any_type(detail::type_data_base_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        type_kind get_kind() const noexcept;

        any_type(const array_type& other) noexcept;
        any_type(const class_type& other) noexcept;
        any_type(const ctor_type& other) noexcept;
        any_type(const enum_type& other) noexcept;
        any_type(const function_type& other) noexcept;
        any_type(const member_type& other) noexcept;
        any_type(const method_type& other) noexcept;
        any_type(const number_type& other) noexcept;
        any_type(const pointer_type& other) noexcept;
        any_type(const reference_type& other) noexcept;
        any_type(const void_type& other) noexcept;

        bool is_array() const noexcept;
        bool is_class() const noexcept;
        bool is_ctor() const noexcept;
        bool is_enum() const noexcept;
        bool is_function() const noexcept;
        bool is_member() const noexcept;
        bool is_method() const noexcept;
        bool is_number() const noexcept;
        bool is_pointer() const noexcept;
        bool is_reference() const noexcept;
        bool is_void() const noexcept;

        array_type as_array() const noexcept;
        class_type as_class() const noexcept;
        ctor_type as_ctor() const noexcept;
        enum_type as_enum() const noexcept;
        function_type as_function() const noexcept;
        member_type as_member() const noexcept;
        method_type as_method() const noexcept;
        number_type as_number() const noexcept;
        pointer_type as_pointer() const noexcept;
        reference_type as_reference() const noexcept;
        void_type as_void() const noexcept;
    private:
        detail::type_data_base_ptr data_;
    };

    class array_type final {
    public:
        array_type() = default;
        array_type(detail::array_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<array_flags> get_flags() const noexcept;

        std::size_t get_extent() const noexcept;
        any_type get_data_type() const noexcept;
    private:
        friend class any_type;
        detail::array_type_data_ptr data_;
    };

    class class_type final {
    public:
        class_type() = default;
        class_type(detail::class_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<class_flags> get_flags() const noexcept;

        std::size_t get_size() const noexcept;

        std::size_t get_arity() const noexcept;
        any_type get_argument_type(std::size_t index) const noexcept;
        const std::vector<any_type>& get_argument_types() const noexcept;

        const ctor_map& get_ctors() const noexcept;
        const class_set& get_bases() const noexcept;
        const function_map& get_functions() const noexcept;
        const member_map& get_members() const noexcept;
        const method_map& get_methods() const noexcept;
        const variable_map& get_variables() const noexcept;

        template < class_kind Derived >
        bool is_base_of() const noexcept;
        bool is_base_of(const class_type& derived) const noexcept;

        template < class_kind Base >
        bool is_derived_from() const noexcept;
        bool is_derived_from(const class_type& base) const noexcept;

        function get_function(std::string_view name) const noexcept;
        member get_member(std::string_view name) const noexcept;
        method get_method(std::string_view name) const noexcept;
        variable get_variable(std::string_view name) const noexcept;

        template < typename... Args >
        ctor get_ctor_with() const noexcept;
        ctor get_ctor_with(std::vector<any_type> args) const noexcept;
        ctor get_ctor_with(std::initializer_list<any_type> args) const noexcept;

        template < typename... Args >
        function get_function_with(std::string_view name) const noexcept;
        function get_function_with(std::string_view name, std::vector<any_type> args) const noexcept;
        function get_function_with(std::string_view name, std::initializer_list<any_type> args) const noexcept;

        template < typename... Args >
        method get_method_with(std::string_view name) const noexcept;
        method get_method_with(std::string_view name, std::vector<any_type> args) const noexcept;
        method get_method_with(std::string_view name, std::initializer_list<any_type> args) const noexcept;
    private:
        friend class any_type;
        detail::class_type_data_ptr data_;
    };

    class ctor_type final {
    public:
        ctor_type() = default;
        ctor_type(detail::ctor_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<ctor_flags> get_flags() const noexcept;

        std::size_t get_arity() const noexcept;
        any_type get_class_type() const noexcept;
        any_type get_argument_type(std::size_t index) const noexcept;
        const std::vector<any_type>& get_argument_types() const noexcept;
    private:
        friend class any_type;
        detail::ctor_type_data_ptr data_;
    };

    class enum_type final {
    public:
        enum_type() = default;
        enum_type(detail::enum_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<enum_flags> get_flags() const noexcept;

        number_type get_underlying_type() const noexcept;

        const evalue_map& get_evalues() const noexcept;

        evalue get_evalue(std::string_view name) const noexcept;

        template < typename Value >
        std::optional<std::string> value_to_name(Value&& value) const noexcept;
        std::optional<value> name_to_value(std::string_view name) const noexcept;
    private:
        friend class any_type;
        detail::enum_type_data_ptr data_;
    };

    class function_type final {
    public:
        function_type() = default;
        function_type(detail::function_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<function_flags> get_flags() const noexcept;

        std::size_t get_arity() const noexcept;
        any_type get_return_type() const noexcept;
        any_type get_argument_type(std::size_t index) const noexcept;
        const std::vector<any_type>& get_argument_types() const noexcept;
    private:
        friend class any_type;
        detail::function_type_data_ptr data_;
    };

    class member_type final {
    public:
        member_type() = default;
        member_type(detail::member_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<member_flags> get_flags() const noexcept;

        class_type get_owner_type() const noexcept;
        any_type get_value_type() const noexcept;
    private:
        friend class any_type;
        detail::member_type_data_ptr data_;
    };

    class method_type final {
    public:
        method_type() = default;
        method_type(detail::method_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<method_flags> get_flags() const noexcept;

        std::size_t get_arity() const noexcept;
        class_type get_owner_type() const noexcept;
        any_type get_return_type() const noexcept;
        any_type get_argument_type(std::size_t index) const noexcept;
        const std::vector<any_type>& get_argument_types() const noexcept;
    private:
        friend class any_type;
        detail::method_type_data_ptr data_;
    };

    class number_type final {
    public:
        number_type() = default;
        number_type(detail::number_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<number_flags> get_flags() const noexcept;

        std::size_t get_size() const noexcept;
    private:
        friend class any_type;
        detail::number_type_data_ptr data_;
    };

    class pointer_type final {
    public:
        pointer_type() = default;
        pointer_type(detail::pointer_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<pointer_flags> get_flags() const noexcept;

        any_type get_data_type() const noexcept;
    private:
        friend class any_type;
        detail::pointer_type_data_ptr data_;
    };

    class reference_type final {
    public:
        reference_type() = default;
        reference_type(detail::reference_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<reference_flags> get_flags() const noexcept;

        any_type get_data_type() const noexcept;
    private:
        friend class any_type;
        detail::reference_type_data_ptr data_;
    };

    class void_type final {
    public:
        void_type() = default;
        void_type(detail::void_type_data_ptr data);

        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        type_id get_id() const noexcept;
        bitflags<void_flags> get_flags() const noexcept;
    private:
        friend class any_type;
        detail::void_type_data_ptr data_;
    };
}

namespace meta_hpp::detail
{
    struct type_data_base {
        const type_id id;
        const type_kind kind;

        explicit type_data_base(type_id id, type_kind kind)
        : id{id}
        , kind{kind} {}
    };

    struct array_type_data final : type_data_base {
        const bitflags<array_flags> flags;
        const std::size_t extent;
        const any_type data_type;

        template < array_kind Array >
        static array_type_data_ptr get();

        template < array_kind Array >
        explicit array_type_data(type_list<Array>);
    };

    struct class_type_data final : type_data_base {
        const bitflags<class_flags> flags;
        const std::size_t size;
        const std::vector<any_type> argument_types;

        ctor_map ctors;
        class_set bases;
        function_map functions;
        member_map members;
        method_map methods;
        variable_map variables;

        template < class_kind Class >
        static class_type_data_ptr get();

        template < class_kind Class >
        explicit class_type_data(type_list<Class>);
    };

    struct ctor_type_data final : type_data_base {
        const bitflags<ctor_flags> flags;
        const any_type class_type;
        const std::vector<any_type> argument_types;

        template < class_kind Class, typename... Args >
        static ctor_type_data_ptr get();

        template < class_kind Class, typename... Args >
        explicit ctor_type_data(type_list<Class>, type_list<Args...>);
    };

    struct enum_type_data final : type_data_base {
        const bitflags<enum_flags> flags;
        const number_type underlying_type;

        evalue_map evalues;

        template < enum_kind Enum >
        static enum_type_data_ptr get();

        template < enum_kind Enum >
        explicit enum_type_data(type_list<Enum>);
    };

    struct function_type_data final : type_data_base {
        const bitflags<function_flags> flags;
        const any_type return_type;
        const std::vector<any_type> argument_types;

        template < function_kind Function >
        static function_type_data_ptr get();

        template < function_kind Function >
        explicit function_type_data(type_list<Function>);
    };

    struct member_type_data final : type_data_base {
        const bitflags<member_flags> flags;
        const class_type owner_type;
        const any_type value_type;

        template < member_kind Member >
        static member_type_data_ptr get();

        template < member_kind Member >
        explicit member_type_data(type_list<Member>);
    };

    struct method_type_data final : type_data_base {
        const bitflags<method_flags> flags;
        const class_type owner_type;
        const any_type return_type;
        const std::vector<any_type> argument_types;

        template < method_kind Method >
        static method_type_data_ptr get();

        template < method_kind Method >
        explicit method_type_data(type_list<Method>);
    };

    struct number_type_data final : type_data_base {
        const bitflags<number_flags> flags;
        const std::size_t size;

        template < number_kind Number >
        static number_type_data_ptr get();

        template < number_kind Number >
        explicit number_type_data(type_list<Number>);
    };

    struct pointer_type_data final : type_data_base {
        const bitflags<pointer_flags> flags;
        const any_type data_type;

        template < pointer_kind Pointer >
        static pointer_type_data_ptr get();

        template < pointer_kind Pointer >
        explicit pointer_type_data(type_list<Pointer>);
    };

    struct reference_type_data final : type_data_base {
        const bitflags<reference_flags> flags;
        const any_type data_type;

        template < reference_kind Reference >
        static reference_type_data_ptr get();

        template < reference_kind Reference >
        explicit reference_type_data(type_list<Reference>);
    };

    struct void_type_data final : type_data_base {
        const bitflags<void_flags> flags;

        template < void_kind Void >
        static void_type_data_ptr get();

        template < void_kind Void >
        explicit void_type_data(type_list<Void>);
    };
}

namespace meta_hpp
{
    struct ctor_index final {
        const ctor_type type;

        explicit ctor_index(ctor_type type)
        : type{std::move(type)} {}

        friend bool operator<(const ctor_index& l, const ctor_index& r) noexcept {
            return l.type < r.type;
        }

        friend bool operator==(const ctor_index& l, const ctor_index& r) noexcept {
            return l.type == r.type;
        }
    };

    struct evalue_index final {
        const enum_type type;
        const std::string name;

        explicit evalue_index(enum_type type, std::string name)
        : type{std::move(type)}
        , name{std::move(name)} {}

        friend bool operator<(const evalue_index& l, const evalue_index& r) noexcept {
            return std::tie(l.type, l.name) < std::tie(r.type, r.name);
        }

        friend bool operator==(const evalue_index& l, const evalue_index& r) noexcept {
            return std::tie(l.type, l.name) == std::tie(r.type, r.name);
        }
    };

    struct function_index final {
        const function_type type;
        const std::string name;

        explicit function_index(function_type type, std::string name)
        : type{std::move(type)}
        , name{std::move(name)} {}

        friend bool operator<(const function_index& l, const function_index& r) noexcept {
            return std::tie(l.type, l.name) < std::tie(r.type, r.name);
        }

        friend bool operator==(const function_index& l, const function_index& r) noexcept {
            return std::tie(l.type, l.name) == std::tie(r.type, r.name);
        }
    };

    struct member_index final {
        const member_type type;
        const std::string name;

        explicit member_index(member_type type, std::string name)
        : type{std::move(type)}
        , name{std::move(name)} {}

        friend bool operator<(const member_index& l, const member_index& r) noexcept {
            return std::tie(l.type, l.name) < std::tie(r.type, r.name);
        }

        friend bool operator==(const member_index& l, const member_index& r) noexcept {
            return std::tie(l.type, l.name) == std::tie(r.type, r.name);
        }
    };

    struct method_index final {
        const method_type type;
        const std::string name;

        explicit method_index(method_type type, std::string name)
        : type{std::move(type)}
        , name{std::move(name)} {}

        friend bool operator<(const method_index& l, const method_index& r) noexcept {
            return std::tie(l.type, l.name) < std::tie(r.type, r.name);
        }

        friend bool operator==(const method_index& l, const method_index& r) noexcept {
            return std::tie(l.type, l.name) == std::tie(r.type, r.name);
        }
    };

    struct scope_index final {
        const std::string name;

        explicit scope_index(std::string name)
        : name{std::move(name)} {}

        friend bool operator<(const scope_index& l, const scope_index& r) noexcept {
            return l.name < r.name;
        }

        friend bool operator==(const scope_index& l, const scope_index& r) noexcept {
            return l.name == r.name;
        }
    };

    struct variable_index final {
        const pointer_type type;
        const std::string name;

        explicit variable_index(pointer_type type, std::string name)
        : type{std::move(type)}
        , name{std::move(name)} {}

        friend bool operator<(const variable_index& l, const variable_index& r) noexcept {
            return std::tie(l.type, l.name) < std::tie(r.type, r.name);
        }

        friend bool operator==(const variable_index& l, const variable_index& r) noexcept {
            return std::tie(l.type, l.name) == std::tie(r.type, r.name);
        }
    };
}

namespace meta_hpp
{
    namespace detail
    {
        template < typename T >
        kind_type_data_ptr<T> get_type_data() {
            static_assert(!std::is_const_v<T> && !std::is_volatile_v<T>);
            return kind_type_data<T>::template get<T>();
        }
    }

    template < typename T >
    auto resolve_type() {
        using raw_type = std::remove_cv_t<T>;
        return detail::kind_type<raw_type>{detail::get_type_data<raw_type>()};
    }

    template < typename T >
    auto resolve_type(T&&) {
        using raw_type = std::remove_cvref_t<T>;
        return detail::kind_type<raw_type>{detail::get_type_data<raw_type>()};
    };
}
