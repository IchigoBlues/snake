#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }


    place_food(cells, *width_p, *height_p);

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;
    

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement!
    
    snake_p->direction = INPUT_NONE;
    
    g_game_over = 0;
    g_score = 0;    

    snake_p->body = NULL;

    if (board_rep != NULL) {
        return decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }

    int head = 42;  

    insert_first(&(snake_p->body),&head,sizeof(int));
    return initialize_default_board(cells_p, width_p, height_p);
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
int intlen(int x) {
    return floor(log10(abs(x))) + 1;
}


enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {


    *height_p = atoi(compressed+1);

    *width_p = atoi(strchr(compressed, 'x') +1);
    
    if (*compressed != 'B') {
        return INIT_ERR_BAD_CHAR;
    }
    if (*(intlen(*height_p) + 1 + compressed) != 'x') {
        return INIT_ERR_BAD_CHAR;
    }


    int* cells = malloc(*width_p * *height_p * sizeof(int));
    *cells_p = cells;

    int pos = 0;
    char* token = strtok(compressed, "|");
    int totalheight = 0;
    int snake_num = 0;
    while (token != NULL) {
        totalheight++;
        token = strtok(NULL, "|");
        if (token == NULL) {
            totalheight-=1;
            break;
        }
        
        if (totalheight > (int) *height_p) {
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }

        char* temp = token;

      
        int totalwidth = 0;
        int n;
        while (*temp != '\0') {
            
            if (totalwidth > (int) *width_p) {
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            n  = atoi(temp+1);
            switch(*temp) {
                case 'W': ;

                    for (int i = pos; i < pos+n; i++) {
                        cells[i] = FLAG_WALL;
                    }
                    totalwidth += n;
                    pos = pos+n;

                    break;
                case 'E': ;
                    for (int i = pos; i < pos+n; i++) {
                        cells[i] = FLAG_PLAIN_CELL;
                    }
                    totalwidth += n;
                    pos = pos+n;

                    break;
                case 'S': ;

                    snake_num++;
                    if (snake_num > 1) {
                        return INIT_ERR_WRONG_SNAKE_NUM;
                    }
                    for (int i = pos; i < pos+n; i++) {
                        if (n > 1) {
                            return INIT_ERR_WRONG_SNAKE_NUM;
                        }
                        insert_first(&(snake_p->body),&i,sizeof(int));
                        cells[i] = FLAG_SNAKE;
                    }
                    totalwidth += n;

                    pos = pos+n;
                    
                    break;
                default:
                    return INIT_ERR_BAD_CHAR;
            }   
            temp += intlen(n) + 1;

            if (*temp == '\0') {
                if (totalwidth != (int) *width_p) {
                    return INIT_ERR_INCORRECT_DIMENSIONS;
                }
            }

        }      

    }


    

    if (totalheight < (int) *height_p) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }
    if (snake_num < 1) {
        return INIT_ERR_WRONG_SNAKE_NUM;
    }
    place_food(cells, *width_p, *height_p);
    return INIT_SUCCESS;

}