#include "../proyecto_poo/src/include/Button.h"

Button::Button(Rectangle r, const std::string &t, int fs, Color nc, Color hc, Color cc, Color tc, Sound hs, Sound cs, bool se, Font fnt)
    : rect(r), text(t), fontSize(fs), normalColor(nc), hoverColor(hc), clickColor(cc), textColor(tc),
      hoverSound(hs), clickSound(cs), soundEffectsEnabled(se), font(fnt) {}

void Button::Draw() {
    // Implementación del Draw tal como en el código original
}

bool Button::IsClicked() const {
    // Implementación
}

void Button::SetText(const std::string &t) {
    text = t;
}
