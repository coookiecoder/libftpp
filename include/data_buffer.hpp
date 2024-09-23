#pragma once

#include <list>
#include <sstream>

template<class TType>
class DataBuffer {
private:
	std::list<u_int8_t> data;

public:
	DataBuffer();
	~DataBuffer();

	void operator<<(TType& item);
	TType& operator>>(TType& item);
};

template <class TType>
DataBuffer<TType>::DataBuffer() {

}

template <class TType>
DataBuffer<TType>::~DataBuffer() {

}

template <class TType>
void DataBuffer<TType>::operator<<(TType& item) {
	std::stringstream stream_buffer(item);
	std::string string_buffer = stream_buffer.str();

	for (const auto &string_item: string_buffer) {
		this->data.push_back(string_item);
	}
}

template <class TType>
TType& DataBuffer<TType>::operator>>(TType& item) {
	for (const auto &data_item: this->data) {
		data_item >> item;
		this->data.pop_back();
	}

	return item;
}