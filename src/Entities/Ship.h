typedef struct _ENTITY_Ship
{
    int pos_x, pos_y;
    int vel_x, vel_y;
}Ship;

void init_ship(Ship *ship, int start_x, int start_y, int velocity);
void draw_ship(Ship *ship, const char *shape);
void move_ship(Ship *ship, int max_x, char direction);
