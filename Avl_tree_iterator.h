#ifndef AVL_TREE_ITERATOR_H
#define AVL_TREE_ITERATOR_H

#include <initializer_list>
#include <vector>

#include"Avl_tree.h"
#include"node.h"

template<typename KeyType, typename ValueType>
class Avl_tree;


template<typename KeyType, typename ValueType, typename PtrType>
class Avl_tree_iterator : public std::iterator<std::input_iterator_tag, PtrType>
{
	friend class Avl_tree<KeyType, ValueType>;
private:
	Avl_tree_iterator(PtrType p, Avl_tree<KeyType, ValueType>* t) : p(p), tr(t) {
		set_parents((*tr).root);
		set_row((*tr).root);
		auto tmp = find_max((*tr).root)->right;
		if (tmp == p) index = row.size();
		row.push_back(tmp);

	}

public:
	Avl_tree_iterator(const Avl_tree_iterator& it) : p(it.p) {
		set_parents((*tr).root);
		set_row((*tr).root);
		auto tmp = find_max((*tr).root)->right;
		if (tmp == p) index = row.size();
		row.push_back(tmp);
	}
	bool operator!=(Avl_tree_iterator const& other) const { return p != other.p; }
	bool operator==(Avl_tree_iterator const& other) const { return p == other.p; }
	typename Avl_tree_iterator::reference operator*() const { return *p; }
	typename Avl_tree_iterator::reference operator->() { return p; }
	Avl_tree_iterator& operator++() {
		p = (row[index + 1]).lock();
		++index;
		return *this;
	}
	Avl_tree_iterator& operator--() {
		p = (row[index - 1]).lock();
		--index;
		return *this;
	}

private:
	PtrType p;
	std::vector<std::weak_ptr<node<KeyType, ValueType>>> row;
	unsigned int index;
	Avl_tree<KeyType, ValueType>* tr;
	void set_row(PtrType t) {
		if (t) {
			set_row(t->left);
			if (t == p) index = row.size();
			row.push_back(t);
			set_row(t->right);
		}
	}
	void set_parents(PtrType p) {
		if (p == (*tr).root) {
			PtrType tmp = nullptr;
			((*tr).root)->parent = tmp;
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
	PtrType find_max(PtrType p) const { return p->right ? find_max(p->right) : p; }
	PtrType end() { return (find_max((*tr).root))->right; }
};

#endif