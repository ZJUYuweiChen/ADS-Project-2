#ifndef BINHEAP_CORE_H
#define BINHEAP_CORE_H

#include <iomanip>
#include <iostream>

#define nullptr NULL

template <class ElementType>
struct BinNode
{
    ElementType element; // element data
    int key; // key value
    bool visited; 
    int degree; // number of children
    BinNode<ElementType>* child; // first child ptr
    BinNode<ElementType>* parent; // parent ptr
    BinNode<ElementType>* next; // next sibling ptr

    // Constructor.
    BinNode(ElementType element, int key);
    BinNode();
    // Destructor.
    ~BinNode();
};

template <class ElementType>
class BinHeap {
private:
    BinNode<ElementType>* m_root; // root of first tree

    // Make node a child of root.
    void MakeChild(BinNode<ElementType>* child, BinNode<ElementType>* root);
    
    // Merge two binheaps.
    BinNode<ElementType>* MergeList(BinNode<ElementType>* h1, BinNode<ElementType>* h2);

    // Merge H1 and H2 and consolidate all trees with same degree.
    BinNode<ElementType>* Combine(BinNode<ElementType>* h1, BinNode<ElementType>* h2);

    // Reverse a binheap and reset their parent ptr, and then return the new head.
    BinNode<ElementType>* Reverse(BinNode<ElementType>* root);

    // Remove min tree and return it.
    BinNode<ElementType>* ExtractMin(BinNode<ElementType>* root);

    // Remove node of key "int key" from list.
    BinNode<ElementType>* Remove(BinNode<ElementType>* root, int key);

    // Find node of key "int key" in list return it.
    BinNode<ElementType>* FindKey(BinNode<ElementType>* head, int key);

    // Find node of element "ElementType element" in list return it.
    BinNode<ElementType>* FindElement(BinNode<ElementType>* head, ElementType element);

    // Increase key of node to "int key".
    void IncreaseKey(BinNode<ElementType>* node, int key);

    // Update key of node to "int key".
    void UpdateKey(BinNode<ElementType>* node, int key);

    // Get min tree and its predecessor.
    void GetMin(BinNode<ElementType>* root, BinNode<ElementType>*& prev_y, BinNode<ElementType>*& y);
    
    // Print node and all its siblings and children.
    // If flag = true, node is a first child.
    // If flag = false, node is a sibling.
    void Print(BinNode<ElementType>* node, BinNode<ElementType>* prev, bool flag);

    // Destroy node.
    void DestroyNode(BinNode<ElementType>* node);

public:
    // Constructor.
    BinHeap();

    // Destructor.
    ~BinHeap();

    // Decrease key of node to "int key".
    void DecreaseKey(BinNode<ElementType>* node, int key);


    // Initialize a new node by element and key, then insert it into heap.
    void Insert(ElementType element, int key);
    void Insert(BinNode<ElementType>* node);

    // Find node of key "int key" in heap.
    BinNode<ElementType>* FindKey(int key);

    // Find node of element "ElementType element" in heap.
    BinNode<ElementType>* FindElement(ElementType element);

    // Find node of key oldKey and then update it to newKey.
    void Update(int oldkey, int newkey);

    // Remove node of key "int key".
    void Remove(int key);

    // Remove min tree from heap.
    void RemoveMin();

    // Combine two binheaps.
    void Combine(BinHeap<ElementType>* binheap);

    // Get min tree.
    BinNode<ElementType>* GetMin();

    // Return whether there exists a node of key "int key".
    bool Contains(int key);

    // Print heap.
    void Print();

    // Destroy heap.
    void Destroy();
};

// struct BinNode<class ElementType>

// Constructor.
template <class ElementType>
BinNode<ElementType>::BinNode(ElementType element, int key) :element(element), key(key), degree(0), child(nullptr), parent(nullptr), next(nullptr) {}

template <class ElementType>
BinNode<ElementType>::BinNode(): degree(0), child(nullptr), parent(nullptr), next(nullptr) {}

// Destructor.
template <class ElementType>
BinNode<ElementType>::~BinNode() {}

// struct BinHeap<class ElementType>

// Private:

