#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"

/** Updates the game by a single step, and modifies the given game struct
 * accordingly. Arguments:
 * - game: a pointer to the current game struct.
 * - input: the next input.
 * - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(game_t* game, input_key_t input, int growing) {
    // `update` should update game to reflect new state. If in the updated
    // position, the snake runs into a wall or itself, it will not move and
    // game_over will be 1. Otherwise it will be moved to the new position. If
    // the snake eats food, the game score increases by 1. This function does
    // not handle updating the high_scores string. This assumes that the board
    // is surrounded by walls so it does not handle the case where a snake runs
    // off the board.
    // TODO: implement!
    int width = game->board->width;
    if (game->game_over == 1)
        return;
    if(input != INPUT_NONE)
    {
        game->board->snake->snake_directions = input;
    }
    // printf("%s\n", "start update");
    // printf("%d\n", game->board->snake->pos);

        int prev_pos = game->board->snake->pos;
        if(game->board->snake->snake_directions == UP)
        {
            // printf("%d\n", game->board->cells[game->board->snake->pos - width]);
            game->board->snake->pos  = game->board->snake->pos - width;
            game->board->cells[game->board->snake->pos+width] = FLAG_PLAIN_CELL;
        }
        else if(game->board->snake->snake_directions == DOWN)
        {
            // printf("%d\n", game->board->cells[game->board->snake->pos + width]);
            game->board->snake->pos  = game->board->snake->pos + width;
            game->board->cells[game->board->snake->pos-width] = FLAG_PLAIN_CELL;
        }
        else if(game->board->snake->snake_directions == LEFT)
        {
            // printf("%d\n", game->board->cells[game->board->snake->pos--]);
            game->board->snake->pos  --;
            game->board->cells[game->board->snake->pos+1] = FLAG_PLAIN_CELL;
        }
        else if(game->board->snake->snake_directions == RIGHT)
        {
            // printf("%d\n", game->board->cells[game->board->snake->pos++]);
            game->board->snake->pos  ++;
            game->board->cells[game->board->snake->pos-1] = FLAG_PLAIN_CELL;
        }
    // printf("%d\n", game->board->snake->pos);
        // printf("%s", "posssssssssssssss:");
        // printf("%d %d\n", game->board->snake->pos, game->board->cells[game->board->snake->pos]);
        if (game->board->cells[game->board->snake->pos] == FLAG_WALL)
        {     
            game->board->snake->pos = prev_pos;
            game->board->cells[game->board->snake->pos] = FLAG_SNAKE;
            // printf("%s\n", "wall    end ");
            // printf("%d %d\n", game->board->cells[game->board->snake->pos],game->board->snake->pos);
            game->game_over = 1;
            return;
        }
        else
        {
            if(game->board->cells[game->board->snake->pos] == FLAG_FOOD)
            {
                // printf("%s", "place_food");
                game->score ++;
                place_food(game->board);
            }
            game->board->cells[game->board->snake->pos] = FLAG_SNAKE;
        }
    // printf("%s\n", "end pos");
    // printf("%d %d\n", game->board->snake->pos, game->board->cells[game->board->snake->pos]);

    // return;    
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - `board`: a pointer to the board to be updated.
 */
void place_food(board_t* board) {
    /* DO NOT MODIFY THIS FUNCTION */
    int food_index = generate_index(board->width * board->height);
    if (*(board->cells + food_index) == FLAG_PLAIN_CELL) {
        *(board->cells + food_index) = FLAG_FOOD;
    } else {
        place_food(board);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    // TODO: implement! (remove the call to strcpy once you begin your
    // implementation)
    strcpy(write_into, "placeholder");
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - `game`: a pointer the current game struct.
 */
void teardown(game_t* game) {
    // TODO: implement!
    free(game->board->cells);
}
