#include <stdio.h>
#include "map.h"
#include "node.h"
#include "moves.h"
#include "loc.h"
#define INT_MAX 2147483647
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void printDayHeader(int day) {
    printf("\n");
    printf("+----------------------------------------+\n");
    printf("|              MARS DAY %-3d              |\n", day);
    printf("+----------------------------------------+\n\n");
}

void printSection(const char* title) {
    printf("\n=== %s ===\n", title);
}

void printMovesList(t_move* moves, int nb_moves) {
    printf("Available moves:\n");
    for (int i = 0; i < nb_moves; i++) {
        printf("  %d. %s\n", i + 1, t_move_to_string(moves[i]));
    }
    printf("\n");
}

void printMissionComplete(int day) {
    printf("\n");
    printf("+------------------------------------------------+\n");
    printf("|                MISSION COMPLETE!                 |\n");
    printf("|    The rover has successfully reached the base  |\n");
    printf("|             Total days on Mars: %-3d            |\n", day);
    printf("+------------------------------------------------+\n");
}

int main() {
    // Display available maps
    printf("Available maps:\n");
    DIR *d;
    struct dirent *dir;
    int map_count = 0;
    char map_files[100][256];
    
    d = opendir(".\\maps");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".map") != NULL) {
                printf("%d. %s\n", map_count + 1, dir->d_name);
                strcpy(map_files[map_count], dir->d_name);
                map_count++;
            }
        }
        closedir(d);
    }
    
    //Get user choice
    int choice;
    do {
        printf("\nChoose a map (1-%d): ", map_count);
        scanf("%d", &choice);
    } while (choice < 1 || choice > map_count);
    
    // Create map path and load selected map
    char map_path[300] = ".\\maps\\";
    strcat(map_path, map_files[choice-1]);
    t_map map = createMapFromFile(map_path);
    t_localisation startLoc = defineRobotPosition(map);
    int day = 1;
    int nb_moves = 5;
    int nb_choices = 4;

    // Initial position
    printDayHeader(day);
    printf("Mission Start: Rover has landed!\n");
    printf("Initial Position: (%d, %d)\n", startLoc.pos.x, startLoc.pos.y);
    printf("Initial Orientation: %s\n", 
           startLoc.ori == NORTH ? "NORTH" : 
           startLoc.ori == SOUTH ? "SOUTH" : 
           startLoc.ori == EAST ? "EAST" : "WEST");
    
    displayMapRobot(map, startLoc.pos, startLoc.ori);
    
    t_move* moves = chooseMove(nb_moves);
    t_node* current_node = NULL;
    t_node* root1 = NULL;
    
    do {
        if (current_node != NULL) {
            day++;
            printDayHeader(day);
        }
        
        // Free previous moves and generate new ones
        if (current_node != NULL) free(moves);
        moves = chooseMove(nb_moves);
        printMovesList(moves, nb_moves);
        
        // Create new tree from current position
        root1 = createNode(current_node ? current_node->value : 1000, 
                          0, nb_moves, F_10, 
                          current_node ? current_node->loc : startLoc);
        createSons(root1, nb_moves, nb_choices, moves, 
                  current_node ? current_node->loc : startLoc, map, 0);
        
        // Find next lowest cost node
        current_node = minCost(root1);

        if (current_node != NULL) {
            printSection("ROVER STATUS");
            printf("Position: (%d, %d)\n", current_node->loc.pos.x, current_node->loc.pos.y);
            printf("Distance to base: %d units\n", current_node->value);
            
            printSection("MOVEMENT PATH");
            displayNodePath(current_node, map);
            
            printSection("CURRENT MAP");
            displayMapRobot(map, current_node->loc.pos, current_node->loc.ori);
        }
        
    } while (current_node != NULL && current_node->value != 0);

    // Free final moves array
    free(moves);

    // Mission complete message
    printMissionComplete(day);

    return 0;
}