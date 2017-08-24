    #include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define initlives (3)

int lives = 3; 
int score = 0;
int seconds = 0;
int secondsFront = 0;
int minuteFront = 0;
int minute = 0;
bool game_over = false;
bool exitGame; 

bool game_paused = true;
bool update_screen = true;

#define DELAY (10) 

#define SHIP_WIDTH (8)

#define SHIP_HEIGHT (5)



#define DIAMOND_WIDTH (5)
#define DIAMOND_HEIGHT (5)

#define GAME_OVER_WIDTH (33)
#define GAME_OVER_HEIGHT (8)

#define HELP_WIDTH (44)
#define HELP_HEIGHT (15)

#define NUMBER_OF_DIAMONDS (10)

int counter = NUMBER_OF_DIAMONDS;


sprite_id setup_many_diamonds(void);
void draw_border_and_status(void);
void move_diamond(sprite_id sprite);
void move_diamonds(sprite_id sprites[], int number);
void draw_helpDialog(void);
void draw_game_overDialog(void);
sprite_id collision_test(sprite_id sprite, sprite_id sprites[], int number);
bool collision( sprite_id sprite1, sprite_id sprite2 );
void setup ( void );
void draw_game( void );

char * ship_image = 
/**/   "   /\\   "
/**/   "  (  )  "
/**/   "  (  )  "
/**/   " /|/\\|\\ "
/**/   "/_||||_\\";

char * diamond_image = 
/**/   "  c  "
/**/   " ccc "
/**/   "ccccc"
/**/   " ccc "
/**/   "  c  ";

char * game_over_img = 
/**/   " ############################### "
/**/   " #                             # "
/**/   " #     The Game is over        # "
/**/   " #      Would you like to      # "
/**/   " #         play again?         # "
/**/   " ############################### "
/**/   " #            y/n              # "
/**/   " ############################### ";

char * help_img = 
/**/   " ########################################## "
/**/   " #          CAB202 Assignment 1           # "
/**/   " #            Diamonds of Doom            # "
/**/   " #        Marius Steller Imingen          # "
/**/   " #              n9884076                  # "
/**/   " ########################################## "
/**/   " #             CONTROLS:                  # "
/**/   " #              q: quit                   # "
/**/   " #              h: help                   # "
/**/   " #            Move left:  a               # "
/**/   " #            Move right: d               # "
/**/   " #       Shoot bullets: space, z, m       # "
/**/   " ########################################## "
/**/   " #       press any key to play            # "
/**/   " ########################################## ";


sprite_id ship;
sprite_id diamonds[NUMBER_OF_DIAMONDS]; 
sprite_id missile;
sprite_id game_over_msg;
sprite_id help_dialog;

timer_id timer;

void setup_spaceship( void ){

    int width = screen_width();
    int height = screen_height();

    ship = sprite_create((width - SHIP_WIDTH)/2, height - SHIP_HEIGHT - 1, SHIP_WIDTH, SHIP_HEIGHT, ship_image);
}

void move_spaceship( int c ){

    int w = screen_width();

    int sx = round(sprite_x(ship));

    if(c == 'a' && sx > 1) sprite_move(ship, -1, 0);
    if(c == 'd' && sx + SHIP_WIDTH < w -1 ) sprite_move(ship, +1, 0);
}

void setup_missile( void ){
  missile = sprite_create( round(sprite_x(ship) + 2), round(sprite_y(ship) + 50), 1, 1, "!" );
  sprite_turn_to( missile, 0.10, 0);
  sprite_turn( missile, 90);
  sprite_hide( missile );
}

void shoot_missile( void ){
    if(sprite_visible(missile) == false){
        sprite_show( missile );
    }
}

