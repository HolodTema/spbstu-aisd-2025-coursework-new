#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <functional>
#include <iostream>

template <class T>
class LinkedList {
public:
	LinkedList() = default;

	LinkedList(const LinkedList& other) {
		if (other.head_ != nullptr) {
			head_ = new Node(*other.head_);
			Node* currentNode = head_;
			Node* currentNodeOther = other.head_->nextNode;
			try {
				while (currentNodeOther != nullptr) {
					currentNode->nextNode = new Node(*currentNodeOther);
					currentNode = currentNode->nextNode;
					currentNodeOther = currentNodeOther->nextNode;
				}
			}
			catch (const std::bad_alloc&) {
				Node* currentNodeToDelete = head_;
				while (currentNodeToDelete != nullptr) {
					Node* nodeToDelete = currentNodeToDelete;
					currentNodeToDelete = currentNodeToDelete->nextNode;
					delete nodeToDelete;
				}
			}
		}
	}

	LinkedList(LinkedList&& other) noexcept:
		head_(other.head_)
	{
		other.head_ = nullptr;
	}

	~LinkedList() {
		Node* currentNode = head_;
		while (currentNode != nullptr) {
			Node* nodeToDelete = currentNode;
			currentNode = currentNode->nextNode;
			delete nodeToDelete;
		}
	}

	void pushFront(const T& element) {
		Node* newHead = new Node(element, head_);
		head_ = newHead;
	}

	bool remove(const T& element) {
		Node* currentNode = head_;
		Node* previousNode = nullptr;
		while (currentNode != nullptr) {
			if (currentNode->data == element) {
				if (previousNode == nullptr) {
					head_ = currentNode->nextNode;
					delete currentNode;
					return true;
				}
				previousNode->nextNode = currentNode->nextNode;
				delete currentNode;
				return true;
			}
			previousNode = currentNode;
			currentNode = currentNode->nextNode;
		}
		return false;
	}

	bool contains(const T& element) const {
		Node* currentNode = head_;
		while (currentNode != nullptr) {
			if (currentNode->data == element) {
				return true;
			}
			currentNode = currentNode->nextNode;
		}
		return false;
	}

	void forEach(const std::function<bool(T&)>& iterationCallback) {
		Node* currentNode = head_;
		while (currentNode != nullptr) {
			bool needIteration = iterationCallback(currentNode->data);
			if (!needIteration) {
				return;
			}
			currentNode = currentNode->nextNode;
		}
	}

	bool isEmpty() const {
		return head_ == nullptr;
	}

	LinkedList& operator=(const LinkedList& other) {
		if (this != &other) {
			LinkedList copy(other);
			swap(copy);
		}
		return *this;
	}

	LinkedList& operator=(LinkedList&& other) noexcept {
		if (this != &other) {
			Node* node = head_;
			while (node != nullptr) {
				Node* nodeToDelete = node;
				node = node->nextNode;
				delete nodeToDelete;
			}
			head_ = other.head_;
			other.head_ = nullptr;
		}
		return *this;
	}

	template <class T1>
	friend std::ostream& operator<<(std::ostream& os, const LinkedList<T1>& list);

private:
	struct Node {
		T data;
		Node* nextNode;

		Node(const T& data, Node* nextNode):
			data(data),
			nextNode(nextNode)
		{

		}
	};

	Node* head_ = nullptr;

	void swap(LinkedList& other) noexcept {
		std::swap(head_, other.head_);
	}
};

template <class T1>
std::ostream& operator<<(std::ostream& os, const LinkedList<T1>& list) {
	std::ostream::sentry sentry(os);
	if (!sentry) {
		return os;
	}
	typename LinkedList<T1>::Node* node = list.head_;
	while (node != nullptr) {
		os << node->data << "\n";
		node = node->nextNode;
	}
	return os;
}

#endif
