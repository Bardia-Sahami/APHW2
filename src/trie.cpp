#include "trie.h"

Trie::Trie()
{
    root = new Node('\0', true);    
}

Trie::Trie(std::initializer_list<std::string> list)
{
    root = new Node('\0', true);
    for (auto i {list.begin()}; i != list.end(); i++) 
        insert(*i);
}

Trie::Trie(const Trie& trie)
{
    // Create copied root node based on trie's root
    root = new Node {*trie.root};

    // oldNodes vector where trie's nodes gets pushed to
    // oldNode points to one of them based on counter 
    std::vector<Node*> oldNodes;
    oldNodes.push_back(trie.root);
    Node* oldNode {trie.root};

    // Same for the new nodes
    std::vector<Node*> newNodes;
    newNodes.push_back(root);
    Node* newNode {root};
    
    // counter is for newNodes/oldNodes vectors index
    size_t counter {}; 

    while (oldNodes[counter])
    {
        oldNode = oldNodes[counter];
        for (size_t j{}; j < oldNode->children.size(); j++)
        {
            if (!oldNode->children[j]) 
                break;
            newNode->children[j] = new Node {*oldNode->children[j]};
            oldNodes.push_back(oldNode->children[j]);
            newNodes.push_back(oldNode->children[j]);
        }
        counter++;
        newNode = newNodes[counter];
    }
}

Trie::Trie(Trie&& trie)
{
    std::swap(root, trie.root);
}

void Trie::operator=(const Trie& trie)
{
    Trie copy {trie};
    std::swap(root, copy.root);
}

void Trie::operator=(Trie&& trie)
{
    std::swap(root, trie.root);
}

Trie::~Trie()
{
    if(root == nullptr) return;
    std::vector<Node*> nodes;
    this->bfs([&nodes](Trie::Node*& node){nodes.push_back(node);});
    for(const auto& node : nodes)
        delete node;
}

void Trie::insert(std::string str)
{
    Node* temp {root};
    for (size_t i{}; i < str.length(); i++)
    {
        bool found_char {false};

        for (size_t j{}; j < temp->children.size(); j++)
        {
            if (str[i] == temp->children[j]->data)
            {
                temp = temp->children[j]; // if found a character, go inside the node
                found_char = true;
                break; // break, i++, and check the next character under the new found node
            }
        }

        if (!found_char)
        {
            Node* node {new Node(str[i], (i == str.length()-1))}; // make a new node of str[i]
            temp->is_finished = false; // change the current node's is_finished to false as a new node is added under it
            temp->children.push_back(node);
            temp = temp->children[(temp->children.size()-1)]; // go inside of the last node
        }
    }
}

bool Trie::search(std::string query)
{
    Node* temp {root};
    size_t counter {}; // once a character is found, counter++. if in the end, counter == str.length and the last node's is_finished=true, return 'found'.
    for (size_t i{}; i < query.length(); i++)
    {
        bool found_char {false};

        for (size_t j{}; j < temp->children.size(); j++)
        {
            if ((!temp->is_finished) && (query[i] == temp->children[j]->data))
            {
                temp = temp->children[j];
                found_char = true;
                break;
                
            }
        }

        if (!found_char)
        {
            return false;
        } else
        {
            counter++;
        }
    }

    if ((counter == query.length()) && (temp->is_finished))
        return true;

    return false;
}

void Trie::bfs(std::function<void(Node*& node)> func)
{
    Node* temp {root};
    std::vector<Node*> visited{};
    std::vector<Node*> queue{};
    queue.push_back(temp);

    while (queue.size() != 0){
        temp = queue[0]; // queue[0] is going to be visited
        for (size_t i {}; i < temp->children.size(); i++) 
        {
        queue.push_back(temp->children[i]); // push queue[0]'s children to queue
        }
        func(queue[0]);
        visited.push_back(queue[0]); // we are done with queue[0]
        queue.erase(queue.begin()); // delete queue[0], queue[1] will be the new queue[0] and will be checked in the next loop.
    }
}