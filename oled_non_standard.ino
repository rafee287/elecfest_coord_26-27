# include <Wire.h>
# include <Adafruit_GFX.h>
# include <Adafruit_SSD1306.h>
# include <Fonts/FreeSansOblique9pt7b.h> 

int w = 128;
int h = 64;

# define OLED_Addr 0x3C

Adafruit_SSD1306 display(w,h,&Wire,-1);

void setup() {

  Wire.begin();
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_Addr))
  {
    Serial.println("oled not detected");
    while(true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setFont(&FreeSansOblique9pt7b);
  display.setCursor(10,18);
  display.print("Elecfest to");
  display.setCursor(10,36);
  display.print("the universe... ");
  display.setCursor(10,54);
  display.print("hello");

  for(int i =4; i<125 ; i++){
    display.drawPixel(i, 4, SSD1306_WHITE);  
    display.drawPixel(i, 60, SSD1306_WHITE);  
    display.drawPixel(i, 3, SSD1306_WHITE);  
    display.drawPixel(i, 61, SSD1306_WHITE);  
  }
  for(int i =4; i<61 ; i++){
    display.drawPixel(4, i, SSD1306_WHITE);  
    display.drawPixel(124, i, SSD1306_WHITE);
    display.drawPixel(3, i, SSD1306_WHITE);  
    display.drawPixel(125, i, SSD1306_WHITE);  
  }
  display.display();
}

void loop() {


}