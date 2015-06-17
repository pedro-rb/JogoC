#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SCREEN_W 500
#define SCREEN_H 800
#define ENEMY1_MAX 22
#define ENEMY_BULLETS_MAX 20
#define ENEMY_CHANCE 100

enum
{
    KEY_RIGHT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_MAX

};

bool keys[KEY_MAX];

typedef struct
{
    int x;
    int y;
    bool live;

} s_Object;

typedef struct
{
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;

}spr_anim;


void enemyshot(s_Object *enemy, s_Object *bullet, int *bulletcount)
{
    if(*bulletcount<ENEMY_BULLETS_MAX)
    {
        bullet[*bulletcount].x=enemy->x;
        bullet[*bulletcount].y=enemy->y;
        bullet[*bulletcount].live=true;
        (*bulletcount)=(*bulletcount)+1;
    }
}

void player_collision_wall(s_Object *player, ALLEGRO_BITMAP* img_player)
{
    if(player->x < 0)
    {
        player->x = 0;
    }
    if(player->y < 0)
    {
        player->y = 0;
    }
    if(player->x > SCREEN_W - al_get_bitmap_width(img_player))
    {
        player->x = SCREEN_W - al_get_bitmap_width(img_player);
    }
    if(player->y > SCREEN_H - al_get_bitmap_height(img_player))
    {
        player->y = SCREEN_H - al_get_bitmap_height(img_player);
    }
}

int main()
{

    srand(time(NULL));

    int i;
    int playerlives=3;

    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_EVENT ev;
    ALLEGRO_TIMER* enemytimer;

    ALLEGRO_BITMAP* img_player;
    ALLEGRO_BITMAP* img_enemy1;
    ALLEGRO_BITMAP* img_enemyBullet;

    ALLEGRO_FONT* fonte;

    s_Object player;
    player.x=SCREEN_W/2;
    player.y=SCREEN_H/2;
    player.live=true;

    s_Object enemy1[ENEMY1_MAX];
    for(i=0; i<ENEMY1_MAX; i++)
    {
        enemy1[i].x=(i%13)*32+45;
        enemy1[i].y=(i/13)*32+116;
        enemy1[i].live=true;
    }

    s_Object enemybullet[ENEMY_BULLETS_MAX];
    for( i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet[1].x=i;
        enemybullet[1].y=i+5;
        enemybullet[i].live=false;
    }

    spr_anim playersheet;
    playersheet.maxFrame=8;
    playersheet.curFrame=0;
    playersheet.frameCount=0;
    playersheet.frameDelay=5;
    playersheet.frameWidth=36;
    playersheet.frameHeight=36;

    int enemybulletcount=0;
    int gamestate=0;
    bool quit=false;




    for(i=0; i<KEY_MAX; i++)
    {
        keys[i]=false;

    }

    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    display=al_create_display(SCREEN_W, SCREEN_H);

    img_player=al_load_bitmap("sprites/player.png");
    img_enemy1=al_load_bitmap("sprites/enemy1a.png");
    img_enemyBullet=al_load_bitmap("sprites/enemybullet1a.png");

    al_convert_mask_to_alpha(img_player, al_map_rgb(255, 0, 255));

    al_reserve_samples(10);

    fonte=al_load_ttf_font("fonts/Joystix.ttf", 24, 0);

    event_queue=al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(enemytimer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    al_start_timer(enemytimer);

    while(!quit)
    {
        switch(gamestate)
        {
        case 0:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                {
                    gamestate=1;
                }
            }

            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-30, ALLEGRO_ALIGN_CENTRE, "Drug Dibre!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "Press space to play!");
                    al_flip_display();
                }
            }

            break;
        case 1:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            {
                quit=true;
            }

            if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(ev.keyboard.keycode)
                {

                case ALLEGRO_KEY_DOWN:
                    keys[KEY_DOWN]=true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[KEY_UP]=true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[KEY_LEFT]=true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[KEY_RIGHT]=true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    keys[KEY_ESCAPE]=true;
                    break;
                }
            }


            if(ev.type==ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {

                case ALLEGRO_KEY_DOWN:
                    keys[KEY_DOWN]=false;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[KEY_UP]=false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[KEY_LEFT]=false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[KEY_RIGHT]=false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    keys[KEY_ESCAPE]=false;
                    break;
                }
            }

            else if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(++playersheet.frameCount>=playersheet.frameDelay)
                {
                    if(++playersheet.curFrame>=playersheet.maxFrame)
                        playersheet.curFrame=0;

                    playersheet.frameCount=0;
                }

            }




            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {

                    if(rand() % ENEMY_CHANCE==0 || rand() % ENEMY_CHANCE==1 || rand() % ENEMY_CHANCE==2)
                    {
                        int coluna;
                        coluna=rand()%11;
                        enemyshot(&enemy1[coluna+11], enemybullet, &enemybulletcount);

                    }



                    if(keys[KEY_DOWN])
                    {
                        player.y+=5;
                    }
                    if(keys[KEY_UP])
                    {
                        player.y-=5;
                    }
                    if(keys[KEY_LEFT])
                    {
                        player.x-=5;
                    }
                    if(keys[KEY_RIGHT])
                    {
                        player.x+=5;
                    }

                    for(i=0; i<ENEMY_BULLETS_MAX; i++)
                    {
                        if(enemybullet[i].live)
                        {
                            enemybullet[i].y+=5;


                            if(enemybullet[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet[i].x+al_get_bitmap_width(img_enemyBullet) &&
                                    (enemybullet[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet[i].y+al_get_bitmap_height(img_enemyBullet)))
                            {
                                enemybullet[i].live=false;
                                enemybulletcount--;
                                playerlives--;
                            }

                            if(playerlives<=0)
                            {
                                gamestate=2;
                            }

                        }
                    }

                    player_collision_wall(&player, img_player);

                    al_clear_to_color(al_map_rgb(0, 0, 0));

                    al_draw_bitmap_region(img_player, playersheet.curFrame*playersheet.frameWidth, 0, playersheet.frameWidth, playersheet.frameHeight, player.x, player.y, 0);


                    for(i=0; i<ENEMY1_MAX; i++)
                    {
                        if(enemy1[i].live)
                        {

                            al_draw_bitmap(img_enemy1, enemy1[i].x, enemy1[i].y, 0);
                        }

                    }


                    for(i=0; i<ENEMY_BULLETS_MAX; i++)
                    {
                        if(enemybullet[i].live)
                        {
                            al_draw_bitmap(img_enemyBullet, enemybullet[i].x, enemybullet[i].y, 0);
                        }

                    }

                    al_flip_display();
                }


            }
            break;

        case 2:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                {
                    quit=true;
                }
            }

            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_textf(fonte, al_map_rgb(255,0,255), SCREEN_W/2, SCREEN_H/2-30, ALLEGRO_ALIGN_CENTRE, "Game Over!");
                    al_draw_textf(fonte, al_map_rgb(255,0,255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "Nem clicou!");
                    al_flip_display();
                }
            }

            break;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_timer(enemytimer);
    al_destroy_bitmap(img_player);
    al_destroy_bitmap(img_enemy1);



    return 0;
}
