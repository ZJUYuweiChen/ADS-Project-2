#ifndef FIBHEAP_CORE_H
#define FIBHEAP_CORE_H

#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cmath>


#define nullptr NULL

template <class ElementType>
struct FibNode
{
    ElementType element; // element data
    bool visited; // whether visited
    int key; // key value
    int degree; // number of children
    FibNode<ElementType>* left; // left sibling ptr
    FibNode<ElementType>* right; // right sibling ptr
    FibNode<ElementType>* child; // first child ptr
    FibNode<ElementType>* parent; // parent ptr
    bool mark; // whether one of children has been deleted when parent remains same

    // Constructor.
    FibNode(ElementType element, int key);
    FibNode();
    // Destructor.
    ~FibNode();
};

template<class ElementType>
class FibHeap {
private:
    int m_size; // number of nodes
    int m_maxDegree; // max degree of trees
    FibNode<ElementType>** m_cons; // ptr array, temporary space for consolidation

    // Remove node from its siblings.
    void RemoveNode(FibNode<ElementType>* node);

    // Add node to root's left.
    void AddNode(FibNode<ElementType>* node, FibNode<ElementType>* root);

    // Catenate successor to predessor's right.
    void CatList(FibNode<ElementType>* predecessor, FibNode<ElementType>* successor);

    // Remove min tree from heap and return it.
    FibNode<ElementType>* ExtractMin();

    // Make node a child of root.
    void MakeChild(FibNode<ElementType>* node, FibNode<ElementType>* root);

    // Alloc space for consolidation.
    void AllocCons();

    // Consolidate trees with same degree.
    void Consolidate();

    // Decrease parent's and all ancestors' degree by subtrahend.
    void DecreaseDegree(FibNode<ElementType>* node, int subtrahend);

    // Cut node from its tree and insert it into forest.
    void Cut(FibNode<ElementType>* node, FibNode<ElementType>* parent);

    // Cascading cut node:
    // If node is marked, cut it and cascading cut its parent.
    // If not, mark it.
    void CascadingCut(FibNode<ElementType>* node);

    // Increase node's key to "int key".
    void Increase(FibNode<ElementType>* node, int key);

    // Update node's key to "int key".
    void Update(FibNode<ElementType>* node, int key);

    // Find node of key "int key" in tree T and return it.
    FibNode<ElementType>* FindKey(FibNode<ElementType>* T, int key);

    // Find node of element "ElementType element" in tree T and its siblings and return it.
    FibNode<ElementType>* FindElement(FibNode<ElementType>* T, ElementType element);

    // Remove node.
    void Remove(FibNode<ElementType>* node);

    // Print node and all its siblings and children.
    // If flag = true, node is a first child.
    // If flag = false, node is a sibling.
    void Print(FibNode<ElementType>* node, FibNode<ElementType>* prev, bool flag);

    // Destroy node.
    void DestroyNode(FibNode<ElementType>* node);

public:

    FibNode<ElementType>* m_min; // ptr to tree with root of min key
    // Constructor.
    FibHeap();

    // Destructor.
    ~FibHeap();

    // Insert node into FibHeap.
    void Insert(FibNode<ElementType>* node);
    
    // Decrease node's key to "int key".
    void Decrease(FibNode<ElementType>* node, int key);

    // Insert a new node initialized by element and key into heap.
    void Insert(ElementType element, int key);

    // Remove min node.
    void RemoveMin();

    // Combine two fibheaps.
    void Combine(FibHeap<ElementType>* fibheap);

    // Get min tree.
    FibNode<ElementType>* GetMin();

    // Find node of key "int key" in heap.
    FibNode<ElementType>* FindKey(int key);

    // Find node of element "ElementType" in heap.
    FibNode<ElementType>* FindElement(ElementType element);

    // Find node of key oldKey and then update it to newKey.
    void Update(int oldKey, int newKey);

    // Remove node of key "int key".
    void Remove(int key);

    // Return whether there exists a node of key "int key".
    bool Contains(int key);

    // Print heap.
    void Print();

    // Destroy heap.
    void Destroy();
};

// struct FibNode<class ElementType>

// Constructor.
template<class ElementType>
FibNode<ElementType>::FibNode(ElementType element, int key) : element(element), key(key), degree(0), mark(false), left(this), right(this), child(nullptr), parent(nullptr) {}

template<class ElementType>
FibNode<ElementType>::FibNode() : degree(0), mark(false), left(this), right(this), child(nullptr), parent(nullptr) {}

// Destructor.
template<class ElementType>
FibNode<ElementType>::~FibNode() {}

// class FibHeap<class ElementType>

// Private:

