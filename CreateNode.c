#include "node.h"
#include <stdio.h>
#include <stdlib.h>

t_node * createNode(int value) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    for (int i=0; i < 3; i++) {
        node->tab[i]= NULL;
    }
    return node;
}

t_tree createTree(t_node * root) {
    t_tree t;
    t.root = root;
    return t;
}

void displayTree(t_tree t) {
    if (t.root != NULL) {
        printf("");
    }
    else {
        printf(t.root->value);
        for(int i=0; i < sizeof(t.root->tab); i++) {
            t_tree tNew = createTree(t.root->tab[i]);
            displayTree(tNew);
        }
    }
}

t_tree createBigTree(int possibilities) {

}