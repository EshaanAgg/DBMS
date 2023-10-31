#include <bits/stdc++.h>
using namespace std;

#define range(x, s, n) for (int x = s; x < n; ++x)
#define all(a) a.begin(), a.end()
#define in(a, b) ((b).find(a) != (b).end())
#define len(a) ((int)(a.size()))

// Debugging template start
void __print(int x) { cout << x; }
void __print(long x) { cout << x; }
void __print(long long x) { cout << x; }
void __print(unsigned x) { cout << x; }
void __print(unsigned long x) { cout << x; }
void __print(unsigned long long x) { cout << x; }
void __print(float x) { cout << x; }
void __print(double x) { cout << x; }
void __print(long double x) { cout << x; }
void __print(char x) { cout << '\'' << x << '\''; }
void __print(const char *x) { cout << '\"' << x << '\"'; }
void __print(const string &x) { cout << '\"' << x << '\"'; }
void __print(bool x) { cout << (x ? "true" : "false"); }

template <typename T, typename V>
void __print(const pair<T, V> &x)
{
    cout << '{';
    __print(x.first);
    cout << ',';
    __print(x.second);
    cout << '}';
}
template <typename T>
void __print(const T &x)
{
    int f = 0;
    cout << '{';
    for (auto &i : x)
        cout << (f++ ? "," : ""), __print(i);
    cout << "}";
}
void _print() { cout << "\n"; }
template <typename T, typename... V>
void _print(T t, V... v)
{
    __print(t);
    if (sizeof...(v))
        cout << ", ";
    _print(v...);
}
#ifndef ONLINE_JUDGE
#define debug(x...)      \
    cout << #x << " = "; \
    _print(x)
#else
#define debug(x...)
#endif
// Debugging template end

// Utility functions
template <typename T>
void eraseAndAdd(vector<T> &base, int index, vector<T> &toAdd)
{
    base.erase(base.begin() + index);
    base.insert(base.begin() + index, toAdd.begin(), toAdd.end());
}

template <typename T>
void erase(vector<T> &base, int start, int end)
{
    base.erase(base.begin() + start, base.begin() + end);
}

template <typename T>
void erase(vector<T> &base, int index)
{
    base.erase(base.begin() + index);
}

template <typename T>
void addToEnd(vector<T> &base, vector<T> &add)
{
    base.insert(base.end(), add.begin(), add.end());
}

// Represents a node in the B+ Tree
class Node
{
private:
    vector<int> keys;

    vector<Node *> children;

    Node *parent;
    Node *next;
    Node *prev;
    bool isLeaf;

    friend class BPlusTree;

public:
    int debugLevel = 2;

    Node(Node *parent = nullptr, bool isLeaf = false, Node *prev = nullptr, Node *next = nullptr)
    {
        this->parent = parent;
        this->isLeaf = isLeaf;
        this->prev = prev;
        this->next = next;

        if (next)
            next->prev = this;

        if (prev)
            prev->next = this;
    }

    int indexOfChild(int key)
    {
        range(i, 0, keys.size()) if (key < keys[i]) return i;
        return keys.size();
    }

    int getKeyIndex(int key)
    {
        range(i, 0, keys.size())
        {
            if (key == keys[i])
                return i;
        }
        return -1;
    }

    void setChild(int key, vector<Node *> child)
    {
        int i = indexOfChild(key);
        keys.insert(keys.begin() + i, key);
        eraseAndAdd(children, i, child);
    }

    pair<int, pair<Node *, Node *>> splitInternal()
    {
        if (debugLevel == 2)
            cout << "[INFO] Splitting a internal node.\n";

        Node *left = new Node(parent, false, nullptr, nullptr);
        int mid = keys.size() / 2;

        copy(keys.begin(), keys.begin() + mid, back_inserter(left->keys));
        copy(children.begin(), children.begin() + mid + 1, back_inserter(left->children));

        for (Node *child : left->children)
            child->parent = left;

        erase(keys, 0, mid + 1);
        erase(children, 0, mid + 1);

        return {keys[mid], {left, this}};
    }