// Remove node from its siblings.
template <class ElementType>
void FibHeap<ElementType>::RemoveNode(FibNode<ElementType>* node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

// Add node to root's left.
template <class ElementType>
void FibHeap<ElementType>::AddNode(FibNode<ElementType>* node, FibNode<ElementType>* root)
{
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

// Catenate successor to predecessor's right.
// Both predecessor and successor are double-linked list.
template <class ElementType>
void FibHeap<ElementType>::CatList(FibNode<ElementType>* predecessor, FibNode<ElementType>* successor)
{
    FibNode<ElementType>* temp;
    temp = predecessor->right;
    predecessor->right = successor->right;
    successor->right->left = predecessor;
    successor->right = temp;
    temp->left = successor;
}

// Insert node into FibHeap.
template <class ElementType>
void FibHeap<ElementType>::Insert(FibNode<ElementType>* node)
{
    if (m_size == 0)
        m_min = node;
    else
    {
        AddNode(node, m_min);
        if (node->key < m_min->key)
            m_min = node;
    }
    m_size++;
}

// Remove min tree from heap and return it.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::ExtractMin()
{
    // Get root T of min tree.
    FibNode<ElementType>* T = m_min;

    // If T is the last tree, update min tree to nullptr.
    if (T == T->right)
        m_min = nullptr;
    // If not, update min tree to T's right.
    else
    {
        RemoveNode(T);
        m_min = T->right;
    }
    T->left = T->right = T;

    return T;
}

// Make node a child of root.
template <class ElementType>
void FibHeap<ElementType>::MakeChild(FibNode<ElementType>* node, FibNode<ElementType>* root)
{
    // Remove node from its siblings.
    RemoveNode(node);

    // If root has no child, make node its first child.
    if (root->child == nullptr)
        root->child = node;
    // If has one, add node to its first child's left.
    else
        AddNode(node, root->child);

    // Link node to root, update degree and reset mark.
    // We shall reset mark because node's parent changes.
    node->parent = root;
    root->degree++;
    node->mark = false;
}

// Alloc space for consolidation.
template <class ElementType>
void FibHeap<ElementType>::AllocCons()
{
    // Update max degree and decide whether reallocation is needed.
    int old = m_maxDegree;
    m_maxDegree = static_cast<int>(log(m_size) / log(2.0)) + 1; // "+1" for rounding up

    // If not needed, return.
    if (old >= m_maxDegree)
        return;
    // If is, realloc.
    m_cons = (FibNode<ElementType>**)realloc(m_cons, sizeof(FibHeap<ElementType>*) * (m_maxDegree + 1)); // "+1" for extra space
}

// Consolidate trees with same degree.
template <class ElementType>
void FibHeap<ElementType>::Consolidate()
{
    // Alloc space for consolidation.
    AllocCons();

    // Initialize ptr array.
    for (int i = 0; i < m_maxDegree + 1; i++)
        m_cons[i] = nullptr;

    // Consolidate trees with same degree into the ptr array.
    while (m_min != nullptr)
    {
        FibNode<ElementType>* x = ExtractMin(); // min tree
        int d = x->degree; // degree of min tree

        // Consolidate all trees of same degree with min tree.
        while (m_cons[d] != nullptr)
        {
            // Find such tree.
            FibNode<ElementType>* y = m_cons[d];

            // Make the one with bigger key a child of the one with smaller key.
            if (x->key > y->key)
                std::swap(x, y);
            MakeChild(y, x);

            // Set ptr to null and check next degree.
            m_cons[d] = nullptr;
            d++;
        }

        // Insert updated min tree into temporary space.
        m_cons[d] = x;
    }
    /*m_min = nullptr;*/

    // Insert trees in temporary space back into forest.
    for (int i = 0; i < m_maxDegree + 1; i++)
    {
        if (m_cons[i] != nullptr)
        {
            if (m_min == nullptr)
                m_min = m_cons[i];
            else
            {
                AddNode(m_cons[i], m_min);
                if ((m_cons[i])->key < m_min->key)
                    m_min = m_cons[i];
            }
        }
    }
}

// Decrease node's and all ancestors' degree by subtrahend.
template <class ElementType>
void FibHeap<ElementType>::DecreaseDegree(FibNode<ElementType>* node, int subtrahend)
{
    node->degree -= subtrahend;
    if (node->parent != nullptr)
        DecreaseDegree(node->parent, subtrahend);
}

// Cut node from its tree and add it into forest.
template <class ElementType>
void FibHeap<ElementType>::Cut(FibNode<ElementType>* node, FibNode<ElementType>* parent)
{
    // Remove node from its siblings.
    RemoveNode(node);

    // Update ancestor's degrees.
    DecreaseDegree(parent, node->degree);

    // Update parent's children.
    if (node == node->right)
        parent->child = nullptr;
    else
        parent->child = node->right;

    // Link node to parent, update degree and reset mark.
    // We shall reset mark because node's parent changes.
    node->parent = nullptr;
    node->left = node->right = node;
    node->mark = false;

    // Add node into forest.
    AddNode(node, m_min);
}

// Cascading cut node:
// If node is marked, cut it and cascading cut its parent.
// If not, mark it.
template <class ElementType>
void FibHeap<ElementType>::CascadingCut(FibNode<ElementType>* node)
{
    FibNode<ElementType>* parent = node->parent;
    if (parent != nullptr)
    {
        // If node is marked, cut it and cascading cut its parent.
        if (node->mark == false)
            node->mark = true;
        // If not, mark it.
        else
        {
            Cut(node, parent);
            CascadingCut(parent);
        }
    }
}

// Decrease node's key to "int key".
template <class ElementType>
void FibHeap<ElementType>::Decrease(FibNode<ElementType>* node, int key)
{
    FibNode<ElementType>* parent = node->parent;

    // If decrease fails, return.
    if (m_min == nullptr || node == nullptr || key >= node->key)
        return;

    // Decrease key.
    node->key = key;

    // If violate min heap law, cut it and cascading cut its parent.
    if (parent != nullptr && node->key < parent->key)
    {
        Cut(node, parent);
        CascadingCut(parent);
    }

    // Update min tree.
    if (node->key < m_min->key)
        m_min = node;
}

// Increase node's key to "int key".
template <class ElementType>
void FibHeap<ElementType>::Increase(FibNode<ElementType>* node, int key)
{
    FibNode<ElementType>* child, * parent, * right;

    // If increase fails, return.
    if (m_min == nullptr || node == nullptr || key <= node->key)
        return;

    // Add all children into forest.
    while (node->child != nullptr)
    {
        // Remove.
        child = node->child;
        RemoveNode(child);
        if (child->right == child)
            node->child = nullptr;
        else
            node->child = child->right;

        // Add.
        AddNode(child, m_min);
        child->parent = nullptr;
    }
    node->degree = 0;
    node->key = key;

    // Cut node.
    parent = node->parent;
    // If node has a parent, cut it and cascading cut its parent.
    if (parent != nullptr)
    {
        Cut(node, parent);
        CascadingCut(parent);
    }
    // If not, check if need to update min tree.
    else if (m_min == node)
    {
        right = node->right;
        while (right != node)
        {
            if (node->key > right->key)
                m_min = right;
            right = right->right;
        }
    }
}

// Update node's key to "int key".
template <class ElementType>
void FibHeap<ElementType>::Update(FibNode<ElementType>* node, int key)
{
    if (key < node->key)
        Decrease(node, key);
    else if (key > node->key)
        Increase(node, key);
}

// Find node of key "int key" in tree T and its siblings and return it.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::FindKey(FibNode<ElementType>* T, int key)
{
    FibNode<ElementType>* cur = T; // current
    FibNode<ElementType>* res = nullptr; // result

    // If tree is nullptr, return nullptr.
    if (T == nullptr)
        return T;

    // Traverse all siblings and their children.
    do
    {
        if (cur->key == key)
        {
            res = cur;
            break;
        }
        else
        {
            if ((res = FindKey(cur->child, key)) != nullptr)
                break;
        }
        cur = cur->right;
    } while (cur != T);
    return res;
}

// Find node of key "int key" in heap.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::FindKey(int key)
{
    if (m_min == nullptr)
        return nullptr;

    return FindKey(m_min, key);
}

// Find node of element "ElementType element" in tree T and its siblings and return it.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::FindElement(FibNode<ElementType>* T, ElementType element)
{
    FibNode<ElementType>* cur = T; // current
    FibNode<ElementType>* res = nullptr; // result

    // If tree is nullptr, return nullptr.
    if (T == nullptr)
        return T;

    // Traverse all siblings and their children.
    do
    {
        if (cur->element == element)
        {
            res = cur;
            break;
        }
        else
        {
            if ((res = FindElement(cur->child, element)) != nullptr)
                break;
        }
        cur = cur->right;
    } while (cur != T);

    return res;
}

// Find node of element "ElementType" in heap.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::FindElement(ElementType element)
{
    if (m_min == nullptr)
        return nullptr;

    return FindElement(m_min, element);
}

// Remove node from heap.
template <class ElementType>
void FibHeap<ElementType>::Remove(FibNode<ElementType>* node)
{
    int m = m_min->key - 1;
    Decrease(node, m - 1);
    RemoveMin();
}

// Print node and all its siblings and children.
// If flag = true, node is a first child.
// If flag = false, node is a sibling.
template <class ElementType>
void FibHeap<ElementType>::Print(FibNode<ElementType>* node, FibNode<ElementType>* pred, bool flag)
{
    FibNode<ElementType>* start = node;

    if (node == nullptr)
        return;

    do
    {
        // Pirnt this.
        if (flag == true)
            std::cout << std::setw(8) << node->key << "(" << node->degree << ") is " << std::setw(2) << pred->key << "'s child" << std::endl;
        else
            std::cout << std::setw(8) << node->key << "(" << node->degree << ") is " << std::setw(2) << pred->key << "'s next" << std::endl;

        // Print children.
        if (node->child != nullptr)
            Print(node->child, node, true);

        // Print siblings
        pred = node;
        node = node->right;
        flag = false;
    } while (node != start);
}

// Destroy node.
template <class ElementType>
void FibHeap<ElementType>::DestroyNode(FibNode<ElementType>* node)
{
    FibNode<ElementType>* start = node;

    if (node == nullptr)
        return;

    do
    {
        DestroyNode(node->child);
        node = node->right;
        
        //delete node->left;

    } while (node != start);
}

// Public:

// Constructor.
template <class ElementType>
FibHeap<ElementType>::FibHeap() : m_size(0), m_maxDegree(0), m_min(nullptr), m_cons(nullptr) {}

// Destructor.
template <class ElementType>
FibHeap<ElementType>::~FibHeap() {}

// Initialize a new node by element and key, then insert it into heap.
template <class ElementType>
void FibHeap<ElementType>::Insert(ElementType element, int key)
{
    FibNode<ElementType>* node = new FibNode<ElementType>(element, key);
    Insert(node);
}

// Remove min node.
template <class ElementType>
void FibHeap<ElementType>::RemoveMin()
{
    FibNode<ElementType>* child = nullptr;
    FibNode<ElementType>* m = m_min;

    if (m_min == nullptr)
        return;

    // Add all min node's children to forest.
    while (m->child != nullptr)
    {
        child = m->child;
        RemoveNode(child);
        if (child->right == child)
            m->child = nullptr;
        else
            m->child = child->right;

        AddNode(child, m_min);
        child->parent = nullptr;
    }

    // Remove min node.
    RemoveNode(m);

    // Consolidate.
    if (m->right == m)
        m_min = nullptr;
    else
    {
        m_min = m->right;
        Consolidate();
    }
    m_size--;

    //delete m;
}

// Combine two fibheaps.
template <class ElementType>
void FibHeap<ElementType>::Combine(FibHeap<ElementType>* fibheap)
{
    // If fibheap is nullptr, do nothing.
    if (fibheap == nullptr)
        return;

    // Fibheap with bigger max degree domains.
    if (fibheap->m_maxDegree > this->m_maxDegree)
        std::swap(*this, *fibheap);

    // Domain one is empty.
    if ((this->m_min) == nullptr)
    {
        this->m_min = fibheap->m_min;
        this->m_size = fibheap->m_size;
        free(fibheap->m_cons);
        delete fibheap;
    }
    // The other is empty.
    else if ((fibheap->m_min) == nullptr)
    {
        free(fibheap->m_cons);
        delete fibheap;
    }
    // Both are nonempty.
    else
    {
        // Catenate the other to domain.
        CatList(this->m_min, fibheap->m_min);

        // Update min tree.
        if (this->m_min->key > fibheap->m_min->key)
            this->m_min = fibheap->m_min;

        // Update other members.
        this->m_size += fibheap->m_size;
        free(fibheap->m_cons);
        delete fibheap;
    }
}

// Get min tree.
template <class ElementType>
FibNode<ElementType>* FibHeap<ElementType>::GetMin()
{
    return m_min;
}

// Find node of key oldKey and then update it to newKey.
template <class ElementType>
void FibHeap<ElementType>::Update(int oldKey, int newKey)
{
    FibNode<ElementType>* node;

    node = FindKey(oldKey);
    if (node != nullptr)
        Update(node, newKey);
}

// Remove node of key "int key".
template <class ElementType>
void FibHeap<ElementType>::Remove(int key)
{
    FibNode<ElementType>* node;

    if (m_min == nullptr)
        return;

    node = FindKey(key);
    if (node == nullptr)
        return;

    Remove(node);
}

// Return whether there exists a node of key "int key".
template <class ElementType>
bool FibHeap<ElementType>::Contains(int key)
{
    return FindKey(key) != nullptr ? true : false;
}

// Print heap.
template <class ElementType>
void FibHeap<ElementType>::Print()
{
    int i = 0;
    FibNode<ElementType>* p;

    if (m_min == nullptr)
        return;

    std::cout << "== 쳲������ѵ���ϸ��Ϣ: ==" << std::endl;
    p = m_min;
    do
    {
        i++;
        std::cout << std::setw(2) << i << ". " << std::setw(4) << p->key << "(" << p->degree << ") is root" << std::endl;
        Print(p->child, p, true);
        p = p->right;
    } while (p != m_min);
    std::cout << std::endl;
}

// Destroy heap.
template <class ElementType>
void FibHeap<ElementType>::Destroy()
{
    //DestroyNode(m_min);
    free(m_cons);
}

#endif