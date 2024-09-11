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

#include <string>
#include <list>
#include <stdexcept>

template <class TType>
class Pool {
private:
	TType* data;
	size_t size;
	size_t used;

public:
	Pool();
	~Pool();

	Pool(const Pool<TType>& source);
	Pool(Pool<TType>&& source) noexcept;

	Pool& operator=(const Pool<TType>& source);
	Pool& operator=(Pool<TType>&& source) noexcept;

	Pool(size_t numberOfObjectStored);

	void resize(const size_t& numberOfObjectStored);

	class Object {
	private:
		TType* data;
	public:
		template<typename ... TArgs>
		explicit Object(TType* allocated_data, TArgs&& ... data);
		~Object();

		TType* operator->();
	};

	template<typename ... TArgs>
	Object acquire(TArgs&& ... p_args);
};

template <class TType>
Pool<TType>::Pool() {
	this->data = nullptr;
	this->size = 0;
	this->used = 0;
}

template <class TType>
Pool<TType>::~Pool() {
	delete[] this->data;
}

template <class TType>
Pool<TType>::Pool(const Pool<TType> &source) {
	this->data = new TType[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;
	this->used = source.used;
}

template <class TType>
Pool<TType>::Pool(Pool<TType>&& source) noexcept {
	this->data = source.data;
	this->size = source.size;
	this->used = source.used;

	source.data = nullptr;
	source.size = 0;
	source.used = 0;
}

template <class TType>
Pool<TType>& Pool<TType>::operator=(const Pool<TType>& source) {
	this->data = new TType[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;
	this->used = source.used;

	return *this;
}

template <class TType>
Pool<TType>& Pool<TType>::operator=(Pool<TType>&& source) noexcept {
	this->data = source.data;
	this->size = source.size;
	this->used = source.used;

	source.data = nullptr;
	source.size = 0;
	source.used = 0;

	return *this;
}

template<class TType>
Pool<TType>::Pool(size_t numberOfObjectStored) {
	this->data = new TType[numberOfObjectStored];
	this->size = numberOfObjectStored;
	this->used = 0;
}

template <class TType>
void Pool<TType>::resize(const size_t &numberOfObjectStored) {
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
Pool<TType>::Object::Object(TType *allocated_data, TArgs&& ... data) {
	this->data = allocated_data;
	this->data = new (this->data) TType(std::forward<TArgs>(data)...);
}

template<class TType>
Pool<TType>::Object::~Object() {
	this->data->~TType();
}

template<class TType>
TType *Pool<TType>::Object::operator->() {
	return this->data;
}

template<class TType>
template<typename ... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&& ... p_args) {
	if (this->used == this->size) {
		throw (std::runtime_error("No more space in Pool left"));
	}

	return Pool<TType>::Object(this->data + this->used++, std::forward<TArgs>(p_args)...);
}