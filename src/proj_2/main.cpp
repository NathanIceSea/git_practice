#include "binary_tree.h"
#include <string>
#include <iostream>

using namespace std;

binary_tree<char> constructFromString(string str) {
    binary_tree<char> bTree;
    for (char c : str) {
        bTree.insert(new char(c));
    }
    return bTree;
}

int main() {
    binary_tree<char> bTree = constructFromString(string("hdacfesrkjv"));
    // lambda
    bTree.preOrderedTraverse([](char *c) { cout << *c; });
    cout << endl;
    bTree.inOrderedTraverse([](char *c) { cout << *c; });
    cout << endl;
    bTree.postOrderedTraverse([](char *c) { cout << *c; });
    cout << endl;
    bTree.preOrderedLoop([](char *c) { cout << *c; });
    cout << endl;
    bTree.inOrderedLoop([](char *c) { cout << *c; });
    cout << endl;
    bTree.postOrderedLoop([](char *c) { cout << *c; });
    cout << endl;
    return 0;
}