#ifndef JUEGOSCREEN_H
#define JUEGOSCREEN_H

#include "screen.h"

class JuegoScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
