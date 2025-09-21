#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include "ArrayList.hpp"
#include "LinkedList.hpp"

template <class Key, class Value, class Hash = std::hash<Key>>
class HashMap {
public:
	struct Pair {
		const Key key;
		Value value;

		Pair(const Key& key, const Value& value):
			key(key), value(value)
		{

		}

		bool operator==(const Pair& other) {
			return key == other.key;
		}
	};

	explicit HashMap(const size_t& capacity):
		amountElements_(0),
		table_(capacity)
	{
		for (int i = 0; i < capacity; i++) {
			table_.add(LinkedList<Pair>());
		}
	}

	HashMap(const HashMap& other):
		amountElements_(other.amountElements_),
		hashFunction_(other.hashFunction_),
		table_(other.table_)
	{ }

	HashMap(HashMap&& other) noexcept:
		amountElements_(other.amountElements_),
		hashFunction_(other.hashFunction_),
		table_(std::move(other.table_))
	{
		other.amountElements_ = 0;
	}

	void insert(const Key& key, const Value& value) {
		LinkedList<Pair>& bucket = table_.get(getTableIndex(key));

		bool isKeyFound = false;
		auto iterationCallback = [&key, &value, &isKeyFound](Pair& pair) {
			if (key == pair.key) {
				pair.value = value;
				isKeyFound = true;
				return false;
			}
			return true;
		};
		bucket.forEach(iterationCallback);
		if (isKeyFound) {
			return;
		}
		bucket.pushFront(Pair(key, value));
		++amountElements_;
	}

	Value get(const Key& key) const {
		LinkedList<Pair>& bucket = table_.get(getTableIndex(key));

		Value valueToFind;
		auto iterationCallback = [&key, &valueToFind](Pair& pair) {
			if (pair.key == key) {
				valueToFind = pair.value;
				return false;
			}
			return true;
		};
		bucket.forEach(iterationCallback);

		return valueToFind;
	}

	bool contains(const Key& key) const {
		LinkedList<Pair>& bucket = table_.get(getTableIndex(key));

		bool isElementFound = false;
		auto iterationCallback = [&isElementFound, &key](Pair& pair) {
			if (pair.key == key) {
				isElementFound = true;
				return false;
			}
			return true;
		};
		bucket.forEach(iterationCallback);

		return isElementFound;
	}

	bool remove(const Key& key) {
		LinkedList<Pair>& bucket = table_.get(getTableIndex(key));
		bool isRemoved = bucket.remove(Pair(key, Value()));
		if (isRemoved) {
			--amountElements_;
		}
		return isRemoved;
	}

	size_t amountElements() const {
		return amountElements_;
	}

	bool isEmpty() const {
		return amountElements_ == 0;
	}

	void forEach(const std::function<bool(Pair& pair)>& iterationCallback) {
		bool needIteration = false;
		for (int i = 0; i < table_.size(); ++i) {
			if (table_.get(i).isEmpty()) {
				continue;
			}
			auto listIterationCallback = [&needIteration, &iterationCallback](Pair& pair) {
				needIteration = iterationCallback(pair);
				if (!needIteration) {
					return false;
				}
				return true;
			};
			table_.get(i).forEach(listIterationCallback);
			if (!needIteration) {
				break;
			}
		}
	}

	void forEach(const std::function<bool(const Pair& pair)>& iterationCallback) const {
		bool needIteration = false;
		for (int i = 0; i < table_.size(); ++i) {
			if (table_.get(i).isEmpty()) {
				continue;
			}
			auto listIterationCallback = [&needIteration, &iterationCallback](const Pair& pair) {
				needIteration = iterationCallback(pair);
				if (!needIteration) {
					return false;
				}
				return true;
			};
			table_.get(i).forEach(listIterationCallback);
			if (!needIteration) {
				break;
			}
		}
	}

	HashMap& operator=(const HashMap& other) {
		if (this != &other) {
			HashMap copy(other);
			swap(copy);
		}
		return *this;
	}

	HashMap& operator=(HashMap&& other) noexcept {
		if (this != &other) {
			table_ = std::move(other.table_);
			amountElements_ = other.amountElements_;
			hashFunction_ = other.hashFunction_;
			other.amountElements_ = 0;
		}
		return *this;
	}

	template<typename Key1, typename Value1>
	friend std::ostream& operator<<(std::ostream& os, const HashMap<Key1, Value1>& hashMap);
private:
	size_t amountElements_;
	Hash hashFunction_;
	ArrayList<LinkedList<Pair>> table_;

	void swap(HashMap& other) noexcept {
		std::swap(amountElements_, other.amountElements_);
		std::swap(hashFunction_, other.hashFunction_);
		std::swap(table_, other.table_);
	}

	size_t getTableIndex(const Key& key) const {
		return hashFunction_(key) % table_.size();
	}
};

template<typename Key1, typename Value1>
std::ostream& operator<<(std::ostream& os, const HashMap<Key1, Value1>& hashMap) {
	std::ostream::sentry sentry(os);
	if (!sentry) {
		return os;
	}
	auto iterationCallback = [&os](const typename HashMap<Key1, Value1>::Pair& pair) {
		os << pair.key << " " << pair.value << "\n";
		return true;
	};
	hashMap.forEach(iterationCallback);
	return os;
}


#endif
