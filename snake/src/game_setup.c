#include "game_setup.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/* Initializes the board struct with walls around the edge of the board.
 * Returns NULL on failure; otherwise returns the pointer to the modified board.
 * Arguments:
 *  - board: a pointer to a board_t
 *  - width: a positive int equal to the desired outer width
 *  - height: a positive int equal to the desired outer height
 */
board_init_status_t initialize_default_board(board_t* board) {
    printf("%s\n", "initialize_default_board");
    board->width = 20;
    board->height = 10;
    board->cells = calloc(20 * 10, sizeof(int));

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        board->cells[i] = FLAG_WALL;
        board->cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        board->cells[i * 20] = FLAG_WALL;
        board->cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    // printf("%s\n", "add snake");
    board->cells[20 * 2 + 2] = FLAG_SNAKE;
    board->snake->snake_directions = RIGHT; 

    return INIT_SUCCESS;
}

/** Initialize the game struct, with a (potentially-empty) board representation.
 * Arguments:
 *  - `game`: a pointer to the game struct to be initialized.
 *  - `board_rep`: a string representing the initial board. May be empty.
 */
board_init_status_t initialize_game(game_t* game, char* board_rep) {
    // TODO: implement!
    game->game_over = 0;
    game->score = 0;
    game->board->snake->snake_directions = RIGHT;
    node_t* snake_head = NULL;
    int position = 42;
    insert_first(&snake_head, &position, sizeof(int));
    game->board->snake->pos = snake_head;
    // printf("%d\n",*((int*)snake_head->data));

    if(board_rep == NULL){
        board_init_status_t status = initialize_default_board(game->board);
        place_food(game->board);
        return status;
    }
    else
    {
        board_init_status_t status = decompress_board_str(game->board,board_rep);
        if(status == INIT_SUCCESS)
        {
            place_food(game->board);
            return status;
        }
        else
        {
            return status;
        }
    }

    }

    // return INIT_SUCCESS;


/** Takes in a string `compressed` and returns a board_t* that contains the
 * matching set of cells that can then be initialized. Arguments:
 *      - board: a pointer to the board that we would like to initialize
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row (delineated
 * by the `|` character), and read out a letter (E, S or W) a number of times
 * dictated by the number that follows the letter.
 */
// board_init_status_t decompress_board_str(board_t* board, char* compressed) {
board_init_status_t decompress_board_str(board_t* board, char* compressed) {
    // TODO: implement!
    // printf("%s\n","ok1");
    // printf("%s\n",compressed);
    int col = 0;
    int row = 0;
    board->width = col;
    board->height = row;
    board->cells = calloc(row * col, sizeof(int));
    // game->board->snake->pos = 42;
    int i = 0;
    int snake_number = 0;
    int max_col = 0;
//test first line
    if(*(compressed+i) != 'B')
        return INIT_ERR_BAD_CHAR;
    i++;
    row = atoi((compressed+i));    
    while(((int)(compressed[i]) - (int)('0'))>= 0 && ((int)(compressed[i]) - (int)('0'))<= 9)
    {
        i++;
    }
    if(compressed[i] != 'x') 
        return INIT_ERR_BAD_CHAR;
    i++;
    col = atoi(compressed+i);
    free(board->cells);
    while(((int)(compressed[i]) - (int)('0'))>= 0 && ((int)(compressed[i]) - (int)('0'))<= 9)
    {
        i++;
    }
    i++;

//test and construct other lines
    
    board->width = col;
    board->height = row;
    board->cells = calloc(row * col, sizeof(int));
    int curr_row = 0;
    compressed = compressed+i;
    char* lines = strsep(&compressed,"|");
    while (lines!= NULL)
    {

        // printf("%s", "line is:");
        // printf("%s\n", lines);
        int lines_len = strlen(lines);
        int curr_col = 0;
        int j=0;
        while(j<lines_len)
        {
        if(curr_row >= row ) {
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
            // char* substr = (char *)calloc(col*row, sizeof(char));
            // printf("%s", "lines[j] is:");
            // printf("%c\n", lines[j]);
            // printf("%s\n", "j is:");
            // printf("%d\n", j);
            if(isalpha(lines[j]) != 0)
            {   
                int number = 0;
                // printf("%c\n", str[j]);
                if(*(lines+j) == 'W')
                {   
                // printf("%s", "wwwwww:");
                // printf("%c\n", lines[j]);
                    //get number
                    j++;
                    
                    while(isdigit(lines[j]))
                    {
                        int digit = (int)(lines[j]) - (int)('0');
                        number = number*10 + digit;
                        // printf("%s\n", "isdigit_j is:");
                        // printf("%d\n", j);
                        j++;
                    }
                    //padding 
                    for(int k=1;k<=number;k++)
                    {
                        // printf("%s", "col is:");
                        // printf("%d %d %d\n", curr_row, curr_col, k);
                        board->cells[curr_row*max_col + curr_col + k -1] = FLAG_WALL;
                        
                    }
                    // printf("%s\n", "j is:");
                    // printf("%d\n", j);
                    
                }

                else if(*(lines+j) == 'S')
                {   
                // printf("%s", "ssssss:");
                // printf("%c\n", lines[j]);
        // printf("%s", "ssssssssssssssss:");
        // printf("%d\n", snake_number);
                    
                    //get number
                    j++;
                    while(isdigit(lines[j]))
                    {
                        int digit = (int)(lines[j]) - (int)('0');
                        number = number*10 + digit;
                        j++;
                    }
                    snake_number += number;
                    //padding 
                    for(int k=1;k<=number;k++)
                    {
                        // printf("%s", "col is:");
                        // printf("%d %d %d\n", curr_row, curr_col, k);
                        board->cells[curr_row*max_col + curr_col + k -1] = FLAG_SNAKE;
                        int* snake_position = board->snake->pos->data;
                        *(snake_position) = curr_row*max_col + curr_col + k -1;
                    }
                    
                }
                
                else if(*(lines+j) == 'E')
                {
                    //get number
                // printf("%s", "eeeeeeee:");
                // printf("%c\n", lines[j]);
                    j++;
                    while(isdigit(lines[j]))
                    {
                        int digit = (int)(lines[j]) - (int)('0');
                        number = number*10 + digit;
                        j++;
                    }
                    //padding 
                    for(int k=1;k<=number;k++)
                    {
                        // printf("%s", "col is:");
                        // printf("%d %d %d\n", curr_row, curr_col, k);
                        board->cells[curr_row*max_col + curr_col + k -1] = FLAG_PLAIN_CELL;
                    }
                    
                }
                else
                    return INIT_ERR_BAD_CHAR;
                    

                // printf("%s", "end_j is:");
                // printf("%d\n", j);

                //
                curr_col = curr_col + number;
            }//another character
            else
            j++;
            
        }//another line
        lines = strsep(&compressed,"|");
        // printf("%s", "next line is:");
        // printf("%s\n", lines);
        if(curr_col >= max_col)
        {
            max_col = curr_col;
        }
        if(curr_col != col)
        return INIT_ERR_INCORRECT_DIMENSIONS;
        curr_col = 0;
        curr_row++;
        
        
    }//end all

        // printf("%s", "rrrrrrrrrrrrrrrrrrrrr:");
        // printf("%d\n", snake_number);
    if(snake_number != 1)
        return INIT_ERR_WRONG_SNAKE_NUM;
    if((curr_row) != row)
        return INIT_ERR_INCORRECT_DIMENSIONS;

    return INIT_SUCCESS;
}