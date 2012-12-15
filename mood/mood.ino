#define RED 9
#define GREEN 10
#define BLUE 11

#define STEP 5

struct light {
	int pin;
	int val;
};

struct light lights[] = {
	{RED, 0},
	{GREEN, 0},
	{BLUE, 0},
};

#define NUM_LIGHTS (sizeof(lights) / sizeof(*lights))

// The light being faded out
static int out = 1;

// The light being faded in
static int in = 0;

void setup() {
	for (int i = 0; i < NUM_LIGHTS; i++) {
		pinMode(lights[i].pin, OUTPUT);
	}
}

void loop() {
	while (lights[in].val < 255) {
		lights[in].val += STEP;
		
		if (lights[out].val > 0) {
			lights[out].val -= STEP;
		}
		
		for (int i = 0; i < NUM_LIGHTS; i++) {
			analogWrite(lights[i].pin, lights[i].val);
		}
		
		delay(100);
	}
	
	int to = random(0, 2);
	for (int i = 0; i < NUM_LIGHTS; i++) {
		if (i != in && to-- == 0) {
			out = in;
			in = i;
			break;
		}
	}
}