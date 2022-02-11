#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    board->cells[20 * 2 + 2] = FLAG_SNAKE;
    board->snake->pos = 42;
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
    // game->game_over = 0;
    // game->score = 0;
    // initialize_default_board(game->board);

    // while(1)
    // {
    // int food_index = generate_index(game->board->width * game->board->height);
    // if (*(game->board->cells + food_index) == FLAG_PLAIN_CELL) 
    // {
    //     *(game->board->cells + food_index) = FLAG_FOOD;
    //     break;
    // } 
    // }

    if(board_rep == NULL){
    game->game_over = 0;
    game->score = 0;
    initialize_default_board(game->board);

    while(1)
    {
    int food_index = generate_index(game->board->width * game->board->height);
    if (*(game->board->cells + food_index) == FLAG_PLAIN_CELL) 
    {
        *(game->board->cells + food_index) = FLAG_FOOD;
        break;
    } 
    }

    }
    else
    {
        board_init_status_t statu = decompress_board_str(game->board,board_rep);
        if(statu == INIT_SUCCESS)
        {
            while(1)
            {
            int food_index = generate_index(game->board->width * game->board->height);
            if (*(game->board->cells + food_index) == FLAG_PLAIN_CELL) 
            {
                *(game->board->cells + food_index) = FLAG_FOOD;
                break;
            } 
            }
        }

    }



    return INIT_SUCCESS;
}

/** Takes in a string `compressed` and returns a board_t* that contains the
 * matching set of cells that can then be initialized. Arguments:
 *      - board: a pointer to the board that we would like to initialize
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row (delineated
 * by the `|` character), and read out a letter (E, S or W) a number of times
 * dictated by the number that follows the letter.
 */
board_init_status_t decompress_board_str(board_t* board, char* compressed) {
    // TODO: implement!
    int i = 0;
    int snake_number = 0;
    int max_col = 0;
//test first line
    if(*(compressed+i) != 'B')
        return INIT_ERR_BAD_CHAR;
    else
        i ++;

    int row = compressed[i];
    i ++;
    if(*(compressed+i) != 'x')
        return INIT_ERR_BAD_CHAR;
    else
        i ++;

    int col = *(compressed+i);
    i++;
    // int len_compressed = strlen(compressed);
    char* str = (char *)calloc(200, sizeof(char));
    i++;
    int curr_row = 0;
//test and construct other lines
    strcpy(str, (compressed+i));
    char* seperate = "|";
    char* lines = NULL;
    while ((lines = strsep(&str,seperate) )!= NULL)
    {
        int lines_len = strlen(lines);
    // for(int j=0; j<lines_len;j++)
        int curr_col = 0;
        // char substr[100];
        int substr_value = 0;
        for(int j=0;j<lines_len;j++)
        {
            char* substr = (char *)calloc(200, sizeof(char));
            if(*(str+j)<'0' ||*(str+j)>'9')
            {   
                int substr_i = 0;
                if(*(str+j) == 'W')
                {   
                    //get number
                    j++;
                    while(*(str+j)>='0' ||*(str+j)<='9' )
                    {
                        substr[substr_i] = str[j];
                        substr_i++;
                        j++;
                    }
                    substr_value = atoi(substr);
                    //padding 
                    for(int k=0;k<substr_value;k++)
                    {
                        board->cells[curr_row*max_col + curr_col + k] = FLAG_WALL;
                    }
                    
                }

                else if(*(str+j) == 'S')
                {   
                    snake_number++;
                    //get number
                    j++;
                    while(*(str+j)>='0' ||*(str+j)<='9' )
                    {
                        substr[substr_i] = str[j];
                        substr_i++;
                        j++;
                    }
                    substr_value = atoi(substr);
                    //padding 
                    for(int k=0;k<substr_value;k++)
                    {
                        board->cells[curr_row*max_col + curr_col + k] = FLAG_WALL;
                    }
                    
                }
                else if(*(str+j) == 'E')
                {
                    //get number
                    j++;
                    while(*(str+j)>='0' ||*(str+j)<='9' )
                    {
                        substr[substr_i] = str[j];
                        substr_i++;
                        j++;
                    }
                    substr_value = atoi(substr);
                    //padding 
                    for(int k=0;k<substr_value;k++)
                    {
                        board->cells[curr_row*max_col + curr_col + k] = FLAG_PLAIN_CELL;
                    }
                    
                }
                else
                    return INIT_ERR_BAD_CHAR;

                //
                curr_col = curr_col + substr_value;
            }//another character
            free(substr);
        }//another line
        if(curr_col >= max_col)
        {
            max_col = curr_col;
        }
        if(curr_col != col)
        return INIT_ERR_INCORRECT_DIMENSIONS;
        curr_col = 0;
        curr_row++;
        
        
    }//end all
    free(str);


    if(snake_number != 1)
        return INIT_ERR_WRONG_SNAKE_NUM;
    if(curr_row != row)
        return INIT_ERR_INCORRECT_DIMENSIONS;

    return INIT_UNIMPLEMENTED;
}

    // for(; i<len_compressed; i++)
    // {
    //     if(*(compressed+i) =='|')  
    //     {

    //         int curr_col = 0;
    //         int len = strlen(str); //redefinition of  len
    //         // char substr[100];
    //         int substr_value = 0;
    //         for(int j=0;j<len;j++)
    //         {
    //             char substr[100];
    //             printf("%d",j);
    //             if(*(str+j)<'0' ||*(str+j)>'9')
    //             {   
    //                 if(*(str+j) == 'W')
    //                 {   
    //                     //get number
    //                     j++;
    //                     while(*(str+j)>='0' ||*(str+j)<='9' )
    //                     {
    //                         substr[0] = str[j];
    //                         j++;
    //                     }
    //                     substr_value = atoi(substr);
    //                     //padding 
    //                     for(int k=0;k<substr_value;k++)
    //                     {
    //                         board->cells[curr_row*max_col + curr_col + k] = FLAG_WALL;
    //                     }
                      
    //                 }

    //                 else if(*(str+j) == 'S')
    //                 {   
    //                     snake_number++;
    //                     //get number
    //                     j++;
    //                     while(*(str+j)>='0' ||*(str+j)<='9' )
    //                     {
    //                         substr[0] = str[j];
    //                         j++;
    //                     }
    //                     substr_value = atoi(substr);
    //                     //padding 
    //                     for(int k=0;k<substr_value;k++)
    //                     {
    //                         board->cells[curr_row*max_col + curr_col + k] = FLAG_WALL;
    //                     }
                       
    //                 }
    //                 else if(*(str+j) == 'E')
    //                 {
    //                     //get number
    //                     j++;
    //                     while(*(str+j)>='0' ||*(str+j)<='9' )
    //                     {
    //                         substr[0] = str[j];
    //                         j++;
    //                     }
    //                     substr_value = atoi(substr);
    //                     //padding 
    //                     for(int k=0;k<substr_value;k++)
    //                     {
    //                         board->cells[curr_row*max_col + curr_col + k] = FLAG_PLAIN_CELL;
    //                     }
                        
    //                 }
    //                 else
    //                     return INIT_ERR_BAD_CHAR;

    //                 //
    //                 curr_col = curr_col + substr_value;
    //                 // *substr = NULL;
    //             }//another character
    //         }
    //         if(curr_col >= max_col)
    //         {
    //             max_col = curr_col;
    //         }
    //         if(curr_col != col)
    //         return INIT_ERR_INCORRECT_DIMENSIONS;
    //         curr_col = 0;
    //         curr_row++;
    //     }//another line
    // }//end all
