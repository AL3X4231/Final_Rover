#include "node.h"
#include "moves.h"
#include "map.h"
#include "loc.h"
#include <stdio.h>
#include <stdlib.h>

t_node * createNode(int value, int nb_sons, t_move move, t_localisation loc) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->nb_sons = nb_sons;
    node->move = move;
    node->loc = loc;
    for (int i=0; i < nb_sons; i++) {
        node->sons[i]= NULL;
    }
    return node;
}

void createSons(t_node* node, int nb_moves, int nb_choices, t_move moves[], t_localisation loc, t_map map) {
    if (nb_choices == 0) {
        return;
    } else {
        for (int i=0; i < nb_moves; i++) {
            // value doit etre remplacée par le cost
            int cost;
            t_localisation newLoc = move(loc, moves[i]); //focntion de newloc

            //test si le robot est toujours sur la carte
            if(isValidPosition(newLoc, map.x_max,  map.y_max)){
                cost = map.costs[newLoc.pos.x][newLoc.pos.y];
            }
            else {
                printf("The robot jumps  of the map !?!");
                cost = 1000;
            }
            node->sons[i] = createNode(0,nb_moves-1, moves[i], newLoc); ///rajouter soil

            t_move new_tab[nb_moves-1];
            // Construire un tableau sans le mouvement actuel
            int index = 0;
            for (int j = 0; j < nb_moves; j++) {
                if (j != i) {
                    new_tab[index++] = moves[j];
                }
            }
            createSons(node->sons[i],nb_moves-1,nb_choices-1, new_tab, newLoc, map);
        }
    }
}


void displayTree(t_node* root, int level) {
    if (root == NULL) {
        return;
    }
    // print la root
    printf("Node cost: %d, Number of sons: %d , Movement: %s \n", root->value, root->nb_sons, t_move_to_string(root->move));

    for (int i = 0; i < level; i++) {
        printf("  ");  // Indentation pour chaque niveau dans l'arbre
    }

    for (int i = 0; i < root->nb_sons; i++) {
        displayTree(root->sons[i], level + 1);
    }
}

