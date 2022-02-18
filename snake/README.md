CSCI 0300 Project 1 - Snake
===========================

## Design Overview:

*TODO!*

## Collaborators:

*TODO!*
Yiran Zhou

## Responsible Computing:

*TODO!*

Question1:
Joe Becker, Lee Collins, and Mark Davis first initiated a universal character encoding scheme called Unicode.
I think these members all have a great infulence.
The Unicode Technical accepts documents from any organization or indivadual.

Question 2:
Loma and Naxi Dongba (Moso) are not yet be supported by Unicode. It would be inconvenient for users who use these languages.

Question 3:
I think that it's not appropriate to unify the Chinese, Japanese, and Korean languages into one common encoding. Because different languages have too many differences in presenting.


## Extra credit:

If you choose to do extra credit, detail it here.

1.Rather than rendering “S” characters, make a solid green snake [Hint: use Unicode characters like we do for the walls] (2 points).
(1) Changed COLOR_SNAKE from COLOR_YELLOW to COLOR_GREEN (comment line 78 and then uncomment line 79 in render.c).
    init_pair(2, COLOR_YELLOW, -1);
    // init_pair(2, COLOR_GREEN, -1);
(2) Change the render of snake from a character to a solid block in render_game in render.c(comment line 96-98 and then uncomment line 99-101)
            char c = 's';
            ADD(i / board->width, i % board->width,
                c | COLOR_PAIR(COLOR_SNAKE));
            // cchar_t s;
            // setcchar(&s, L"\u2588", WA_NORMAL, COLOR_SNAKE, NULL);
            // ADDW(i / board->width, i % board->width, &s);

2.Extra life snake.
(1) Add 'int rest_life' in common.h which used to store the rest life of the snake.(uncomment line 81 in common.h).
    //int rest_life;
(2) Set the value of rest life in innitialize game function in game_setup.c(uncomment line 64).
    // game->rest_life = 3;
(3) Check whether rest life equal to zero when game over in update funtion in game.c. If both of these conditions met, return game over.(comment line 29-30 and uncomment line 27-28) in game.c.
    // if (game->game_over == 1 && game->rest_life <= 0)
    //     return ;
    if(game->game_over == 1)
        return;
(4) When snake hits the wall, check the rest of life. If it still has life, continue game, else, game over. (uncomment line 138-146 and comment line 147-148 in game.c)
    // if (game->rest_life >0)
    // {
    //     game->rest_life -= 1;
    // }
    // else
    // {
    //     game->game_over = 1;
    //     return;
    // }
    game->game_over = 1;
    return;
(5) When snake hits itself, check the rest of life. If it still has life, remove the element of the linked list, change the value of the cell, then continue game, else, game over. (uncomment line 155-165 and comment line 166-167 in game.c).
    // if(game->rest_life > 0)
    // {
    //     game->rest_life -= 1;
    //     remove_element(&(game->board->snake->pos), &new_pos, sizeof(int));
    //     game->board->cells[new_pos] = FLAG_PLAIN_CELL;
    // }
    // else
    // {
    //     game->game_over = 1;
    //     return;
    // }
    game->game_over = 1;
    return;
(6) Add the value of the rest life below the board in render.c(uncomment line 119 in render.c).
    // WRITEW(board->height + 1, 0, "rest life: %d", game->rest_life);

## How long did it take to complete Snake?

*TODO!*
About 30 hours.
