#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

t_node * createNode(int value, int nb_sons) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->nb_sons = nb_sons;
    //node->sons = (t_node **)malloc(nb_sons * sizeof(t_node *));
    for (int i=0; i < nb_sons; i++) {
        node->sons[i]= NULL;
    }
    return node;
}

void createSons(t_node* node, int height, int moves[]) {
    if (height == 0) {
        return;
    } else {
        for (int i=0; i < height; i++) {
            node->sons[i] = createNode(moves[i], sizeof(moves));
            
            int new_tab[sizeof(moves)-1];
            for (int j=0; j < sizeof(moves)-1; j++) {
                bool test = true;
                if (i==j) {
                    test = false;
                }
                if (test){
                    new_tab[j] = moves[j];
                } else {
                    new_tab[j] = moves[i+1];
                }
            }
            createSons(height-1, new_tab);
        }
    }

}


/*
void displayTree(t_tree t) {
    if (t.root != NULL) {
        printf("");
    }
    else {
        printf(t.root->value);
        for(int i=0; i < sizeof(t.root->sons); i++) {
            t_tree tNew = createTree(t.root->sons[i]);
            displayTree(tNew);
        }
    }
}*/

