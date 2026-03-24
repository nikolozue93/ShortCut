#include <iostream>
#include <map>
#include <queue>
#include <bitset>

using namespace std;

struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

void generateHuffmanCode(Node* root, string code, map<char, string>& huffmanCode) {
    if (root) {
        if (!root->left && !root->right) {
            huffmanCode[root->data] = code;
        }

        generateHuffmanCode(root->left, code + "0", huffmanCode);
        generateHuffmanCode(root->right, code + "1", huffmanCode);
    }
}

map<char, string> buildHuffmanTree(map<char, int>& frequencies) {
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    for (auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* newNode = new Node('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    Node* root = nullptr;
    if (!pq.empty()) {
        root = pq.top();
    }

    map<char, string> huffmanCode;
    generateHuffmanCode(root, "", huffmanCode);

    return huffmanCode;
}

void displayHuffmanCodes(const map<char, string>& huffmanCode) {
    cout << "Huffman Codes:\n";
    for (const auto& pair : huffmanCode) {
        cout << pair.first << ": " << pair.second << "\n";
    }
    cout << "\n";
}

string encodeText(const string& text, const map<char, string>& huffmanCode) {
    string encodedText = "";
    for (char ch : text) {
        auto it = huffmanCode.find(ch);
        if (it != huffmanCode.end()) {
            encodedText += it->second;
        } else {
            cerr << "Error: Huffman code not found for character '" << ch << "'\n";
            // Handle the error as needed
        }
    }
    return encodedText;
}

string decodeText(const string& encodedText, const map<string, char>& reverseHuffmanCode) {
    string decodedText = "";
    string currentCode = "";
    for (size_t i = 0; i < encodedText.size(); ++i) {
        currentCode += encodedText[i];
        auto it = reverseHuffmanCode.find(currentCode);
        if (it != reverseHuffmanCode.end()) {
            decodedText += it->second;
            currentCode = "";
        }
    }
    return decodedText;
}

int main() {
    string originalText;

    cout << "Enter the original text: ";
    getline(cin, originalText);

    map<char, int> frequencies;
    for (char ch : originalText) {
        frequencies[ch]++;
    }

    map<char, string> huffmanCode = buildHuffmanTree(frequencies);
    displayHuffmanCodes(huffmanCode);

    string encodedText = encodeText(originalText, huffmanCode);
    cout << "Original Text: " << originalText << "\n";
    cout << "Encoded Text: " << encodedText << "\n";

    // Optionally, let the user choose to decode the text
    char choice;
    cout << "Do you want to decode the text? (y/n): ";
    cin >> choice;

    if (tolower(choice) == 'y') {
        map<string, char> reverseHuffmanCode;
        for (const auto& pair : huffmanCode) {
            reverseHuffmanCode[pair.second] = pair.first;
        }

        string decodedText = decodeText(encodedText, reverseHuffmanCode);
        cout << "Decoded Text: " << decodedText << "\n";
    }

    return 0;
}
