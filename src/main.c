#include "../includes/Game2048.h"
void	*ft_memset(void *s, int c, size_t n)
{
    size_t	i;
    char	*ptr;

    ptr = (char *)s;
    i = 0;
    while (i < n)
    {
        ptr[i] = c;
        i++;
    }
    return (s);
}

void add_new_tile(t_grid *g)
{
    int empty[16];
    int count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (g->grid[i][j] == 0) {
                empty[count++] = i * 4 + j;
            }
        }
    }
    if (count > 0) {
        int k = rand() % count;
        int index = empty[k];
        int i = index / 4;
        int j = index % 4;
        g->grid[i][j] = (rand() % 10 == 0 ? 4 : 2);
    }
}

bool slide_left(int row[4])
{
    int temp[4];
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (row[i] != 0) {
            temp[count++] = row[i];
        }
    }
    int new_row[4] = {0};
    int pos = 0;
    for (int i = 0; i < count; i++) {
        if (i < count - 1 && temp[i] == temp[i + 1]) {
            new_row[pos] = temp[i] * 2;
            i++;
        } else {
            new_row[pos] = temp[i];
        }
        pos++;
    }
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        if (row[i] != new_row[i]) {
            changed = true;
        }
        row[i] = new_row[i];
    }
    return changed;
}

bool slide_right(int row[4])
{
    for (int i = 0; i < 2; i++) {
        int temp = row[i];
        row[i] = row[3 - i];
        row[3 - i] = temp;
    }
    bool changed = slide_left(row);
    for (int i = 0; i < 2; i++) {
        int temp = row[i];
        row[i] = row[3 - i];
        row[3 - i] = temp;
    }
    return changed;
}

bool move_left(t_grid *g)
{
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        if (slide_left(g->grid[i])) {
            changed = true;
        }
    }
    return changed;
}

bool move_right(t_grid *g)
{
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        if (slide_right(g->grid[i])) {
            changed = true;
        }
    }
    return changed;
}

bool move_up(t_grid *g)
{
    bool changed = false;
    for (int j = 0; j < 4; j++) {
        int col[4];
        for (int i = 0; i < 4; i++) {
            col[i] = g->grid[i][j];
        }
        if (slide_left(col)) {
            changed = true;
        }
        for (int i = 0; i < 4; i++) {
            g->grid[i][j] = col[i];
        }
    }
    return changed;
}

bool move_down(t_grid *g)
{
    bool changed = false;
    for (int j = 0; j < 4; j++) {
        int col[4];
        for (int i = 0; i < 4; i++) {
            col[i] = g->grid[i][j];
        }
        if (slide_right(col)) {
            changed = true;
        }
        for (int i = 0; i < 4; i++) {
            g->grid[i][j] = col[i];
        }
    }
    return changed;
}

bool is_game_over(t_grid *g)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (g->grid[i][j] == 0) {
                return false;
            }
            if (j < 3 && g->grid[i][j] == g->grid[i][j + 1]) {
                return false;
            }
            if (i < 3 && g->grid[i][j] == g->grid[i + 1][j]) {
                return false;
            }
        }
    }
    return true;
}

bool has_won(t_grid *g)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (g->grid[i][j] >= 2048) {
                return true;
            }
        }
    }
    return false;
}

