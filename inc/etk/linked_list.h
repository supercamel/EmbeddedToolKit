#ifndef ETK_LINKED_LIST_H
#define ETK_LINKED_LIST_H


#include <etk/pool.h>


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
        Iterator(linked_list::Node* head) : node(head) { }
        void next()
        {
            node = node->next;
        }

        void back()
        {
            node = node->back;
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

        Iterator& operator-- ()
        {
            node = node->back;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator iter(*this);
            --(*this);
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

    private:
        linked_list::Node* node;
    };

    linked_list(MemPool& pool) : pool(pool)
    {
    }

    ~linked_list()
    {
        Node* node = head;
        while(node)
        {
            Node* pnext = node->next;
            pool.free<Node>(node);
            node = pnext;
        }
    }


    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(&tail);
    }

    uint32_t size()
    {
        return sz;
    }

    void append()
    {
        T t = T();
        append(t);
    }

    void append(T t)
    {
        sz++;
        Node* node = pool.malloc<Node>(sizeof(Node));
        node->data = t;
        node->next = &tail;
        node->back = nullptr;

        tail.back = node;
        tail.next = nullptr;
        if(head == nullptr)
        {
            head = node;
            return;
        }

        Node* pnode = head;
        while(pnode->next != &tail)
            pnode = pnode->next;

        pnode->next = node;
        node->back = pnode;
    }

    void insert_before(Iterator iter, T t)
    {
        Node* nnode = pool.malloc<Node>(sizeof(Node));
        nnode->data = t;

        iter.node->back->next = nnode;
        nnode->next = iter.node;
        nnode->back = iter.node->back;
        iter.node->back = nnode;
    }

    void remove_item(T t)
    {
        Node* node = head;
        if(node->data == t)
        {
            node = node->next;
            pool.free<Node>(head);
            head = node;
            if(sz)
                sz--;
            return;
        }

        while(node->next != &tail)
        {
            if(node->next->data == t)
            {
                Node* pnext = node->next->next;
                pool.free<Node>(node->next);
                node->next = pnext;
                if(sz)
                    sz--;
                return;
            }
            node = node->next;
        }
    }

    void remove_n(uint32_t n)
    {
        uint32_t c = 0;
        if(n == 0)
        {
            if(head)
            {
                Node* pnext = head->next;
                pool.free<Node>(head);
                head = pnext;
                if(sz)
                    sz--;
            }
            return;
        }

        Node* node = head;
        while(node->next != &tail)
        {
            c++;
            if(c > n)
                return;
            if(n == c)
            {
                Node* pnext = node->next->next;
                pool.free<Node>(node->next);
                node->next = pnext;
                if(sz)
                    sz--;
                return;
            }
        }
    }

    T* get(uint32_t n)
    {
        if(n == 0)
            return head;

        Node* node = head;
        uint32_t c = 0;
        while(node->next != &tail)
        {
            c++;
            if(c > n)
                return nullptr;
            if(n == c)
                return node->next;
            node = node->next;
        }
        return nullptr;
    }

private:
    struct Node
    {
        T data;
        Node* next = nullptr;
        Node* back = nullptr;
    };

    Node* head = nullptr;
    Node tail;
    MemPool& pool;
    uint32_t sz = 0;
};

}


#endif

