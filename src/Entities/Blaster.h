typedef struct _ENTITY_blaster
{
    int pos_x, pos_y;
    int vel_x, vel_y;
    int active;
}Blaster;

void init_blaster(Blaster *blaster, int velocity);
void draw_blaster(Blaster *blaster);
void move_blaster(Blaster *blaster, int direction);
