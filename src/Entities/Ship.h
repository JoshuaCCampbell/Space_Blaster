typedef struct _ENTITY_ship SHIP;

void init_ship(SHIP *ship, int start_x, int start_y, int velocity);
void draw_ship(SHIP *ship);
void move_ship(SHIP *ship, int max_x, char direction);
