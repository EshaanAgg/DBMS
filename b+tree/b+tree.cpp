#include <bits/stdc++.h>
using namespace std;

// // Utility to join/concatenate two vectors
// template <typename T>
// vector<T> joinVectors(const vector<T> v1, const vector<T> v2)
// {
//     vector<T> result = v1;
//     result.insert(result.end(), v2.begin(), v2.end());
//     return result;
// }

// // Base case for the recursion: concatenating one vector
// template <typename T>
// vector<T> joinVectors(const vector<T> v)
// {
//     return v;
// }

// // Recursive function to concatenate/join any number of vectors
// template <typename T, typename... args>
// vector<T> joinVectors(const vector<T> first, const args... rest)
// {
//     return joinVectors(first, joinVectors(rest...));
// }

class Node
{
private:
    // Keeps track if the Node is a leaf
    bool isLeaf;

    // Stores the key value pairs in seperate vectors
    vector<string> keys;
    vector<vector<string>> values;

    // Stores all the children Node of the given Node
    vector<Node> children;

    // Maximum number of keys that can be stored in the node
    int order;

    // Variable controlling that should the debug output be shown
    bool debug = true;

public:
    Node() {}

    // Initaliases a new leaf node with no key-values stored in the same
    Node(int order)
    {
        this->order = order;
        this->keys.clear();
        this->values.clear();
        this->isLeaf = true;
    }

    // Adds a key-value pair to the given Node
    void add(string key, string value)
    {
        // Handle the case that this is the first insertion in the tree
        if (keys.empty())
        {
            keys.push_back(key);
            values.push_back({value});
            return;
        }

        // Find the appropiate place to add this key
        for (int i = 0; i < keys.size(); i++)
        {
            if (keys[i] == key)
            {
                values[i].push_back(value);
                return;
            }

            if (key < keys[i])
            {
                keys.insert(keys.begin() + i, key);
                values.insert(values.begin() + i, {value});
                return;
            }
        }

        // If it is the greatest key in the Node, add it to the end
        keys.push_back(key);
        values.push_back({value});
    }

    // Splits a given node into 2 nodes and stores the information related to the same on the Node instance
    void split()
    {
        Node l = Node(order);
        Node r = Node(order);
        int mid = order / 2;

        l.keys.assign(keys.begin(), keys.begin() + mid);
        l.values.assign(values.begin(), values.begin() + mid);

        r.keys.assign(keys.begin() + mid, keys.end());
        r.values.assign(values.begin() + mid, values.end());

        keys = vector<string>{r.keys[0]};
        values.clear();

        children.push_back(l);
        children.push_back(r);

        isLeaf = false;
    }

    // Returns if the node is full
    bool isFull()
    {
        return keys.size() == order;
    }

    // Used to print the given node to the console
    void show(int counter = 0)
    {
        cout << counter << " ";
        for (string k : keys)
            cout << k << " ";
        cout << endl;

        if (!isLeaf)
            for (Node child : children)
                child.show(counter + 1);
    };
};

// Represnts a B+ Tree
class Tree
{
private:
    // The root of the tree
    Node root;

public:
    // Used to initialise a B+ tree of the given order
    Tree(int order)
    {
        root = Node(order);
    }
};