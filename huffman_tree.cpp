#include "huffman_tree.h"
#include <fstream>
#include <queue>
#include <stack>
#include <map>
#include <string>

using namespace std;

//The comparator for our priority queue
//Sort based on frequency: lower frequency = higher priority
struct comp
{
    bool operator()(Node* leftNode, Node* rightNode)
    {
        return leftNode->frequency > rightNode->frequency;
    }
};

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){
    //Read in the file
    ifstream input(file_name);

    //Create a table with each character and their frequencies
    char character;
    map<char, int> charFrequency;
    while (input.get(character)) {
        charFrequency[character]++;
    }

    //Create the priority queue we will fill with the trees
    priority_queue<Node*, vector<Node*>, comp> priorityQueue;

    //Turn each character into a leaf and put it in the priority queue
    for (auto pair: charFrequency) {

        //Create the new node for the leaf
        Node* node = new Node();
        node->character = pair.first;
        node->frequency = pair.second;
        node->left = nullptr;
        node->right = nullptr;

        //Push the new node into the priority queue
        priorityQueue.push(node);
    }

    //While the priority queue has more than one item
    while (priorityQueue.size() > 1){

        //Remove the two nodes with the smallest weights (frequencies)
        Node *left = priorityQueue.top();
        priorityQueue.pop();
        Node *right = priorityQueue.top();
        priorityQueue.pop();

        //Combine them into a new binary tree in which the weight of the tree is the sum of the weights of its children
        int sum = left->frequency + right->frequency;

        //Insert newly created tree back into priority queue
        Node* node = new Node();
        node->character = '\0';
        node->frequency = sum;
        node->left = left;
        node->right = right;
        priorityQueue.push(node);
    }

    //Store the root of the tree
    theRoot = priorityQueue.top();
}

huffman_tree::~huffman_tree(){

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {

    //Our decoded string
    string code;

    stack<Node*> nodeStack;
    nodeStack.push(theRoot);
    map<Node*, Node*> parent;

    //For single letter
    if(theRoot->character == character){
        code.append("0");
        return code;
    }

    //Traverse the resulting tree to obtain binary codes for characters
    while(!nodeStack.empty()){
        // Pop the top item from stack
        Node* current = nodeStack.top();
        nodeStack.pop();
        //We found a leaf
        if (!(current->left) && !(current->right)){
            //If it's the one we're searching for
            if(current->character == character) {
                //Traverse backwards
                stack<string> stk;
                while (parent[current] != nullptr) {
                    //If this is the character we're looking for
                    if (parent[current]->left == current) {
                        stk.push("0");
                    } else if (parent[current]->right == current) {
                        stk.push("1");
                    }
                    current = parent[current];
                }
                while(!stk.empty()){
                    code.append(stk.top());
                    stk.pop();
                }
                //We can end it now
                return code;
            }
        }
        //Push children to the stack and set their parents
        if (current->right) {
            parent[current->right] = current;
            nodeStack.push(current->right);
        }
        if (current->left) {
            parent[current->left] = current;
            nodeStack.push(current->left);
        }
    }
    return code;
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {

    string output;
    string x;
    ifstream input(file_name);
    if(input.fail()){
        return "";
    }

    char character;
    while (input.get(character)) {
        x = get_character_code(character);
        //cout<<"Charcter: "<<character<<", Code: "<<x<<endl;
        if (x == ""){
            return "";
        }
        output.append(x);
    }
    return output;

}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {

    string output = "";
    Node* current;
    current = theRoot;
    int index = 0;

    //If there is no tree
    if (theRoot == nullptr){
        return "";
    }

    while (string_to_decode[index] != NULL) {
        if (string_to_decode[index] == '0' && current->left) {
            //cout<<string_to_decode[index];
            current = current->left;
        }
        if (string_to_decode[index] == '1' && current->right) {
            //cout<<string_to_decode[index];
            current = current->right;
        }
        index++;
        //If we've reached a leaf
        if (!current->left && !current->right ) {
            output += current->character;
            //cout << " Character: " << current->character << endl;
            //Reset
            current = theRoot;
        }
    }

    return output;
}