void move_missile( int c ){
    if ( sprite_visible( missile ) ){
        sprite_step( missile );
         } 
    if (sprite_y(missile) < 4 ) {
        
        sprite_hide(missile);
        
        }
    if(c == ' ' && !sprite_visible(missile)){
        sprite_move_to(missile, round( sprite_x(ship)) + 3, round(sprite_y(ship)));
        shoot_missile();
    }
    else if(c == 'z' && !sprite_visible(missile)){
        sprite_move_to(missile, round(sprite_x(ship)),  round(sprite_y(ship) + 4));
        shoot_missile();
    }

    else if(c == 'm' && !sprite_visible(missile)){
        sprite_move_to(missile, round(sprite_x(ship)) + 7,  round(sprite_y(ship) + 4));
        shoot_missile();
    }
}



void setup_diamond( void ){
    for(int i = 0; i < NUMBER_OF_DIAMONDS; i++){
        diamonds[i] = setup_many_diamonds();
    }
}

sprite_id setup_many_diamonds( void ){
    int x = screen_width() - DIAMOND_WIDTH - 3;

    int diamond_x = 1 + rand() % x;
    int diamond_y = screen_height() - screen_height() + DIAMOND_HEIGHT + 3;
    
    sprite_id diamond = sprite_create(diamond_x, diamond_y, DIAMOND_WIDTH, DIAMOND_HEIGHT, diamond_image);

    sprite_turn_to(diamond, 0.0,  0.10); 
    int angle = rand() % - 180;
    sprite_turn(diamond, angle);
    
    return diamond;

}

void move_diamonds(sprite_id sprites[], int number){
    
    for(int i = 0; i < number; i++){
        sprite_id current_sprite = sprites[i];
        move_diamond(current_sprite);
        if(collision(missile, current_sprite)){
            sprite_hide(missile);
            sprite_hide(current_sprite);
            sprite_move_to(current_sprite, screen_width(), 0);
            counter--;
            score++;
        }
    }
}

void move_diamond(sprite_id sprite){
    sprite_step(sprite);

    int x = round(sprite_x(sprite));
    int y = round(sprite_y(sprite));
    
    double dx = sprite_dx(sprite);
    double dy = sprite_dy(sprite);

    if( (x <= 0 ) || (x >= screen_width() - sprite_width(sprite)) ) dx = -dx;
    if( (y <= 4 ) || (y >= screen_height() - sprite_height(sprite)) ) dy = -dy;
    
    if(dx != sprite_dx(sprite) || dy != sprite_dy(sprite)){
        sprite_back(sprite);
        sprite_turn_to(sprite, dx, dy);
    }

}



void draw_game( void ){
    clear_screen();

    if(!game_paused && !game_over){
        draw_border_and_status();
        
        sprite_draw(ship);
        sprite_draw(missile);

        for(int i = 0; i < NUMBER_OF_DIAMONDS; i++){
            sprite_draw(diamonds[i]);
        }
    }
    else if(game_over){
        draw_game_overDialog();
    }
    else if(game_paused){
        draw_helpDialog();

    }
}

void draw_border_and_status( void ){

    int width = screen_width();
    int height = screen_height();
    
    draw_line(width - width , height - height, width - 1, height - height , '#');
    draw_line(width -1, height - height , width - 1, height - 1, '#');
    draw_line(width -  1, height - 1, width - width , height - 1, '#');
    draw_line(width - width , height - 1, width - width , height - height , '#');
    //Draw status bar
    draw_line(width - width , height - height + 2, width - 1, height - height + 2 , '#');
    //Presentation of the number of lives left
    draw_formatted(width - width + 2, height - height + 1, "Lives = %d", lives);
    //Presenting the score 
    draw_formatted(width - (width/3)*2, height - height + 1, "# Score = %d", score);
    //Elapsed time
    draw_formatted(width - width/8, height - height + 1, "#Time = %d%d:%d%d", minuteFront, minute, secondsFront, seconds);

}

void draw_game_overDialog( void ){
    int x = screen_width();
    int y = screen_height();

    game_over_msg = sprite_create(x/2 - GAME_OVER_WIDTH/2, y/2 - GAME_OVER_HEIGHT/2, 
    GAME_OVER_WIDTH, GAME_OVER_HEIGHT, game_over_img );
    sprite_draw(game_over_msg);
}