    pair<int, pair<Node *, Node *>> splitLeaf()
    {
        if (debugLevel == 2)
            cout << "[INFO] Splitting a leaf node.\n";

        Node *left = new Node(parent, true, prev, this);
        int mid = keys.size() / 2;

        left->keys = vector<int>(keys.begin(), keys.begin() + mid);
        erase(keys, 0, mid);

        return {keys[0], {left, this}};
    }

    void add(int key)
    {
        if (!find(key))
        {
            if (debugLevel)
                cout << "[INFO] The key was not found in the tree.\n";
            keys.insert(keys.begin() + indexOfChild(key), key);
            return;
        }

        if (debugLevel)
            cout << "[ERROR] The key was not found in the tree.\n";
    }

    bool find(int key)
    {
        return getKeyIndex(key) != -1;
    }
};

class BPlusTree
{
public:
    int debugLevel = 2;

private:
    Node *root;
    int maxCapacity;
    int minCapacity;
    int depth;

    Node *findLeaf(int key)
    {
        Node *node = root;
        while (!node->isLeaf)
            node = node->children[node->indexOfChild(key)];
        return node;
    }

    void insert(pair<int, pair<Node *, Node *>> result)
    {
        int key = result.first;
        Node *left = result.second.first;
        Node *right = result.second.second;
        Node *parent = right->parent;

        if (parent == nullptr)
        {
            if (debugLevel > 1)
                cout << "[INFO] Creating a new node as the parent is NULL.\n";
            left->parent = right->parent = root =
                new Node(nullptr, false, nullptr, nullptr);
            depth += 1;
            root->keys = {key};
            root->children = {left, right};
            return;
        }

        parent->setChild(key, {left, right});
        if (parent->keys.size() > maxCapacity)
            insert(parent->splitInternal());
    }

    void removeFromLeaf(int key, Node *node)
    {
        if (debugLevel == 2)
            cout << "[INFO] Removing the element from the leaf.\n";
        int index = node->getKeyIndex(key);
        node->keys.erase(node->keys.begin() + index);
        if (node->parent)
        {
            int indexInParent = node->parent->indexOfChild(key);
            if (indexInParent)
                node->parent->keys[indexInParent - 1] = node->keys.front();
        }
    }

    void removeFromInternal(int key, Node *node)
    {
        if (debugLevel == 2)
            cout << "[INFO] Removing the element from internal node.\n";

        int index = node->getKeyIndex(key);
        if (index != -1)
        {
            Node *leftMostLeaf = node->children[index + 1];
            while (!leftMostLeaf->isLeaf)
                leftMostLeaf = leftMostLeaf->children.front();

            node->keys[index] = leftMostLeaf->keys.front();
        }
    }

    void borrowKeyFromNeighbor(Node *node, Node *neighbor, bool borrowFromRight)
    {
        if (debugLevel == 2)
        {
            string nei = borrowFromRight ? "right" : "left";
            cout << "[INFO] Borrowing key from the " + nei + " neighbour to the node.\n";
        }

        if (borrowFromRight)
        {
            node->keys.push_back(neighbor->keys.front());
            erase(neighbor->keys, 0);
        }
        else
        {
            node->keys.insert(node->keys.begin(), neighbor->keys.back());
            neighbor->keys.pop_back();
        }

        for (int i = 0; i < node->parent->children.size(); i++)
        {
            if (node->parent->children[i] == (borrowFromRight ? neighbor : node))
            {
                node->parent->keys[i - (borrowFromRight ? 1 : 0)] = node->keys.front();
                break;
            }
        }
    }

    void mergeNodes(Node *node, Node *neighbor, bool mergeWithRight)
    {
        if (debugLevel == 2)
        {
            string nei = mergeWithRight ? "right" : "left";
            cout << "[INFO] Merging the node with the " + nei + " neighbour.\n";
        }

        if (mergeWithRight)
        {
            addToEnd(node->keys, neighbor->keys);
            node->next = neighbor->next;
        }
        else
        {
            addToEnd(neighbor->keys, node->keys);
            neighbor->next = node->next;
        }

        if (node->next)
            node->next->prev = node;

        for (int i = 0; i < node->parent->children.size(); i++)
        {
            if (node->parent->children[i] == (mergeWithRight ? neighbor : node))
            {
                erase(node->parent->keys, i - 1);
                erase(node->parent->children, i);
                break;
            }
        }
    }

