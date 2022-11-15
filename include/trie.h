#ifndef TRIE_H
#define TRIE_H

#include <vector>     // for std::vector<Node*>
#include <string>     // for insert and search
#include <iostream>   // used during debugging, may be removed
#include <functional> // for std::function

class Trie
{
    public:
    /* Constructors, operators, and destructor*/
    Trie();
    Trie(std::initializer_list<std::string> list);
    Trie(const Trie& trie);
    Trie(Trie&& trie);
    void operator=(const Trie& trie);
    void operator=(Trie&& trie);
    ~Trie();

    /* Member functions and variables*/
    void insert(std::string str);
    bool search(std::string query);
    class Node 
    {
        public:
        Node(char data, bool is_finished)
        {
            this->data = data;
            this->is_finished = is_finished;
        }
        
        std::vector<Node*> children;
        char data;
        bool is_finished;

    };

    Node* root {};
    void bfs(std::function<void(Node*& node)> func);
};


#endif