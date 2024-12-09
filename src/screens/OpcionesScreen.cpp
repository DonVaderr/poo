#include "../proyecto_poo/src/include/screens/OpcionesScreen.h"
#include "../proyecto_poo/src/include/Game.h"
#include "../proyecto_poo/src/include/Button.h"
#include "raylib.h"

void OpcionesScreen::Update(Game &game) {
    Vector2 mousePos=GetMousePosition();
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos,{10,30,50,50})) {
            game.ChangeScreen(PantallaJuego::MENU);
        }
        if (CheckCollisionPointRec(mousePos,{ancho/2-100,(float)alto/2 -50,300,50})) {
            game.SetMusicaEnabled(!game.IsMusicaEnabled());
        }
        if (CheckCollisionPointRec(mousePos,{ancho/2-100,(float)alto/2 +50,300,50})) {
            game.SetEfectosEnabled(!game.IsEfectosEnabled());
        }
    }
}

void OpcionesScreen::Draw(Game &game) {
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    Font f=game.GetFontGame();
    Texture2D fondo=game.GetFondoOpciones();
    float ratioAncho=ancho/(float)fondo.width;
    float ratioAlto=alto/(float)fondo.height;
    float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
    DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

    Button btnBack({10,30,50,50},"<-",30,WHITE,GRAY,DARKGRAY,DARKGRAY,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    btnBack.Draw();

    Button btnMusica({ancho/2 -100,(float)alto/2 -50,300,50}, game.IsMusicaEnabled()?"Desactivar Música":"Activar Música",
                     30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    btnMusica.Draw();

    Button btnEfectos({ancho/2 -100,(float)alto/2 +50,300,50}, game.IsEfectosEnabled()?"Desactivar Efectos":"Activar Efectos",
                      30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    btnEfectos.Draw();
}

PantallaJuego OpcionesScreen::GetType() const {
    return PantallaJuego::OPCIONES;
}
