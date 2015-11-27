#ifndef _llrb_h
#define _llrb_h

#include <iostream>
#include <cassert>

#ifndef nullptr
#define nullptr 0
#endif

#define GRAPHVIZ		// printing

enum Color { RED, BLACK };

class LLRB;

class Node {
     friend class LLRB;
private:
     int key;
     int count;
     Color color;
     Node *left, *right;
     Node *next;		// for printing;
     int level;			// for printing;
public:
     Node(int k, Color c): key(k), count(1), color(c),
			  left(nullptr), right(nullptr) {}
     ~Node() {
	  delete left;
	  delete right;
     }

     friend std::ostream& operator<<(std::ostream &out, LLRB &tree);
};

class LLRB {
private:
     Node* root;
     Node* left_rotate(Node *node);
     Node* right_rotate(Node *node);
     void color_flip(Node *node);
     Node* insert(Node *node, int k);
public:
     LLRB(): root(nullptr) {}
     ~LLRB() {
	  delete root;
     }
     
     bool find(int k) const;

     void insert(int k);
     bool remove(int k);

     friend std::ostream& operator<<(std::ostream &out, LLRB &tree);
};

// private
Node* LLRB::left_rotate(Node *node)
{
     assert(node != nullptr);
     assert(node->right != nullptr);

     // maintain color _links_
     Color temp_color = node->color;
     node->color = node->right->color;
     node->right->color = temp_color;

     Node *temp = node->right;
     node->right = node->right->left;
     temp->left = node;
     
     return temp;
}

Node* LLRB::right_rotate(Node *node)
{
     assert(node != nullptr);
     assert(node->left != nullptr);

     // maintain color _links_
     Color temp_color = node->color;
     node->color = node->left->color;
     node->left->color = temp_color;

     Node *temp = node->left;
     node->left = node->left->right;
     temp->right = node;
     
     return temp;
}

void LLRB::color_flip(Node *node)
{
     // when a node has two red links to children
     assert(node != nullptr && node->color == BLACK);
     assert(node->left != nullptr && node->left->color == RED);
     assert(node->right != nullptr && node->right->color == RED);
     
     node->color = RED;
     node->left->color = BLACK;
     node->right->color = BLACK;
}

Node* LLRB::insert(Node *node, int k)
{
     if (node->key == k)
	  ++node->count;
     else {
	  if (node->key > k) {
               // insert into left subtree
	       if (node->left == nullptr)
		    node->left = new Node(k, RED);
	       else
		    node->left = insert(node->left, k);
	       
	       if (node->left->left != nullptr &&
		   node->left->color == RED && node->left->left->color == RED) {
		    node = right_rotate(node);
		    color_flip(node);
	       }
	  } else {
               // insert into right subtree
	       if (node->right == nullptr) {
		    node->right = new Node(k, RED);
	       } else
		    node->right = insert(node->right, k);
	       
	       if (node->right->color == RED) {
		    if (node->left != nullptr && node->left->color == RED)
			 color_flip(node);
		    else
			 node = left_rotate(node);
	       }
	  }
     }
     return node;
}

// public
bool LLRB::find(int k) const
{
     Node *curr = root;
     // top-down, no recursion
     while (curr != nullptr) {
	  if (curr->key == k)
	       return (curr->count > 0);
	  else if (curr->key > k)
	       curr = curr->left;
	  else
	       curr = curr->right;
     }
     if (curr == nullptr)
	  return false;
}

void LLRB::insert(int k)
{
     if (root == nullptr)
	  root = new Node(k, BLACK);
     else {
	  root = insert(root, k);
	  root->color = BLACK;
     }
}

// an extremely lazy deletion
bool LLRB::remove(int k)
{
     Node *curr = root;
     while (curr != nullptr) {
	  if (curr->key == k) {
	       if (curr->count > 0) {
		    // so far only decrease count
		    --curr->count;
		    return true;
	       } else
		    return false;
	  }
	  else if (curr->key > k)
	       curr = curr->left;
	  else
	       curr = curr->right;
     }
     if (curr == nullptr)
	  return false;
}

std::ostream& operator<<(std::ostream &out, LLRB &tree)
{
     // level-order traversal
     if (tree.root != nullptr) {
	  Node *head, *tail;
	  head = tail = tree.root;
	  head->level = 0;
	  head->next = nullptr;
	  int level = 0;
#ifdef GRAPHVIZ
	  out << "strict graph {" << std::endl;
	  out << "\tnode [shape=circle];" << std::endl;
	  out << "\t" << head->key << ";" << std::endl;
#endif
	  while (head != nullptr) {
	       if (head->left != nullptr) {
		    tail->next = head->left;
		    tail = tail->next;
		    tail->next = nullptr;
		    tail->level = head->level + 1;
#ifdef GRAPHVIZ
		    out << "\t" << tail->key << ";" << std::endl;
		    out << "\t" << head->key << "--" << tail->key << " ["
			<< (tail->color == RED ? "color=red, " : "")
			<< "side=left]" << std::endl;
#endif
	       }
	       if (head->right != nullptr) {
		    tail->next = head->right;
		    tail = tail->next;
		    tail->next = nullptr;
		    tail->level = head->level + 1;
#ifdef GRAPHVIZ
		    out << "\t" << tail->key << ";" << std::endl;
		    out << "\t" << head->key << "--" << tail->key << " ["
			<< (tail->color == RED ? "color=red, " : "")
			<< "side=right]" << std::endl;
#endif
	       }
#ifndef GRAPHVIZ
	       if (head->level > level) {
		    out << std::endl;
		    level = head->level;
	       }

	       out << head->key << ":"
		   << (head->color == RED ? "RED" : "BLACK");
	       out << "  ";
#endif
	       head = head->next;
	  }

#ifdef GRAPHVIZ
	  out << "}" << std::endl;
#endif
     }
     
     return out;
}

#endif
