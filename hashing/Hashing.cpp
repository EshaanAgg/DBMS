#include <bits/stdc++.h>
using namespace std;

class Bucket
{
private:
    // Count of the elements currently stored in the bucket
    int size = 0;
    // Count of the maximum number of elements that can be stored in a bucket
    int capacity = 0;
    // The local depth of the bucket
    int depth = 0;
    // A set containing all the elements currently stored in the bucket
    set<int> elements;

public:
    // Initialise a bucket with a capacity
    Bucket(int capacity)
    {
        this->capacity = capacity;
    }

    // Returns if a bucket is full
    bool isFull()
    {
        return size == capacity;
    }

    // Returns a boolean indicating if the bucket is full or not
    bool find(int ele)
    {
        return elements.find(ele) != elements.end();
    }

    // Inserts an element in the bucket
    void insert(int ele)
    {
        // Perform integrity checks
        if (isFull())
        {
            throw "There seems to be some error! The program is trying to insert elements to a bucket which is already full."s;
        }
        if (find(ele))
        {
            cout << "The element has already been added to a bucket. Skipping the operation...\n";
            return;
        }

        // Insert the element
        elements.insert(ele);
        size++;
    }
};

class Directory
{
private:
    int depth;
    int bucketCapacity;
    map<string, Bucket *> bucketMap;

    string getLSBBinaryDigits(int n, int pos)
    {
        string digits = "";
        while (pos--)
        {
            digits += n % 2 ? '1' : '0';
            n /= 2;
        }
    }

public:
    Directory(int bucketCapacity)
    {
        depth = 0;
        this->bucketCapacity = bucketCapacity;
        Bucket *bucket = new Bucket(bucketCapacity);
        bucketMap[""] = bucket;
    }

    void insert(int element)
    {
        for (int d = 0; d <= depth; d++)
        {
            string key = getLSBBinaryDigits(element, d);
            if (bucketMap.find(key) == bucketMap.end())
                continue;

            if (!bucketMap[key]->isFull())
            {
                bucketMap[key]->insert(element);
                return;
            }
        }

        throw "Was not able to find any bucket for the provided element."s;
    }
};