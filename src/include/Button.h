#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "raylib.h"

class Button {
private:
    Rectangle rect;
    std::string text;
    int fontSize;
    Color normalColor;
    Color hoverColor;
    Color clickColor;
    Color textColor;
    Sound hoverSound;
    Sound clickSound;
    bool soundEffectsEnabled;
    Font font;
    bool hoverSoundPlayed = false;

public:
    Button(Rectangle r, const std::string &t, int fs, Color nc, Color hc, Color cc, Color tc, Sound hs, Sound cs, bool se, Font f);
    void Draw();
    bool IsClicked() const;
    void SetText(const std::string &t);
};

#endif