// Make node a child of root.
template <class ElementType>
void BinHeap<ElementType>::MakeChild(BinNode<ElementType>* node, BinNode<ElementType>* root)
{
    node->parent = root;
    node->next = root->child;
    root->child = node;
    root->degree++;
}

// Merge two binheaps into a forest.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::MergeList(BinNode<ElementType>* H1, BinNode<ElementType>* H2)
{
    BinNode<ElementType>* root = nullptr; // root of combined binheap
    BinNode<ElementType>** cur = &root; // ptr to node ptrs along the tree list

    // Insert sorted nodes from two heaps into new heap.
    while (H1 && H2)
    {
        if (H1->degree < H2->degree)
        {
            *cur = H1;
            H1 = H1->next;
        }
        else
        {
            *cur = H2;
            H2 = H2->next;
        }
        cur = &(*cur)->next;
    }

    // If H2 reaches end first, catenate remain parts of H1 to new heap.
    if (H1)
        *cur = H1;
    // If H1 reaches end first, catenate remain parts of H2 to new heap.
    else
        *cur = H2;

    return root;
}

// Merge H1 and H2 and consolidate all trees with same degree.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::Combine(BinNode<ElementType>* H1, BinNode<ElementType>* H2)
{
    // Merge H1 and H2 into a new list.
    BinNode<ElementType>* head = MergeList(H1, H2);
    if (head == nullptr)
        return nullptr;

    // Consolidate all trees with same degree.

    BinNode<ElementType>* pre = nullptr; // predecessor of current
    BinNode<ElementType>* cur = head; // current
    BinNode<ElementType>* suc = cur->next; // successor of current

    // Traverse the list and consolidate all nodes with same degree.
    while (suc != nullptr)
    {
        // If cur and suc don't consolidate:
        // 1. cur->degree != suc->degree;
        // 2. cur->degree == suc->degree == suc->next->degree;
        // We do nothing but move forward.
        if ((cur->degree != suc->degree) || ((suc->next != nullptr) && (suc->degree == suc->next->degree)))
        {
            pre = cur;
            cur = suc;
        }
        // If they consolidate and cur->key <= suc->key, make suc a child of cur.
        else if (cur->key <= suc->key)
        {
            cur->next = suc->next;
            MakeChild(suc, cur);
        }
        // If they consolidate and cur->key > suc->key, make cur a child of suc.
        else
        {
            if (pre == nullptr)
                head = suc;
            else
                pre->next = suc;
            MakeChild(cur, suc);
            cur = suc;
        }

        // Update suc.
        suc = cur->next;
    }

    return head;
}

// Reverse a binheap and reset their parent ptr, and then return the new head.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::Reverse(BinNode<ElementType>* head)
{
    BinNode<ElementType>* next; // head's next
    BinNode<ElementType>* tail = nullptr; // tail of new list

    // If list is empty, return nullptr.
    if (head == nullptr)
        return head;

    // Reverse the list.
    head->parent = nullptr;
    while (head->next)
    {
        next = head->next;
        head->next = tail;
        tail = head;
        head = next;
        head->parent = nullptr;
    }
    head->next = tail;

    return head;
}

// Remove min tree and return it.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::ExtractMin(BinNode<ElementType>* head)
{
    BinNode<ElementType>* min, * pre; // min tree and its predecessor.

    if (head == nullptr)
        return head;

    // Find min tree and its predecessor.
    GetMin(head, pre, min);

    // Remove min from list.
    if (pre == nullptr)
        head = head->next;
    else
        pre->next = min->next;

    // Combine child list and original list to keep order.
    head = Combine(head, Reverse(min->child));

    // delete and return.
    delete min;
    return head;
}

// Remove node of key "int key" from list.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::Remove(BinNode<ElementType>* head, int key)
{
    if (head == nullptr)
        return head;

    BinNode<ElementType>* node = FindKey(head, key); // node of key "int key"

    // If can't find node of key "int key", return.
    if (node == nullptr)
        return head;

    // Upsurge node to root.
    BinNode<ElementType>*  parent = node->parent;
    while (parent != nullptr)
    {
        // Swap data.
        std::swap(node->element, parent->element);
        std::swap(node->key, parent->key);
        // Move up.
        node = parent;
        parent = node->parent;
    }

    // Find predecessor of node.
    BinNode<ElementType>* pre = nullptr;
    BinNode<ElementType>* cur = head;
    while (cur != node)
    {
        pre = cur;
        cur = cur->next;
    }

    // Remove node.
    if (pre)
        pre->next = node->next;
    else
        head = node->next;

    // Combine child list and original list to keep order.
    head = Combine(head, Reverse(node->child));

    // Delete and return.
    delete node;
    return head;
}

