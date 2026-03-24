#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <bitset>

using namespace std;

struct Node
{
    char data;
    int frequency;
    Node *left;
    Node *right;

    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes
{
    bool operator()(Node *a, Node *b)
    {
        return a->frequency > b->frequency;
    }
};

void generateHuffmanCode(Node *root, string code, map<char, string> &huffmanCode)
{
    if (root)
    {
        if (!root->left && !root->right)
        {
            huffmanCode[root->data] = code;
        }

        generateHuffmanCode(root->left, code + "0", huffmanCode);
        generateHuffmanCode(root->right, code + "1", huffmanCode);
    }
}

map<char, string> buildHuffmanTree(map<char, int> &frequencies)
{
    priority_queue<Node *, vector<Node *>, CompareNodes> pq;

    for (auto &pair : frequencies)
    {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *newNode = new Node('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    Node *root = nullptr;
    if (!pq.empty())
    {
        root = pq.top();
    }

    map<char, string> huffmanCode;
    generateHuffmanCode(root, "", huffmanCode);

    return huffmanCode;
}

void saveHuffmanCodes(const map<char, string> &huffmanCode, const string &fileName)
{
    ofstream outputFile(fileName);
    for (const auto &pair : huffmanCode)
    {
        outputFile << pair.first << ": " << pair.second << "\n";
    }
    outputFile.close();
}

string encodeText(const string &text, const map<char, string> &huffmanCode)
{
    string encodedText = "";
    for (char ch : text)
    {
        auto it = huffmanCode.find(ch);
        if (it != huffmanCode.end())
        {
            encodedText += it->second;
        }
        else
        {
            cerr << "Error: Huffman code not found for character '" << ch << "'\n";
            // Handle the error as needed
        }
    }
    return encodedText;
}

void saveTextToFile(const string &text, const string &fileName)
{
    ofstream outputFile(fileName);
    outputFile << text;
    outputFile.close();
}

int main()
{
    string inputFileName;
    string huffmanCodesFileName = "huffman_codes.txt";
    string originalTextFileName = "original_text.txt";
    string encodedTextFileName = "encoded_text.txt";

    cout << "Enter the input file name: ";
    cin >> inputFileName;

    ifstream inputFile(inputFileName);
    if (!inputFile)
    {
        cerr << "Error: Unable to open input file '" << inputFileName << "'\n";
        return 1;
    }

    // Read the original text from the file
    string originalText((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Count frequencies only for characters present in the text
    map<char, int> frequencies;
    for (char ch : originalText)
    {
        frequencies[ch]++;
    }

    // Build Huffman codes and save them to a file
    map<char, string> huffmanCode = buildHuffmanTree(frequencies);
    saveHuffmanCodes(huffmanCode, huffmanCodesFileName);

    // Encode the original text and save it to a file
    string encodedText = encodeText(originalText, huffmanCode);
    saveTextToFile(originalText, originalTextFileName);
    saveTextToFile(encodedText, encodedTextFileName);

    cout << "Huffman codes saved to '" << huffmanCodesFileName << "'\n";
    cout << "Original text saved to '" << originalTextFileName << "'\n";
    cout << "Encoded text saved to '" << encodedTextFileName << "'\n";

    return 0;
}
