#define RED 3
#define GREEN 5
#define BLUE 6

#define MOOD_STEP 5

#define SUCCESS {0, 0, 255}
#define UNSTABLE {255, 255, 0}
#define FAILURE {155, 0, 0}

struct color {
	int red;
	int green;
	int blue;
};

// The color the light should be set to: call update_light() to set it
static struct color light;
static struct color last_status = SUCCESS;

// If the last build was successful: determines if we go into mood mode
static int successful = 1;

static void update_light() {
	analogWrite(RED, light.red < 0 ? 0 : light.red);
	analogWrite(GREEN, light.green < 0 ? 0 : light.green);
	analogWrite(BLUE, light.blue < 0 ? 0 : light.blue);
}

/**
 * Fades the status of the last build, like the little status indicators on
 * the Jenkins dashboard
 */
static void fade() {
	struct color status = light = last_status;
	struct color delta = {status.red / 15, status.green / 15, status.blue / 15};
	
	int dir = -1;
	int steps = 10;
	while (Serial.available() == 0) {
		light.red += dir * delta.red;
		light.green += dir * delta.green;
		light.blue += dir * delta.blue;
		update_light();
		
		if (steps-- == 0) {
			dir = -dir;
			steps = 10;
		}
		
		delay(50);
	}
}

static void mood() {
	static int fading[] = {255, 0, 0};
	static int out = 0;
	static int in = 1;
	
	fading[in] += MOOD_STEP;
	if (fading[out] > 0) {
		fading[out] -= MOOD_STEP;
	}
	
	// If we're at the end, pick a new direction
	if (fading[out] == 0) {
		int to = random(0, 2);
		for (int i = 0; i < 3; i++) {
			if (i != in && to-- == 0) {
				out = in;
				in = i;
				break;
			}
		}
	}
	
	light.red = fading[0];
	light.green = fading[1];
	light.blue = fading[2];
	update_light();
}

void setup() {
	Serial.begin(9600);
	
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
}

void loop() {
	if (Serial.available() != 0) { // New status update
		switch (Serial.read()) {
			case '0':
				fade();
				return;
			case 'S':
				last_status = SUCCESS;
				successful = 1;
				break;
			case 'U':
				last_status = UNSTABLE;
				successful = 0;
				break;
			case 'F':
				last_status = FAILURE;
				successful = 0;
				break;
		}
		
		light = last_status;
		update_light();
		delay(5000);
	} else if (successful) { // If that last build was sucessful, go into "mood" mode
		mood();
		delay(100);
	} else { // The last build failed, and there are no updates, so don't change the color
		delay(100);
	}
}