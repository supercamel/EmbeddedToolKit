
#ifndef ETK_LINKED_LIST_H
#define ETK_LINKED_LIST_H

#include "pool.h"


namespace etk
{

template <typename T> class linked_list
{
private:
    struct Node;

public:
    class Iterator
    {
        friend class linked_list;
    public:
        Iterator() { }

        Iterator(typename linked_list::Node* head) : node(head) { }

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
        typename linked_list::Node* node;
    };

    linked_list(Pool* pool) : pool(pool)
    {
    }

    ~linked_list()
    {
        Node* node = head;
        while(node != nullptr)
        {
            Node* pnext = node->next;
            pool->free(node);
            node = pnext;
        }
    }


    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(tail);
    }

    bool append(T t)
    {
        Node* node = static_cast<Node*>(pool->alloc(sizeof(Node)));
        if(node == nullptr)
            return false;

        node->data = t;
        node->next = nullptr;
        node->prev = nullptr;
        if(head == nullptr)
        {
            head = node;
            return true;
        }

        Node* pnode = head;
        while(pnode->next)
        {
            pnode = pnode->next;
        }

        pnode->next = node;
        node->prev = pnode;
        return true;
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
            pool->free(head);
            head = next;
            return ret;
        }
    }

    /**
     * \brief Removes an item from the list.
     */
    void remove(Iterator iter)
    {
        if(iter.node->prev)
            iter.node->prev->next = iter.node->next;
        else
            head = iter.node->next;
        if(iter.node->next)
            iter.node->next->prev = iter.node->prev;
        pool->free(iter.node);
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

private:
    struct Node
    {
        T data;
        Node* next = nullptr;
        Node* prev = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    Pool* pool;
};

}


#endif