void draw_helpDialog( void ){

    int x = screen_width();
    int y = screen_height();
    help_dialog = sprite_create(x/2 - (HELP_WIDTH/2), y/2 - HELP_HEIGHT/2, 
    HELP_WIDTH, HELP_HEIGHT, help_img );
    sprite_draw(help_dialog);

}

bool collision( sprite_id sprite1, sprite_id sprite2 ){

    // bool collision = true;

    int sprite1_top = round(sprite_y(sprite1)),
    sprite1_bot = sprite1_top + sprite_height(sprite1) - 1,
    sprite1_left = round(sprite_x(sprite1)),
    sprite1_right = sprite1_left + sprite_width(sprite1) - 1;

    int sprite2_top = round(sprite_y(sprite2)),
    sprite2_bot = sprite2_top + sprite_height(sprite2) - 1,
    sprite2_left = round(sprite_x(sprite2)),
    sprite2_right = sprite2_left + sprite_width(sprite2) - 1;

    return !(
        sprite1_bot < sprite2_top
        || sprite1_top > sprite2_bot
        || sprite1_right < sprite2_left
        || sprite1_left > sprite2_right
        );


}

sprite_id collision_test(sprite_id sprite, sprite_id sprites[], int number){
    for(int i = 0; i < number; i++){
        sprite_id current_sprite = sprites[i];

        if(sprite != current_sprite && collision(current_sprite, sprite)){
            return current_sprite;
        }
    }
    return NULL;
}

/*------------------------------------------------------------------*/

void pause( void ){
    // int c = get_char();

    // if(c > 0 && c != 'q'){

    game_paused = false;   
    timer_pause(1500);
    while ( get_char() >= 0 ) {}
    wait_char();

}

void process( void ){
    int c = get_char();

    move_diamonds(diamonds, NUMBER_OF_DIAMONDS);
    move_spaceship(c); 
    
    if(timer_expired(timer)){
        if(seconds == 9){
            seconds = 0;
            secondsFront++;
        }
        if(secondsFront == 5 && seconds > 9){
            seconds = 0;
            secondsFront = 0;
            minute++;
        }
        else{
            seconds++;
        }
    }

    if(c == 'h'){
        game_paused = true; 
        
    }
    else if(c == 'q'){
        game_over = true;
    }
    
    else if(collision_test(ship, diamonds, NUMBER_OF_DIAMONDS)){
        setup_diamond(); 
        setup_spaceship();
        lives--;  
    }
    else if(collision_test(missile, diamonds, NUMBER_OF_DIAMONDS)){
        setup_missile();
        score++;
    }
    else if(lives == 0){
        game_over = true; 
        lives = 3;
    }
    else if(counter == 0){
        counter = NUMBER_OF_DIAMONDS;
        setup_diamond();
    }
    else if(game_over){
        if(c == 'n'){
            exitGame = true;
        }
        else if(c == 'y'){
            score = 0;
            game_over = false;
            clear_screen();
            setup();
            seconds = 0;
            secondsFront = 0;
            minute = 0; 
            minuteFront = 0;
            game_paused = true;
        }
    }
    move_missile(c);

    draw_game();


}

void setup ( void ){
    game_paused = true;
    setup_spaceship();   
    setup_diamond(); 
    setup_missile();
    timer = create_timer(1000);
    draw_game();
}

int main( void ){
    setup_screen();
    setup();
    show_screen();


    while(!exitGame){
        if ( update_screen && !game_paused) {
           
            process();
			show_screen();
		}

        if(game_paused && update_screen ){
            clear_screen();
            pause();
            draw_helpDialog();
            show_screen();
            
          }
          if(game_over && update_screen){
            clear_screen();
            draw_game_overDialog();
            show_screen();

          }

        timer_pause(DELAY);
    }
    exit(0);
    cleanup_screen();
    return 0;
}
