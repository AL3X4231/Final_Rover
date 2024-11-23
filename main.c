#include <stdio.h>
#include "map.h"
#include "node.h"
#include "moves.h"
#include "loc.h"
#define INT_MAX 2147483647

int main() {
    t_map map;

    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile(".\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++){
        for (int j = 0; j < map.x_max; j++){
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++){
        for (int j = 0; j < map.x_max; j++){
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);


    //localisation de base
    t_localisation startLoc = defineRobotPosition(map);
    displayMapRobot( map, startLoc.pos);

    // tests arbre
    int nb_moves = 9;  // Number of unique moves we want
    int nb_choices = 5;  // Number of moves to consider for each path
    int level = 0;  // Start at level 0 for the root node
    
    // First execution
    t_move* moves = chooseMove(nb_moves);
    
    // Verify moves are unique
    printf("\nAvailable moves for this execution:\n");
    for (int i = 0; i < nb_moves; i++) {
        printf("%d. %s\n", i + 1, t_move_to_string(moves[i]));
    }
    
    t_node* root1 = createNode(1000, level, nb_moves, F_10, startLoc);
    createSons(root1, nb_moves, nb_choices, moves, startLoc, map, level);

    //printf("\nArbre associe (chemin prefix) :\n");
    //displayTree(root1, nb_choices);
    //printf("\n");

    t_path best_path = {0};
    int min_cost = evaluateTree(root1, &best_path);

    if (min_cost == INT_MAX) {
        printf("No valid path to base found.\n");
    } else {
        printf("\nOptimal path to base (total cost: %d):\n", min_cost);
        for (int i = 0; i < best_path.num_moves; i++) {
            t_localisation newLoc = move(startLoc, best_path.moves[i]);
            int move_cost = map.costs[newLoc.pos.y][newLoc.pos.x];
            printf("%d. %s (cost: %d)\n", i+1, t_move_to_string(best_path.moves[i]), move_cost);
            startLoc = newLoc;
        }
    }

    t_node* lowest_cost_node = minCost(root1);
    if (lowest_cost_node != NULL) {
        printf("Node with lowest cost found:\n");
        printf("Position: (%d, %d)\n", lowest_cost_node->loc.pos.x, lowest_cost_node->loc.pos.y);
        printf("Cost: %d\n", lowest_cost_node->value);
        
        // Display the path to this node
        printf("Path to this node: ");
        for (int i = 0; i < 5; i++) {
            if (lowest_cost_node->movements[i] != F_10) {  // F_10 is our sentinel value
                printf("%s -> ", t_move_to_string(lowest_cost_node->movements[i]));
            }
        }
        printf("Last move: %s\n", t_move_to_string(lowest_cost_node->move));
    }
    startLoc = lowest_cost_node->loc;

    // Second execution
    moves = chooseMove(nb_moves);
    
    // Display available moves
    printf("\nAvailable moves for this execution:\n");
    for (int i = 0; i < nb_moves; i++) {
        printf("%d. %s\n", i + 1, t_move_to_string(moves[i]));
    }
    
    root1 = createNode(1000, level, nb_moves, F_10, startLoc);
    createSons(root1, nb_moves, nb_choices, moves, startLoc, map, level);

    //printf("\nArbre associe (chemin prefix) :\n");
    // displayTree(root1, nb_choices);
    //printf("\n");

    best_path = (t_path){0};
    min_cost = evaluateTree(root1, &best_path);

    if (min_cost == INT_MAX) {
        printf("No valid path to base found.\n");
    } else {
        printf("\nOptimal path to base (total cost: %d):\n", min_cost);
        for (int i = 0; i < best_path.num_moves; i++) {
            t_localisation newLoc = move(startLoc, best_path.moves[i]);
            int move_cost = map.costs[newLoc.pos.y][newLoc.pos.x];
            printf("%d. %s (cost: %d)\n", i+1, t_move_to_string(best_path.moves[i]), move_cost);
            startLoc = newLoc;
        }
    }

    lowest_cost_node = minCost(root1);
    if (lowest_cost_node != NULL) {
        printf("Node with lowest cost found:\n");
        printf("Position: (%d, %d)\n", lowest_cost_node->loc.pos.x, lowest_cost_node->loc.pos.y);
        printf("Cost: %d\n", lowest_cost_node->value);
        
        // Display the path to this node
        printf("Path to this node: ");
        for (int i = 0; i < 5; i++) {
            if (lowest_cost_node->movements[i] != F_10) {  // F_10 is our sentinel value
                printf("%s -> ", t_move_to_string(lowest_cost_node->movements[i]));
            }
        }
        printf("Last move: %s\n", t_move_to_string(lowest_cost_node->move));
    }
    startLoc = lowest_cost_node->loc;

    return 0;
}