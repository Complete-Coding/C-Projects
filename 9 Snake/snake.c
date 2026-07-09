#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<time.h>

#define WIDTH 90
#define HEIGHT 25

int score;
int tail_length;
int tail_x[1000], tail_y[1000];
int head_x, head_y;
int fruit_x, fruit_y;
int paused;
int bonus_fruit_x, bonus_fruit_y;
int bonus_active;
int bonus_timer;
int wall_mode;

enum Direction{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	STOP
};
enum Direction dir;

void clear_screen();
struct termios oldt;
void set_terminal_attributes();
void reset_terminal_attributes();
int input_available();
void setup_values();
void draw();
void input();
void game_play();
void restart_game();
void menu();

int main(){
	srand(time(NULL));
	set_terminal_attributes();
	menu();
	
	while(1) {
	draw();
	input();
	
	if(!paused){
		game_play();
	} else {
        printf("\nGame Paused. Press 'p' to resume.\n");
    }

	int sleep_time = 3000000 / (score != 0 ? score : 10); 
	usleep(sleep_time);	
}
	return 0;
}

void input(){
	if(input_available()){
		char ch = getchar();
		
		switch(ch){
			case 'a':
				dir = LEFT;
			break;
			case 's':
				dir = DOWN;
			break;
			case 'd':
				dir = RIGHT;
			break;
			case 'w':
				dir = UP;
			break;
			case 'p':
				paused = !paused;
			break;	
			case 'x':
				exit(0);
			    break;
			default:
			break;
		}
	}
}

void game_play(){
	
	for(int i = tail_length; i > 0; i--) {
		tail_x[i] = tail_x[i-1];
		tail_y[i] = tail_y[i-1];
	}
	
	tail_x[0] = head_x;
	tail_y[0] = head_y;
	
	switch(dir){
		case UP:
			head_y--;
		break;
		case DOWN:
			head_y++;
		break;
		case LEFT:
			head_x--;
		break;
		case RIGHT:
			head_x++;
		break;
		case STOP:
			
		break;
	}
	
	if(wall_mode == 1) {
		if(head_x < 0 || head_x >= WIDTH || head_y < 0 || head_y >= HEIGHT){
			   printf("\nGame over. You hit the wall!\n\n");
			   restart_game();
		}
	}
	 else {
	 	
		if(head_x < 0) {
		head_x = WIDTH - 1;
	} else if(head_x >= WIDTH) {
		head_x = 0;
	}
	
		if(head_y < 0) {
		head_y = HEIGHT - 1;
	} else if(head_y >= HEIGHT) {
		head_y = 0;
		}
	
	}
	
	for(int i = 0; i < tail_length; i++){
		if(tail_x[i] == head_x && tail_y[i] == head_y){
			printf("\nGame over. You hit your tail!\n\n");
			restart_game();
		}
	}
	
	if(head_x == fruit_x && head_y == fruit_y){
		
		score = score + 10;
		
		if(tail_length < 999){
			tail_length++;
		}
		
		fruit_x = rand() % WIDTH;
		fruit_y = rand() % HEIGHT;
	}
	
	if(bonus_active){
        bonus_timer--;
        if(bonus_timer <= 0){
            bonus_active = 0;
        } else if(head_x == bonus_fruit_x && head_y == bonus_fruit_y){
            score += 50;
            if(tail_length < 999){
                tail_length += 3;
            }
            bonus_active = 0;
        }
    } else {
       
        if(rand() % 50 == 0){
            bonus_fruit_x = rand() % WIDTH;
            bonus_fruit_y = rand() % HEIGHT;
            bonus_timer = 50;
            bonus_active = 1;
        }
    }
    
}

void setup_values(){
	head_x = WIDTH / 2;
	head_y = HEIGHT / 2;
	
	fruit_x = rand() % WIDTH;
	fruit_y = rand() % HEIGHT;
	
	dir = STOP; 
	
	score = 0;
	tail_length = 0;
	paused = 0;
	
	bonus_active = 0;
    bonus_timer = 0;
}

void draw(){
	clear_screen();
	printf("\n");
	for(int i = 0; i < WIDTH + 2; i++){
		printf("#");
	}
	
	for(int i = 0; i < HEIGHT; i++) {
		printf("\n#");
		for(int j = 0; j < WIDTH; j++) {
			if(i == head_y && j == head_x){
				printf("O");
				
			} else if(i == fruit_y && j == fruit_x){
				printf("~");
			} else if(bonus_active && i == bonus_fruit_y && j == bonus_fruit_x){
    			printf("@");
			}
			else {
				int tail_found = 0;
				for(int k = 0; k < tail_length; k++){
					if(tail_x[k] == j && tail_y[k] == i) {
						printf("o");
						tail_found = 1;
						break;
					}
				}
				if(!tail_found){
					printf(" ");
				}
			
			}
		}
		
		printf("#");
	}
	
		printf("\n");
		for(int i = 0; i < WIDTH + 2; i++){
		printf("#");
	}

	
	printf("\nScore : %d\n\n", score);
}

void restart_game(){
	char choice;
	printf("Press 'r' to restart or 'x' to exit: ");
	
	while(1){
		choice = getchar();
			if(choice == 'r'){
				setup_values();
				break;
			} else if (choice == 'x'){
				exit(0);
		}
	}
}

void menu() {
	clear_screen();

 	printf("=============================================\n");
    printf("\t\tWelcome to Snake Game\n");
    printf("=============================================\n");
    
    printf("1. Start Game\n");
    printf("2. Instructions\n");
    printf("3. Toggle Wall Mode (Currently %s)\n", wall_mode ? "ON" : "OFF");
    printf("4. Exit\n");
    printf("Enter choice: ");
    
    char ch = getchar();
//    getchar();
    
    switch(ch){
    	case '1':
    	setup_values();
            break;
    	case '2':
    		clear_screen();
            printf("Instructions:\n");
            printf("- Use W A S D to move\n");
            printf("- Eat ~ for fruit (+10 points)\n");
            printf("- Eat @ for bonus (+50 points, +3 length)\n");
            printf("- Press P to pause\n");
            printf("- Press X to exit\n");
            printf("- Avoid hitting walls (if wall mode ON) or yourself\n");
            printf("Press any key to return to menu...");
            getchar();
            menu();
            break;
            
        case '3':
        	wall_mode = !wall_mode;
        	printf("\nWall mode set to %s\n", wall_mode ? "ON" : "OFF");
        	printf("\nPress any key to return to menu...");
            getchar();
        	menu();
            break;
            
        case '4':
        	exit(0);
        	break;
        	
        default:
        	printf("Invalid choice. Try again.\n");
            menu();
            break;
    }
}

int input_available(){
	struct timeval tv = { 0L , 0L };
	fd_set fds;
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

void set_terminal_attributes(){
	
	tcgetattr(STDIN_FILENO, &oldt);
	atexit(reset_terminal_attributes);
	struct termios newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); 
	
}


void reset_terminal_attributes(){
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void clear_screen(){
	#ifdef _WIN32
		system("cls");
	#else 
		system("clear");	
	#endif
}
