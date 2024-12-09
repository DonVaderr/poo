#include "../proyecto_poo/src/include/screens/ConfirmarSalidaScreen.h"
#include "../proyecto_poo/src/include/Game.h"
#include "../proyecto_poo/src/include/Button.h"
#include "raylib.h"

void ConfirmarSalidaScreen::Update(Game &game) {
    Vector2 mousePos=GetMousePosition();
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos,{(float)ancho/2 -100,(float)alto/2,80,40})) {
            // Sí
            game.ChangeScreen(PantallaJuego::MENU);
            game.ResetJuego();
        }
        if (CheckCollisionPointRec(mousePos,{(float)ancho/2 +20,(float)alto/2,80,40})) {
            // No
            game.ChangeScreen(PantallaJuego::JUEGO);
        }
    }
}

void ConfirmarSalidaScreen::Draw(Game &game) {
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    Font f=game.GetFontGame();
    Texture2D fondo=game.GetFondoConfirmar();
    float ratioAncho=ancho/(float)fondo.width;
    float ratioAlto=alto/(float)fondo.height;
    float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
    DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,(float)fondo.width*ratio,(float)fondo.height*ratio},{0,0},0,WHITE);

    Button btnSi({(float)ancho/2 -100,(float)alto/2,80,40},"Sí",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    Button btnNo({(float)ancho/2 +20,(float)alto/2,80,40},"No",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);

    btnSi.Draw();
    btnNo.Draw();
}

PantallaJuego ConfirmarSalidaScreen::GetType() const {
    return PantallaJuego::CONFIRMAR_SALIDA;
}
