#ifndef NODE_H
#define NODE_H
#include<memory>
#include<iostream>

template <class KeyType, class ValueType>
struct node {	
	const KeyType key;
	ValueType value;
	std::shared_ptr<node<KeyType, ValueType>> left;
	std::shared_ptr<node<KeyType, ValueType>> right;
	std::weak_ptr<node<KeyType, ValueType>> parent;
	unsigned int height;
	bool is_value;

	node(const KeyType& input_key, const ValueType& input_value, std::shared_ptr<node<KeyType, ValueType>> p) :
		key{ input_key }, value{ input_value }, parent(p) {
		left = nullptr;
		right = nullptr;
		height = 1;
		is_value = true;
	}
	node(const KeyType& input_key, std::shared_ptr<node<KeyType, ValueType>> p) :
		key{ input_key }, parent(p) {
		left = nullptr;
		right = nullptr;
		height = 1;
		is_value = false;
	}
	~node() { 
		//std::cout << "node key: " << key << " deleted" << std::endl;
	}
};
#endif