// Find node of key "int key" in list return it.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::FindKey(BinNode<ElementType>* head, int key)
{
    BinNode<ElementType>* child = nullptr;
    BinNode<ElementType>* parent = head;

    while (parent != nullptr)
    {
        // If parent is the node, return it.
        if (parent->key == key)
            return parent;
        // If not, search its children.
        else
        {
            if ((child = FindKey(parent->child, key)) != nullptr)
                return child;
            parent = parent->next;
        }
    }

    return nullptr;
}

// Find node of element "ElementType element" in list return it.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::FindElement(BinNode<ElementType>* head, ElementType element)
{
    BinNode<ElementType>* child = nullptr;
    BinNode<ElementType>* parent = head;

    while (parent != nullptr)
    {
        // If parent is the node, return it.
        if (parent->element == element)
            return parent;
        // If not, search its children.
        else
        {
            if ((child = FindElement(parent->child, element)) != nullptr)
                return child;
            parent = parent->next;
        }
    }

    return nullptr;
}

// Find node of key "int key" in heap.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::FindKey(int key)
{
    if(m_root == nullptr)
        return nullptr;

    return FindKey(m_root, key);
}

// Find node of element "ElementType element" in heap.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::FindElement(ElementType element)
{
    if (m_root == nullptr)
        return nullptr;

    return FindElement(m_root, element);
}

// Increase key of node to "int key".
template <class ElementType>
void BinHeap<ElementType>::IncreaseKey(BinNode<ElementType>* node, int key)
{
    // If increase fails, return.
    if (key <= node->key || Contains(key))
        return;

    // Increase.
    node->key = key;

    // Keep min heap.
    BinNode<ElementType>* cur = node;
    BinNode<ElementType>* child = cur->child;
    while (child != nullptr)
    {
        // If violates min heap law, adjust heap.
        if (cur->key > child->key)
        {
            // Find min descendant and swap.
            BinNode<ElementType>* least = child;
            while (child->next != nullptr)
            {
                if (least->key > child->next->key)
                {
                    least = child->next;
                }
                child = child->next;
            }
            std::swap(least->key, cur->key);

            // Upsurge node to keep a min heap.
            cur = least;
            child = cur->child;
        }
        // Else check next child.
        else
        {
            child = child->next;
        }
    }
}

// Decrease key of node to "int key".
template <class ElementType>
void BinHeap<ElementType>::DecreaseKey(BinNode<ElementType>* node, int key)
{
    // If decrease fails, return.
    if (key >= node->key || Contains(key))
       return;

    // Decrease.
    node->key = key;

    // Upsurge node to keep a min heap.
    BinNode<ElementType>* child = node;
    BinNode<ElementType>* parent = node->parent;
    while (parent != nullptr && child->key < parent->key)
    {
        std::swap(parent->key, child->key);
        child = parent;
        parent = child->parent;
    }
}

// Update key of node to "int key".
template <class ElementType>
void BinHeap<ElementType>::UpdateKey(BinNode<ElementType>* node, int key)
{
    if (node == nullptr)
        return;

    if (key < node->key)
        DecreaseKey(node, key);
    else if (key > node->key)
        IncreaseKey(node, key);
}

// Get min tree and its predecessor.
template <class ElementType>
void BinHeap<ElementType>::GetMin(BinNode<ElementType>* head, BinNode<ElementType>*& preMin, BinNode<ElementType>*& min)
{
    // If list is empty, return.
    if (head == nullptr)
        return;

    // Initialization.
    BinNode<ElementType>* preCur = head; // predecessor of current
    BinNode<ElementType>* cur = head->next; // current
    preMin = nullptr;
    min = head;

    // Find min.
    while (cur != nullptr)
    {
        if (cur->key < min->key)
        {
            min = cur;
            preMin = preCur;
        }
        preCur = cur;
        cur = cur->next;
    }
}

