#include <bits/stdc++.h>
using namespace std;

// Generates a `pos` length key which is used to lookup the number `n` in extensible hashing
string getLSBBinaryDigits(int n, int pos)
{
    string digits = "";
    while (pos--)
    {
        digits = (n % 2 ? "1" : "0") + digits;
        n /= 2;
    }

    return digits;
}

// Represents a bucket which is stored the elements on the disk
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

    // Returns the local depth of the bucket
    int getDepth()
    {
        return depth;
    }

    // Splits the current bucket into two buckets
    // The orginal buckets retains the EVEN elements and the new bucket is assigned the ODD elements
    // The method returns the new bucket
    Bucket split()
    {
        depth++;

        Bucket newBucket = Bucket(capacity);
        newBucket.depth = depth;

        vector<int> elementsToDelete;
        for (int ele : elements)
        {
            string key = getLSBBinaryDigits(ele, depth);
            if (key[0] == '1')
            {
                elementsToDelete.push_back(ele);
                size--;
                newBucket.insert(ele);
            }
        }
        for (int &ele : elementsToDelete)
            elements.erase(elements.find(ele));

        return newBucket;
    }
};

class Directory
{
private:
    int depth;
    int bucketCapacity;
    map<string, int> bucketMap;
    vector<Bucket> buckets;
    int bucketIndex;

public:
    Directory(int bucketCapacity)
    {
        depth = 0;
        bucketIndex = 1;
        this->bucketCapacity = bucketCapacity;

        Bucket bucket = Bucket(bucketCapacity);
        bucketMap[""] = 0;
        buckets.push_back(bucket);
    }

    // Inserts an element into the global directory by insering it into a bucket
    void insert(int element)
    {
        for (int d = 0; d <= depth; d++)
        {
            string key = getLSBBinaryDigits(element, d);
            if (bucketMap.find(key) == bucketMap.end())
                continue;

            Bucket &bucket = buckets[bucketMap[key]];
            if (!bucket.isFull())
            {
                cout << "[INFO] Adding element to already exisiting bucket.\n";
                bucket.insert(element);
                return;
            }

            if (bucket.getDepth() < depth)
            {
                cout << "[INFO] Spliting a bucket.\n";
                Bucket newBucket = bucket.split();

                string complementaryKey = (key[0] == '0' ? '1' : '0') + key.substr(1);
                bucketMap[complementaryKey] = bucketIndex;
                bucketIndex++;
                buckets.push_back(newBucket);

                insert(element);
                return;
            }

            cout << "[INFO] Increasing the global depth of the directory.\n";
            increaseDepth();
            insert(element);
            return;
        }

        throw "Was not able to find any bucket for the provided element"s;
    }

    // Increments the depth of the global directory by 1
    void increaseDepth()
    {
        depth++;

        vector<string> keys;
        for (auto [key, _] : bucketMap)
            keys.push_back(key);

        for (string &key : keys)
        {
            bucketMap["0" + key] = bucketMap[key];
            bucketMap["1" + key] = bucketMap[key];
            bucketMap.erase(key);
        }
    }
};

int main()
{
    Directory d = Directory(2);
    while (true)
    {
        int n;
        cin >> n;
        d.insert(n);
    }
}