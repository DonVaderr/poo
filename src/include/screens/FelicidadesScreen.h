#ifndef FELICIDADESSCREEN_H
#define FELICIDADESSCREEN_H

#include "screen.h"

class FelicidadesScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
