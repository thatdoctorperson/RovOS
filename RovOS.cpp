#include <allegro.h>
#include <iostream>
#include <ctime>
using  namespace std;

#define screenW 640
#define screenH 480
#define WHITE makecol(255, 255, 255)
#define DRED makecol(150, 0, 0)


volatile long fpsLock = 0;
volatile long ticks = 0;
void fps_lock(){
    fpsLock++;
}
END_OF_FUNCTION(fps_lock);

void init();
void deinit();

int main() {
	init();

	srand(time(NULL));
	BITMAP *buffer = create_bitmap(screenW, screenH);
	BITMAP *mouth = load_bitmap("mouth.bmp", NULL);
	SAMPLE *speech = load_sample("speech.wav");
	if (speech == NULL)
		cout << "sound fail" << endl;
	
	BITMAP *tmp = create_bitmap(screenW / 1.88235294118 * 5, screenH / 2.82352941176);
	stretch_blit(mouth, tmp, 0, 0, 1700, 170, 0, 0, screenW / 1.88235294118 * 5, screenH / 2.82352941176);
	mouth = tmp;

	int load = 0;
	
	while (!key[KEY_ESC] && !key[KEY_X]) {
		while(fpsLock > 0 && !key[KEY_ESC] && !key[KEY_X]){
			rectfill(buffer, load + screenW / 6, screenH / 2 - screenH / 96, load + screenW / 6 + 1, 
				screenH / 2 + screenH / 96, WHITE);
				
			blit(buffer, screen, 0, 0, 0, 0, screenW, screenH);
			
			load++;
			
			fpsLock--;
		}
		if(load >= screenW * 0.6666)
			break;
	}
	
	clear_to_color(buffer, DRED);
	
	rectfill(buffer, screenW / 4.26666666667, screenH / 4.8, screenW / 4.26666666667 + screenW / 16, 
		screenH / 4.8 + screenH / 12, WHITE);
	rectfill(buffer, screenW / 1.42222222222, screenH / 4.8, screenW / 1.42222222222 + screenW / 16, 
		screenH / 4.8 + screenH / 12, WHITE);
	rectfill(buffer, screenW / 4.26666666667, screenH / 1.92, screenW / 1.42222222222 + screenW / 16, 
		screenH / 1.92 + screenH / 2.82352941176, WHITE);
	blit(buffer, screen, 0, 0, 0, 0, screenW, screenH);
	
	int spriteNum = 0;
	int increment = 1;
	int speechTick = 0;
	bool speaking = true;
	
	while (!key[KEY_ESC] && !key[KEY_ENTER]) {
		while(fpsLock > 0 && !key[KEY_ESC] && !key[KEY_ENTER]){
			if(speechTick == 0)
				play_sample(speech, 255, 127, 1000, false);
				
			if(speaking){
				if(spriteNum == 4)
					spriteNum = 0;
				if(ticks == rand() % 2 + 2){
					spriteNum += increment;
					if(spriteNum >= 3)
						increment = -1;
					else if (spriteNum <= 0)
						increment = 1;
				}
			}else{
				spriteNum = 4;
			}
			
			blit(mouth, buffer, spriteNum * screenW / 1.88235294118, 0, screenW / 4.26666666667, screenH / 1.92, 
				screenW / 1.88235294118 + 3, screenH / 2.82352941176);
				
			blit(buffer, screen, 0, 0, 0, 0, screenW, screenH);
			
			if(ticks >= 5)
				ticks = 0;
				
			ticks++;
			fpsLock--;
			speechTick++;
			
			if(speechTick > 180)
				speaking = false;
			if(speechTick > 360){
				speechTick = 0;
				speaking = true;
			}
		}
	}
	
	if(key[KEY_ENTER]){
		cout << "INSERT REST OF CODE HERE";
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT, screenW, screenH, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	
	install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0 );
	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
	
	LOCK_VARIABLE(fpsLock);
	LOCK_FUNCTION(fps_lock);
	install_int_ex(fps_lock, BPS_TO_TIMER(60));
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
