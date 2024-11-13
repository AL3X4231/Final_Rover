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
            node->sons[i] = createNode(2,height);

            int new_tab[height-1];
            for (int j=0; j < height-1; j++) {
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
            createSons(node->sons[i],height-1, new_tab);
        }
    }

}
void displayTree(t_node* node, int level) {
    if (node == NULL) {
        return;
    }

    // Affiche le nœud actuel avec une indentation selon son niveau
    for (int i = 0; i < level; i++) {
        printf("  ");  // Indentation pour chaque niveau
    }
    printf("Node value: %d, Number of sons: %d\n", node->value, node->nb_sons);

    // Parcours récursif des fils du nœud actuel
    for (int i = 0; i < node->nb_sons; i++) {
        displayTree(node->sons[i], level + 1);
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

