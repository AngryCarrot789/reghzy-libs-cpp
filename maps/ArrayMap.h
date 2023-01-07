//
// Created by kettl on 07/01/2023.
//

#ifndef REGHZYLIBS_ARRAYMAP_H
#define REGHZYLIBS_ARRAYMAP_H

#include <stdint.h>
#include <stdlib.h>

namespace maps {
    template<class K, class V>
    class Node {
    public:
        int32_t mKeyHash;
        K mKey;
        V mVal;

        Node(int32_t mKeyHash, K mKey, V mVal) : mKeyHash(mKeyHash), mKey(mKey), mVal(mVal) {

        }

        Node() : mKeyHash(0), mKey(0), mVal(0) {
        }
    };

#define SIZEOF_NODE(count) (sizeof(Node<K, V>) * (count))

    template<class K, class V>
    class ArrayMap {
        typedef int32_t (* KHashFunc)(K);

        typedef bool (* KCompFunc)(K, K);

    private:
        KHashFunc mKeyHashFunc;
        KCompFunc mKeyCompFunc;

        int32_t mTabCap;
        int32_t mTabLen;
        Node<K, V>* mTab;

    public:
        ArrayMap(KHashFunc keyHashFunction) {
            mKeyHashFunc = keyHashFunction;
            mKeyCompFunc = keyCompFuncDef;
            mTabCap = mTabLen = 0;
            mTab = nullptr;
        }

        ArrayMap(KHashFunc keyHashFunction, KCompFunc keyEqualFunc) {
            mKeyHashFunc = keyHashFunction;
            mKeyCompFunc = keyEqualFunc;
            mTabCap = mTabLen = 0;
            mTab = nullptr;
        }

    public:
        Node<K, V>* begin() {
            return mTab;
        }

        Node<K, V>* end() {
            return mTab + mTabLen;
        }

        // Adds the given key/val entry to this map. If the key already exists, the val is replaced
        // and the replaced parameter will contain the old value, and the function returns true.
        // If the key was never inserted, false is returned and the key is inserted
        V put(K key, V val) {
            V output = {};
            if (put(key, val, &output)) {
                return val;
            }

            return 0;
        }

        bool put(K key, V val, V* replaced) {
            int index = indexForKey(key);
            if (index == -1) {
                insert(key, val);
                return false;
            }

            Node<K, V>* node = &mTab[index];
            *replaced = node->mVal;
            node->mVal = val;
            return true;
        }

        V get(K key) {
            int index = indexForKey(key);
            if (index == -1) {
                return 0;
            }

            Node<K, V>* node = &mTab[index];
            return node->mVal;
        }

        bool tryGet(K key, V* val) {
            int index = indexForKey(key);
            if (index == -1) {
                return false;
            }

            Node<K, V>* node = &mTab[index];
            *val = node->mVal;
            return true;
        }

        V remove(K key) {
            int index = indexForKey(key);
            if (index == -1) {
                return 0;
            }

            Node<K, V>* node = &mTab[index];
            V val = node->mVal;
            removeAt(index);
            return val;
        }

    private:
        void removeAt(int index) {
            memmove(&mTab[index], &mTab[index + 1], SIZEOF_NODE(mTabLen - index));
            memset(&mTab[mTabLen], 0, sizeof(Node<K, V>));
            mTabLen--;
        }

        void insert(K key, V val) {
            if (mTabCap == 0 || mTabLen >= mTabCap) {
                int newCapacity = mTabCap + (mTabCap >> 1);
                if (newCapacity < 1) {
                    newCapacity = 1;
                }
                else if (newCapacity == mTabCap) {
                    newCapacity++;
                }

                // Node<K, V>* table = new Node<K, V>[newCapacity];
                auto* table = (Node<K, V>*) malloc(SIZEOF_NODE(newCapacity));
                if (table == 0) {
                    throw std::exception("Failed to allocate table");
                }

                if (mTab != nullptr) {
                    memcpy(table, mTab, SIZEOF_NODE(mTabCap));
                    free(mTab);
                }

                mTabCap = newCapacity;
                mTab = table;
            }

            mTab[mTabLen++] = Node<K, V>(mKeyHashFunc(key), key, val);
        }

    private:
        int indexForKey(K key) {
            int32_t hash = mKeyHashFunc(key);
            for (int i = 0; i < mTabLen; i++) {
                Node<K, V>* node = &mTab[i];
                if (node->mKeyHash == hash && mKeyCompFunc(node->mKey, key)) {
                    return i;
                }
            }

            return -1;
        }

    public:
        static bool keyCompFuncDef(K a, K b) {
            return a == b;
        }
    };
}

#endif //REGHZYLIBS_ARRAYMAP_H
