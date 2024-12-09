#include "../proyecto_poo/src/include/screens/FelicidadesScreen.h"
#include "../proyecto_poo/src/include/Game.h"
#include "../proyecto_poo/src/include/Button.h"
#include "raylib.h"

void FelicidadesScreen::Update(Game &game) {
    Vector2 mousePos=GetMousePosition();
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/4,150,50})) {
        game.ChangeScreen(PantallaJuego::MENU);
        game.ResetJuego();
    }
}

void FelicidadesScreen::Draw(Game &game) {
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    Font f=game.GetFontGame();
    Texture2D fondo=game.GetFondoGanaste();
    float ratioAncho=ancho/(float)fondo.width;
    float ratioAlto=alto/(float)fondo.height;
    float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
    DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,(float)fondo.width*ratio,(float)fondo.height*ratio},{0,0},0,WHITE);

    Button btnMenu({(float)ancho/2 -100,(float)alto/4,150,50},"Menú",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    btnMenu.Draw();
}

PantallaJuego FelicidadesScreen::GetType() const {
    return PantallaJuego::FELICITACIONES;
}
