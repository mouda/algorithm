// **************************************************************************
// File       [ hash.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/11/09 created ]
// **************************************************************************

#ifndef _COMMON_HASH_H_
#define _COMMON_HASH_H_

#include <cstring>
#include <cstdio>

namespace CommonNs {

// some good prime numbers for hash table. Ranging from 2^6 ~ 2^31
// reference http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
static const unsigned PRIMES[] = {
           97,       193,       389,       769,       1543,
         3079,      6151,     12289,     24593,      49157,
        98317,    196613,    393241,    786433,    1572869,
      3145739,   6291469,  12582917,  25165843,   50331653,
    100663319, 201326611, 402653189, 805306457, 1610612741
};

template <class T>
class StringHashNode {
public:
                   StringHashNode(const char * const key, const T &value);
                   ~StringHashNode();
    char           *key_;
    T              value_;
    StringHashNode *next_;
};

template <class T>
class StringHash {
public:
                      StringHash(const unsigned &nbank = PRIMES[0]);
                      ~StringHash();
    bool              lookup(const char * const key, T &value) const;
    bool              add(const char * const key, const T &value);
    bool              remove(const char * const key, const T &value);
    int               count(const char * const key) const;
    unsigned          size() const;
    unsigned          nbank() const;
    StringHashNode<T> **head();

private:
    unsigned          hash(const char * const key) const;
    StringHashNode<T> **table_;
    unsigned          nbank_;
    unsigned          size_;
};


template <class T>
inline StringHashNode<T>::StringHashNode(const char * const key,
                                         const T &value) {
    value_ = value;
    key_ = strcpy(new char[strlen(key) + 1], key);
    next_ = NULL;
}

template <class T>
inline StringHashNode<T>::~StringHashNode() {
    delete [] key_;
}

template <class T>
inline StringHash<T>::StringHash(const unsigned &nbank) {
    table_ = new StringHashNode<T> *[nbank];
    nbank_ = nbank;
    size_ = 0;
    for (unsigned i = 0; i < nbank; ++i)
        table_[i] = NULL;
}

template <class T>
inline StringHash<T>::~StringHash() {
    for (unsigned i = 0; i < nbank_; ++i) {
        StringHashNode<T> *cur = table_[i];
        while (cur) {
            StringHashNode<T> *next = cur->next_;
            delete cur;
            cur = next;
        }
    }
    delete [] table_;
}

template <class T>
inline bool StringHash<T>::lookup(const char * const key, T &value) const {
    unsigned bankNo = hash(key);
    StringHashNode<T> *cur = table_[bankNo];
    while (cur) {
        if (strcmp(cur->key_, key) == 0) {
            value = cur->value_;
            return true;
        }
        cur = cur->next_;
    }
    return false;
}

template <class T>
inline bool StringHash<T>::add(const char * const key, const T &value) {
    unsigned bankNo = hash(key);
    StringHashNode<T> *head = table_[bankNo];
    StringHashNode<T> *cur = head;
    StringHashNode<T> *prev = head;
    while (cur) {
        if (strcmp(cur->key_, key) == 0) {
            cur->value_ = value;
            return true;
        }
        prev = cur;
        cur = cur->next_;
    }
    if (!head) {
        table_[bankNo] = new StringHashNode<T>(key, value);
        table_[bankNo]->next_ = NULL;
    }
    else {
        prev->next_ = new StringHashNode<T>(key, value);
        prev->next_->next_ = NULL;
    }
    size_++;
    return true;
}

template <class T>
inline bool StringHash<T>::remove(const char * const key, const T &value) {
    unsigned bankNo = hash(key);
    StringHashNode<T> *head = &table_[bankNo];
    if (!head)
        return false;
    StringHashNode<T> *cur = head;
    StringHashNode<T> *prev = head;
    while (cur && strcmp(cur->key_, key) != 0) {
        prev = cur;
        cur = cur->next_;
    }
    if (!cur)
        return false;
    if (cur == head)
        table_[bankNo] = cur->next_;
    else
        prev->next_ = cur->next_;
    delete cur;
    size_--;
    return true;
}

template <class T>
inline int StringHash<T>::count(const char * const key) const {
    unsigned bankNo = hash(key);
    StringHashNode<T> *cur = table_[bankNo];
    int count = 0;
    while (cur) {
        if (strcmp(cur->key_, key) == 0) {
            count++;
            break;
        }
        cur = cur->next_;
    }
    return count;
}

template <class T>
inline unsigned StringHash<T>::size() const {
    return size_;
}

template <class T>
inline unsigned StringHash<T>::nbank() const {
    return nbank_;
}

template <class T>
inline StringHashNode<T> **StringHash<T>::head() {
    return table_;
}

template <class T>
inline unsigned StringHash<T>::hash(const char * const key) const {
    unsigned hash = 0;
    unsigned len = strlen(key);

    for (unsigned i = 0; i < len; ++i)
        hash = hash * 9 ^ (unsigned)key[i];
    hash = hash % nbank_;

    return hash;
}


};

#endif