// Print node and all its siblings and children.
// If flag = true, node is a first child.
// If flag = false, node is a sibling.
template <class ElementType>
void BinHeap<ElementType>::Print(BinNode<ElementType>* node, BinNode<ElementType>* pred, bool flag)
{
    while (node != nullptr)
    {
        // Print this.
        if (flag == true)
            std::cout << "\t" << std::setw(2) << node->key << "(" << node->degree << ") is " << std::setw(2) << pred->key << "'s child" << std::endl;
        else
            std::cout << "\t" << std::setw(2) << node->key << "(" << node->degree << ") is " << std::setw(2) << pred->key << "'s next" << std::endl;

        // Print children.
        if (node->child != nullptr)
            Print(node->child, node, 1);

        // Print siblings.
        pred = node;
        node = node->next;
        flag = false;
    }
}

// Destroy node.
template <class ElementType>
void BinHeap<ElementType>::DestroyNode(BinNode<ElementType>* node)
{
    BinNode<ElementType>* suc;

    while (node != nullptr)
    {
        suc = node->next;
        DestroyNode(node->child);
        delete node;
        node = suc;
    }
}

// Public:

// Constructor.
template <class ElementType>
BinHeap<ElementType>::BinHeap() :m_root(nullptr) {}

// Destructor.
template <class ElementType>
BinHeap<ElementType>::~BinHeap() {}

// Initialize a new node by element and key, then insert it into heap.
template <class ElementType>
void BinHeap<ElementType>::Insert(ElementType element, int key)
{
    BinNode<ElementType>* node = new BinNode<ElementType>(element, key);
    m_root = Combine(m_root, node);
}
template <class ElementType>
void BinHeap<ElementType>::Insert(BinNode<ElementType>* node){
    m_root = Combine(m_root, node);
}

// Find node of key oldKey and then update it to newKey.
template <class ElementType>
void BinHeap<ElementType>::Update(int oldKey, int newKey)
{
    BinNode<ElementType>* node = FindKey(m_root, oldKey);
    if (node != nullptr)
        UpdateKey(node, newKey);
}

// Remove node of key "int key".
template <class ElementType>
void BinHeap<ElementType>::Remove(int key)
{
    m_root = Remove(m_root, key);
}

// Remove min tree from heap.
template <class ElementType>
void BinHeap<ElementType>::RemoveMin()
{
    m_root = ExtractMin(m_root);
}

// Combine two binheaps.
template <class ElementType>
void BinHeap<ElementType>::Combine(BinHeap<ElementType>* binheap)
{
    if (binheap != nullptr && binheap->m_root != nullptr)
        m_root = Combine(m_root, binheap->m_root);
}

// Get min tree.
template <class ElementType>
BinNode<ElementType>* BinHeap<ElementType>::GetMin()
{
    BinNode<ElementType>* pre, * min;
    GetMin(m_root, pre, min);
    return min;
}

// Return whether there exists a node of key "int key".
template <class ElementType>
bool BinHeap<ElementType>::Contains(int key)
{
    return FindKey(m_root, key) != nullptr ? true : false;
}

// Print heap.
template <class ElementType>
void BinHeap<ElementType>::Print()
{
    BinNode<ElementType>* p;
    if (m_root == nullptr)
        return;

    std::cout << "== �����( ";
    p = m_root;
    while (p != nullptr)
    {
        std::cout << "B" << p->degree << " ";
        p = p->next;
    }
    std::cout << ")����ϸ��Ϣ��" << std::endl;

    int i = 0;
    p = m_root;
    while (p != nullptr)
    {
        i++;
        std::cout << i << ". ������B" << p->degree << ":" << std::endl;
        std::cout << "\t" << std::setw(2) << p->key << "(" << p->degree << ") is root" << std::endl;

        Print(p->child, p, 1);
        p = p->next;
    }
    std::cout << std::endl;
}

// Destroy heap.
template <class ElementType>
void BinHeap<ElementType>::Destroy()
{
    DestroyNode(m_root);
}

#endif
