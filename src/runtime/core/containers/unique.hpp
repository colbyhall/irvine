// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/memory.hpp>
#include <core/non_copyable.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

template <typename Base>
class Unique : private NonCopyable {
public:
    template <typename Derived = Base>
    Unique(Unique<Derived>&& move) noexcept : m_ptr(move.m_ptr) {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");

        move.m_ptr = nullptr;
    }
    template <typename Derived = Base>
    Unique& operator=(Unique<Derived>&& m) noexcept {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");

        Unique<Base> to_destroy = core::move(*this);
        m_ptr = m.m_ptr;
        m.m_ptr = nullptr;
        return *this;
    }

    ~Unique() {
        if (m_ptr) {
            m_ptr->~Base();
            mem::free(m_ptr);
            m_ptr = nullptr;
        }
    }

    FORCE_INLINE explicit operator NonNull<Base>() { return m_ptr; }
    FORCE_INLINE explicit operator NonNull<Base const>() const { return m_ptr; }
    FORCE_INLINE explicit operator Base*() { return m_ptr; }
    FORCE_INLINE explicit operator Base const *() const { return m_ptr; }
    FORCE_INLINE Base* operator ->() { return m_ptr; }
    FORCE_INLINE Base const* operator ->() const { return m_ptr; }
    FORCE_INLINE Base& operator *() { return *m_ptr; }
    FORCE_INLINE Base const& operator *() const { return *m_ptr; }

private:
    Unique() = default;

    template <typename T, typename... Args>
    friend Unique<T> make_unique(Args&&... args);

    template <typename Derived>
    friend class Unique;

    FORCE_INLINE explicit Unique(Base&& base) {
        void* ptr = mem::alloc(mem::Layout::single<Base>);
        m_ptr = new (ptr) Base(core::forward<Base>(base));
    }

    Base* m_ptr;
};

template <typename T, typename... Args>
FORCE_INLINE Unique<T> make_unique(Args&&... args) {
    return Unique<T>(core::move(T(forward<Args>(args)...)));
}

CORE_NAMESPACE_END

// Export Unique out of core namespace
using core::Unique;
using core::make_unique;
