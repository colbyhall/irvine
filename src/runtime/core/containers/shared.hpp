// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/memory.hpp>
#include <core/non_copyable.hpp>
#include <core/async/atomic.hpp>
#include <core/containers/option.hpp>
#include <core/math/math.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

enum class SMode {
    NonAtomic,
    Atomic
};

template <SMode Mode>
class SharedCounter;

template <>
class SharedCounter<SMode::NonAtomic> {
public:
    FORCE_INLINE SharedCounter(u32 strong, u32 weak) : m_strong(strong), m_weak(weak) {}

    FORCE_INLINE u32 strong() const { return m_strong; }
    FORCE_INLINE u32 weak() const { return m_weak; }

    FORCE_INLINE u32 add_strong() const { m_strong += 1; return m_strong; }
    FORCE_INLINE u32 remove_strong() const { m_strong -= 1; return m_strong; }

    FORCE_INLINE u32 add_weak() const { m_weak += 1; return m_weak; }
    FORCE_INLINE u32 remove_weak() const { m_weak -= 1; return m_weak; }

private:
    mutable u32 m_strong;
    mutable u32 m_weak;
};

template <>
class SharedCounter<SMode::Atomic> {
public:
    FORCE_INLINE SharedCounter(u32 strong, u32 weak) : m_strong(strong), m_weak(weak) {}

    FORCE_INLINE u32 strong() const { return m_strong.load(); }
    FORCE_INLINE u32 weak() const { return m_weak.load(); }

    FORCE_INLINE u32 add_strong() const { return m_strong.fetch_add(1); }
    FORCE_INLINE u32 remove_strong() const { return m_strong.fetch_sub(1); }

    FORCE_INLINE u32 add_weak() const { return m_weak.fetch_add(1); }
    FORCE_INLINE u32 remove_weak() const { return m_weak.fetch_sub(1); }

private:
    Atomic<u32> m_strong;
    Atomic<u32> m_weak;
};

template <typename Base, SMode Mode>
class Weak;

template <typename Base, SMode Mode = SMode::NonAtomic>
class Shared : private NonCopyable {
public:
    using Counter = SharedCounter<Mode>;

    template <typename Derived = Base>
    Shared(Shared<Derived, Mode>&& move) noexcept : m_ptr(move.m_ptr) {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");
        move.m_ptr = nullptr;
    }
    template <typename Derived = Base>
    Shared& operator=(Shared<Derived, Mode>&& m) noexcept {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");

        Shared<Base, Mode> to_destroy = core::move(*this);
        m_ptr = m.m_ptr;
        m.m_ptr = nullptr;
        return *this;
    }

    ~Shared();

    Weak<Base, Mode> downgrade() const;
    Shared<Base, Mode> clone() const;

    // Accessors
    FORCE_INLINE explicit operator NonNull<Base>() { return &value(); }
    FORCE_INLINE explicit operator NonNull<Base const>() const { return &value(); }
    FORCE_INLINE explicit operator Base* () { return &value(); }
    FORCE_INLINE explicit operator Base const* () const { return &value(); }
    FORCE_INLINE Base* operator ->() { return &value(); }
    FORCE_INLINE Base const* operator ->() const { return &value(); }
    FORCE_INLINE Base& operator *() { return value(); }
    FORCE_INLINE Base const& operator *() const { return value(); }

    FORCE_INLINE u32 strong() const { return counter().strong(); }
    FORCE_INLINE u32 weak() const { return counter().weak(); }

private:
    Shared() = default;
    explicit Shared(void* ptr) : m_ptr(ptr) {}

    template <typename T, SMode Mode, typename... Args>
    friend Shared<T, Mode> make_shared(Args&&... args);

    template <typename Derived, SMode Mode>
    friend class Shared;

    template <typename Derived, SMode Mode>
    friend class Weak;

    FORCE_INLINE Counter const& counter() const { return *((Counter*)m_ptr); }
    FORCE_INLINE Base& value() const {
        void* ptr = &((Counter*)m_ptr)[1];
        return *((Base*)ptr);
    }

    void* m_ptr;
};

template <typename T, SMode Mode, typename... Args>
FORCE_INLINE Shared<T, Mode> make_shared(Args&&... args);

template <typename Base, SMode Mode>
class Weak : private NonCopyable {
public:
    using Counter = SharedCounter<Mode>;

    template <typename Derived = Base>
    Weak(Weak<Derived, Mode>&& move) noexcept : m_ptr(move.m_ptr) {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");
        move.m_ptr = nullptr;
    }
    template <typename Derived = Base>
    Weak& operator=(Weak<Derived, Mode>&& m) noexcept {
        static_assert(core::is_base_of<Base, Derived>, "Base is not a base of Derived");

        Weak<Base, Mode> to_destroy = core::move(*this);
        m_ptr = m.m_ptr;
        m.m_ptr = nullptr;
        return *this;
    }

    ~Weak();

    Option<Shared<Base, Mode>> upgrade() const;
    Weak<Base, Mode> clone() const;

    FORCE_INLINE u32 strong() const { return counter().strong(); }
    FORCE_INLINE u32 weak() const { return counter().weak(); }

private:
    Weak() = default;
    explicit Weak(void* ptr) : m_ptr(ptr) {}

    template <typename Derived, SMode Mode>
    friend class Shared;

    template <typename Derived, SMode Mode>
    friend class Weak;

    FORCE_INLINE Counter const& counter() const { return *((Counter*)m_ptr); }

    void* m_ptr;
};

template <typename T, SMode Mode>
class SharedFromThis {
public:
    using Counter = SharedCounter<Mode>;

private:
    Option<Weak<T, Mode>> m_this;
};

CORE_NAMESPACE_END

#include <core/containers/shared.inl>

// Export Shared, Weak, SharedFromThis, SMode, and make_shared out of core namespace
using core::SMode;
using core::Shared;
using core::make_shared;
using core::Weak;
using core::SharedFromThis;