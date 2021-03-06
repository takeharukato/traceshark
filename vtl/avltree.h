/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2015-2017  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 * This file is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 *
 *  a) This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public
 *     License along with this library; if not, write to the Free
 *     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *     MA 02110-1301 USA
 *
 * Alternatively,
 *
 *  b) Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *     2. Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __AVLTREE_H
#define __AVLTREE_H

#include <utility>
#include <cstdlib>

namespace vtl {

typedef enum { AVLBALANCE_USEPOINTERS,  AVLBALANCE_USESWAP } avlbalance_t;

#define __AVLTREEMAX(A, B) ((A) >= (B) ? A:B)

template <class T>
class AVLSampleCompare {
public:
	__always_inline static int compare(const T &a, const T &b) {
		if (a < b)
			return -1;
		if (a > b)
			return 1;
		return 0;
	}
};

template <class T>
class AVLDefaultCompare {
};

template <class T, class U>
class AVLNode {
public:
	AVLNode();
	__always_inline void swapNodes(AVLNode<T, U> *newChild);
	__always_inline void stealParent(AVLNode<T, U> *newChild,
					 AVLNode<T, U> **rootptr);
	__always_inline void setHeightFromChildren();
	class AVLNode<T, U> *small;
	class AVLNode<T, U> *large;
	int height;
	class AVLNode<T, U> *parent;
	T key;
	U value;
};

template <class T, class U>
class AVLDefaultAllocator {
public:
	AVLDefaultAllocator(void */*data*/) {}
	__always_inline AVLNode<T, U> *alloc(const T &key) {
		AVLNode<T, U> *node = new AVLNode<T, U>;
		node->key = key;
		return node;
	}
	__always_inline void dealloc(AVLNode<T,U> *node) {
		delete node;
	}
};

/*
 * This is a type trait class that determines whether a class has the
 * compare(U, U) function.
 */
template <typename T, typename U>
class has_compare {
private:
	typedef char Yes;
	typedef Yes No[2];
	template<typename C> static auto Test(void*)
		-> decltype(int{std::declval<C const>().compare(U(), U())},
			    Yes{});

	template<typename> static No& Test(...);
public:
	static bool const value = sizeof(Test<T>(0)) == sizeof(Yes);
};

/*
 * This is a type trait class that determines whether a class has the
 * clear() function.
 */
template <typename K>
class has_clear {
private:
	typedef char Yes;
	typedef Yes No[2];
	template<typename C> static auto Test(void*)
		-> decltype(int{std::declval<C>().clear()},
			    Yes{});

	template<typename> static No& Test(...);
public:
	static bool const value = sizeof(Test<K>(0)) == sizeof(Yes);
};

template<bool b> class __AVLFindInsertSelect;
template<bool b> class __AVLFindSelect;
template<bool b> class __AVLFindValueSelect;
template<bool b> class __AVLAddValueSelect;
template<bool b> class __AVLClearSelect;

template<class T, class U, avlbalance_t BALANCE = AVLBALANCE_USESWAP,
	typename ALLOC = AVLDefaultAllocator<T, U>,
	typename CF = AVLDefaultCompare<T>>
class AVLTree
{
	friend class __AVLFindInsertSelect<false>;
	friend class __AVLFindSelect<false>;
	friend class __AVLFindValueSelect<false>;
	friend class __AVLAddValueSelect<AVLBALANCE_USEPOINTERS>;
	friend class __AVLFindInsertSelect<true>;
	friend class __AVLFindSelect<true>;
	friend class __AVLFindValueSelect<true>;
	friend class __AVLAddValueSelect<AVLBALANCE_USESWAP>;
public:
	class iterator {
		friend class AVLTree<T, U, BALANCE, ALLOC, CF>;
	public:
		iterator();
		__always_inline T &key() const;
		__always_inline U &value() const;
		__always_inline bool atEnd() const;
		__always_inline void next();
		__always_inline void prev();
		__always_inline iterator &operator++();
		__always_inline iterator operator++(int);
		__always_inline iterator &operator--();
		__always_inline iterator operator--(int);
		__always_inline bool operator!=(const iterator &other) const;
		__always_inline bool operator==(const iterator &other) const;
	protected:
		AVLNode<T, U> *pos;
	};

