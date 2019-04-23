#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>

//A node in our tree
struct Node
{
    char character;
    int frequency;
    Node *left, *right;
};

class huffman_tree {
	public:
		Node* theRoot;
        huffman_tree(const std::string &file_name);
		~huffman_tree();
		
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	
};

#endif