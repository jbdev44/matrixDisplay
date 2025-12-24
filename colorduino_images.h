/*
  colorduino_images.h
  Images/frames pour matrice 8x8 stockées en PROGMEM.

  Usage :
    #include "colorduino_images.h"
    // lire une couleur :
    uint32_t c = pgm_read_dword_near(ghost8x8 + idx);
    uint8_t r = (c >> 16) & 0xFF;
    uint8_t g = (c >> 8) & 0xFF;
    uint8_t b = c & 0xFF;

  Remarque :
    - Définit WIDTH et HEIGHT = 8.
    - Contient : ghost8x8, pacman_closed, pacman_open, mario_frameA, mario_frameB, heart8x8
*/

#ifndef COLORUDINO_IMAGES_H
#define COLORUDINO_IMAGES_H

#include <avr/pgmspace.h>
#include <stdint.h>

#define WIDTH  8
#define HEIGHT 8

// Couleurs (format 0xRRGGBB)
#ifndef CLR_BG
#define CLR_BG       0x000000
#endif
#ifndef CLR_WHITE
#define CLR_WHITE    0xFFFFFF
#endif
#ifndef CLR_BLACK
#define CLR_BLACK    0x000000
#endif

#ifndef CLR_PINK
#define CLR_PINK     0xFF69B4
#endif
#ifndef CLR_YELLOW
#define CLR_YELLOW   0xFFFF00
#endif
#ifndef CLR_RED
#define CLR_RED      0xFF0000
#define CLR_RED_DARK 0x320000
#endif
#ifndef CLR_RED_DARK
#define CLR_RED_DARK 0x990000
#endif
#ifndef CLR_BLUE
#define CLR_BLUE     0x0000A8
#endif
#ifndef CLR_SKIN
#define CLR_SKIN     0xF1C27D
#endif
#ifndef CLR_BROWN
#define CLR_BROWN     0xa85400
#endif

// -----------------------------------------------------------------------------
// Fantôme : mask 8x8 (valeurs 0..3) stocké en PROGMEM (row-major)
// 0 = background, 1 = body, 2 = eye white, 3 = pupil black
//
// Mask dérivé du fantôme fourni : toutes les variantes de rouge sont considérées
// comme "body" (1). Les blancs = yeux (2), les noirs = pupilles (3).
// -----------------------------------------------------------------------------
const uint8_t ghost_mask[WIDTH * HEIGHT] PROGMEM = {
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 2, 3, 1, 2, 3, 1,
  1, 1, 2, 2, 1, 2, 2, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 1, 1, 0, 1, 1, 1
};

// -----------------------------------------------------------------------------
// Pac-Man fermé / ouvert (simplifié)
const uint32_t pacman_closed[WIDTH * HEIGHT] PROGMEM = {
  CLR_BG, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG,
  CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_BG,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW,
  CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG,
  CLR_BG, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG
};

const uint32_t pacman_open[WIDTH * HEIGHT] PROGMEM = {
  CLR_BG, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG,
  CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_BG,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG, CLR_BG,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG, CLR_BG, CLR_BG,
  CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG,
  CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG,
  CLR_BG, CLR_BG, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_YELLOW, CLR_BG, CLR_BG
};

// -----------------------------------------------------------------------------
// Mario : 2 frames simples pour marche (8x8)
const uint32_t mario[WIDTH * HEIGHT] PROGMEM = {
  CLR_BG, CLR_RED, CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED,  CLR_BG, CLR_BG,
  CLR_RED, CLR_RED, CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED, CLR_RED,
  CLR_BROWN, CLR_BROWN, CLR_BROWN, CLR_SKIN,  CLR_RED,  CLR_BLUE, CLR_RED, CLR_BG,
  CLR_BROWN, CLR_SKIN,CLR_BROWN, CLR_SKIN, CLR_SKIN, CLR_BROWN, CLR_SKIN,CLR_SKIN,
  CLR_SKIN, CLR_SKIN, CLR_SKIN, CLR_SKIN, CLR_BROWN, CLR_BROWN, CLR_BROWN,CLR_BG,
  CLR_BG, CLR_BROWN, CLR_RED, CLR_BROWN, CLR_RED, CLR_BROWN, CLR_BG, CLR_BG,
  CLR_SKIN, CLR_RED,  CLR_SKIN, CLR_RED,  CLR_SKIN, CLR_RED,  CLR_SKIN, CLR_BG,
  CLR_BG, CLR_RED,  CLR_RED, CLR_BG,  CLR_RED,  CLR_RED,  CLR_BG, CLR_BG
};

