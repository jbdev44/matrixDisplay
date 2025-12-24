# Colorduino Examples

Collection d'exemples pour matrice Colorduino 8×8 — affichages et petites animations
(Pac‑Man, fantôme paramétrable, Mario qui marche, cœur, sélection aléatoire).

Ce dépôt contient des sketches Arduino prêts à l'emploi et un header `colorduino_images.h`
regroupant les images/frames stockées en PROGMEM. Le fantôme a été implémenté comme
un "mask" afin de pouvoir lui passer la couleur du corps (bodyColor) au moment du rendu.

## Sommaire
- Description
- Matériel requis
- Schéma de câblage / alimentation
- Installation & compilation
- Sketches fournis
- Personnalisation (changer la couleur du fantôme, adapter la librairie)
- Notes techniques et contraintes mémoire
- Licence

---

## Description
Ces exemples montrent comment :
- stocker des images 8×8 en PROGMEM,
- générer dynamiquement une frame à partir d'un mask (fantôme avec couleur paramétrable),
- afficher des animations simples (Pac‑Man bouche ouverte/fermée, Mario qui marche),
- sélectionner aléatoirement un affichage via un `switch/case`.

Les exemples sont pensés pour une matrice 8×8 (row-major). Le code est facilement
adaptable à d'autres tailles en modifiant `WIDTH` / `HEIGHT` et en fournissant
de nouveaux tableaux.

---

## Matériel requis
- Arduino Uno / Nano / Pro Mini (ou tout MCU compatible Arduino)
- Carte Colorduino / matrice 8×8 adressable (ou équivalent)
- Câblage :
  - Data pin (ex. D6) → entrée data de la matrice/Colorduino
  - GND commun entre Arduino et alimentation LED
  - +5V alimentation suffisante pour la matrice
- Alimentation 5V capable de délivrer le courant nécessaire (selon le nombre d'LED)
- Câbles, breadboard, etc.

---

## Schéma de câblage (général)
- Arduino D6 (ou pin choisie dans le sketch) → Data IN du driver/matrice
- Arduino GND → GND du driver/matrice et alimentation
- Alimentation 5V → +5V du driver/matrice
- Si la matrice consomme beaucoup, alimente-les directement (ne pas alimenter depuis le 5V de l'Arduino si l'intensité est élevée).

Ajoute un condensateur de 1000 µF entre +5V et GND au niveau de l'alimentation si nécessaire,
et une résistance 220–470 Ω sur la ligne Data pour protéger les premières LEDs.

---

## Installation & compilation

### Dépendances
- `Colorduino.h` : bibliothèque / header qui expose les fonctions d'initialisation et
  d'écriture de pixels (ex. `Colorduino.Init()`, `Colorduino.setPixel(x,y,r,g,b)`, `Colorduino.FlipPage()`).
  - Si tu n'as pas une librairie dédiée, adapte `setPixelToColorduino()` dans les sketches
    pour utiliser l'API que tu as (ou écris directement dans le buffer matériel).

### Arduino IDE
1. Ouvrir l'IDE Arduino.
2. Copier les fichiers du dépôt dans un dossier de projet (ou ouvrir le sketch `.ino`).
3. Vérifier/installer la librairie `Colorduino` si disponible, sinon place `Colorduino.h` dans le dossier du sketch ou dans `C:\Users\YOU\Documents\Arduino\libraries\`.
4. Sélectionner la carte et le port, puis téléverser.

### PlatformIO
Exemple minimal `platformio.ini` :
```
[env:uno]
platform = atmelavr
board = uno
framework = arduino
```
Place les fichiers dans `src/` et compile / upload avec PlatformIO.

---

## Fichiers principaux fournis
- `colorduino_images.h` — toutes les images/frames en PROGMEM et le `ghost_mask` + helpers :
  - `ghost_mask[]` (mask 0..3)
  - `fillGhostFrame(dest, bodyColor)` et `ghostColorAt(idx, bodyColor)`
  - `pacman_closed`, `pacman_open`, `mario_frameA`, `mario_frameB`, `heart8x8`
- `colorduino_random_show.ino` — sketch principal qui sélectionne aléatoirement un affichage
  (utilise `chooseRandomDisplay()` et `showGhost(bodyColor)`).
- `colorduino_mario_walk.ino`, `colorduino_pacman.ino`, `colorduino_pacman_ghost.ino` — exemples individuels (si inclus).

---

## Utilisation (exemples)
- Pour afficher le fantôme rose :
  - Dans le sketch principal, appeler `showGhost(CLR_PINK);`
- Dans `colorduino_random_show.ino`, la fonction `chooseRandomDisplay()` choisit une animation aléatoirement.
- Pour modifier la couleur par défaut du fantôme, change `CLR_PINK` ou passe la couleur désirée
  (format 0xRRGGBB) à `showGhost()`.

Exemple d'appel direct :
```c++
uint32_t myColor = 0x8B0000; // rouge foncé
showGhost(myColor, 1500);   // afficher le fantôme 1.5 s avec cette couleur
```

---

## Personnalisation & adaptation
- Adapter l'API Colorduino :
  - Si ta librairie n'a pas `Colorduino.setPixelRGB(x,y,r,g,b)`, édite la fonction `setPixelToColorduino(...)`
    dans les sketches pour appeler la bonne fonction (ou écrire dans le buffer matériel).
- Modifier la taille de la matrice :
  - Mettre à jour `WIDTH`/`HEIGHT` et fournir de nouveaux tableaux PROGMEM.
- Changer vitesse d'animation :
  - Ajuster les `delay()` dans les fonctions d'animation (`animatePacman`, `animateMarioWalk`, ...).
- Réduire l'utilisation RAM :
  - `fillGhostFrame()` alloue 64 × 4 = 256 bytes si on remplit un buffer local.
    Sur des microcontrôleurs avec peu de RAM, tu peux dessiner pixel par pixel sans buffer.

---

## Notes techniques
- `ghost_mask` est un tableau de 8×8 octets (0..3). Lors du rendu, `fillGhostFrame()` remplace
  les "body" par la couleur passée en paramètre et laisse les yeux/pupilles identifiés.
- La conversion mask → frame se fait en RAM ; la pile requise pour un buffer 8×8 de uint32_t
  est de 256 octets (généralement acceptable sur Arduino Uno/Nano mais à surveiller).
- Protection contre le "tearing" :
  - Si tu utilises un double buffering et des interruptions, protège les swaps (ex. `ATOMIC_BLOCK`),
    comme évoqué dans les commentaires précédents.

---

## Licence
Ce dépôt est fourni sous licence MIT — libre d'utilisation, modification et partage.
(Insère un fichier LICENSE si tu veux formaliser.)

---

## Contribuer
- Ouvrir une issue pour signaler un bug ou demander une fonctionnalité.
- Proposer des PRs pour ajouter des sprites 8×8, améliorer les animations, ou ajouter support pour d'autres
  drivers/board.

Amuse‑toi bien avec ta matrice ! 👾
