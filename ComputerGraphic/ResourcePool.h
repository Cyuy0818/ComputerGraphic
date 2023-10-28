#ifndef __RESOURCE_POOL__
#define __RESOURCE_POOL__
#include<exception>
#include<iostream>

class myException : public std::exception
{
public:
    myException(const std::string& what_arg)
        : m_what_arg(what_arg)
    {
    }
    virtual const char* what(void) const noexcept override
    {
        auto text = "\r\nError: " + m_what_arg;
        return text.c_str();
    }
private:
    std::string m_what_arg;
};


template<typename T>
class ResourcePool {
private:
    struct Node {
        T data;
        int next;
    };
    int MAX_SIZE;
    Node* nodes;  // ��̬����Ľڵ�����
    int head;  // ͷ�ڵ������
    int freeList;  // ���нڵ������ͷ�ڵ�����

public:
    ResourcePool(int max_size) :
        MAX_SIZE(max_size), head(-1), freeList(0) {
        nodes = new Node[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE - 1; i++) {
            nodes[i].next = i + 1;  // ��ʼ��ÿ���ڵ��nextָ�룬ʹ��ָ����һ���ڵ�
        }
        nodes[MAX_SIZE - 1].next = -1;  // ���һ���ڵ��nextָ��Ϊ-1����ʾ�������
    }

    T operator[](int index) {
        return this->nodes[index].data;
    }

    int allocate() {
        if (freeList == -1) {
            throw myException("Resource pool has no Empty");
            return -1;  // û�п�����Դ
        }

        int newIndex = freeList;
        freeList = nodes[freeList].next;

        nodes[newIndex].next = head;
        head = newIndex;

        return newIndex;
    }

    void deallocate(int index) {
        if (index < 0 || index >= MAX_SIZE) {
            throw myException("resource pool Invalid index.");
            return;
        }

        if (head == -1) {
            throw myException("resource pool is empty.");
            return;
        }

        if (index == head) {
            head = nodes[head].next;
        }
        else {
            int prevIndex = head;
            while (nodes[prevIndex].next != index) {
                prevIndex = nodes[prevIndex].next;
            }
            nodes[prevIndex].next = nodes[index].next;
        }

        nodes[index].next = freeList;
        freeList = index;
    }

    ~ResourcePool() {
        delete[] this->nodes;
    }
    void set(int index, const T& ele) {
        if (index < 0 || index >= MAX_SIZE) {
            throw myException("resource pool Invalid index.");
            return;
        }
        this->nodes[index].data = ele;
    }
};

#endif