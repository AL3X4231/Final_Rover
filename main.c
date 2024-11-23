#include <stdio.h>
#include "map.h"
#include "node.h"
#include "moves.h"
#include "loc.h"
#define INT_MAX 2147483647
#include <stdlib.h>

int main() {
    // creation of the map using predefine patern in the file "example1.map"
    t_map map= createMapFromFile(".\\maps\\example1.map");

    // definition of the random localisation of the rover
    t_localisation startLoc = defineRobotPosition(map);
    displayMapRobot( map, startLoc.pos);

    // try to reach the base station with nb_choices among nb_moves
    int nb_moves = 5;  // Number of unique moves we want
    int nb_choices = 4;  // Number of moves to consider for each path

    
    // First execution
    t_move* moves = chooseMove(nb_moves);
    
    // Print moves
    printf("\nAvailable moves for this execution:\n");
    for (int i = 0; i < nb_moves; i++) {
        printf("%d. %s\n", i + 1, t_move_to_string(moves[i]));
    }

    //creation of the tree's root
    t_node* root1 = createNode(1000, 0, nb_moves, F_10, startLoc);
    //creation of the tree (recursive function that creates all the sons and the sons of the sons and so on)
    createSons(root1, nb_moves, nb_choices, moves, startLoc, map, 0);

    t_path best_path = {0};
    t_node* min_cost_node = minCost(root1);
    int min_cost = min_cost_node->value;

    // find the best path to reach the base station with respect to the cost
    printf("\nOptimal path to base (total cost: %d):\n", min_cost);
    for (int i = 0; i < best_path.num_moves; i++) {
        t_localisation newLoc = move(startLoc, best_path.moves[i]);
        int move_cost = map.costs[newLoc.pos.y][newLoc.pos.x];
        printf("%d. %s (cost: %d)\n", i+1, t_move_to_string(best_path.moves[i]), move_cost);
        startLoc = newLoc;
    }

    // define the node that has the lowest cost possible from the previous try
    t_node* lowest_cost_node = minCost(root1);
    // if the rover doesn't reach the base yet, do it again with new movements
    if (lowest_cost_node != NULL) {
        printf("Node with lowest cost found:\n");
        printf("Position: (%d, %d)\n", lowest_cost_node->loc.pos.x, lowest_cost_node->loc.pos.y);
        printf("Cost: %d\n", lowest_cost_node->value);
        
        // Display the complete path including the first move
        displayNodePath(lowest_cost_node);
        
        // Display map with new position
        displayMapRobot(map, lowest_cost_node->loc.pos);
    }

    // define the new localisation for the new try
    t_localisation newLoc = lowest_cost_node->loc;
    t_node* current_node = lowest_cost_node;

    // while the rover didn't reach the base station :
    while (current_node != NULL && current_node->value != 0) {
        // Free previous moves and generate new ones
        free(moves);
        moves = chooseMove(nb_moves);
        
        // Print moves
        printf("\nAvailable moves for this execution:\n");
        for (int i = 0; i < nb_moves; i++) {
            printf("%d. %s\n", i + 1, t_move_to_string(moves[i]));
        }
        
        // Create new tree from current position
        root1 = createNode(current_node->value, 0, nb_moves, F_10, current_node->loc);
        createSons(root1, nb_moves, nb_choices, moves, current_node->loc, map, 0);
        
        // Find next lowest cost node
        current_node = minCost(root1);

        if (current_node != NULL) {
            printf("Node with lowest cost found:\n");
            printf("Position: (%d, %d)\n", current_node->loc.pos.x, current_node->loc.pos.y);
            printf("Cost: %d\n", current_node->value);
            displayNodePath(current_node);

            newLoc = current_node->loc;
            displayMapRobot(map, newLoc.pos);
        }
    }

    // Free final moves array before exit
    free(moves);

    // the rover is arrived hopefully :)
    printf("\nALERT : The Rover is arriving in the base station ! Well Played !\n");

    return 0;
}