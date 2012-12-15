#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RED 9
#define GREEN 10
#define BLUE 11

struct light {
	int pin;
	int val;
	int dir;
};

struct light lights[] = {
	{RED, 255/3, 0},
	{GREEN, 255/3, 0},
	{BLUE, 255/3, 0},
};

// The light that is currently fading up
static int master = 0;

static int random_(int min, int max) {
	return (random() % (max - min)) + min;
}

static void delay(int ms) {
	usleep(ms * 1000);
}

static void randomize() {
	int dir = lights[master].dir = random_(2, 6);
	
	// Ensure that both other lights will change at the same time
	int reverse_dir = random_(1, dir);
	
	int dirs[] = { -reverse_dir, -(dir - reverse_dir) };
	int which = 0;
	
	for (int i = 0; i < 3; i++) {
		if (i != master) {
			lights[i].dir = dirs[which++];
		}
	}
	
	// printf("Master: %d -> ", master);
	// for (int i = 0; i < 3; i++) {
	// 	printf("%d ", lights[i].dir);
	// }
	// printf("\n");
}

void setup() {
	randomize();
	
	// analogWrite(RED, 102);
	// analogWrite(GREEN, 0);
	// analogWrite(BLUE, 60);
}

void loop() {
	// printf("Vals: ");
	// for (int i = 0; i < 3; i++) {
	// 	printf("%d ", lights[i].val);
	// }
	// printf("\n");
	
	assert((lights[0].val + lights[1].val + lights[2].val) == 255);
	
	int new_master = -1;
	for (int i = 0; i < 3; i++) {
		// analogWrite(lights[i].pin, lights[i].val);
		lights[i].val += lights[i].dir;
		
		assert(lights[i].val >= 0);
		assert(lights[i].val < 220);
		
		if (lights[i].val < 35) {
			new_master = i;
		}
	}
	
	if (new_master != -1) {
		master = new_master;
		randomize();
	}
	
	// delay(100);
}

int main() {
	setup();
	while (1) {
		loop();
	}
}