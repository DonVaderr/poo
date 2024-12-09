#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "screen.h"

class MenuScreen : public Screen {
public:
    void Update(Game &game) override;
    void Draw(Game &game) override;
    PantallaJuego GetType() const override;
};

#endif
