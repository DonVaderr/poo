#ifndef CONFIRMARSALIDASCREEN_H
#define CONFIRMARSALIDASCREEN_H

#include "screen.h"

class ConfirmarSalidaScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
