#pragma once
template <typename T>
class Stack
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &_data, Node *_next = nullptr) : data(_data), next(_next)
        {
        }
    };
    Node *tos;
    int size;

public:
    Stack() : tos(nullptr), size(0)
    {
    }

    void push(const T &data)
    {
        this->tos = new Node(data, this->tos);
        size++;
    }

    void pop()
    {
        if (this->tos == nullptr)
        {
            return;
        }
        Node *temp = this->tos;
        this->tos = this->tos->next;
        delete temp;
        size--;
    }

    const T &top() const
    {
        return tos->data;
    }

    bool empty() const
    {
        return this->tos == nullptr;
    }

    int getSize() const
    {
        return this->size;
    }

    ~Stack()
    {
        while (!empty())
        {
            Node *currentTos = tos;
            tos = tos->next;
            delete currentTos;
        }
    }
};