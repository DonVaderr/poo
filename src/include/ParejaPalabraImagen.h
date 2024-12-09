#ifndef PAREJAPALABRAIMAGEN_H
#define PAREJAPALABRAIMAGEN_H

#include <string>
#include "raylib.h"

class ParejaPalabraImagen {
private:
    std::string palabra;
    std::string silabas[9];
    Texture2D imagen;

public:
    ParejaPalabraImagen();
    ParejaPalabraImagen(const char* p, const char* s[9], Texture2D img);
    const std::string &GetPalabra() const;
    const std::string &GetSilaba(int i) const;
    Texture2D GetImagen() const;
};

#endif
