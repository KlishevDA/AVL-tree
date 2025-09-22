#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "node.h"

#include<memory>
#include<iostream>
#include"Avl_tree_iterator.h"
#include <initializer_list>



template <class KeyType, class ValueType>
class Avl_tree
{
public:
	typedef std::shared_ptr<node<KeyType, ValueType>> node_ptr_t;

	typedef Avl_tree_iterator<KeyType, ValueType, node_ptr_t> iterator;

	typedef Avl_tree_iterator<const KeyType, const ValueType, const node_ptr_t> const_iterator;

	friend class Avl_tree_iterator<KeyType, ValueType, node_ptr_t>;

	using node_ptr_t = std::shared_ptr<node<KeyType, ValueType>>;

private:
	//variables
	node_ptr_t root; //tree root
	std::size_t size_ = 0; // fast size counter

	// functions
	// const
	// recursive print of tree
	void print(node_ptr_t p, int level) const;
	// getter height of node
	unsigned int get_height(node_ptr_t p) const { return p != nullptr ? p->height : 0; }
	// node's balance factor
	int bfactor(node_ptr_t p) const { return get_height(p->right) - get_height(p->left); }
	// search for the minimum key of a given node
	node_ptr_t find_min(node_ptr_t p) const { return p->left ? find_min(p->left) : p; }
	// search for the maximum key of a given node
	node_ptr_t find_max(node_ptr_t p) const { return p->right ? find_max(p->right) : p; }
	// recursive search of key
	node_ptr_t find(node_ptr_t p, const KeyType& input_key) const;

	//not const
	// recursive insert key with value
	node_ptr_t insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t leaf);
	// recursive insert key without value
	node_ptr_t insert(const KeyType& input_key, node_ptr_t leaf);
	// update height of node
	void fixheight(node_ptr_t p);
	// right rotate around p
	node_ptr_t rotateright(node_ptr_t p);
	//left rotate around q
	node_ptr_t rotateleft(node_ptr_t q);
	// balance p node
	node_ptr_t balance(node_ptr_t p);
	//removing the minimum node from p
	node_ptr_t remove_min(node_ptr_t p);
	//delete key
	node_ptr_t erase(node_ptr_t p, const KeyType& input_key);
	// set parents
	void set_parents(node_ptr_t p);

public:
	// variables

	// functions

	// const
	// iterator to begin
	iterator begin();
	// iterator to end
	iterator end();
	//empty check
	bool empty() const;
	// count of nodes
	unsigned int size() const;

	// search of key
	node_ptr_t find(const KeyType& input_key) const;

	//not const
	//default constuctor
	Avl_tree() { root = nullptr; };
	//constructor with initialization list
	Avl_tree(const std::initializer_list<std::pair<KeyType, ValueType>>& list) :
		Avl_tree() {
		for (auto& element : list)
			insert(element.first, element.second);
	}
	// insert key with value
	void insert(const KeyType& input_key, const ValueType& input_value);
	// insert key without value
	void insert(const KeyType& input_key);
	//clearing tree
	void clear() { root = nullptr; }
	// delete key
	void erase(const KeyType& input_key);
	//operator[] overload
	ValueType& operator[](const KeyType& input_key);
	// print tree
	void print();

};

// operator[] overload
template <typename KeyType, typename ValueType>
ValueType& Avl_tree<KeyType, ValueType>::operator[](const KeyType& input_key) {
	auto n = find(input_key);
	if (!n) {
		insert(input_key);
		n = find(input_key);
	}
	return n->value;
}

// iterator to begin
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::iterator Avl_tree<KeyType, ValueType>::begin() {
	return typename Avl_tree<KeyType, ValueType>::iterator(find_min(root), this);
}

// iterator to end
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::iterator Avl_tree<KeyType, ValueType>::end() {
	if (root) return typename Avl_tree<KeyType, ValueType>::iterator((find_max(root))->right, this);
	else return begin();
}

//empty check
template <typename KeyType, typename ValueType>
bool Avl_tree<KeyType, ValueType>::empty() const {
	return ((root != nullptr) ? false : true);
}

// size of tree
template <typename KeyType, typename ValueType>
unsigned int Avl_tree<KeyType, ValueType>::size() const {
	return static_cast<unsigned int>(size_);
}

