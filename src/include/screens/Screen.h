#ifndef SCREEN_H
#define SCREEN_H

#include "../proyecto_poo/src/include/Game.h" // o usar forward declaration si se prefiere
// Si se produce dependencia circular, se puede usar forward declaration de class Game y luego incluir game.h en el cpp

class Game;

class Screen {
public:
    virtual ~Screen() {}
    virtual void Update(Game &game) = 0;
    virtual void Draw(Game &game) = 0;
    virtual PantallaJuego GetType() const = 0;
};

#endif
