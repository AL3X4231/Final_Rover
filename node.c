#include "node.h"
#include "moves.h"
#include "map.h"
#include "loc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647

t_node * createNode(int value, int level, int nb_sons, t_move move, t_localisation loc) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->nb_sons = nb_sons;
    node->loc = loc;
    node->move = move;
    
    // Initialize movements array with the current move
    for (int i = 0; i < 5; i++) {
        if (i == level) {
            node->movements[i] = move;
        } else {
            node->movements[i] = STAY;  // Use STAY as sentinel value
        }
    }
    
    // Initialize sons array
    for (int i = 0; i < 9; i++) {
        node->sons[i] = NULL;
    }
    
    return node;
}

void createSons(t_node* node, int nb_moves, int nb_choices, t_move moves[], t_localisation loc, t_map map, int level) {
    if (level >= nb_choices) {
        return;
    }
    
    for (int i = 0; i < nb_moves; i++) {
        t_localisation newLoc = move(loc, moves[i]);
        if (isValidLocalisation(newLoc.pos, map.x_max, map.y_max)) {
            t_node* son = createNode(map.costs[newLoc.pos.y][newLoc.pos.x], 
                                   level + 1, 
                                   nb_moves, 
                                   moves[i], 
                                   newLoc);
            
            // Copy parent's movement history
            for (int j = 0; j < level; j++) {
                son->movements[j] = node->movements[j];
            }
            // Add current move
            son->movements[level] = moves[i];
            
            node->sons[i] = son;
            createSons(son, nb_moves, nb_choices, moves, newLoc, map, level + 1);
        }
    }
}

void displayTree(t_node* root, int level) {
    if (root == NULL) {
        return;
    }
    
    // Print indentation
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    for (int i = 0; i < root->nb_sons; i++) {
        displayTree(root->sons[i], level + 1);
    }
}

t_node* minCost(t_node* root) {
    if (root == NULL) {
        return NULL;
    }
    
    // Check if current position is valid (not a crevasse and within bounds)
    if (root->value >= 10000) {
        return NULL;
    }
    
    // Start with the current node as minimum
    t_node* min_node = root;
    int min_cost = root->value;
    
    // Check all children
    for (int i = 0; i < root->nb_sons; i++) {
        t_node* son_min = minCost(root->sons[i]);
        
        if (son_min != NULL && son_min->value < min_cost) {
            min_node = son_min;
            min_cost = son_min->value;
        }
    }
    
    return min_node;
}

void displayNodePath(t_node* node, t_map map) {
    printf("Path to this node: ");
    for (int i = 0; i < 4; i++) {
        if (node->movements[i] != STAY) {
            printf("%s -> ", t_move_to_string(node->movements[i]));
        }
    }
    t_position basePos = getBaseStationPosition(map);
    if (node->loc.pos.x == basePos.x && node->loc.pos.y == basePos.y) {
        printf("BASE");
    } else {
        printf("%s", t_move_to_string(node->movements[4]));
    }
    printf("\n");
}