void setup_colors() {
    start_color();
    init_pair(COLOR_EMPTY, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_2, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COLOR_8, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(COLOR_16, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_32, COLOR_WHITE, COLOR_CYAN);
    init_pair(COLOR_64, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_128, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_256, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COLOR_512, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(COLOR_1024, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_2048, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_OTHER, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_BORDER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_TEXT, COLOR_WHITE, COLOR_BLACK);
}

int get_color_for_value(int value) {
    switch(value) {
        case 0: return COLOR_EMPTY;
        case 2: return COLOR_2;
        case 4: return COLOR_4;
        case 8: return COLOR_8;
        case 16: return COLOR_16;
        case 32: return COLOR_32;
        case 64: return COLOR_64;
        case 128: return COLOR_128;
        case 256: return COLOR_256;
        case 512: return COLOR_512;
        case 1024: return COLOR_1024;
        case 2048: return COLOR_2048;
        default: return COLOR_OTHER;
    }
}

void display_grid(t_grid *g, bool won)
{
    clear();
    
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    
    int cell_width = 12;        
    int cell_height = 5;       
    int grid_width = cell_width * 4 + 1;
    int grid_height = (cell_height * 4) + 1;  
    
    int h_padding = 2;
    int v_padding = 1;
    
    int start_x = (term_width - grid_width) / 2;
    int start_y = (term_height - grid_height) / 2;
    
    attron(COLOR_PAIR(COLOR_BORDER));
    
    mvaddch(start_y, start_x, ACS_ULCORNER);
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < cell_width; k++) {
            mvaddch(start_y, start_x + j * cell_width + k + 1, ACS_HLINE);
        }
        if (j < 3) {
            mvaddch(start_y, start_x + (j + 1) * cell_width + 1, ACS_TTEE);
        }
    }
    mvaddch(start_y, start_x + grid_width, ACS_URCORNER);
    
    for (int i = 0; i < 4; i++) {
        for (int row = 0; row < cell_height; row++) {
            mvaddch(start_y + (i * cell_height) + row + 1, start_x, ACS_VLINE);
            
            for (int j = 0; j < 4; j++) {
                attroff(COLOR_PAIR(COLOR_BORDER));
                
                int value = g->grid[i][j];
                int color_pair = get_color_for_value(value);
                
                attron(COLOR_PAIR(COLOR_BORDER));
                for (int k = 1; k < cell_width; k++) {
                    mvaddch(start_y + (i * cell_height) + row + 1, 
                            start_x + j * cell_width + k + 1, ' ');
                }
                
                if (row >= v_padding && row < cell_height - v_padding) {
                    attron(COLOR_PAIR(color_pair));
                    
                    for (int k = h_padding + 1; k < cell_width - h_padding; k++) {
                        mvaddch(start_y + (i * cell_height) + row + 1,
                                start_x + j * cell_width + k + 1, ' ');
                    }
                    
                    if (value != 0 && row == cell_height / 2) {
                        char buf[8];
                        sprintf(buf, "%d", value);
                        int len = strlen(buf);
                        int box_width = cell_width - (2 * h_padding); 
                        int pos = start_x + j * cell_width + h_padding + 1 + ((box_width - len) / 2);
                        
                        attron(A_BOLD);
                        mvprintw(start_y + (i * cell_height) + row + 1, pos, "%s", buf);
                        attroff(A_BOLD);
                    }
                    attroff(COLOR_PAIR(color_pair));
                }
                
                attron(COLOR_PAIR(COLOR_BORDER));
                
                if (j < 3) {
                    mvaddch(start_y + (i * cell_height) + row + 1, 
                            start_x + (j + 1) * cell_width + 1, ACS_VLINE);
                }
            }
            
            mvaddch(start_y + (i * cell_height) + row + 1, 
                    start_x + grid_width, ACS_VLINE);
        }
        
        if (i < 3) {
            mvaddch(start_y + (i + 1) * cell_height, start_x, ACS_LTEE);
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < cell_width; k++) {
                    mvaddch(start_y + (i + 1) * cell_height, 
                            start_x + j * cell_width + k + 1, ACS_HLINE);
                }
                if (j < 3) {
                    mvaddch(start_y + (i + 1) * cell_height, 
                            start_x + (j + 1) * cell_width + 1, ACS_PLUS);
                }
            }
            mvaddch(start_y + (i + 1) * cell_height, 
                    start_x + grid_width, ACS_RTEE);
        }
    }
    
    mvaddch(start_y + grid_height, start_x, ACS_LLCORNER);
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < cell_width; k++) {
            mvaddch(start_y + grid_height, start_x + j * cell_width + k + 1, ACS_HLINE);
        }
        if (j < 3) {
            mvaddch(start_y + grid_height, start_x + (j + 1) * cell_width + 1, ACS_BTEE);
        }
    }
    mvaddch(start_y + grid_height, start_x + grid_width, ACS_LRCORNER);
    attroff(COLOR_PAIR(COLOR_BORDER));
    
    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    mvprintw(start_y + grid_height + 2, start_x + (grid_width - 9) / 2, "2048 GAME");
    attroff(A_BOLD);
    mvprintw(start_y + grid_height + 3, start_x + (grid_width - 38) / 2, "Use arrow keys to move, ESC to quit");
    attroff(COLOR_PAIR(COLOR_TEXT));
    
    attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    if (has_won(g) && !won) {
        mvprintw(start_y - 2, start_x + (grid_width - 40) / 2, "YOU WIN! Keep playing or press ESC to quit");
    }
    
    if (is_game_over(g)) {
        mvprintw(start_y - 2, start_x + (grid_width - 27) / 2, "GAME OVER! Press ESC to quit");
    }
    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
    
    refresh();
}

