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

static void randomize() {
	int dir = lights[master].dir = random(2, 6);
	
	// Ensure that both other lights will change at the same time
	int reverse_dir = random(1, dir);
	
	int dirs[] = { -reverse_dir, -(dir - reverse_dir) };
	int which = 0;
	
	for (int i = 0; i < 3; i++) {
		if (i != master) {
			lights[i].dir = dirs[which++];
		}
	}
}

void setup() {
	// Serial.begin(9600);
	randomize();
	
	// analogWrite(RED, 102);
	// analogWrite(GREEN, 0);
	// analogWrite(BLUE, 60);
}

void loop() {
	int new_master = -1;
	
	// Serial.print("Vals: ");
	// for (int i = 0; i < 3; i++) {
	// 	Serial.print(" (");
	// 	Serial.print(lights[i].dir);
	// 	Serial.print(", ");
	// 	Serial.print(lights[i].val);
	// 	Serial.print(")");
	// }
	// Serial.println();
	
	for (int i = 0; i < 3; i++) {
		analogWrite(lights[i].pin, lights[i].val);
		lights[i].val += lights[i].dir;
		
		if (lights[i].val < 35) {
			new_master = i;
		}
	}
	
	if (new_master != -1) {
		master = new_master;
		randomize();
	}
	
	delay(100);
}