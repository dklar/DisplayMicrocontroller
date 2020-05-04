#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(180, 6, NEO_GRB + NEO_KHZ800);

volatile uint8_t* colors;

enum resultType{
	OKAY,ERROR
};

void setup() {
	 colors = (uint8_t*) malloc(3*sizeof(uint8_t)*180);
	 Serial.begin(9600);
	 pixels.begin();
}



/**
 * Sending 3 bytes each byte equals one color component
 * Problems:
 * -> No error detecting (4th Byte position?)
 *
 */
resultType readPixelWithoutError(int acceptedPos){
	enum resultType result;

	while(Serial.available()<3){}

	uint8_t r,g,b;
	b = Serial.read();
	g = Serial.read();
	r = Serial.read();
	delay(5);

	colors[acceptedPos*3+0] = r;
	colors[acceptedPos*3+1] = g;
	colors[acceptedPos*3+2] = b;


/*
	Serial.print(r,DEC);//r
	Serial.print(";");
	Serial.print(g,DEC);//g
	Serial.print(";");
	Serial.print(b,DEC);//b
	Serial.print("\n");
*/

	result = OKAY;
	return result;

}

void createErrStr(int i){
	char cstr[16];
	itoa(i,cstr,10);
	char errMSG[] ="Error ";
	char msg[9+16+1];
	strcpy(msg,errMSG);
	strcat(msg,cstr);
	Serial.println(msg);
}

void loop() {
	int i = 0;
	while(i<180){
		readPixelWithoutError(i);
		i+=1;
	}
	Serial.print("Empfangen okay\n");
	for (int x = 0;x<180;x++){
	   pixels.setPixelColor(x, pixels.Color(colors[x*3+0],colors[x*3+1],colors[x*3+2]));
	}
	pixels.show();

}
