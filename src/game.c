#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    // TODO: implement!
    if (g_game_over != 1) {

    if (input != INPUT_NONE) { 
        snake_p->direction = input;
    }

    int* head_p = (int*) get_first(snake_p->body);
    int dist;

    
    if (snake_p->direction == INPUT_DOWN) {
        dist = 20;
    }
    else if (snake_p->direction == INPUT_UP) {
        dist = -20;
    }
    else if (snake_p->direction == INPUT_LEFT) {
        dist = -1;
    }
    else if (snake_p->direction == INPUT_RIGHT) {
        dist = 1;
    }
    else {
        dist = 1;
    }


    if (growing == 0) {


    if (cells[*head_p+dist] == FLAG_WALL) {
        g_game_over = 1;
    }
    else if (cells[*head_p+1] == FLAG_FOOD  && snake_p->direction == INPUT_RIGHT) {
        g_score = g_score + 1;
        cells[*head_p+1] = FLAG_SNAKE;
        cells[*head_p] = FLAG_PLAIN_CELL;
        *head_p = *head_p + 1;
        place_food(cells, width, height);
    } else if (cells[*head_p+20] == FLAG_FOOD  && snake_p->direction == INPUT_DOWN) {
        g_score = g_score + 1;
        cells[*head_p+20] = FLAG_SNAKE;
        cells[*head_p] = FLAG_PLAIN_CELL;
        *head_p = *head_p + 20;
        place_food(cells, width, height);
    }
    else if (cells[*head_p-20] == FLAG_FOOD  && snake_p->direction == INPUT_UP){
        g_score = g_score + 1;
        cells[*head_p-20] = FLAG_SNAKE;
        cells[*head_p] = FLAG_PLAIN_CELL;
        *head_p = *head_p - 20;
        place_food(cells, width, height);

    }
    else if (cells[*head_p-1] == FLAG_FOOD  && snake_p->direction == INPUT_LEFT) {
        g_score = g_score + 1;
        cells[*head_p-1] = FLAG_SNAKE;
        cells[*head_p] = FLAG_PLAIN_CELL;
        *head_p = *head_p - 1;
        place_food(cells, width, height);
    }
    else {
        
        if (snake_p->direction == INPUT_DOWN) {
            cells[*head_p+20] = FLAG_SNAKE;
            cells[*head_p] = FLAG_PLAIN_CELL;
            *head_p = *head_p + 20;
        }
        else if (snake_p->direction == INPUT_UP) {
            cells[*head_p-20] = FLAG_SNAKE;
            cells[*head_p] = FLAG_PLAIN_CELL;
            *head_p = *head_p - 20;
        
        }
        else if (snake_p->direction == INPUT_LEFT) {
            cells[*head_p-1] = FLAG_SNAKE;
            cells[*head_p] = FLAG_PLAIN_CELL;
            *head_p = *head_p - 1;
        }
        else if (snake_p->direction == INPUT_RIGHT) {
            cells[*head_p+1] = FLAG_SNAKE;
            cells[*head_p] = FLAG_PLAIN_CELL;
            *head_p = *head_p + 1;
        }
        else {
            cells[*head_p+1] = FLAG_SNAKE;
            cells[*head_p] = FLAG_PLAIN_CELL;
            *head_p = *head_p + 1;
        }

    }

    }
    else if (growing == 1) {
        if (cells[*head_p+dist] != FLAG_WALL && cells[*head_p+dist] != FLAG_SNAKE) {
            if (cells[*head_p+dist] != FLAG_FOOD) {
                if (length_list(snake_p->body)>1) {
                    // void* i = get_first(snake_p->body);//43
                    int i = *(int*)get_first(snake_p->body)+dist;
                    // *(int*)i += dist;//44
                    insert_first(&snake_p->body, &i, sizeof(int));//44,43
                    int remove_i = *(int*)get_last(snake_p->body);
                    free(remove_last(&snake_p->body));
                    
                    // void* remove_i = remove_last(&snake_p->body);//42--
                    // free(remove_i);
                    cells[remove_i] = FLAG_PLAIN_CELL;//[]S
                    cells[i] = FLAG_SNAKE;
 
                }
                else {
                    cells[(*(int*)snake_p->body->data)] = FLAG_PLAIN_CELL;
                    cells[*(int*)snake_p->body->data + dist] = FLAG_SNAKE;
                    *(int*)snake_p->body->data += dist;
                }
            }
            else {
                void* i = get_first(snake_p->body);//42
                // *(int*)i += dist;//43
                int new_pos = *(int*) i;
                new_pos += dist;
                insert_first(&(snake_p->body), &new_pos, sizeof(int));//43, 42
                cells[new_pos] = FLAG_SNAKE;//43, 42
                // cells[*(int*)i - dist] = FLAG_PLAIN_CELL;

                // void* i = get_first(snake_p->body);//42
                // *(int*)i += dist;
                // insert_first(&(snake_p->body), i, sizeof(int));//43, 42
                // cells[*(int*)i] = FLAG_SNAKE;

                
                place_food(cells, width, height);
                g_score = g_score + 1;
            }
        }
        else {
            g_game_over = 1;
        }

    }



    }
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
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
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    // TODO: implement!
    free(cells);

    while (snake_p->body != NULL) {
        free(remove_last(&snake_p->body));
    }
    // free(snake_p);
}
