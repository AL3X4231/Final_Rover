//
// Created by flasque on 19/10/2024.
//

#include <stdlib.h>
#include "moves.h"
#include <time.h>
#include <stdio.h>
/* prototypes of local functions */
/* local functions are used only in this file, as helper functions */

/**
 * @brief Function to rotate the robot according to a move and its actual orientation
 * @param ori : the actual orientation of the robot
 * @param move : the move to do
 * @return the new orientation of the robot
 */
t_orientation rotate(t_orientation, t_move );

/**
 * @brief function to translate the robot according to a move and its actual position
 * @param loc : the actual localisation of the robot
 * @param move : the move to do
 * @return the new localisation of the robot
 */
t_localisation translate(t_localisation , t_move);

/* definition of local functions */

t_orientation rotate(t_orientation ori, t_move move)
{
    int rst=0;
    switch (move)
    {
        case T_LEFT:
            rst=3;
            break;
        case T_RIGHT:
            rst=1;
            break;
        case U_TURN:
            rst=2;
            break;
        default: // should not happen
            rst=0;
            break;
    }
    return (ori+rst)%4;
}

t_localisation translate(t_localisation loc, t_move move)
{
    /** rules for coordinates:
     *  - x grows to the right with step of +1
     *  - y grows to the bottom with step of +1
     *  - the origin (x=0, y=0) is at the top left corner
     */
    t_position res=loc.pos;
    switch (move) {
        case F_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 1;
                    break;
                case EAST:
                    res.x = loc.pos.x + 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 1;
                    break;
                case WEST:
                    res.x = loc.pos.x - 1;
                    break;
                default:
                    break;
            }
            break;
        case F_20:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 2;
                    break;
                case EAST:
                    res.x = loc.pos.x + 2;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 2;
                    break;
                case WEST:
                    res.x = loc.pos.x - 2;
                    break;
                default:
                    break;
            }
            break;
        case F_30:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 3;
                    break;
                case EAST:
                    res.x = loc.pos.x + 3;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 3;
                    break;
                case WEST:
                    res.x = loc.pos.x - 3;
                    break;
                default:
                    break;
            }
            break;
        case B_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y + 1;
                    break;
                case EAST:
                    res.x = loc.pos.x - 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y - 1;
                    break;
                case WEST:
                    res.x = loc.pos.x + 1;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return loc_init(res.x, res.y, loc.ori);

}

t_localisation move(t_localisation loc, t_move move)
{
    t_localisation new_loc=loc;
    if ((move >=T_LEFT) && (move <= U_TURN))
    {
        new_loc.ori = rotate(loc.ori, move);
    }
    else
    {
        new_loc = translate(loc, move);
    }

    return new_loc;
}

void updateLocalisation(t_localisation *p_loc, t_move m)
{
    *p_loc = move(*p_loc, m);
    return;
}

t_move *getRandomMoves(int N)
{
    int nbmoves[]={22,15,7,7,21,21,7};
    int total_moves=100;
    t_move *moves = (t_move *)malloc(N * sizeof(t_move));
    for (int i = 0; i < N; i++)
    {
        int r = rand() % total_moves;
        int type=0;
        while (r >= nbmoves[type])
        {
            r -= nbmoves[type];
            type++;
        }
        nbmoves[type]--;
        total_moves--;
        moves[i] = (t_move )type;
    }
    return moves;
}

t_move* chooseMove(int nbmove) {
    // Add static counter to ensure different seeds
    static unsigned int counter = 0;
    srand(time(NULL) + counter++ * 7777); // Multiply counter by 1000 for more variation

    t_move* list = malloc(nbmove * sizeof(t_move));
    if (list == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Initialize remaining moves count with initial probabilities
    int remaining_moves[7] = {
        22, // F_10
        15, // F_20
        7,  // F_30
        7,  // B_10
        21, // T_RIGHT
        21, // T_LEFT
        7   // U_TURN
    };
    int total = 100;

    for (int i = 0; i < nbmove; i++) {
        int r = rand() % total;
        int move_type = 0;
        int sum = 0;

        // Select move based on remaining probabilities
        for (int j = 0; j < 7; j++) {
            sum += remaining_moves[j];
            if (r < sum) {
                move_type = j;
                break;
            }
        }

        // Update remaining moves
        remaining_moves[move_type]--;
        total--;
        list[i] = (t_move)move_type;
    }

    return list;
}

const char* t_move_to_string(t_move move) {
    switch (move) {
        case F_10:    return "Forward 10 m";
        case F_20:    return "Forward 20 m";
        case F_30:    return "Forward 30 m";
        case B_10:    return "Backward 10 m";
        case T_LEFT:  return "Turn left +90";
        case T_RIGHT: return "Turn right -90";
        case U_TURN:  return "U-Turn 180";
        case STAY:  return "Stay";
        default:      return "Unknown move";
    }
}