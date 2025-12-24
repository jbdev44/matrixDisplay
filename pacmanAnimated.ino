/*
  colorduino_draw.ino
  Exemple : afficher un dessin 8x8 sur une matrice Colorduino
  - Dépendance : colorduino.h (ta librairie)
  - Adapter les noms de fonctions si nécessaire (voir commentaires)
  - Bitmap stocké en PROGMEM (format 0xRRGGBB)
*/

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Colorduino.h>
#include "gamma8.h"
#include "colorduino_images.h"

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct 
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

// --- Configuration matrice ---
const float PlasmaScaling = 10.0; 
unsigned char plasma[ColorduinoScreenWidth][ColorduinoScreenHeight];
long paletteShift;

// Couleurs
#define CLR_BG    0x000000  // noir (fond)
#define CLR_BLACK 0x000000
#define CLR_WHITE 0xFFFFFF
#define CLR_PINK  0xFF69B4  // rose fantôme (hot pink)
#define CLR_YELLOW 0xFFFF00
#define CLR_YELLOW_MARIO 0xFFD700

#define CLR_SKIN   0xF1C27D

#define CLR_RED    0xC40000
#define CLR_RED_DARK 0x320000

#define CLR_BLUE   0x0000A8

#define CLR_BROWN  0x7B3F00


//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV) 
{
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //if saturation > 0, more complex calculations are needed
  else
  {
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);

  // gamma correction
  colorRGB->r = pgm_read_byte(&gamma8[colorRGB->r]);
  colorRGB->g = pgm_read_byte(&gamma8[colorRGB->g]);
  colorRGB->b = pgm_read_byte(&gamma8[colorRGB->b]);
}


// Helper pour lire une couleur depuis un tableau en PROGMEM
uint32_t readFrameColor(const uint32_t *frame, uint16_t idx) {
  return pgm_read_dword_near(frame + idx);
}

// Exemple supposé : Colorduino.setPixelRGB(x, y, r, g, b);
void setPixelToColorduino(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  Colorduino.SetPixel(x, y, r, g, b); // <-- si nom différent, remplace ici
}

// Dessine une frame entière 8x8 depuis un buffer en RAM (uint32_t[WIDTH*HEIGHT])
void drawFrameFromRAM(const uint32_t *frame) {
  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint16_t idx = y * WIDTH + x;
      uint32_t c = frame[idx];
      uint8_t r = (c >> 16) & 0xFF;
      uint8_t g = (c >> 8) & 0xFF;
      uint8_t b = c & 0xFF;
      setPixelToColorduino(x, y, r, g, b);
    }
  }
  Colorduino.FlipPage();
}

// Dessine une frame (tableau PROGMEM)
void drawFramePROGMEM(const uint32_t *frame) {
  clearMatrix();
  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint16_t idx = y * WIDTH + x;
      uint32_t c = readFrameColor(frame, idx);
      uint8_t r = (c >> 16) & 0xFF;
      uint8_t g = (c >> 8) & 0xFF;
      uint8_t b = c & 0xFF;
      setPixelToColorduino(x, y, r, g, b);
    }
  }
  Colorduino.FlipPage();
}

void clearMatrix() {
  // Remplit chaque pixel avec la couleur CLR_BG (format 0xRRGGBB)
  uint8_t r = (CLR_BG >> 16) & 0xFF;
  uint8_t g = (CLR_BG >> 8) & 0xFF;
  uint8_t b = CLR_BG & 0xFF;

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      setPixelToColorduino(x, y, r, g, b);
    }
  }

  // Applique l'affichage si ta bibliothèque utilise .show() pour pousser le buffer
  Colorduino.FlipPage();
}

float dist(float a, float b, float c, float d) 
{
  return sqrt((c-a)*(c-a)+(d-b)*(d-b));
}

void plasma_morph()
{
  unsigned char x,y;
  float value;
  ColorRGB colorRGB;
  ColorHSV colorHSV;

  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++) {
      {
	value = sin((x + paletteShift) / PlasmaScaling)
	  + sin(dist(x, y, 64.0, 64.0) / PlasmaScaling)
	  + sin((y + paletteShift / 7.0) / PlasmaScaling)
	  + sin(dist(x, y, 192.0, 100.0) / PlasmaScaling);
	colorHSV.h=(unsigned char)((value) * 128)&0xff;
	colorHSV.s=255; 
	colorHSV.v=255;
	HSVtoRGB(&colorRGB, &colorHSV);
	
	Colorduino.SetPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
      }
  }
  paletteShift++;

  Colorduino.FlipPage(); // swap screen buffers to show it
}

/********************************************************
Name: ColorFill
Function: Fill the frame with a color
Parameter:R: the value of RED.   Range:RED 0~255
          G: the value of GREEN. Range:RED 0~255
          B: the value of BLUE.  Range:RED 0~255
********************************************************/
void ColorFill(unsigned char R,unsigned char G,unsigned char B)
{
  PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
    }
  }
  
  Colorduino.FlipPage();
}