	AVLTree();
	AVLTree(void *allocdata);
	~AVLTree();
	__always_inline void insert(const T &key, const U &value);
	__always_inline U &findValue(const T &key, bool &newEntry);
	__always_inline U value(const T &key, const U &defaultValue = U())
		const;
	__always_inline bool contains(const T &key) const;
	__always_inline bool isEmpty() const;
	__always_inline int size() const;
	__always_inline U &operator[](const T &key);
	__always_inline iterator find(const T &key) const;
	__always_inline iterator findInsert(const T &key, bool &newEntry);

	void clear();
	__always_inline iterator begin() const;
	__always_inline iterator end() const;
	protected:
	__always_inline U &__findValue(const T &key, bool &newEntry);
	__always_inline U &__findValueCmp(const T &key, bool &newEntry);
	__always_inline iterator __findCmp(const T &key) const;
	__always_inline iterator __find(const T &key) const;
	__always_inline iterator __findInsertCmp(const T &key, bool &newEntry);
	__always_inline iterator __findInsert(const T &key, bool &newEntry);
	__always_inline AVLNode<T, U> *__addValue(const T &key,
						  AVLNode<T, U> *&parent,
						  AVLNode<T, U> **&aentry);
	__always_inline AVLNode<T, U> *__addValueSwap(const T &key,
						      AVLNode<T, U> *&parent,
						      AVLNode<T, U> **&aentry);
private:
	void deleteNode(AVLNode<T, U> *node);
	__always_inline AVLNode<T, U> *addValue(const T &key,
						AVLNode<T, U> *&parent,
						AVLNode<T, U> **&aentry);
	AVLNode<T, U> *root;
	int _size;
	ALLOC alloc;
};

template <bool b>
	class __AVLFindInsertSelect {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
		findInsert(AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
		     const T &key, bool &newEntry) {
		return obj.__findInsert(key, newEntry);
	}
};

template <>
	class __AVLFindInsertSelect<true> {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
		findInsert(AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
		     const T &key, bool &newEntry) {
		return obj.__findInsertCmp(key, newEntry);
	}
};

template <bool b>
	class __AVLFindSelect {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
		find(const AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
		     const T &key) {
		return obj.__find(key);
	}
};

template <>
	class __AVLFindSelect<true> {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
		find(const AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
		     const T &key) {
		return obj.__findCmp(key);
	}
};

template<bool b>
	class __AVLFindValueSelect {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static U &findValue(AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
				    const T &key, bool &newEntry) {
			return obj.__findValue(key, newEntry);
		}
};

template<>
	class __AVLFindValueSelect<true> {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static U &findValue(AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
				    const T &key, bool &newEntry) {
			return obj.__findValueCmp(key, newEntry);
		}
};

template<>
	class __AVLAddValueSelect<AVLBALANCE_USEPOINTERS> {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static AVLNode<T, U> *addValue(
			AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
			const T &key,
			AVLNode<T, U> *&parent,
			AVLNode<T, U> **&aentry) {
		return obj.__addValue(key, parent, aentry);
	}
};

template<>
	class __AVLAddValueSelect<AVLBALANCE_USESWAP> {
public:
	template <typename T, typename U, avlbalance_t BALANCE, typename ALLOC,
		typename CF>
		__always_inline
		static AVLNode<T, U> *addValue(
			AVLTree<T, U, BALANCE, ALLOC, CF> &obj,
			const T &key,
			AVLNode<T, U> *&parent,
			AVLNode<T, U> **&aentry) {
			return obj.__addValueSwap(key, parent, aentry);
		}
};

template<bool b>
	class __AVLClearSelect {
public:
	template <typename T, typename U, typename ALLOC>
		static void clear(AVLNode<T, U> *node,
				  ALLOC &alloc) {
		if (node->small != nullptr)
			clear(node->small, alloc);
		if (node->large != nullptr)
			clear(node->large, alloc);
		alloc.dealloc(node);
       }
};

template<>
	class __AVLClearSelect<true> {
public:
	template <typename T, typename U, typename ALLOC>
		__always_inline static void clear(AVLNode<T, U> */*node*/,
						  ALLOC &alloc) {
		alloc.clear();
	}
};


template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::iterator():
pos(nullptr)
{}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline T &AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::key() const
{
	return pos->key;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline U &AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::value() const
{
	return pos->value;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline bool AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::atEnd() const
{
	return pos == nullptr;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	void AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::next()
{
	AVLNode<T, U> *prev;
	if (pos->large != nullptr) {
		pos = pos->large;
		while (pos->small != nullptr)
			pos = pos->small;
		return;
	}
	while (pos->parent != nullptr) {
		prev = pos;
		pos = pos->parent;
		if (pos->small == prev)
			return;
	}
	pos = nullptr;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	&AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::operator++()
{
	next();
	return *this;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::operator++(int)
{
	iterator prev = *this;
	next();
	return prev;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	void AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::prev()
{
	AVLNode<T, U> *prev;
	if (pos->small != nullptr) {
		pos = pos->small;
		return;
	}
	while (pos->parent != nullptr) {
		prev = pos;
		pos = pos->parent;
		if (pos->large == prev)
			return;
	}
	pos = nullptr;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	&AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::operator--()
{
	prev();
	return *this;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::operator--(int)
{
	iterator prev = *this;
	prev();
	return prev;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline bool AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::
	operator!=(const iterator &other) const
{
	return pos != other.pos;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline bool AVLTree<T, U, BALANCE, ALLOC, CF>::iterator::
	operator==(const iterator &other) const
{
	return pos == other.pos;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
AVLTree<T, U, BALANCE, ALLOC, CF>::AVLTree():
root(nullptr), _size(0), alloc(nullptr)
{}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
AVLTree<T, U, BALANCE, ALLOC, CF>::AVLTree(void *allocdata):
root{nullptr}, _size{0}, alloc{allocdata}
{}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
AVLTree<T, U, BALANCE, ALLOC, CF>::~AVLTree()
{
	clear();
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline void AVLTree<T, U, BALANCE, ALLOC, CF>::insert(const T &key,
							       const U &value)
{
	bool useless;
	U &ref = findValue(key, useless);
	ref = value;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::__findCmp(const T &key) const
{
	AVLNode<T, U> *entry = root;
	iterator iter;
	while (entry != nullptr) {
		int cmp = CF::compare(key, entry->key);
		if (cmp == 0)
			break;
		if (cmp < 0)
			entry = entry->small;
		else
			entry = entry->large;
	}
	iter.pos = entry;
	return iter;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::__find(const T &key) const
{
	AVLNode<T, U> *entry = root;
	iterator iter;
	while (entry != nullptr) {
		if (key == entry->key)
			break;
		if (key < entry->key)
			entry = entry->small;
		else
			entry = entry->large;
	}
	iter.pos = entry;
	return iter;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::__findInsertCmp(const T &key,
							   bool &newEntry)
{
	AVLNode<T, U> **aentry;
	AVLNode<T, U> *entry;
	AVLNode<T, U> *parent = nullptr;
	iterator iter;

	aentry = &root;
	entry = root;

	while (entry != nullptr) {
		int cmp = CF::compare(key, entry->key);
		if (cmp == 0) {
			iter.pos = entry;
			newEntry = false;
			return iter;
		}
		parent = entry;
		if (cmp < 0)
			aentry = &entry->small;
		else
			aentry = &entry->large;
		entry = *aentry;
	}
	iter.pos = addValue(key, parent, aentry);
	newEntry = true;
	return iter;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::__findInsert(const T &key,
							bool &newEntry)
{
	AVLNode<T, U> **aentry;
	AVLNode<T, U> *entry;
	AVLNode<T, U> *parent = nullptr;
	iterator iter;

	aentry = &root;
	entry = root;

	while (entry != nullptr) {
		if (key == entry->key) {
			iter.pos = entry;
			newEntry = false;
			return iter;
		}
		parent = entry;
		if (key < entry->key)
			aentry = &entry->small;
		else
			aentry = &entry->large;
		entry = *aentry;
	}
	iter.pos = addValue(key, parent, aentry);
	newEntry = true;
	return iter;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::find(const T &key) const
{
	return __AVLFindSelect<has_compare<CF, T>::value>::find(*this, key);
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::findInsert(const T &key,
						      bool &newEntry)
{
	return __AVLFindInsertSelect<has_compare<CF, T>::value>::
		findInsert(*this, key, newEntry);
}


template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	U &AVLTree<T, U, BALANCE, ALLOC, CF>::findValue(const T &key,
								bool &newEntry)
{
	return __AVLFindValueSelect<has_compare<CF, T>::value>::
		findValue(*this, key, newEntry);
}


template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	U &AVLTree<T, U, BALANCE, ALLOC, CF>::
	__findValue(const T &key, bool &newEntry)
{
	return __findInsert(key, newEntry).value();
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline U &AVLTree<T, U, BALANCE, ALLOC, CF>::__findValueCmp(
	const T &key, bool &newEntry)
{
	return __findInsertCmp(key, newEntry).value();
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline
AVLNode<T, U> *AVLTree<T, U, BALANCE, ALLOC, CF>::
	addValue(const T &key, AVLNode<T, U> *&parent, AVLNode<T, U> **&aentry)
{
	return __AVLAddValueSelect<BALANCE>::addValue(*this, key, parent,
						      aentry);
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline AVLNode<T, U> *AVLTree<T, U, BALANCE, ALLOC, CF>::
	__addValue(const T &key,
		   AVLNode<T, U> *&parent,
		   AVLNode<T, U> **&aentry)
{
	AVLNode<T, U> *entry;
	AVLNode<T, U> *newentry;
	AVLNode<T, U> *sibling;
	AVLNode<T, U> *grandParent;
	AVLNode<T, U> *smallChild;
	AVLNode<T, U> *largeChild;
	int diff;
	int smallH;
	int largeH;

	_size++;
	newentry = alloc.alloc(key);
	entry = newentry;
	*aentry = entry;

	entry->small = nullptr;
	entry->large = nullptr;
	entry->height = 0;
	entry->parent = parent;
	if (parent == nullptr)
		return newentry;

	if (parent->height > 0)
		return newentry;

	parent->height = 1;
	grandParent = parent->parent;

	while(grandParent != nullptr) {
		smallH = grandParent->small == nullptr ?
			-1 : grandParent->small->height;
		largeH = grandParent->large == nullptr ?
			-1 : grandParent->large->height;
		diff = smallH - largeH;
		if (diff == 0)
			break;
		if (diff > 1)
			goto rebalanceSmall;
		if (diff < -1)
			goto rebalanceLarge;
		grandParent->height = parent->height + 1;
		entry = parent;
		parent = grandParent;
		grandParent = grandParent->parent;
	}
	return newentry;
rebalanceSmall:
	/* Do small rebalance here (case 1 and case 2) */
	if (entry == parent->small) {
		/* Case 1 */
		sibling = parent->large;

		grandParent->stealParent(parent, &root);
		parent->large = grandParent;
		grandParent->parent = parent;
		grandParent->small = sibling;
		grandParent->height--;
		if (sibling != nullptr)
			sibling->parent = grandParent;
	} else {
		/* Case 2 */
		smallChild = entry->small;
		largeChild = entry->large;

		grandParent->stealParent(entry, &root);
		entry->small = parent;
		entry->large = grandParent;
		entry->height = grandParent->height;

		grandParent->parent = entry;
		grandParent->small = largeChild;
		grandParent->setHeightFromChildren(); // Fixme: faster

		parent->parent = entry;
		parent->large = smallChild;
		parent->setHeightFromChildren();

		if (largeChild != nullptr)
			largeChild->parent = grandParent;
		if (smallChild != nullptr)
			smallChild->parent = parent;
	}
	return newentry;
rebalanceLarge:
	/* Do large rebalance here */
	if (entry == parent->small) {
		/* Case 3 */
		smallChild = entry->small;
		largeChild = entry->large;

		grandParent->stealParent(entry, &root);
		entry->small = grandParent;
		entry->large = parent;
		entry->height = grandParent->height;

		grandParent->parent = entry;
		grandParent->large = smallChild;
		grandParent->setHeightFromChildren(); // Fixme: faster

		parent->parent = entry;
		parent->small = largeChild;
		parent->setHeightFromChildren();

		if (largeChild != nullptr)
			largeChild->parent = parent;
		if (smallChild != nullptr)
			smallChild->parent = grandParent;
	} else {
		/* Case 4 */
		sibling = parent->small;

		grandParent->stealParent(parent, &root);
		parent->small = grandParent;
		grandParent->parent = parent;
		grandParent->large = sibling;
		grandParent->height--;
		if (sibling != nullptr)
			sibling->parent = grandParent;
	}
	return newentry;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline AVLNode<T, U> *AVLTree<T, U, BALANCE, ALLOC, CF>::
	__addValueSwap(const T &key, AVLNode<T, U> *&parent,
		       AVLNode<T, U> **&aentry)
{
	AVLNode<T, U> *entry;
	AVLNode<T, U> *newentry;
	AVLNode<T, U> *grandParent;
	AVLNode<T, U> *smallChild;
	AVLNode<T, U> *largeChild;
	int diff;
	int smallH;
	int largeH;
	int gHeight;

	_size++;
	newentry = alloc.alloc(key);
	entry = newentry;
	*aentry = entry;

	entry->small = nullptr;
	entry->large = nullptr;
	entry->height = 0;
	entry->parent = parent;
	if (parent == nullptr)
		return newentry;

	if (parent->height > 0)
		return newentry;

	parent->height = 1;
	grandParent = parent->parent;

	while(grandParent != nullptr) {
		smallH = grandParent->small == nullptr ?
			-1 : grandParent->small->height;
		largeH = grandParent->large == nullptr ?
			-1 : grandParent->large->height;
		diff = smallH - largeH;
		if (diff == 0)
			break;
		if (diff > 1)
			goto rebalanceSmall;
		if (diff < -1)
			goto rebalanceLarge;
		grandParent->height = parent->height + 1;
		entry = parent;
		parent = grandParent;
		grandParent = grandParent->parent;
	}
	return newentry;
rebalanceSmall:
	/* Do small rebalance here (case 1 and case 2) */
	if (entry == parent->small) {
		/* Case 1 */
		grandParent->small = entry;
		entry->parent = grandParent;

		parent->small = parent->large;
		parent->height--;

		parent->large = grandParent->large;
		if (parent->large != nullptr)
			parent->large->parent = parent;

		grandParent->large = parent;
		parent->swapNodes(grandParent);
	} else {
		/* Case 2 */
		smallChild = entry->small;
		largeChild = entry->large;
		gHeight = 0;
		if (grandParent->large != nullptr) {
			gHeight = grandParent->large->height + 1;
			grandParent->large->parent = entry;
		}

		parent->large = smallChild;
		if (smallChild != nullptr)
			smallChild->parent = parent;
		parent->height = grandParent->height - 1;

		entry->large = grandParent->large;
		entry->small = largeChild;
		if (largeChild != nullptr)
			largeChild->parent = entry;

		grandParent->large = entry;
		entry->parent = grandParent;

		entry->height = gHeight;
		entry->swapNodes(grandParent);
		if (entry == newentry)
			return grandParent;
	}
	return newentry;
rebalanceLarge:
	/* Do large rebalance here */
	if (entry == parent->small) {
		/* Case 3 */
		smallChild = entry->small;
		largeChild = entry->large;
		gHeight = 0;
		if (grandParent->small != nullptr) {
			gHeight = grandParent->small->height + 1;
			grandParent->small->parent = entry;
		}

		parent->small = largeChild;
		if (largeChild != nullptr)
			largeChild->parent = parent;
		parent->height = grandParent->height - 1;

		entry->small = grandParent->small;
		entry->large = smallChild;
		if (smallChild != nullptr)
			smallChild->parent = entry;

		grandParent->small = entry;
		entry->parent = grandParent;

		entry->height = gHeight;
		entry->swapNodes(grandParent);
		if (entry == newentry)
			return grandParent;
	} else {
		/* Case 4 */
		grandParent->large = entry;
		entry->parent = grandParent;

		parent->large = parent->small;
		parent->height--;

		parent->small = grandParent->small;
		if (parent->small != nullptr)
			parent->small->parent = parent;

		grandParent->small = parent;
		parent->swapNodes(grandParent);
	}
	return newentry;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	U AVLTree<T, U, BALANCE, ALLOC, CF>::value(const T &key,
						    const U &defaultValue)
	const
{
	iterator iter = find(key);
	if (iter.pos == nullptr)
		return defaultValue;
	return iter.value();
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	bool AVLTree<T, U, BALANCE, ALLOC, CF>::contains(const T &key)
	const
{
	iterator iter = find(key);
	return iter.pos != nullptr;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	bool AVLTree<T, U, BALANCE, ALLOC, CF>::isEmpty() const
{
	return _size == 0;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	int AVLTree<T, U, BALANCE, ALLOC, CF>::size() const
{
	return _size;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline	U &AVLTree<T, U, BALANCE, ALLOC, CF>::operator[](const T &key)
{
	bool useless;
	return findValue(key, useless);
}

template <class T, class U>
	AVLNode<T, U>::AVLNode():
small(nullptr), large(nullptr)
{}

template <class T, class U>
__always_inline void AVLNode<T, U>::stealParent(
	AVLNode<T, U> *newChild,
	AVLNode<T, U> **rootptr)
{
	newChild->parent = parent;
	if (parent == nullptr) {
		/* Oops this is a root node */
		*rootptr = newChild;
		return;
	}
	if (parent->small == this)
		parent->small = newChild;
	else
		parent->large = newChild;
}

template <class T, class U>
__always_inline void AVLNode<T, U>::setHeightFromChildren()
{
	int lh;
	int rh;

	lh = (small != nullptr) ? (small->height) : -1;
	rh = (large != nullptr) ? (large->height) : -1;
	height = __AVLTREEMAX(lh, rh) + 1;
}

template <class T, class U>
__always_inline void AVLNode<T, U>::swapNodes(AVLNode<T, U> *node)
{
	T tmpk = node->key;
	U tmpv = node->value;
	node->key = key;
	node->value = value;
	key = tmpk;
	value = tmpv;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::begin()
	const
{
	iterator _begin;
	_begin.pos = root;
	if (_begin.pos == nullptr)
		return _begin;
	while (_begin.pos->small != nullptr)
		_begin.pos = _begin.pos->small;
	return _begin;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
__always_inline typename AVLTree<T, U, BALANCE, ALLOC, CF>::iterator
	AVLTree<T, U, BALANCE, ALLOC, CF>::end()
	const
{
	iterator _end;
	return _end;
}

template <class T, class U, avlbalance_t BALANCE, typename ALLOC, typename CF>
void AVLTree<T, U, BALANCE, ALLOC, CF>::clear()
{
	if (root != nullptr) {
		__AVLClearSelect<has_clear<ALLOC>::value>::clear(root, alloc);
		root = nullptr;
		_size = 0;
	}
}

}

#endif /* __AVLTREE_H */
