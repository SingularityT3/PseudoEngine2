#include <sstream>
#include "nodes/base.h"
#include "psc/types.h"

Node::Node(const Token &token)
    : token(token)
{}

std::string Node::toStr() const {
    std::stringstream ss;
    ss << "Node{Token: " << token << "}";
    return ss.str();
}

const Token &Node::getToken() {
    return token;
}


UnaryNode::UnaryNode(const Token &token, Node &node)
    : Node(token), node(node)
{}

std::string UnaryNode::toStr() const {
    std::stringstream ss;
    ss << "UnaryNode{Token: (" << token << "), Node: (" << node.toStr() << ")}";
    return ss.str();
} 


BinaryNode::BinaryNode(const Token &token, Node &left, Node &right)
    : Node(token), left(left), right(right)
{}

std::string BinaryNode::toStr() const {
    std::stringstream ss;
    ss << "BinaryNode{Token: (" << token << "), leftNode: (" << left.toStr() << "), rightNode: (" << right.toStr() << ")}";
    return ss.str();
} 