const uint32_t luigi[WIDTH * HEIGHT] PROGMEM = {
  CLR_BG, CLR_BG, CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED,  CLR_BG, CLR_BG,
  CLR_BG, CLR_RED, CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED,  CLR_RED, CLR_BG,
  CLR_RED, CLR_RED, CLR_BLUE, CLR_RED,  CLR_RED,  CLR_BLUE, CLR_RED, CLR_RED,
  CLR_RED, CLR_BLUE,CLR_BLUE, CLR_BLUE, CLR_BLUE, CLR_BLUE, CLR_BLUE,CLR_RED,
  CLR_RED, CLR_BLUE,CLR_BLUE, CLR_SKIN, CLR_SKIN, CLR_BLUE, CLR_BLUE,CLR_RED,
  CLR_BG, CLR_BLUE,CLR_BLUE, CLR_BLUE, CLR_BLUE, CLR_BLUE, CLR_BLUE,CLR_BG,
  CLR_BG, CLR_BG,  CLR_BG,   CLR_BLUE, CLR_BG,  CLR_BG,  CLR_BG, CLR_BG,
  CLR_BG, CLR_BG,  CLR_BLUE, CLR_BG,  CLR_BG,  CLR_BG,  CLR_BG, CLR_BG
};

// -----------------------------------------------------------------------------
// Coeur 8x8 (rouge simple)
const uint32_t heart8x8[WIDTH * HEIGHT] PROGMEM = {
  CLR_BG, CLR_RED, CLR_RED, CLR_BG,  CLR_BG,  CLR_RED, CLR_RED, CLR_BG,
  CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED,
  CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED,
  CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED,
  CLR_BG, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_BG,
  CLR_BG, CLR_BG, CLR_RED, CLR_RED, CLR_RED, CLR_RED, CLR_BG, CLR_BG,
  CLR_BG, CLR_BG, CLR_BG, CLR_RED, CLR_RED, CLR_BG, CLR_BG, CLR_BG,
  CLR_BG, CLR_BG, CLR_BG, CLR_BG, CLR_BG, CLR_BG, CLR_BG, CLR_BG
};

// -----------------------------------------------------------------------------
// Helpers pour le fantôme paramétrable
// -----------------------------------------------------------------------------

// Retourne la couleur 0xRRGGBB pour le pixel idx du fantôme en utilisant bodyColor
static inline uint32_t ghostColorAt(uint16_t idx, uint32_t bodyColor) {
  uint8_t m = pgm_read_byte_near(ghost_mask + idx);
  switch (m) {
    case 0: return CLR_BG;
    case 1: return bodyColor;
    case 2: return CLR_WHITE;
    case 3: return CLR_BLACK;
    default: return CLR_BG;
  }
}

// Remplit un buffer (taille WIDTH*HEIGHT) en RAM avec la frame du fantôme colorée.
// dest doit être un tableau uint32_t[WIDTH*HEIGHT] (ou équivalent alloué).
static inline void fillGhostFrame(uint32_t *dest, uint32_t bodyColor) {
  for (uint16_t i = 0; i < (WIDTH * HEIGHT); i++) {
    dest[i] = ghostColorAt(i, bodyColor);
  }
}

#endif // COLORUDINO_IMAGES_H