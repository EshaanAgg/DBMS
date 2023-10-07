#include <bits/stdc++.h>
using namespace std;

// Generates a `len` length key which is used to lookup the number `n` in extensible hashing
string getLSBBinaryDigits(int n, int len)
{
    string digits = "";
    while (len--)
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

    // Returns the number of elements stored in the bucket
    int getSize()
    {
        return size;
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

    // Returns a boolean indicating if the bucket is empty
    bool empty()
    {
        return size == 0;
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
            cout << "[WARNING] The element has already been added to a bucket. Skipping the operation.\n";
            return;
        }

        // Insert the element
        elements.insert(ele);
        size++;
        cout << "[SUCCESS] The element was added successfully\n";
    }

    // Removes an element from the buket
    void remove(int ele)
    {
        if (!find(ele))
        {
            cout << "[WARNING] The element does not exist in the bucket. Skipping the operation.\n";
            return;
        }

        // Remove the element
        elements.erase(elements.find(ele));
        cout << "[SUCCESS] The element was removed successfully\n";
        size--;
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
                newBucket.elements.insert(ele);
                newBucket.size++;
            }
        }

        for (int &ele : elementsToDelete)
            elements.erase(elements.find(ele));

        return newBucket;
    }

    // Merges the content of another bucket into itself
    void merge(Bucket &bucket)
    {
        depth--;
        for (const int &ele : bucket.elements)
        {
            elements.insert(ele);
            size++;
        }
    }

    // Prints the contents of the bucket to the STDOUT in a comma separated manner
    void display()
    {
        auto it = elements.begin();
        while (it != elements.end())
        {
            cout << *it;
            ++it;
            if (it != elements.end())
                cout << ", ";
        }
        cout << endl;
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
    int debug = 0;
    int tableWidth = 120;

    // Prints a line of dashes to the STDOUT
    void printLine()
    {
        for (int i = 0; i < tableWidth; i++)
            cout << "-";
        cout << "\n";
    }

    // Utility to print the keys vector
    void printKeys(vector<string> &v)
    {
        for (int i = 0; i < v.size();)
        {
            cout << "'" << v[i] << "'";
            i++;
            if (i != v.size())
                cout << ", ";
        }
    }

    // Returns the split key for a particular bucket key
    string getComplementaryKey(string key)
    {
        return (key[0] == '0' ? '1' : '0') + key.substr(1);
    }

    // Checks if the string `str` has the suffix `suffix`
    bool hasSuffix(const string &str, const string &suffix)
    {
        if (str.length() < suffix.length())
            return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    // Returns the suffix of the string `str` with length `len`
    string suffix(string str, int len)
    {
        return str.substr(str.size() - len);
    }

public:
    Directory() {}

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
        string key = getLSBBinaryDigits(element, depth);
        if (bucketMap.find(key) == bucketMap.end())
            throw "Can't find a bucket corresponding to this element's key"s;

        Bucket &bucket = buckets[bucketMap[key]];
        if (!bucket.isFull())
        {
            bucket.insert(element);
            if (debug)
            {
                cout << "[INFO] Adding element to already exisiting bucket.\n";
                display();
            }
            return;
        }

        if (bucket.getDepth() < depth)
        {
            if (debug)
                cout << "[INFO] Spliting a bucket.\n";

            Bucket newBucket = bucket.split();

            string targetKey = "1" + getLSBBinaryDigits(element, bucket.getDepth() - 1);
            cout << bucket.getDepth() << " " << targetKey << " " << depth << "\n";
            for (int i = 0; i < (1 << depth); i++)
            {
                string key = getLSBBinaryDigits(i, depth);
                if (hasSuffix(key, targetKey))
                    bucketMap[key] = bucketIndex;
            }
            bucketIndex++;
            buckets.push_back(newBucket);

            insert(element);
            return;
        }

        increaseDepth();
        insert(element);
        return;
    }

    // Checks if the bucket corresponding to a directory key can be merged with any other bucket, and performs the merge if possible
    void checkForMerge(string key)
    {
        Bucket &bucket = buckets[bucketMap[key]];

        // Check if the bucket can be collapsed with another
        string bucketKey = suffix(key, bucket.getDepth());
        string bucketCompKey = getComplementaryKey(bucketKey);
        string accessCompKey = string(depth - bucketCompKey.size(), '0') + bucketCompKey;

        if (buckets[bucketMap[accessCompKey]].getSize() + bucket.getSize() > bucketCapacity)
            return;

        if (debug)
            cout << "[INFO] Merging two buckets togther.\n";

        bucket.merge(buckets[bucketMap[accessCompKey]]);
        for (int i = 0; i < (1 << depth); i++)
        {
            string newKey = getLSBBinaryDigits(i, depth);
            if (suffix(newKey, bucket.getDepth()) == suffix(key, bucket.getDepth()))
                bucketMap[newKey] = bucketMap[key];
        }

        checkForMerge(key);
        decrementDepth();
    }

    // Removes an element from the global directory by deleting it from a bucket if found
    void remove(int element)
    {
        string key = getLSBBinaryDigits(element, depth);
        if (bucketMap.find(key) == bucketMap.end())
            throw "Can't find a bucket corresponding to this element's key"s;

        Bucket &bucket = buckets[bucketMap[key]];
        bucket.remove(element);

        cout << "[SUCCESS] Removed the element successfully from a bucket.\n";

        checkForMerge(key);

        // Display the rearranged structure in the debug mode
        if (debug)
            display();

        return;
    }

    // Increments the depth of the global directory by 1
    void increaseDepth()
    {

        if (debug)
            cout << "[INFO] Increasing the global depth of the directory.\n";

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

    // Decrements the depth of the global directory by 1 if possible
    void decrementDepth()
    {
        // For the decrement to be possible, the depths of all the buckets must be strictly
        for (int i = 0; i < (1 << depth); i++)
            if (depth == buckets[bucketMap[getLSBBinaryDigits(i, depth)]].getDepth())
                return;

        if (debug)
            cout << "[INFO] Increasing the global depth of the directory.\n";

        for (int i = 0; i < (1 << depth); i++)
        {
            string key = getLSBBinaryDigits(i, depth);
            string newKey = getLSBBinaryDigits(i, depth - 1);
            bucketMap[newKey] = bucketMap[key];
            bucketMap.erase(key);
        }
        depth--;
    }

    // Prints the content of the directory to the STDOUT in a formatted manner
    void display()
    {
        cout << "\n[LOG] Displaying the directory content\n\n";
        cout << "Global depth: " << depth << "\n";
        printLine();
        cout << "Directory Identifier ---> Bucket Identifier (Local Depth) ---> Elements\n";
        printLine();

        map<int, vector<string>> mappings;
        for (auto &[key, bucketIndex] : bucketMap)
            mappings[bucketIndex].push_back(key);

        for (auto &[bucketIndex, keys] : mappings)
        {
            printKeys(keys);

            string key = keys[0];
            int localDepth = buckets[bucketIndex].getDepth();
            string bucketKey = key.substr(key.size() - localDepth);

            cout << " ---> '" << bucketKey << "' (" << localDepth << ") ---> ";
            buckets[bucketIndex].display();
        }
        printLine();
        cout << "\n";
    }

    // Enables the debugging output
    void startDebugging()
    {
        debug = 1;
    }

    // Disables the debugging output
    void stopDebugging()
    {
        debug = 0;
    }
};

// A wrapper class that exposes the functionalities and REPL for managing the directory and buckets.
class Hash
{
private:
    int bucketCapacity;
    Directory directory;
    string demarcator = ">> ";

    // Shows help and gives an overview of the suported commands
    void showHelp()
    {
        cout << "Hi! This is a case-sensitive hasher that supports the following operations: \n";
        cout << "I X\t| Used to insert the value 'X' into the directory\n";
        cout << "D X\t| Used to delete the value 'X' from the directory\n";
        cout << "\nPlease note that all the commands are CASE-SENSITIVE in nature.\n";
    }

    // Utility function to check if a string can be converted to a number
    bool isStringConvertibleToNumber(const string &str)
    {
        istringstream iss(str);
        double number;

        if (iss >> number)
            return true;
        else
            return false;
    }

public:
    Hash(int capacity)
    {
        bucketCapacity = capacity;
        directory = Directory(bucketCapacity);
    }

    // Enables the debugging output
    void startDebugging()
    {
        directory.startDebugging();
    }

    // Stop the debugging output
    void stopDebugging()
    {
        directory.stopDebugging();
    }

    // Start an interactive terminal
    void REPL()
    {
        showHelp();
        vector<string> words;

        while (true)
        {
            cout << demarcator;
            words.clear();
            string line;
            getline(std::cin, line);

            // Create a stringstream to tokenize the line
            stringstream ss(line);
            string word;

            // Tokenize the line and store words in a vector
            while (ss >> word)
                words.push_back(word);

            if (words[0] == "I")
            {
                if (words.size() != 2)
                {
                    cout << "[ERROR] The insert command expects one argument: value to added. You supplied " << words.size() - 1 << ".\n";
                    continue;
                }
                if (!isStringConvertibleToNumber(words[1]))
                {
                    cout << "[ERROR] The valiue supplied is not a valid number.\n";
                    continue;
                }
                directory.insert(stoi(words[1]));
            }
            else if (words[0] == "D")
            {
                if (words.size() != 2)
                {
                    cout << "[ERROR] The delete command expects one argument: value to be deleted. You supplied " << words.size() - 1 << ".\n";
                    continue;
                }
                if (!isStringConvertibleToNumber(words[1]))
                {
                    cout << "[ERROR] The valiue supplied is not a valid number.\n";
                    continue;
                }
                directory.remove(stoi(words[1]));
            }
            else
                cout << "[ERROR] Unrecognized command: " + words[0] << ".\n";
        }
    }
};

int main()
{
    int bucketCapacity = 4;
    Hash hasher = Hash(bucketCapacity);
    hasher.startDebugging();
    hasher.REPL();
}