// -----------------------------------------------------------------------------
// --- Affichages / Animations -----------------------------------------------
// -----------------------------------------------------------------------------

// showGhost : on passe la couleur du corps (bodyColor) en paramètre
void showGhost(uint32_t bodyColor, uint16_t holdMs = 1500) {
  // allouer buffer local sur la pile (64 * 4 = 256 bytes) - raisonnable pour AVR Uno/Nano
  uint32_t buf[WIDTH * HEIGHT];
  // remplir le buffer en utilisant le mask + bodyColor
  fillGhostFrame(buf, bodyColor);
  // dessiner depuis le buffer RAM
  drawFrameFromRAM(buf);
  delay(holdMs);
}

// Animation Pac-Man : ouvre/ferme la bouche pendant 'cycles' cycles
void animatePacman(uint8_t cycles = 6, uint16_t delayOpen = 120, uint16_t delayClosed = 180) {
  for (uint8_t i = 0; i < cycles; i++) {
    drawFramePROGMEM(pacman_closed);
    delay(delayClosed);
    drawFramePROGMEM(pacman_open);
    delay(delayOpen);
  }
}

// Animation Mario : simple marche (déplacement horizontal)
void animateMarioWalk(uint8_t steps = 12, uint16_t frameDelay = 140) {
  // On déplace Mario de -8 (hors écran gauche) à WIDTH (hors écran droite)
  for (int8_t pos = -8; pos <= WIDTH; pos++) {
    clearMatrix();
    // Choisir frame selon pos pour varier (A/B)
    const uint32_t *frame = (pos % 2 == 0) ? mario_frameA : mario_frameB;
    // dessiner le sprite à la position horizontale 'pos'
    for (uint8_t y = 0; y < HEIGHT; y++) {
      for (uint8_t x = 0; x < WIDTH; x++) {
        int8_t tx = pos + x;
        if (tx < 0 || tx >= WIDTH) continue;
        uint16_t idx = y * WIDTH + x;
        uint32_t c = readFrameColor(frame, idx);
        uint8_t r = (c >> 16) & 0xFF;
        uint8_t g = (c >> 8) & 0xFF;
        uint8_t b = c & 0xFF;
        setPixelToColorduino((uint8_t)tx, y, r, g, b);
      }
    }
    Colorduino.FlipPage();
    delay(frameDelay);
  }
}

// Affiche cœur statique
void showHeart(uint16_t holdMs = 1500) {
  drawFramePROGMEM(heart8x8);
  delay(holdMs);
}

// -----------------------------------------------------------------------------
// --- Choix aléatoire (switch/case) -----------------------------------------
// -----------------------------------------------------------------------------

// Nombre total de modes disponibles
const uint8_t MODE_COUNT = 4;

// Sélectionne et lance un affichage aléatoire
void chooseRandomDisplay() {
  uint8_t mode = random(MODE_COUNT); // 0 .. MODE_COUNT-1

  switch (mode) {
    case 0:
      // Fantôme : choisir couleur aléatoire pour le corps (exemples)
      uint32_t colors[] = { CLR_PINK, CLR_RED, CLR_BLUE, 0x00FF00 /*vert*/};
      uint8_t ci = random(sizeof(colors)/sizeof(colors[0]));
      showGhost(colors[ci], 1200);
      break;

    case 1:
      // Pac-Man animé
      animatePacman(6);
      break;

    case 2:
      // Mario marche
      animateMarioWalk();
      break;

    case 3:
      // Coeur
      showHeart();
      break;
    
    case 4:
      //
      plasma_morph();
      break;

    default:
      // Cas improbable : clear
      clearMatrix();
      delay(500);
      break;
  }
}

void setup() {
  // Initialisation (adapter si la librairie diffère)
  Colorduino.Init();    // ou Colorduino.init();
    // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  unsigned char whiteBalVal[3] = {36,63,63}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);

    // start with morphing plasma, but allow going to color cycling if desired.
  paletteShift=128000;
  unsigned char bcolor;

  //generate the plasma once
  for(unsigned char y = 0; y < ColorduinoScreenHeight; y++)
    for(unsigned char x = 0; x < ColorduinoScreenWidth; x++)
    {
      //the plasma buffer is a sum of sines
      bcolor = (unsigned char)
      (
            128.0 + (128.0 * sin(x*8.0 / 16.0))
          + 128.0 + (128.0 * sin(y*8.0 / 16.0))
      ) / 2;
      plasma[x][y] = bcolor;
    }
    
 // to adjust white balance you can uncomment this line
 // and comment out the plasma_morph() in loop()
 // and then experiment with whiteBalVal above
 // ColorFill(255,255,255);

  Colorduino.FlipPage();     // afficher
  
}

void loop() {
  chooseRandomDisplay();
   delay(5000);
}