#ifndef OPCIONESSCREEN_H
#define OPCIONESSCREEN_H

#include "screen.h"

class OpcionesScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
