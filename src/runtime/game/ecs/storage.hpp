// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/bucket_array.hpp>

#include <game/ecs/component.hpp>

GAME_NAMESPACE_BEGIN

constexpr usize storage_bucket_size = 1024;

class Storage {
public:
	virtual Unique<Storage> make_empty() const = 0; // Used to make storage when we dont know type at compile time
	virtual Option<const Component&> get(usize index) const = 0;
	virtual Option<Component&> get_mut(usize index) = 0;
	virtual void transfer(usize src_index, usize dst_index, Storage& dst) = 0;
	virtual void remove(usize index) = 0;
	virtual ~Storage() {}
};

template <typename T>
class BucketStorage : public Storage {
public:
	explicit BucketStorage() = default;

	void insert(usize index, T&& component) {
		if (index == m_components.len()) m_components.push(nullptr);

		ASSERT(!m_components[index].is_set(), "Slot must be empty to insert component into");
		m_components[index] = Option<T>(core::forward<T>(component));
	}

	// Storage
	Unique<Storage> make_empty() const {
		return make_unique<BucketStorage<T>>();
	}

	Option<const Component&> get(usize index) const override {
		auto& slot = m_components[index];
		if (slot) {
			auto& component = slot.as_ref().unwrap();
			return component;
		} else {
			return nullptr;
		}
	}

	Option<Component&> get_mut(usize index) override {
		auto& slot = m_components[index];
		if (slot) {
			auto& component = slot.as_mut().unwrap();
			return component;
		}
		else {
			return nullptr;
		}
	}

	void transfer(usize src_index, usize dst_index, Storage& dst) override {
		T component = m_components[src_index].unwrap();
		BucketStorage<T>& dst_bucket_storage = static_cast<BucketStorage<T>&>(dst);
		dst_bucket_storage.insert(dst_index, core::move(component));
	}

	void remove(usize index) override {
		m_components[index].unwrap();
	}
	// ~Storage

private:
	BucketArray<game::storage_bucket_size, Option<T>> m_components;
};

GAME_NAMESPACE_END