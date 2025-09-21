#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <functional>
#include <iostream>
#include <stdexcept>
#include "ArrayList.hpp"

template <class T>
class ArrayList {
public:
	static constexpr size_t START_CAPACITY = 20;
	static constexpr float CAPACITY_INCREASE_COEFFICIENT = 1.5;

	ArrayList():
		capacity_(START_CAPACITY),
		size_(0)
	{
		array_ = new T[capacity_];
	}

	explicit ArrayList(const size_t& capacity):
		capacity_(capacity),
		size_(0)
	{
		array_ = new T[capacity_];
	}

	ArrayList(const ArrayList& other):
		capacity_(other.capacity_),
		size_(other.size_)
	{
		array_ = new T[capacity_];
		for (int i = 0; i < capacity_; ++i) {
			array_[i] = other.array_[i];
		}
	}

	ArrayList(ArrayList&& other):
		capacity_(other.capacity_),
		size_(other.size_),
		array_(other.array_)
	{
		other.array_ = nullptr;
	}

	~ArrayList() {
		delete[] array_;
	}

	void add(const T& element) {
		if (size_ == capacity_) {
			expandArray();
		}
		array_[size_] = element;
		++size_;
	}

	void add(const T& element, const size_t& index) {
		if (index >= size_) {
			throw std::out_of_range("Error: ArrayList index is out of range.");
		}
		if (size_ == capacity_) {
			expandArray();
		}
		for (size_t i = size_; i > index; --i) {
			array_[i] = array_[i-1];
		}
		array_[index] = element;
		++size_;
	}

	void replace(const T& element, const size_t& index) {
		if (index >= size_) {
			throw std::out_of_range("Error: ArrayList index is out of range.");
		}
		array_[index] = element;
	}

	void remove(const size_t& index) {
		if (index >= size_) {
			throw std::out_of_range("Error: ArrayList index is out of range.");
		}
		for (size_t i = index+1; i < size_; ++i) {
			array_[i-1] = array_[i];
		}
		--size_;
	}

	void clear() {
		delete[] array_;
		size_ = 0;
		capacity_ = START_CAPACITY;
		array_ = new T[capacity_];
	}

	void sort(const std::function<int(const T&, const T&)>& comparator) {
		quickSort(comparator, 0, size_ -1);
	}

	T& get(const size_t& index) const {
		if (index >= size_) {
			throw std::out_of_range("Error: ArrayList index is out of range.");
		}
		return array_[index];
	}

	size_t size() const {
		return size_;
	}

	size_t capacity() const {
		return capacity_;
	}

	ArrayList& operator=(const ArrayList& other) {
		if (this != &other) {
			ArrayList copy(other);
			swap(copy);
		}
		return *this;
	}

	ArrayList& operator=(ArrayList&& other) noexcept {
		if (this != &other) {
			delete[] array_;
			capacity_ = other.capacity_;
			size_ = other.size_;
			array_ = other.array_;
			other.array_ = nullptr;
		}
		return *this;
	}

	template <class T1>
	friend std::ostream& operator<<(std::ostream& os, const ArrayList<T1>& list);

private:
	size_t capacity_;
	size_t size_;
	T* array_;

	void swap(ArrayList& other) noexcept {
		std::swap(capacity_, other.capacity_);
		std::swap(size_, other.size_);
		std::swap(array_, other.array_);
	}

	void expandArray() {
		capacity_ = static_cast<size_t>(CAPACITY_INCREASE_COEFFICIENT*capacity_);
		T* arrayNew = new T[capacity_];
		for (int i = 0; i < size_; ++i) {
			arrayNew[i] = array_[i];
		}
		delete[] array_;
		array_ = arrayNew;

	}

	int partition(const std::function<int(const T&, const T&)>& comparator, const int& low, const int& high) {
		T pivot = array_[high];
		int i = low - 1;
		for (int j = low; j < high; ++j) {
			int compareResult = comparator(array_[j], pivot);
			if ((compareResult == -1) || (compareResult == 0)) {
				++i;
				std::swap(array_[i], array_[j]);
			}
		}

		std::swap(array_[i+1], array_[high]);
		return i + 1;
	}

	void quickSort(const std::function<int(const T&, const T&)>& comparator, int low, int high) {
		if (low < high) {
			int partitionIndex = partition(comparator, low, high);
			quickSort(comparator, low, partitionIndex - 1);
			quickSort(comparator, partitionIndex + 1, high);
		}
	}
};

template <class T1>
std::ostream& operator<<(std::ostream& os, const ArrayList<T1>& list) {
	std::ostream::sentry sentry(os);
	if (!sentry) {
		return os;
	}

	for (int i = 0; i < list.size(); ++i) {
		os << list.get(i) << "\n";
	}
	return os;
}

#endif