//print tree
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print() {
	std::cout << std::endl;
	set_parents(root);
	if (!empty()) print(root, 0);
	else std::cout << "Tree is empty";
	std::cout << std::endl;
}

// recursive print tree
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print(node_ptr_t p, int level) const {
	if (p) {
		print(p->right, level + 1);
		for (int i = 0; i < level; i++) std::cout << "  ";
		if ((p->parent).lock()) std::cout << p->key << ";" << ((p->parent).lock())->key << std::endl;
		else std::cout << p->key << ";" << "n" << std::endl;
		print(p->left, level + 1);
	}
}

//set parents
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::set_parents(node_ptr_t p) {
	if (p == root) {
		node_ptr_t tmp = nullptr;
		root->parent = tmp;
	}
	if (p->left) {
		(p->left)->parent = p;
		set_parents(p->left);
	}
	if (p->right) {
		(p->right)->parent = p;
		set_parents(p->right);
	}
}

// update height
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::fixheight(node_ptr_t p) {
	unsigned int hl = get_height(p->left);
	unsigned int hr = get_height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

// right rotate
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::rotateright(node_ptr_t p) {
	node_ptr_t q(p->left);
	p->left = q->right;
	q->right = p;
	p->parent = q;
	fixheight(p);
	fixheight(q);
	return q;
}

// left rotate
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::rotateleft(node_ptr_t q) {
	node_ptr_t p(q->right);
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

// balance node
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::balance(node_ptr_t p) {
	fixheight(p);
	switch (bfactor(p)) {
	case -2:
		if (bfactor(p->left) > 0) {
			p->left = rotateleft(p->left);
		}
		return rotateright(p);
	case 2:
		if (bfactor(p->right) < 0) {
			p->right = rotateright(p->right);
		}
		return rotateleft(p);
	default:
		return p;
	}
}

// search of key
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::find(const KeyType& input_key) const {
	return find(root, input_key);
}

// recursive search of key
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::find(node_ptr_t p, const KeyType& input_key) const {
	if (!p) return nullptr;
	if (input_key < p->key) return find(p->left, input_key);
	else if (input_key > p->key) return find(p->right, input_key);
	return p;
}


// insert key without value
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key) {
	if (!find(input_key)) { // count only new keys
		root = (root == nullptr)
			? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, nullptr))
			: insert(input_key, root);
	}
}

// insert key with value
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value) {
	auto n = find(input_key);
	if (!n) {
		root = (root == nullptr)
			? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, input_value, nullptr))
			: insert(input_key, input_value, root);
	}
	else {
		n->value = input_value;
	}
}

//recursive insert key without value
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, node_ptr_t leaf) {
	if (input_key < leaf->key)
		leaf->left = (leaf->left == nullptr) ? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, leaf)) : insert(input_key, leaf->left);
	else if (input_key > leaf->key)
		leaf->right = (leaf->right == nullptr) ? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, leaf)) : insert(input_key, leaf->right);
	return  balance(leaf);
}

//recursive insert key with value
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t leaf) {
	if (input_key < leaf->key)
		leaf->left = (leaf->left == nullptr) ? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, input_value, leaf)) : insert(input_key, input_value, leaf->left);
	else if (input_key > leaf->key)
		leaf->right = (leaf->right == nullptr) ? (size_++, std::make_shared<node<KeyType, ValueType>>(input_key, input_value, leaf)) : insert(input_key, input_value, leaf->right);
	else if (input_key == leaf->key)
		leaf->value = input_value;
	return  balance(leaf);
}

// delete key with value
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::erase(const KeyType& input_key) {
	if (find(input_key)) {
		root = erase(root, input_key);
		--size_;
	}
}

//delete removing the minimum node from p
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::remove_min(node_ptr_t p) {
	if (p->left == nullptr) return p->right;
	p->left = remove_min(p->left);
	return balance(p);
}

// recursive delete key
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::erase(node_ptr_t p, const KeyType& input_key) {
	if (!p) return 0;
	if (input_key < p->key)
		p->left = erase(p->left, input_key);
	else if (input_key > p->key)
		p->right = erase(p->right, input_key);
	else {
		node_ptr_t q(p->left);
		node_ptr_t r(p->right);
		if (!r) return q;
		node_ptr_t min(find_min(r));
		min->right = remove_min(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

#endif // AVL_TREE_H