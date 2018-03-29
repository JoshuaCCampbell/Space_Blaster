typedef struct _ENTITY_Ship
{
    int pos_x, pos_y;
    int vel_x, vel_y;
}Ship;

void init_ship(Ship *player, int start_x, int start_y, int velocity);
void draw_ship(Ship *player);
void move_ship(Ship *player, int max_x, char direction);
