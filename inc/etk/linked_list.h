
#ifndef ETK_LINKED_LIST_H
#define ETK_LINKED_LIST_H

#include "objpool.h"

namespace etk
{

template <typename T, uint32 SLAB_SIZE = 12> class SingleLinkedList 
{
private:
    struct Node;

public:
    class Iterator
    {
        friend class SingleLinkedList;
    public:
        Iterator() { }

        Iterator(typename SingleLinkedList::Node* head) : node(head) { }

        void next()
        {
            node = node->next;
        }

        T& operator*()
        {
            return node->data;
        }

        Iterator& operator++ ()
        {
            node = node->next;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator iter(*this);
            ++(*this);
            return iter;
        }

        T* operator->()
        {
            return &node->data;
        }

        bool operator==(Iterator iter)
        {
            return (node == iter.node);
        }

        bool operator!=(Iterator iter)
        {
            return (node != iter.node);
        }

        operator bool() const
        {
            return (node != nullptr);
        }

    private:
        typename SingleLinkedList::Node* node;
    };

    SingleLinkedList(Pool* pool) : pool(pool)
    {
        ObjectArrayAllocator<Node, SLAB_SIZE>* objpool_ptr = 
            (ObjectArrayAllocator<Node, SLAB_SIZE>*)pool->alloc(sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>));
        // call placement new on new memory

        if(objpool_ptr == nullptr) {
            node_pool_size = 0;
            return;
        }
        objpool_ptr = new (objpool_ptr) ObjectArrayAllocator<Node, SLAB_SIZE>();

        node_pool_list = (ObjectArrayAllocator<Node, SLAB_SIZE>**)pool->alloc(sizeof(objpool_ptr));
        if(node_pool_list == nullptr) {
            node_pool_size = 0;
            pool->free(objpool_ptr);
            return;
        }
        node_pool_list[0] = objpool_ptr;
        node_pool_size = 1;
        current_node_pool = 0;
    }

    ~SingleLinkedList()
    {
        clear();
    }


    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator append(T t)
    {
        Node* node = allocate_node();
        if(node != nullptr) {
            node->data = t;
            if(head == nullptr) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
            return Iterator(node);
        }
        else {
            return Iterator(nullptr);
        }
    }

    Iterator insert(T t, uint32_t index) {
        if(index == 0) {
            Node* node = allocate_node();
            if(node != nullptr) {
                node->data = t;
                node->next = head;
                head = node;
                return Iterator(node);
            }
            else {
                return Iterator(nullptr);
            }
        }
        else {
            Node* node = head;
            uint32_t i = 0;
            while(node != nullptr) {
                if(i == index - 1) {
                    Node* new_node = allocate_node();
                    if(new_node != nullptr) {
                        new_node->data = t;
                        new_node->next = node->next;
                        node->next = new_node;
                        return Iterator(new_node);
                    }
                    else {
                        return Iterator(nullptr);
                    }
                }
                node = node->next;
                i++;
            }
            return Iterator(nullptr);
        }
    }

    /**
     * \brief Removed and returns the first item on the list.
     */
    T pop_head()
    {
        if(head != nullptr)
        {
            T ret = head->data;
            auto next = head->next;
            free_node(head);
            head = next;
            return ret;
        }
        return T();
    }

    /**
     * \brief Removes an item from the list.
     */
    void remove(Iterator iter)
    {
        if(iter == begin()) {
            pop_head();
            return;
        }

        Iterator prev = begin();
        Iterator i = begin();
        i++;
        while(i != iter)
        {
            i++;
            prev++;
        }
        if(i) {
            prev.node->next = i.node->next;
            free_node(i.node);
        }
    }

    void clear() {
        while(head != nullptr) {
            auto next = head->next;
            free_node(head);
            head = next;
        }
        tail = nullptr;
    }

    /**
     * \brief Returns the number of items in the list.
     */
    uint32 size() const
    {
        Node* pnode = head;
        uint32 count = 0;
        while(pnode->next)
        {
            pnode = pnode->next;
            count++;
        }
        return count;
    }

    bool front(T& t) const
    {
        if(tail != nullptr)
        {
            t = tail->data;
            return true;
        }
        return false;
    }

    uint32 get_slab_size() {
        return sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>);
    }

private:
    Node* allocate_node() {
        Node* node = nullptr;
        while(current_node_pool < node_pool_size) {
           node = node_pool_list[current_node_pool]->alloc(); 
           if(node == nullptr) {
               current_node_pool++;
           } else {
                node->next = nullptr;
                break;
           }
        } 

        // try add another pool
        if(node == nullptr) {
            node_pool_size++;
            // try to allocate another pool, if it fails, return false
            
            // create new entry in node_pool_list
            ObjectArrayAllocator<Node, SLAB_SIZE>** new_pool_list = 
                (ObjectArrayAllocator<Node, SLAB_SIZE>**)pool->realloc(node_pool_list, sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>*) * node_pool_size);
            if(new_pool_list == nullptr) {
                node_pool_size--;
                return nullptr;
            }
            node_pool_list = new_pool_list;

            // now allocate a new object pool
            node_pool_list[current_node_pool] = (ObjectArrayAllocator<Node, SLAB_SIZE>*)
                    (pool->alloc(sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>)));

            // check if allocation was a win
            if(node_pool_list[current_node_pool] == nullptr) {
                // reallocate the pool list back to original size
                node_pool_size--;
                node_pool_list = (ObjectArrayAllocator<Node, SLAB_SIZE>**)
                        pool->realloc(node_pool_list, sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>*) * node_pool_size);
                return nullptr;
            }

            // call placement new on the object pool
            new (node_pool_list[current_node_pool]) ObjectArrayAllocator<Node, SLAB_SIZE>();

            node = node_pool_list[current_node_pool]->alloc();
            node->next = nullptr;
        }
        return node;
    }

    void free_node(Node* node)
    {
        //iterate over node_pool_list and free the node
        for(uint32 i = 0; i < node_pool_size; i++) {
            if(node_pool_list[i]->free(node)) {
                if(i < current_node_pool) {
                    current_node_pool = i;
                }
                // if the pool is empty, free it
                pool->free(node_pool_list[i]);
                // move the last pool to the current position
                node_pool_list[i] = node_pool_list[node_pool_size - 1];
                node_pool_size--;

                if(node_pool_size > 0) {
                    // try to shrink the list
                    node_pool_list = (ObjectArrayAllocator<Node, SLAB_SIZE>**)
                        pool->realloc(node_pool_list, sizeof(ObjectArrayAllocator<Node, SLAB_SIZE>*) * node_pool_size);
                }
                else {
                    // free the list
                    pool->free(node_pool_list);
                    node_pool_list = nullptr;
                }

                current_node_pool = 0;
                return;
            }
        }
    }

    struct Node
    {
        T data;
        Node* next = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    Pool* pool;

    ObjectArrayAllocator<Node, SLAB_SIZE>** node_pool_list;
    uint32 node_pool_size = 0;
    uint32 current_node_pool = 0;
};

}


#endif