int display_start_menu() {
    clear();
    
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    
    char *options[MENU_OPTIONS] = {
        "Play Game",
        "Exit"
    };
    
    int selected = 0;
    int key;
    bool done = false;
    
    init_pair(16, COLOR_WHITE, COLOR_BLACK);
    init_pair(17, COLOR_BLACK, COLOR_CYAN);
    
    while (!done) {
        clear();
        
        attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
        mvprintw(term_height/2 - 5, (term_width - 11)/2, "2048 GAME");
        mvprintw(term_height/2 - 4, (term_width - 24)/2, "---------------------");
        attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
        
        for (int i = 0; i < MENU_OPTIONS; i++) {
            if (i == selected) {
                attron(COLOR_PAIR(17) | A_BOLD);
                mvprintw(term_height/2 + i, (term_width - strlen(options[i]))/2, "%s", options[i]);
                attroff(COLOR_PAIR(17) | A_BOLD);
            } else {
                attron(COLOR_PAIR(16));
                mvprintw(term_height/2 + i, (term_width - strlen(options[i]))/2, "%s", options[i]);
                attroff(COLOR_PAIR(16));
            }
        }
        
        attron(COLOR_PAIR(COLOR_TEXT));
        mvprintw(term_height/2 + MENU_OPTIONS + 2, (term_width - 36)/2, 
                 "Use UP/DOWN arrows and ENTER to select");
        attroff(COLOR_PAIR(COLOR_TEXT));
        
        refresh();
        
        key = getch();
        
        switch(key) {
            case KEY_UP:
                selected = (selected - 1 + MENU_OPTIONS) % MENU_OPTIONS;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % MENU_OPTIONS;
                break;
            case 10:
                done = true;
                break;
        }
    }
    
    return selected;
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support colors\n");
        return 1;
    }
    
    setup_colors();
    srand(time(NULL));
    
    while (true) {
        int menu_choice = display_start_menu();
        
        if (menu_choice == MENU_EXIT) {
            break;
        } else {
            t_grid game;
            ft_memset(&game, 0, sizeof(t_grid));
            
            add_new_tile(&game);
            add_new_tile(&game);
            bool won = false;
            
            while (true) {
                display_grid(&game, won);
                
                attron(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
                if (has_won(&game) && !won) {
                    won = true;
                }
                
                if (is_game_over(&game)) {
                    mvprintw(11, 1, "GAME OVER! Press ESC to quit");
                    attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
                    refresh();
                    while (getch() != 27);
                    break;
                }
                attroff(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
                
                int ch = getch();
                if (ch == 27) {
                    break;
                }
                
                bool changed = false;
                switch (ch) {
                    case KEY_LEFT:
                        changed = move_left(&game);
                        break;
                    case KEY_RIGHT:
                        changed = move_right(&game);
                        break;
                    case KEY_UP:
                        changed = move_up(&game);
                        break;
                    case KEY_DOWN:
                        changed = move_down(&game);
                        break;
                }
                
                if (changed) {
                    add_new_tile(&game);
                }
            }
        }
    }
    
    endwin();
    return 0;
}


