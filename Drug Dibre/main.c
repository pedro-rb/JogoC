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
#define ENEMY1_MAX 11
#define ENEMY_BULLETS_MAX 20000
#define ENEMY_CHANCE 100
#define FPS 60.0
#define lives 100

enum
{
    KEY_RIGHT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_P,
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

} spr_anim;


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
    if(player->x < 76)
    {
        player->x = 76;
    }
    if(player->y < 36)
    {
        player->y = 36;
    }
    if(player->x > (SCREEN_W-70) - al_get_bitmap_width(img_player))
    {
        player->x = (SCREEN_W-70) - al_get_bitmap_width(img_player);
    }
    if(player->y > (SCREEN_H-52) - al_get_bitmap_height(img_player))
    {
        player->y = (SCREEN_H-52) - al_get_bitmap_height(img_player);
    }
}

int main()
{
    srand(time(NULL));

    int i;
    int k=lives;
    int j=0;
    int velocidade_tiro=50;
    int enemybulletcount=0;
    int gamestate=0;
    int playerstate=0;
    int playerlives=lives;
    int maconha=30;
    int patolino=60;
    int cachaca=90;
    int score=0;
    int efeitomaconha=0;
    int timermaconha=0;
    int efeitocachaca=0;
    int timercachaca=0;
    int efeitolsd=0;
    int timerlsd=0;
    int velocidade=3;
    int velocidade_vida=60;
    char b = '%';
    bool quit=false;

    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_EVENT ev;

    ALLEGRO_BITMAP* img_player;
    ALLEGRO_BITMAP* img_enemy1;
    ALLEGRO_BITMAP* img_enemyBullet;
    ALLEGRO_BITMAP* img_street;
    ALLEGRO_BITMAP* img_enemyBullet1;
    ALLEGRO_BITMAP* img_enemyBullet2;
    ALLEGRO_BITMAP* img_enemyBullet3;
    ALLEGRO_BITMAP* img_enemyBullet4;
    ALLEGRO_BITMAP* img_player_sheet;
    ALLEGRO_BITMAP* img_player_idle;
    ALLEGRO_BITMAP* img_player_dead;
    ALLEGRO_BITMAP* img_background_dead;
    ALLEGRO_BITMAP* img_player_hit;
    ALLEGRO_BITMAP* img_welcome_background;

    ALLEGRO_SAMPLE* gamesound;
    ALLEGRO_SAMPLE* lifeup;

    ALLEGRO_SAMPLE_INSTANCE* gamesoundInstance;
    ALLEGRO_SAMPLE_INSTANCE* lifeupInstance;

    ALLEGRO_FONT* fonte;

    s_Object player;
    player.x=SCREEN_W/2;
    player.y=SCREEN_H/2;
    player.live=true;

    s_Object enemy1[ENEMY1_MAX];
    for(i=0; i<ENEMY1_MAX; i++)
    {
        enemy1[i].x=(i%11)*32+76;
        enemy1[i].y=(i/11)*32;
        enemy1[i].live=true;
    }

    /*-------------------------------------------------------------------------*/
    s_Object enemybullet[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet[i].x=i;
        enemybullet[i].y=i+5;
        enemybullet[i].live=false;
    }

    s_Object enemybullet1[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet1[i].x=i;
        enemybullet1[i].y=i+5;
        enemybullet1[i].live=false;
    }

    s_Object enemybullet2[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet2[i].x=i;
        enemybullet2[i].y=i+5;
        enemybullet2[i].live=false;
    }

    s_Object enemybullet3[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet3[i].x=i;
        enemybullet3[i].y=i+5;
        enemybullet3[i].live=false;
    }

    s_Object enemybullet4[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet4[i].x=i;
        enemybullet4[i].y=i+5;
        enemybullet4[i].live=false;
    }


    spr_anim playermove;
    playermove.maxFrame=2;
    playermove.curFrame=0;
    playermove.frameCount=0;
    playermove.frameDelay=10;
    playermove.frameWidth=36;
    playermove.frameHeight=36;

    spr_anim playeridle;
    playeridle.maxFrame=2;
    playeridle.curFrame=0;
    playeridle.frameCount=0;
    playeridle.frameDelay=30;
    playeridle.frameWidth=35;
    playeridle.frameHeight=35;

    spr_anim enemymove;
    enemymove.maxFrame=2;
    enemymove.curFrame=0;
    enemymove.frameCount=0;
    enemymove.frameDelay=400;
    enemymove.frameWidth=35;
    enemymove.frameHeight=35;

    spr_anim playerhit;
    playerhit.maxFrame=2;
    playerhit.curFrame=0;
    playerhit.frameCount=0;
    playerhit.frameDelay=4;
    playerhit.frameWidth=38;
    playerhit.frameHeight=38;


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

    timer = al_create_timer(1/FPS);

    img_player=al_load_bitmap("sprites/player.png");
    img_enemy1=al_load_bitmap("sprites/enemya.png");
    img_enemyBullet=al_load_bitmap("sprites/crack.png");
    img_enemyBullet1=al_load_bitmap("sprites/weed.png");
    img_enemyBullet2=al_load_bitmap("sprites/patolino.png");
    img_street=al_load_bitmap("sprites/street.png");
    img_enemyBullet3=al_load_bitmap("sprites/cachaca.png");
    img_enemyBullet4=al_load_bitmap("sprites/vida.png");
    img_player_sheet=al_load_bitmap("sprites/player_sheet.png");
    img_player_idle=al_load_bitmap("sprites/player_idle.png");
    img_player_dead=al_load_bitmap("sprites/player_dead.png");
    img_background_dead=al_load_bitmap("sprites/background_dead.png");
    img_player_hit=al_load_bitmap("sprites/player_hit.png");
    img_welcome_background=al_load_bitmap("sprites/main.png");
    gamesound=al_load_sample("sounds/songdefault.wav");
    lifeup=al_load_sample("sounds/1up.wav");

    al_convert_mask_to_alpha(img_player, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_enemy1, al_map_rgb(255, 255, 255));
    al_convert_mask_to_alpha(img_enemyBullet, al_map_rgb(0, 0, 0));
    al_convert_mask_to_alpha(img_enemyBullet1, al_map_rgb(255, 255, 255));
    al_convert_mask_to_alpha(img_enemyBullet2, al_map_rgb(255, 255, 255));
    al_convert_mask_to_alpha(img_player_sheet, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_player_idle, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_enemyBullet3, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_enemyBullet4, al_map_rgb(255, 255, 255));
    al_convert_mask_to_alpha(img_player_dead, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_player_hit, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(img_enemyBullet4, al_map_rgb(255, 0, 255));

    al_reserve_samples(2);

    gamesoundInstance=al_create_sample_instance(gamesound);
    al_set_sample_instance_playmode(gamesoundInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(gamesoundInstance, al_get_default_mixer());

    lifeupInstance=al_create_sample_instance(lifeup);
    al_set_sample_instance_playmode(lifeupInstance, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(lifeupInstance, al_get_default_mixer());

    fonte=al_load_ttf_font("fonts/SHOWG.ttf", 24, 0);

    event_queue=al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    while(!quit)
    {
        switch(gamestate)
        {
        case 0:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            {
                quit=true;
            }
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
                    al_draw_bitmap(img_welcome_background, 0, 0, 0);
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-150, ALLEGRO_ALIGN_CENTRE, "Drug Dibre!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-120, ALLEGRO_ALIGN_CENTRE, "Press space to play!");
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
            if(ev.keyboard.keycode==ALLEGRO_KEY_P)
            {
                gamestate=3;
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
                case ALLEGRO_KEY_P:
                    keys[KEY_P]=true;
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
                case ALLEGRO_KEY_P:
                    keys[KEY_P]=false;
                    break;
                }
            }
            /*-------------------------------------------------------------------------*/
            if ( 20 > enemybulletcount && enemybulletcount > 2)
            {
                velocidade_tiro = 65;
            };
            if ( 45 > enemybulletcount && enemybulletcount > 20)
            {
                velocidade_tiro = 60;
            };
            if ( 60 > enemybulletcount && enemybulletcount > 45)
            {
                velocidade_tiro = 45;
            };
            if ( 100 > enemybulletcount && enemybulletcount > 60)
            {
                velocidade_tiro = 48;
            };
            /*-------------------------------------------------------------------------*/

            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    /*-------------------------------------------------------------------------*/

                    if(rand() % velocidade_tiro == 0)
                    {
                        int coluna;
                        coluna=rand()%11;
                        enemyshot(&enemy1[coluna], enemybullet, &enemybulletcount);
                    }
                    if (enemybulletcount > maconha)
                    {
                        if(rand() % velocidade_tiro == 0)
                        {
                            int coluna;
                            coluna=rand()%11;
                            enemyshot(&enemy1[coluna], enemybullet1, &enemybulletcount);
                        }
                    }
                    if (enemybulletcount > patolino)
                    {


                        if(rand() % velocidade_tiro == 0)
                        {
                            int coluna;
                            coluna=rand()%11;
                            enemyshot(&enemy1[coluna], enemybullet2, &enemybulletcount);
                        }
                    }
                    if (enemybulletcount > cachaca)
                    {


                        if(rand() % velocidade_tiro == 0)
                        {
                            int coluna;
                            coluna=rand()%11;
                            enemyshot(&enemy1[coluna], enemybullet3, &enemybulletcount);
                        }
                    }
                    if (enemybulletcount > velocidade_vida)
                    {

                        if(rand() % 500 == 0)
                        {
                            int coluna;
                            coluna=rand()%11;
                            enemyshot(&enemy1[coluna], enemybullet4, &enemybulletcount);
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    timermaconha++;
                    timercachaca++;
                    timerlsd++;

                    if(efeitocachaca==1 && efeitomaconha==1)
                    {
                        velocidade=-1;
                        if(timermaconha && timercachaca>200)
                        {
                            efeitomaconha=0;
                            efeitocachaca=0;
                        }
                    }
                    if(efeitocachaca==1 && efeitomaconha==0)
                    {
                        velocidade=-3;
                        if(timercachaca>200)
                        {
                            efeitocachaca=0;
                            efeitomaconha=0;
                        }
                    }

                    printf("%d\n",timermaconha);
                    if(efeitomaconha==1 && efeitocachaca==0)
                    {
                        velocidade=1;
                        if(timermaconha>200)
                        {
                            efeitomaconha=0;
                            efeitocachaca=0;
                        }
                    }
                    if (efeitomaconha==0 && efeitocachaca==0)
                        velocidade=3;

                    if(keys[KEY_DOWN])
                    {
                        player.y+=velocidade;
                    }
                    if(keys[KEY_UP])
                    {
                        player.y-=velocidade;
                    }
                    if(keys[KEY_LEFT])
                    {
                        player.x-=velocidade;
                    }
                    if(keys[KEY_RIGHT])
                    {
                        player.x+=velocidade;
                    }
                    if(efeitolsd==1)
                    {
                        al_clear_to_color(al_map_rgb(rand()%255, rand()%255, rand()%255));
                        if(timerlsd>200)
                        {
                            efeitolsd=0;
                        }
                    }

                    /*-------------------------------------------------------------------------*/
                    for(i=0; i<ENEMY_BULLETS_MAX; i++)
                    {
                        if(enemybullet[i].live)
                        {
                            enemybullet[i].y+=10;

                            if(enemybullet[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet[i].x+al_get_bitmap_width(img_enemyBullet) &&
                                    (enemybullet[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet[i].y+al_get_bitmap_height(img_enemyBullet)))
                            {
                                enemybullet[i].live=false;
                                enemybulletcount--;
                                playerlives-=2;

                            }
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > maconha)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet1[i].live)
                            {
                                enemybullet1[i].y+=7;

                                if(enemybullet1[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet1[i].x+al_get_bitmap_width(img_enemyBullet1) &&
                                        (enemybullet1[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet1[i].y+al_get_bitmap_height(img_enemyBullet1)))
                                {
                                    enemybullet1[i].live=false;
                                    enemybulletcount--;
                                    playerlives-=5;
                                    efeitomaconha=1;
                                    timermaconha=0;
                                }
                            }
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > velocidade_vida )
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet4[i].live)
                            {
                                enemybullet4[i].y+=7;

                                if(enemybullet4[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet4[i].x+al_get_bitmap_width(img_enemyBullet1) &&
                                        (enemybullet4[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet4[i].y+al_get_bitmap_height(img_enemyBullet4)))
                                {
                                    enemybullet4[i].live=false;
                                    enemybulletcount--;
                                    playerlives+=10;
                                    al_play_sample_instance(lifeupInstance);
                                }
                            }
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if (enemybulletcount > patolino)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet2[i].live)
                            {
                                enemybullet2[i].y+=7;

                                if(enemybullet2[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet2[i].x+al_get_bitmap_width(img_enemyBullet2) &&
                                        (enemybullet2[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet2[i].y+al_get_bitmap_height(img_enemyBullet2)))
                                {
                                    enemybullet2[i].live=false;
                                    enemybulletcount--;
                                    playerlives-=10;
                                    efeitolsd=1;
                                    timerlsd=0;
                                }
                            }
                        }
                    }
                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > cachaca)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet3[i].live)
                            {
                                enemybullet3[i].y+=7;

                                if(enemybullet3[i].x<player.x+al_get_bitmap_width(img_player) && player.x<enemybullet3[i].x+al_get_bitmap_width(img_enemyBullet3) &&
                                        (enemybullet3[i].y<player.y+al_get_bitmap_height(img_player) && player.y<enemybullet3[i].y+al_get_bitmap_height(img_enemyBullet3)))
                                {
                                    enemybullet3[i].live=false;
                                    enemybulletcount--;
                                    playerlives-=4;
                                    efeitocachaca=1;
                                    timercachaca=0;
                                }
                            }
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if(playerlives<=0)
                    {
                        gamestate=2;
                    }

                    /*-------------------------------------------------------------------------*/

                    player_collision_wall(&player, img_player);

                    if(!efeitolsd==1)
                    {
                        al_draw_bitmap(img_street, 0, 0, 0);
                    }

                    al_draw_textf(fonte, al_map_rgb(0, 100, 255), 90, 750, 0, "Vida: %d%c", playerlives,b);

                    al_draw_textf(fonte, al_map_rgb(255, 0, 255), 90, 780, 0, "Score: %d", score);

                    al_play_sample_instance(gamesoundInstance);

                    /*-------------------------------------------------------------------------*/

                    for(i=0; i<ENEMY1_MAX; i++)
                    {
                        if(enemy1[i].live)
                        {

                            if(++enemymove.frameCount >= enemymove.frameDelay)
                            {
                                if(++enemymove.curFrame >= enemymove.maxFrame)
                                {
                                    enemymove.curFrame = 0;
                                }
                                enemymove.frameCount = 0;
                            }

                            al_draw_bitmap_region(img_enemy1, (enemymove.curFrame) * enemymove.frameWidth, 0, enemymove.frameWidth, enemymove.frameHeight, enemy1[i].x, enemy1[i].y, 0);
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if(!keys[KEY_UP] || !keys[KEY_DOWN] || !keys[KEY_LEFT] || !keys[KEY_RIGHT])
                    {
                        playerstate=0;
                    }

                    if(keys[KEY_UP] && !keys[KEY_DOWN] && !keys[KEY_LEFT] && !keys[KEY_RIGHT])
                    {
                        playerstate=1;
                    }

                    if(!keys[KEY_UP] && keys[KEY_DOWN] && !keys[KEY_LEFT] && !keys[KEY_RIGHT])
                    {
                        playerstate=2;
                    }

                    if(!keys[KEY_UP] && !keys[KEY_DOWN] && keys[KEY_LEFT] && !keys[KEY_RIGHT])
                    {
                        playerstate=3;
                    }

                    if(!keys[KEY_UP] && !keys[KEY_DOWN] && !keys[KEY_LEFT] && keys[KEY_RIGHT])
                    {
                        playerstate=4;
                    }

                    if((keys[KEY_UP] && !keys[KEY_DOWN]) && (keys[KEY_LEFT] || keys[KEY_RIGHT]))
                    {
                        playerstate=5;
                    }

                    if((!keys[KEY_UP] && keys[KEY_DOWN]) && (keys[KEY_LEFT] || keys[KEY_RIGHT]))
                    {
                        playerstate=6;
                    }

                    /*-------------------------------------------------------------------------*/

                    if(playerstate==0)
                    {
                        if(++playeridle.frameCount >= playeridle.frameDelay)
                        {
                            if(++playeridle.curFrame >= playeridle.maxFrame)
                            {
                                playeridle.curFrame = 0;
                            }
                            playeridle.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_idle, playeridle.curFrame * playeridle.frameWidth, 0, playeridle.frameWidth, playeridle.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==1)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame+2) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==2)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==3)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame+6) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==4)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame+4) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==5)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame+2) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }

                    if(playerstate==6)
                    {
                        if(++playermove.frameCount >= playermove.frameDelay)
                        {
                            if(++playermove.curFrame >= playermove.maxFrame)
                            {
                                playermove.curFrame = 0;
                            }
                            playermove.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_player_sheet, (playermove.curFrame) * playermove.frameWidth, 0, playermove.frameWidth, playermove.frameHeight, player.x, player.y, 0);
                    }


                    if(k>playerlives)
                    {
                        if(k<j)
                        {
                            if(++playerhit.frameCount >= playerhit.frameDelay)
                            {
                                if(++playerhit.curFrame >= playerhit.maxFrame)
                                {
                                    playerhit.curFrame = 0;
                                }
                                playerhit.frameCount = 0;
                            }

                            al_draw_bitmap_region(img_player_hit, (playerhit.curFrame) * playerhit.frameWidth, 0, playerhit.frameWidth, playerhit.frameHeight, player.x, player.y, 0);
                        }

                        k=playerlives;

                    }

                    /*-------------------------------------------------------------------------*/

                    for(i=0; i<ENEMY_BULLETS_MAX; i++)
                    {
                        if(enemybullet[i].live)
                        {
                            al_draw_bitmap(img_enemyBullet, enemybullet[i].x, enemybullet[i].y, 0);
                        }

                    }

                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > maconha)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet1[i].live)
                            {
                                al_draw_bitmap(img_enemyBullet1, enemybullet1[i].x, enemybullet1[i].y, 0);
                            }
                        }
                    }

                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > patolino)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet2[i].live)
                            {
                                al_draw_bitmap(img_enemyBullet2, enemybullet2[i].x, enemybullet2[i].y, 0);
                            }
                        }
                    }
                    /*-------------------------------------------------------------------------*/

                    if ( enemybulletcount > patolino)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet3[i].live)
                            {
                                al_draw_bitmap(img_enemyBullet3, enemybullet3[i].x, enemybullet3[i].y, 0);
                            }
                        }
                    }

                    if ( enemybulletcount > 1)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet4[i].live)
                            {
                                al_draw_bitmap(img_enemyBullet4, enemybullet4[i].x, enemybullet4[i].y, 0);
                            }
                        }
                    }

                    if(enemybulletcount)
                    {
                        score=enemybulletcount;
                    }

                    al_flip_display();

                }
            }
            break;

        case 2:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
                {
                    quit=true;
                }
            }

            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_draw_bitmap(img_background_dead, 0, 0, 0);
                    al_draw_bitmap(img_player_dead, SCREEN_W/2-20, SCREEN_H/2-15, ALLEGRO_ALIGN_CENTER);
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+15, ALLEGRO_ALIGN_CENTRE, "Game Over!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+45, ALLEGRO_ALIGN_CENTRE, "As drogas te consumiram!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+75, ALLEGRO_ALIGN_CENTRE, "Sua pontuacao:%d", score);
                    al_flip_display();
                }
            }

            break;

        case 3:
            al_wait_for_event(event_queue, &ev);
            if(ev.keyboard.keycode==ALLEGRO_KEY_P || ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
            {
                gamestate=1;
            }
            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_clear_to_color(al_map_rgb (0, 0, 0));
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+15, ALLEGRO_ALIGN_CENTRE, "Paused");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+45, ALLEGRO_ALIGN_CENTRE, "Aperte espaco ou P para continuar");
                    al_flip_display();
                }
            }
            break;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(img_player);
    al_destroy_bitmap(img_enemy1);
    al_destroy_bitmap(img_street);
    al_destroy_bitmap(img_player_sheet);
    al_destroy_bitmap(img_player_idle);
    al_destroy_bitmap(img_enemyBullet);
    al_destroy_bitmap(img_enemyBullet1);
    al_destroy_bitmap(img_enemyBullet2);
    al_destroy_bitmap(img_enemyBullet3);
    al_destroy_bitmap(img_enemyBullet4);
    al_destroy_bitmap(img_player_hit);
    al_destroy_bitmap(img_background_dead);
    al_destroy_bitmap(img_welcome_background);
    al_destroy_sample(gamesound);
    al_destroy_sample(lifeup);
    al_destroy_sample_instance(gamesoundInstance);
    al_destroy_sample_instance(lifeupInstance);


    return 0;
}
