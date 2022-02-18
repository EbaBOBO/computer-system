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

    
    if(game->game_over == 1)
        return;
    int width = game->board->width;
    // printf("%d\n",*((int*)game->board->snake->pos->data));
    int* snake_position = game->board->snake->pos->data;
    // if(input != INPUT_NONE)
    // {
    //     game->board->snake->snake_directions = input;
    // }

    if(growing == 0)
    {
        if(input != INPUT_NONE)
        {
            game->board->snake->snake_directions = input;
        }
        int input_direction1;
        if(game->board->snake->snake_directions == UP)
        {
            input_direction1 = -width;
        }
        else if(game->board->snake->snake_directions == DOWN)
        {
            input_direction1 = width;
        }
        else if(game->board->snake->snake_directions == LEFT)
        {
            input_direction1 = -1;
        }
        else if(game->board->snake->snake_directions == RIGHT)
        {
            input_direction1 = 1;
        }

        if (game->board->cells[*(snake_position)+input_direction1] == FLAG_WALL)
        {     
            game->game_over = 1;
            return;
        }
        else if (game->board->cells[*(snake_position) +input_direction1] == FLAG_FOOD)
        {
                game->score ++;
                game->board->cells[*(snake_position) +input_direction1] = FLAG_SNAKE;
                game->board->cells[*(snake_position)] = FLAG_PLAIN_CELL;
                *(snake_position) = *(snake_position) + input_direction1;
                place_food(game->board);
                
        }
        else
        {
                game->board->cells[*(snake_position) +input_direction1] = FLAG_SNAKE;   
                game->board->cells[*(snake_position)] = FLAG_PLAIN_CELL; 
                *(snake_position) = *(snake_position) + input_direction1;
        }
    }
    else if(growing == 1)
    {
        //check if we need to change direction
        int not_change_dir = 0;
        if((input==INPUT_RIGHT) && (game->board->snake->snake_directions == LEFT))
        {
            not_change_dir = 1;
        }
        else if((input==INPUT_LEFT && game->board->snake->snake_directions == RIGHT))
        {
            not_change_dir = 1;
        }
        else if((input==INPUT_UP && game->board->snake->snake_directions == DOWN))
        {
            not_change_dir = 1;
        }
        else if((input==INPUT_DOWN && game->board->snake->snake_directions == UP))
        {
            not_change_dir = 1;
        }
        else if(input == INPUT_NONE)
        {
            not_change_dir = 1;
        }
        if(not_change_dir == 0)
        {
            game->board->snake->snake_directions = input;
        }
        //get input_direction
        int input_direction;
        if(game->board->snake->snake_directions == UP)
        {
            input_direction = -width;
        }
        else if(game->board->snake->snake_directions == DOWN)
        {
            input_direction = width;
        }
        else if(game->board->snake->snake_directions == LEFT)
        {
            input_direction = -1;
        }
        else if(game->board->snake->snake_directions == RIGHT)
        {
            input_direction = 1;
        }
        int new_pos = *(snake_position)+input_direction;
        int* last = get_last(game->board->snake->pos);

        // printf("%d\n",*((int*)snake_position));
        // printf("%d\n",*(snake_position)+input_direction);
        // printf("%d\n",game->board->cells[*(snake_position)+input_direction]);
        if (game->board->cells[new_pos] == FLAG_WALL)
        {     
            game->game_over = 1;
            return;
        }
        else if (game->board->cells[new_pos] == FLAG_SNAKE && new_pos != *last)// && new_pos != *last
        {   
            // printf("%s\n", "tail game over");
            // printf("%d %d\n", new_pos, *last);
            game->game_over = 1;
            return;
        }
        else if (game->board->cells[new_pos] == FLAG_FOOD)
        {
                game->score += 1;

                // int position = *(snake_position) +input_direction;
                insert_first(&game->board->snake->pos, &new_pos, sizeof(int));

                game->board->cells[new_pos] = FLAG_SNAKE;

                place_food(game->board);
                // printf("%d %d\n", new_pos, *last);
                
        }
        // else if(game->board->cells[new_pos] == FLAG_PLAIN_CELL || ((game->board->cells[new_pos] == FLAG_SNAKE) && (new_pos == *last)))
        else
        {       //insert first
                game->board->cells[*(last)] = FLAG_PLAIN_CELL;    
                insert_first(&(game->board->snake->pos), &new_pos, sizeof(int));
                game->board->cells[new_pos] = FLAG_SNAKE;
                // get last and remove last
                int* removed = remove_last(&(game->board->snake->pos)); 
                //change cells to plain_cell
                // printf("%d %d\n", new_pos, *last);
                free(removed);
        }
    }
    // }

    // return ;

    
    

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
    // scanf("%[^\n]",str);
    printf("%s\n","Name > ");
    scanf("%[^\n]", write_into);//"%[^\n]"
    // fflush(stdin);
    // int len = strlen(write_into);
    // size_t top;
    // int top;
    read(0, write_into, 1000);
    fflush(stdin);
    int len = strlen(write_into);
    // printf("%s\n", write_into);
    mbslen(write_into);
    // printf("%d %d\n", len, top);
    while(len<1)
    {
        // scanf("%s", write_into);

        read(0,write_into, 1000);
        fflush(stdin);
        len = strlen(write_into);
        printf("%s\n", "Name Invalid: must be longer than 0 characters.");
        // top = read(0, write_into, 1000);
        // printf("%s\n", write_into);
        // printf("%d %d\n", len, top);
    }
    // game_t name_len = top;

    // strcpy(write_into, "placeholder");
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - `game`: a pointer the current game struct.
 */
void teardown(game_t* game) {
    // TODO: implement!
    // game->board->snake->pos;
    while(game->board->snake->pos)
    {
        void* removed = remove_last(&game->board->snake->pos);
        free(removed);
    }
    free(game->board->cells);

}
