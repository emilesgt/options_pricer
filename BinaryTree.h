#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>



template <typename T>
class BinaryTree {
private:
    int _depth;
    std::vector<std::vector<T>> _tree;
public:

    BinaryTree();


    void setDepth(int N);
    void setNode(int n, int i, const T& value);

    T getNode(int n, int i) const;
    int getDepth() const { return _depth; }

    void display() const;
};

template <typename T>

BinaryTree<T>::BinaryTree() : _depth(0) {}

template <typename T>
void BinaryTree<T>::setDepth(int N)
{
    _depth = N;
    _tree.clear();                            // we first clear the previous content
    _tree.resize(N + 1);                      // we create N+1 levels (from 0 to N)
    for (int i = 0; i <= N; i++)
    {
        _tree[i].resize(i + 1);               // each level has n+1 nods
    }

}

template <typename T>

void BinaryTree<T>::setNode(int n, int i, const T& value)
{
    if (n < 0 || n > _depth || i < 0 || i > n)
    {
        throw std::out_of_range("Invalid node indices");
    }

    _tree[n][i] = value;                     // we assign the given value to node (n,i)

}

template <typename T>
T BinaryTree<T>::getNode(int n, int i) const {
    if (n < 0 || n > _depth || i < 0 || i > n)
    {
        throw std::out_of_range("Invalid node indices");
    }

    return _tree[n][i];

}

template <typename T>
void BinaryTree<T>::display() const {
    for (int n = 0; n <= _depth; n++) {
        for (int i = 0; i <= n; i++)
            std::cout << _tree[n][i] << " ";
        std::cout << std::endl;
    }
}

