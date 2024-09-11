/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abareux <abareux@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 23:33:32 by abareux           #+#    #+#             */
/*   Updated: 2024/09/11 16:37:49 by abareux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <string>
#include <stdexcept>
#include <algorithm>

template <class TType>
class Pool {
private:
	TType* data;
	size_t size;
	size_t used;
	std::list<TType *> data_used;

public:
	[[maybe_unused]] Pool();
	[[maybe_unused]] ~Pool();

	[[maybe_unused]] Pool(const Pool<TType>& source);
	[[maybe_unused]] Pool(Pool<TType>&& source) noexcept;

	[[maybe_unused]] Pool& operator=(const Pool<TType>& source);
	[[maybe_unused]] Pool& operator=(Pool<TType>&& source) noexcept;

	[[maybe_unused]] explicit Pool(size_t numberOfObjectStored);

	[[maybe_unused]] void resize(const size_t& numberOfObjectStored);

	class Object {
	private:
		TType* data;
		Pool& origin;
	public:
		template<typename ... TArgs>
		[[maybe_unused]] explicit Object(Pool& origin, TType* allocated_data, TArgs&& ... data);
		[[maybe_unused]] ~Object();

		[[maybe_unused]] TType* operator->();
	};

	template<typename ... TArgs>
	[[maybe_unused]] Object acquire(TArgs&& ... p_args);
	[[maybe_unused]] void release(TType* data_to_release);
};

template <class TType>
[[maybe_unused]] Pool<TType>::Pool() : size(0), used(0) {
	this->data = nullptr;
}

template <class TType>
[[maybe_unused]] Pool<TType>::~Pool() {
	delete[] this->data;
}

template <class TType>
[[maybe_unused]] Pool<TType>::Pool(const Pool<TType> &source) : size(0), used(0) {
	this->data = new TType[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;
	this->used = source.used;
}

template <class TType>
[[maybe_unused]] Pool<TType>::Pool(Pool<TType>&& source) noexcept : size(0), used(0) {
	this->data = source.data;
	this->size = source.size;
	this->used = source.used;

	source.data = nullptr;
	source.size = 0;
	source.used = 0;
}

template <class TType>
[[maybe_unused]] Pool<TType>& Pool<TType>::operator=(const Pool<TType>& source) {
	if (source == this)
		return *this;

	this->data = new TType[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;
	this->used = source.used;

	return *this;
}

template <class TType>
[[maybe_unused]] Pool<TType>& Pool<TType>::operator=(Pool<TType>&& source) noexcept {
	this->data = source.data;
	this->size = source.size;
	this->used = source.used;

	source.data = nullptr;
	source.size = 0;
	source.used = 0;

	return *this;
}

template<class TType>
[[maybe_unused]] Pool<TType>::Pool(size_t numberOfObjectStored) : size(0), used(0) {
	this->data = new TType[numberOfObjectStored];
	this->size = numberOfObjectStored;
	this->used = 0;
}

template <class TType>
[[maybe_unused]] void Pool<TType>::resize(const size_t &numberOfObjectStored) {
	auto* buffer = new TType[numberOfObjectStored];

	for (int iteration = 0; iteration < this->size && iteration < numberOfObjectStored; ++iteration) {
		buffer[iteration] = std::move(this->data[iteration]);
	}

	for (int iteration = this->size; iteration < numberOfObjectStored; ++iteration) {
		buffer[iteration] = TType();
	}

	delete[] this->data;

	this->data = buffer;
}

template<class TType>
template<typename ... TArgs>
[[maybe_unused]] Pool<TType>::Object::Object(Pool& origin, TType *allocated_data, TArgs&& ... data) : origin(origin){
	this->data = allocated_data;
	this->data = new (this->data) TType(std::forward<TArgs>(data)...);
}

template<class TType>
[[maybe_unused]] Pool<TType>::Object::~Object() {
	this->origin.release(this->data);
	this->data->~TType();
}

template<class TType>
[[maybe_unused]] TType *Pool<TType>::Object::operator->() {
	return this->data;
}

template<class TType>
template<typename ... TArgs>
[[maybe_unused]] typename Pool<TType>::Object Pool<TType>::acquire(TArgs&& ... p_args) {
	if (this->used == this->size) {
		throw (std::runtime_error("No more space in Pool left"));
	}

	TType* buffer;

	for (size_t iteration = 0; iteration < this->size; ++iteration) {
		if (find(this->data_used.begin(), this->data_used.end(), this->data + iteration) == this->data_used.end()) {
			this->data_used.push_back(this->data + iteration);
			buffer = this->data + iteration;
			break;
		}
	}

	return Pool<TType>::Object(*this, buffer, std::forward<TArgs>(p_args)...);
}

template<class TType>
[[maybe_unused]] void Pool<TType>::release(TType* data_to_release) {
	this->data_used.remove(data_to_release);
}