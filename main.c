#include <raylib.h>
#include <string.h> /* memcpy */

#define GRID_SIZE 5
#define TILE_SIZE 4

/* This is the field area the program actually calculate */
#define FIELD_WIDTH 500
#define FIELD_HEIGHT 500
/* This is the field that user will see */
#define VISIBLE_FIELD_WIDTH 350
#define VISIBLE_FIELD_HEIGHT 190

#define SCREEN_WIDTH VISIBLE_FIELD_WIDTH * GRID_SIZE
#define SCREEN_HEIGHT VISIBLE_FIELD_HEIGHT * GRID_SIZE

#define REFRESH_TIME 0.1

/* These array represents the field. 0 is dead cell, 1 is live cell */
char field_output[FIELD_WIDTH * FIELD_HEIGHT];
char field_buffer[FIELD_WIDTH * FIELD_HEIGHT];

/*** Function Prototypes ***/
/* Count live neighbour of specified cell */
int count_live_neighbour(int x, int y);
/* Apply game of life rule, and reflect result on the field buffer */
void apply_gol_rule(int x, int y, int n_count, int is_cell_alive);

int main(void)
{
    int x, y;
    double timer = 0.0;
    
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Game of life");
    SetTargetFPS(60);               
    
    /* Create random field */
    for (y = 0; y < FIELD_HEIGHT; y++){
        for (x = 0; x < FIELD_WIDTH; x++){
            field_output[y * FIELD_WIDTH + x] = (char)GetRandomValue(0, 1);
        }
    }
    memcpy(field_buffer, field_output, sizeof(field_buffer));
    
    while (!WindowShouldClose()){
        /* Update timer */
        timer += GetFrameTime();
        /* Update field output every REFRESH_TIME*/
        if (timer >= REFRESH_TIME){
            /* Apply game of life rule for each cell*/
            for (y = 0; y <= FIELD_HEIGHT; y++){
                for (x = 0; x <= FIELD_WIDTH; x++){
                    apply_gol_rule(x, y,
                                   count_live_neighbour(x, y),
                                   field_output[y * FIELD_WIDTH +x]);
                }
            }
            /* Copy buffer to output */
            memcpy(field_output, field_buffer, sizeof(field_output));
            timer = 0;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        /* Draw field output */
        for (y = 0; y < VISIBLE_FIELD_HEIGHT; y++){
            for (x = 0; x < VISIBLE_FIELD_WIDTH; x++){
                if (field_output[y * FIELD_WIDTH + x])
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
            }
        }
        EndDrawing();
    }
    CloseWindow();        
    return 0;
}

int count_live_neighbour(int x, int y)
{
    int c, h, w;
    
    c = 0;
    for (h = y - 1; h <= y + 1; h++){
        if (h < 0 || h >= FIELD_HEIGHT) /* Ignore (count as dead) over field boundary */
            continue;
        for (w = x - 1; w <= x + 1; w++){
            if (w < 0 || w >= FIELD_WIDTH)  /* Ignore (count as dead) over field boundary */
                continue;
            
            if (!(h == y && w == x)) 
                c += (field_output[h * FIELD_WIDTH + w]) ? 1 : 0;
        }
    }
    
    return c;
}

void apply_gol_rule(int x, int y, int n_count, int is_cell_alive)
{
    if (is_cell_alive)
        field_buffer[y * FIELD_WIDTH + x] = (n_count == 2 || n_count == 3) ? 1 : 0;
    else
        field_buffer[y * FIELD_WIDTH + x] = (n_count == 3) ? 1 : 0;
}