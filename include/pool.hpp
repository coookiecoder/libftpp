/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abareux <abareux@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 23:33:32 by abareux           #+#    #+#             */
/*   Updated: 2024/09/11 12:59:31 by abareux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

template <class T>
class Pool {
private:
	T* data;
	size_t size;

public:
	Pool();

	Pool(const Pool<T>& source);
	Pool(Pool<T>&& source) noexcept;

	Pool& operator=(const Pool<T>& source);
	Pool& operator=(Pool<T>&& source) noexcept;

	Pool(size_t numberOfObjectStored);
};

template <class T>
Pool<T>::Pool() {
	this->data = NULL;
	this->size = 0;
}

template <class T>
Pool<T>::Pool(const Pool<T> &source) {
	this->data = new T[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;
}

template <class T>
Pool<T>::Pool(Pool<T>&& source) noexcept {
	this->data = source.data;
	this->size = source.size;

	source.data = nullptr;
	source.size = 0;
}

template <class T>
Pool<T>& Pool<T>::operator=(const Pool<T>& source) {
	this->data = new T[source.size];

	for (int iteration = 0; iteration < source.size; ++iteration) {
		this->data[iteration] = source.data[iteration];
	}

	this->size = source.size;

	return *this;
}

template <class T>
Pool<T>& Pool<T>::operator=(Pool<T>&& source) noexcept {
	this->data = source.data;
	this->size = source.size;

	source.data = nullptr;
	source.size = 0;

	return *this;
}