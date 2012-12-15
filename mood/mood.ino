#define RED 9
#define GREEN 10
#define BLUE 11

#define MIN_LED_VAL 5

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

#define NUM_LIGHTS (sizeof(lights) / sizeof(*lights))

// The light that is currently fading up
static int master = 0;

static void randomize() {
	int dir = lights[master].dir = random(2, MIN_LED_VAL+1);
	
	// Ensure that both non-master lights will change at the same time (min == 1)
	int reverse_dir = random(1, dir);
	
	int dirs[] = { -reverse_dir, -(dir - reverse_dir) };
	int which = 0;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {
		if (i != master) {
			lights[i].dir = dirs[which++];
		}
	}
}

void setup() {
	for (int i = 0; i < NUM_LIGHTS; i++) {
		pinMode(lights[i].pin, OUTPUT);
	}
	
	randomize();
	
	analogWrite(RED, 5);
	analogWrite(GREEN, 245);
	analogWrite(BLUE, 5);
}

void loop() {
	int new_master = -1;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {
		analogWrite(lights[i].pin, lights[i].val < 0 ? 0 : lights[i].val);
		lights[i].val += lights[i].dir;
		
		if (lights[i].val < MIN_LED_VAL) {
			new_master = i;
		}
	}
	
	if (new_master != -1) {
		master = new_master;
		randomize();
	}
	
	delay(100);
}