    void borrowKeyFromInternal(int myPositionInParent, Node *node, Node *neighbor, bool borrowFromRight)
    {
        if (debugLevel == 2)
        {
            string nei = borrowFromRight ? "right" : "left";
            cout << "[INFO] Borrowing keys from the " + nei + " neighbour.\n";
        }

        if (borrowFromRight)
        {
            node->keys.push_back(node->parent->keys[myPositionInParent]);
            node->parent->keys[myPositionInParent] = neighbor->keys.front();
            erase(neighbor->keys, 0);

            node->children.push_back(neighbor->children.front());
            neighbor->children.front()->parent = node;
            erase(neighbor->children, 0);
        }
        else
        {
            node->keys.insert(node->keys.begin(), node->parent->keys[myPositionInParent - 1]);
            node->parent->keys[myPositionInParent - 1] = neighbor->keys.back();
            neighbor->keys.pop_back();

            node->children.insert(node->children.begin(), neighbor->children.back());
            neighbor->children.back()->parent = node;
            neighbor->children.pop_back();
        }
    }

    // Utility function to merge an internal node with its neighbor
    void mergeInternalNodes(int myPositionInParent, Node *node, Node *neighbor, bool mergeWithRight)
    {
        if (debugLevel == 2)
        {
            string nei = mergeWithRight ? "right" : "left";
            cout << "[INFO] Merging the node with the " + nei + " neighbour.\n";
        }

        if (mergeWithRight)
        {
            node->keys.push_back(node->parent->keys[myPositionInParent]);
            erase(node->parent->keys, myPositionInParent);

            addToEnd(node->keys, neighbor->keys);
            addToEnd(node->children, neighbor->children);
        }
        else
        {
            neighbor->keys.push_back(node->parent->keys[myPositionInParent - 1]);
            erase(node->parent->keys, myPositionInParent - 1);

            neighbor->keys.insert(neighbor->keys.end(), node->keys.begin(), node->keys.end());
            neighbor->children.insert(neighbor->children.end(), node->children.begin(), node->children.end());
        }

        for (Node *child : node->children)
            child->parent = node;

        erase(node->parent->children, myPositionInParent + (mergeWithRight ? 1 : 0));
    }

    void updateRootIfNeeded(Node *&root, int &depth)
    {
        if (!root->keys.empty() || root->children.empty())
            return;

        root = root->children[0];
        root->parent = nullptr;
        depth -= 1;
    }

    bool canBorrowKeyFromNeighbor(Node *node, Node *neighbor, int minCapacity)
    {
        return neighbor && neighbor->parent == node->parent && neighbor->keys.size() > minCapacity;
    }

    bool canMergeWithNeighbor(Node *node, Node *neighbor, int minCapacity)
    {
        return neighbor && neighbor->parent == node->parent && neighbor->keys.size() <= minCapacity;
    }

    int findMyPositionInParent(Node *node)
    {
        for (int i = 0; i < node->parent->children.size(); i++)
            if (node->parent->children[i] == node)
                return i;
        return -1;
    }

    void handleLeafNodeNeighbor(Node *node, int minCapacity)
    {
        Node *next = node->next;
        Node *prev = node->prev;

        if (canBorrowKeyFromNeighbor(node, next, minCapacity))
            borrowKeyFromNeighbor(node, next, true);
        else if (canBorrowKeyFromNeighbor(node, prev, minCapacity))
            borrowKeyFromNeighbor(node, prev, false);
        else if (canMergeWithNeighbor(node, next, minCapacity))
            mergeNodes(node, next, true);
        else if (canMergeWithNeighbor(node, prev, minCapacity))
            mergeNodes(node, prev, false);
    }

    void handleInternalNodeNeighbor(Node *node, int myPositionInParent, int minCapacity)
    {
        Node *next = (myPositionInParent < node->parent->children.size() - 1) ? node->parent->children[myPositionInParent + 1] : nullptr;
        Node *prev = (myPositionInParent > 0) ? node->parent->children[myPositionInParent - 1] : nullptr;

        if (canBorrowKeyFromNeighbor(node, next, minCapacity))
            borrowKeyFromInternal(myPositionInParent, node, next, true);
        else if (canBorrowKeyFromNeighbor(node, prev, minCapacity))
            borrowKeyFromInternal(myPositionInParent, node, prev, false);
        else if (canMergeWithNeighbor(node, next, minCapacity))
            mergeInternalNodes(myPositionInParent, node, next, true);
        else if (canMergeWithNeighbor(node, prev, minCapacity))
            mergeInternalNodes(myPositionInParent, node, prev, false);
    }

public:
    BPlusTree() {}

