#include "../proyecto_poo/src/include/screens/SeleccionNivelScreen.h"
#include "../proyecto_poo/src/include/Game.h"
#include "../proyecto_poo/src/include/Button.h"
#include "raylib.h"

void SeleccionNivelScreen::Update(Game &game) {
    Vector2 mousePos = GetMousePosition();
    float ancho = GetScreenWidth();
    float alto = GetScreenHeight();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2 -100,200,50})) {
            game.SeleccionarNivelFacil();
        }
        if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2 -25,200,50})) {
            game.SeleccionarNivelMedio();
        }
        if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2 +50,200,50})) {
            game.SeleccionarNivelDificil();
        }
    }
}

void SeleccionNivelScreen::Draw(Game &game) {
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    Font f=game.GetFontGame();
    Texture2D fondo=game.GetFondoNiveles();
    float ratioAncho=ancho/(float)fondo.width;
    float ratioAlto=alto/(float)fondo.height;
    float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
    DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,(float)fondo.width*ratio,(float)fondo.height*ratio},{0,0},0,WHITE);

    Button btnFacil({ancho/2 -100,alto/2 -100,200,50},"Fácil",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    Button btnMedio({ancho/2 -100,alto/2 -25,200,50},"Medio",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    Button btnDificil({ancho/2 -100,alto/2 +50,200,50},"Difícil",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);

    btnFacil.Draw();
    btnMedio.Draw();
    btnDificil.Draw();
}

PantallaJuego SeleccionNivelScreen::GetType() const {
    return PantallaJuego::SELECCION_NIVEL;
}
