/*******************************************************
Instituto Federal de Santa Catarina
Alunos: Bruno Mengarda e Pedro Ruschel
Turma: 722
Disciplina: Programação C
Professor: Fernando Pacheco
Jogo: Drug Dibre
*********************************************************/
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

//Definição das teclas
enum
{
    KEY_RIGHT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_P,
    KEY_I,
    KEY_MAX

};

bool keys[KEY_MAX];

//Estrutura geral para player, inimigos e tiros
typedef struct
{
    int x;
    int y;
    bool live;

} s_Object;

//Estrutura geral para animações
typedef struct
{
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;

} spr_anim;

//Função para o tiro dos inimigos
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

//Função para colisão do player com a parede
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

//Função principal
int main()
{
    //Gerador Pseudo-Aleatório
    srand(time(NULL));

    //Variáveis globais
    int i;
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
    int timermusica=0;
    int randinfo=0;
    char b = '%';
    bool quit=false;

    //Argumentos para inicialização dos componentes do allegro
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
    ALLEGRO_BITMAP* img_welcome_background;
    ALLEGRO_BITMAP* img_howtoplay;
    ALLEGRO_BITMAP* img_info;
    ALLEGRO_BITMAP* img_info_weed;
    ALLEGRO_BITMAP* img_info_beer;
    ALLEGRO_BITMAP* img_info_smoke;
    ALLEGRO_BITMAP* img_info_lsd;

    ALLEGRO_SAMPLE* gamesound;
    ALLEGRO_SAMPLE* lifeup;
    ALLEGRO_SAMPLE* darude;
    ALLEGRO_SAMPLE* wasted;

    ALLEGRO_SAMPLE_INSTANCE* gamesoundInstance;
    ALLEGRO_SAMPLE_INSTANCE* lifeupInstance;
    ALLEGRO_SAMPLE_INSTANCE* darudeInstance;
    ALLEGRO_SAMPLE_INSTANCE* wastedInstance;

    ALLEGRO_FONT* fonte;

    //Estrutura do player
    s_Object player;
    player.x=SCREEN_W/2;
    player.y=SCREEN_H/2;
    player.live=true;

    //Estrutura do inimigo
    s_Object enemy1[ENEMY1_MAX];
    for(i=0; i<ENEMY1_MAX; i++)
    {
        enemy1[i].x=(i%11)*32+76;
        enemy1[i].y=(i/11);
        enemy1[i].live=true;
    }

    /*-------------------------------------------------------------------------*/

    //Estrutura do primeiro tipo de tiro
    s_Object enemybullet[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet[i].x=i;
        enemybullet[i].y=i+5;
        enemybullet[i].live=false;
    }

    //Estrutura do segundo tipo de tiro
    s_Object enemybullet1[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet1[i].x=i;
        enemybullet1[i].y=i+5;
        enemybullet1[i].live=false;
    }

    //Estrutura do terceiro tipo de tiro
    s_Object enemybullet2[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet2[i].x=i;
        enemybullet2[i].y=i+5;
        enemybullet2[i].live=false;
    }

    //Estrutura do quarto tipo de tiro
    s_Object enemybullet3[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet3[i].x=i;
        enemybullet3[i].y=i+5;
        enemybullet3[i].live=false;
    }

    //Estrutura do quinto tipo de tiro
    s_Object enemybullet4[ENEMY_BULLETS_MAX];
    for(i=0; i<ENEMY_BULLETS_MAX; i++)
    {
        enemybullet4[i].x=i;
        enemybullet4[i].y=i+5;
        enemybullet4[i].live=false;
    }

    //Estrutura do player andando
    spr_anim playermove;
    playermove.maxFrame=2;
    playermove.curFrame=0;
    playermove.frameCount=0;
    playermove.frameDelay=10;
    playermove.frameWidth=36;
    playermove.frameHeight=36;

    //estrutura do player parado
    spr_anim playeridle;
    playeridle.maxFrame=2;
    playeridle.curFrame=0;
    playeridle.frameCount=0;
    playeridle.frameDelay=30;
    playeridle.frameWidth=35;
    playeridle.frameHeight=35;

    //estrutura da animação inimigo
    spr_anim enemymove;
    enemymove.maxFrame=2;
    enemymove.curFrame=0;
    enemymove.frameCount=0;
    enemymove.frameDelay=400;
    enemymove.frameWidth=35;
    enemymove.frameHeight=35;

    //Laço para a utilização de teclas
    for(i=0; i<KEY_MAX; i++)
    {
        keys[i]=false;

    }

    //Inicialização do allegro, addons, teclado, etc
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //Cria o display
    display=al_create_display(SCREEN_W, SCREEN_H);

    //Cria o timer
    timer = al_create_timer(1/FPS);

    //Carregamento das imagens, áudios e fontes
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
    img_welcome_background=al_load_bitmap("sprites/intro.png");
    img_howtoplay=al_load_bitmap("sprites/howtoplay.png");
    img_info=al_load_bitmap("sprites/info.png");
    img_info_weed=al_load_bitmap("sprites/info_weed.png");
    img_info_beer=al_load_bitmap("sprites/info_beer.png");
    img_info_smoke=al_load_bitmap("sprites/info_smoke.png");
    img_info_lsd=al_load_bitmap("sprites/info_lsd.png");

    gamesound=al_load_sample("sounds/songdefault.wav");
    lifeup=al_load_sample("sounds/1up.wav");
    darude=al_load_sample("sounds/darude.wav");
    wasted=al_load_sample("sounds/wasted.wav");

    fonte=al_load_ttf_font("fonts/SHOWG.ttf", 24, 0);

    //Correção do fundo dos sprites
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
    al_convert_mask_to_alpha(img_enemyBullet4, al_map_rgb(255, 0, 255));

    //Define quantos áudios serão tocados ao mesmo tempo
    al_reserve_samples(10);

    //Instâncias de áudio
    gamesoundInstance=al_create_sample_instance(gamesound);
    al_set_sample_instance_playmode(gamesoundInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(gamesoundInstance, al_get_default_mixer());

    lifeupInstance=al_create_sample_instance(lifeup);
    al_set_sample_instance_playmode(lifeupInstance, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(lifeupInstance, al_get_default_mixer());

    darudeInstance=al_create_sample_instance(darude);
    al_set_sample_instance_playmode(darudeInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(darudeInstance, al_get_default_mixer());

    wastedInstance=al_create_sample_instance(wasted);
    al_set_sample_instance_playmode(wastedInstance, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(wastedInstance, al_get_default_mixer());

    //Cria a fila de eventos
    event_queue=al_create_event_queue();

    //Registra as fontes dos eventos como display, timer e teclado
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //inicializa o timer
    al_start_timer(timer);

    //Laço principal
    while(!quit)
    {
        //Switch para trocar de menu para jogo, jogo para menu, etc
        switch(gamestate)
        {

        //Tela inicial
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
                if(ev.keyboard.keycode==ALLEGRO_KEY_I)
                {
                    gamestate=4;
                }
            }

            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_play_sample_instance(gamesoundInstance);
                    al_draw_bitmap(img_welcome_background, 0, 0, 0);
                    al_flip_display();
                }
            }

            break;

        //Tela do jogo
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
                case ALLEGRO_KEY_I:
                    keys[KEY_I]=true;
                    break;
                case ALLEGRO_KEY_ENTER:
                    keys[KEY_ENTER]=true;
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
                case ALLEGRO_KEY_I:
                    keys[KEY_I]=false;
                    break;
                case ALLEGRO_KEY_ENTER:
                    keys[KEY_ENTER]=false;
                    break;
                }
            }
            /*-------------------------------------------------------------------------*/

            //Frequência de disparo das drogas
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
                    //Laços para randomização dos disparos
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

                    //Timer para os efeitos das drogas
                    timermaconha++;
                    timercachaca++;
                    timerlsd++;
                    randinfo=rand()%4;

                    //Efeito das drogas
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

                    //Colisão das drogas com o player
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

                    if (enemybulletcount > maconha)
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

                    if (enemybulletcount > velocidade_vida )
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
                                    timermusica=1;

                                }
                            }
                        }
                    }
                    /*-------------------------------------------------------------------------*/

                    if (enemybulletcount > cachaca)
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

                    //Game Over
                    if(playerlives<=0)
                    {
                        gamestate=2;
                        al_play_sample_instance(wastedInstance);
                    }

                    /*-------------------------------------------------------------------------*/

                    player_collision_wall(&player, img_player);

                    //Desenha o bitmap de fundo
                    if(!efeitolsd==1)
                    {
                        al_draw_bitmap(img_street, 0, 0, 0);
                    }

                    //Desenha a vida e a pontuação na tela
                    al_draw_textf(fonte, al_map_rgb(0, 100, 255), 90, 750, 0, "Vida: %d%c", playerlives,b);

                    al_draw_textf(fonte, al_map_rgb(255, 0, 255), 90, 780, 0, "Score: %d", score);

                    //Toca a música de fundo
                    if(!timermusica==1)
                    {
                        al_play_sample_instance(gamesoundInstance);
                    }

                    if(timermusica==1)
                    {
                        al_stop_sample_instance(gamesoundInstance);
                        al_play_sample_instance(darudeInstance);
                    }

                    /*-------------------------------------------------------------------------*/

                    //Movimentação dos inimigos
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

                    //Definição se o player está parado ou andando
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

                    //Animação do player
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

                    /*-------------------------------------------------------------------------*/

                    //Desenha a droga quando a mesma é atirada
                    for(i=0; i<ENEMY_BULLETS_MAX; i++)
                    {
                        if(enemybullet[i].live)
                        {
                            al_draw_bitmap(img_enemyBullet, enemybullet[i].x, enemybullet[i].y, 0);
                        }

                    }

                    /*-------------------------------------------------------------------------*/

                    if (enemybulletcount > maconha)
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

                    if (enemybulletcount > patolino)
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

                    if (enemybulletcount > patolino)
                    {
                        for(i=0; i<ENEMY_BULLETS_MAX; i++)
                        {
                            if(enemybullet3[i].live)
                            {
                                al_draw_bitmap(img_enemyBullet3, enemybullet3[i].x, enemybullet3[i].y, 0);
                            }
                        }
                    }

                    if (enemybulletcount > 1)
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

        //Game over
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
                    al_stop_sample_instance(gamesoundInstance);
                    al_stop_sample_instance(darudeInstance);
                    al_draw_bitmap(img_background_dead, 0, 0, 0);
                    al_draw_bitmap(img_player_dead, SCREEN_W/2-20, SCREEN_H/2-15, ALLEGRO_ALIGN_CENTER);
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+15, ALLEGRO_ALIGN_CENTRE, "Game Over!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+45, ALLEGRO_ALIGN_CENTRE, "As drogas te consumiram!");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+75, ALLEGRO_ALIGN_CENTRE, "Sua pontuacao:%d", score);
                    al_flip_display();
                }
            }

            break;

        //Jogo pausado
        case 3:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            {
                quit=true;
            }
            if(ev.keyboard.keycode==ALLEGRO_KEY_P || ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
            {
                gamestate=1;
            }
            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    //Desenha as informações sobre drogas quando o jogo é pausado
                    if(randinfo==0)
                    {
                        al_draw_bitmap(img_info_beer, 0, 0, 0);
                    }
                    else if(randinfo==1)
                    {
                        al_draw_bitmap(img_info_weed, 0, 0, 0);
                    }
                    else if(randinfo==2)
                    {
                        al_draw_bitmap(img_info_lsd, 0, 0, 0);
                    }
                    else if(randinfo==3)
                    {
                        al_draw_bitmap(img_info_smoke, 0, 0, 0);
                    }

                    al_stop_sample_instance(gamesoundInstance);
                    al_stop_sample_instance(darudeInstance);
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+300, ALLEGRO_ALIGN_CENTRE, "Paused");
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+325, ALLEGRO_ALIGN_CENTRE, "Aperte espaco ou P para continuar");
                    al_flip_display();
                }
            }
            break;

        //Tela de instruções
        case 4:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            {
                quit=true;
            }
            if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
            {
                gamestate=5;
            }
            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_draw_bitmap(img_howtoplay, 0, 0, 0);
                    al_flip_display();
                }
            }
            break;

        //Tela de informação sobre o jogo
        case 5:
            al_wait_for_event(event_queue, &ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE || ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            {
                quit=true;
            }
            if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
            {
                gamestate=1;
            }
            if(ev.type==ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source==timer)
                {
                    al_draw_bitmap(img_info, 0, 0, 0);
                    al_flip_display();
                }
            }
            break;

        }
    }

    //Destrói os componentes inicializados
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
    al_destroy_bitmap(img_background_dead);
    al_destroy_bitmap(img_welcome_background);
    al_destroy_bitmap(img_howtoplay);
    al_destroy_bitmap(img_info);
    al_destroy_bitmap(img_info_weed);
    al_destroy_bitmap(img_info_beer);
    al_destroy_bitmap(img_info_smoke);
    al_destroy_bitmap(img_info_lsd);
    al_destroy_sample(gamesound);
    al_destroy_sample(lifeup);
    al_destroy_sample(darude);
    al_destroy_sample(wasted);
    al_destroy_sample_instance(gamesoundInstance);
    al_destroy_sample_instance(lifeupInstance);
    al_destroy_sample_instance(darudeInstance);
    al_destroy_sample_instance(wastedInstance);

    return 0;
}