    BPlusTree(int maxCapacity)
    {
        root = new Node(nullptr, true, nullptr, nullptr);
        this->maxCapacity = maxCapacity;
        minCapacity = maxCapacity / 2;
        depth = 0;
        debugLevel = 2;
    }

    void find(int key)
    {
        if (findLeaf(key)->find(key))
            cout << "The key EXISTS in the tree.\n";
        else
            cout << "The key DOES NOT EXISTS in the tree.\n";
    }

    void add(int key)
    {
        Node *leaf = findLeaf(key);
        leaf->add(key);
        if (debugLevel)
            cout << "[SUCCESS] The key has been inserted successfully.\n";

        if (leaf->keys.size() > maxCapacity)
            insert(leaf->splitLeaf());
    }

    void print(Node *node = nullptr, string _prefix = "", bool _last = true)
    {
        if (!node)
            node = root;
        cout << _prefix << "|- [";

        range(i, 0, node->keys.size())
        {
            cout << node->keys[i];
            if (i != node->keys.size() - 1)
                cout << ", ";
        }

        cout << "]" << endl;

        _prefix += _last ? "   " : "|  ";

        if (!node->isLeaf)
        {
            range(i, 0, node->children.size())
            {
                bool _last = (i == node->children.size() - 1);
                print(node->children[i], _prefix, _last);
            }
        }
    }

    void remove(int key, Node *node = nullptr)
    {
        if (node == nullptr)
            node = findLeaf(key);

        if (node->isLeaf)
            removeFromLeaf(key, node);
        else
            removeFromInternal(key, node);

        if (node->keys.size() < minCapacity)
        {
            if (node == root)
            {
                updateRootIfNeeded(root, depth);
                return;
            }
            else if (node->isLeaf)
                handleLeafNodeNeighbor(node, minCapacity);
            else
            {
                int myPositionInParent = findMyPositionInParent(node);
                handleInternalNodeNeighbor(node, myPositionInParent, minCapacity);
            }
        }

        if (node->parent)
            remove(key, node->parent);

        if (debugLevel)
            cout << "[SUCCESS] The key was successfully removed from the tree.\n";
    }
};

class REPL
{
private:
    int capacity;
    BPlusTree tree;
    string demarcator = ">> ";

    // Shows help and gives an overview of the suported commands
    void showHelp()
    {
        cout << "Hi! This is a case-sensitive REPL that supports the following operations: \n";
        cout << "I X\t| Used to insert the value 'X' into the tree\n";
        cout << "D X\t| Used to delete the value 'X' from the tree\n";
        cout << "S X\t| Used to search the value 'X' in the tree\n";
        cout << "P \t| Used to print the tree\n";
        cout << "\nPlease note that all the commands are CASE-SENSITIVE in nature.\n";
    }

public:
    int debugLevel;

    REPL()
    {
        cout << "Please enter the capacity of a node in the tree: ";
        cin >> capacity;
        tree = BPlusTree(capacity);
        tree.debugLevel = debugLevel;
    }

    void start()
    {
        showHelp();

        while (true)
        {
            cout << demarcator;
            char op;
            cin >> op;

            // Insert operation
            if (op == 'I')
            {
                int ele;
                cin >> ele;
                tree.add(ele);
            }
            // Delete operation
            else if (op == 'D')
            {
                int ele;
                cin >> ele;
                tree.remove(ele);
            }
            // Search operation
            else if (op == 'S')
            {
                int ele;
                cin >> ele;
                tree.find(ele);
            }
            // Print operation
            else if (op == 'P')
            {
                tree.print();
            }
            else
                cout << "[ERROR] Unrecognized command: " << op << ".\n";
        }
    }
};

int main()
{
    REPL repl = REPL();
    repl.debugLevel = 2;
    repl.start();
}