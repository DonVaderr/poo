#ifndef SELECCIONNIVELSCREEN_H
#define SELECCIONNIVELSCREEN_H

#include "screen.h"

class SeleccionNivelScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
