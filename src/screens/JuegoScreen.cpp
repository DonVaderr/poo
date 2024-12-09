#include "../proyecto_poo/src/include/screens/JuegoScreen.h"
#include "../proyecto_poo/src/include/Game.h"
#include "../proyecto_poo/src/include/Button.h"
#include "raylib.h"
#include <cstring>

void JuegoScreen::Update(Game &game) {
    if (game.GetVidas()<=0) {
        game.ChangeScreen(PantallaJuego::JUEGO_TERMINADO);
        return;
    }

    game.ActualizarRotacion();

    Vector2 mousePos=GetMousePosition();
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();

    int contadorSeleccion=game.GetContadorSeleccion();
    int maxSelecciones=game.GetMaxSelecciones();
    char *textoConcatenado=game.GetTextoConcatenado();
    const char **textosMezclados=game.GetTextosMezclados();

    float anchoRecuadro = ancho / 8;
    float altoRecuadro = alto / 8;
    int espacioX = (int)(anchoRecuadro + ancho / 50);
    int espacioY = (int)(altoRecuadro + alto / 50);
    int inicioX = (int)(ancho / 2 - 0.25f * espacioX);
    int inicioY = (int)(alto / 4);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && contadorSeleccion < maxSelecciones && game.GetVidas() > 0 && !game.GetMostrandoResultado()) {
        for (int fila=0; fila<3; fila++) {
            for (int columna=0; columna<3; columna++) {
                float posX = inicioX + columna * espacioX;
                float posY = inicioY + fila * espacioY;
                if (CheckCollisionPointRec(mousePos,{posX,posY,anchoRecuadro,altoRecuadro})) {
                    strncat(textoConcatenado,textosMezclados[fila*3+columna],sizeof(game.GetTextoConcatenado()) - strlen(textoConcatenado)-1);
                    game.GetContadorSeleccion()++;
                }
            }
        }
    }

    if (game.GetContadorSeleccion()==maxSelecciones && !game.GetMostrandoResultado()) {
        game.ComprobarAcierto();
    }

    if (game.GetMostrandoResultado()) {
        game.ActualizarResultado();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos,{10,30,50,50})) {
        game.ChangeScreen(PantallaJuego::CONFIRMAR_SALIDA);
    }
}

void JuegoScreen::Draw(Game &game) {
    float ancho=GetScreenWidth();
    float alto=GetScreenHeight();
    Font f=game.GetFontGame();
    Texture2D fondo=game.GetFondoJuego();
    float ratioAncho=ancho/(float)fondo.width;
    float ratioAlto=alto/(float)fondo.height;
    float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
    DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,(float)fondo.width*ratio,(float)fondo.height*ratio},{0,0},0,WHITE);

    Button btnBack({10,30,50,50},"<-",30,WHITE,GRAY,DARKGRAY,DARKGRAY,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
    btnBack.Draw();

    DrawRectangle(0,0,(int)(ancho*(game.GetAciertos()/10.0f)),20,GREEN);

    ParejaPalabraImagen &palabraActualPareja=game.GetPalabraActualPareja();

    float anchoRecuadro = ancho / 8;
    float altoRecuadro = alto / 8;
    int espacioX = (int)(anchoRecuadro + ancho / 50);
    int espacioY = (int)(altoRecuadro + alto / 50);
    int inicioX = (int)(ancho / 2 - 0.25 * espacioX);
    int inicioY = (int)(alto / 4);

    int anchoRectGrande = (int)(ancho/3);
    int altoRectGrande = (int)(alto - 175);
    float imagenX = 50 + anchoRectGrande / 2.0f;
    float imagenY = 50 + altoRectGrande / 2.0f;

    Rectangle sourceRec={0,0,(float)palabraActualPareja.GetImagen().width,(float)palabraActualPareja.GetImagen().height};
    Rectangle destRec={imagenX,imagenY,(float)palabraActualPareja.GetImagen().width,(float)palabraActualPareja.GetImagen().height};
    Vector2 origin={(float)palabraActualPareja.GetImagen().width/2,(float)palabraActualPareja.GetImagen().height/2};

    DrawTexturePro(palabraActualPareja.GetImagen(),sourceRec,destRec,origin,game.GetAnguloRotacion(),WHITE);

    const char **textosMezclados=game.GetTextosMezclados();
    int tamañoFuente=10+(int)(ancho/80);
    for (int fila=0; fila<3; fila++) {
        for (int columna=0; columna<3; columna++) {
            float posX = inicioX + columna * espacioX;
            float posY = inicioY + fila * espacioY;
            Button btnS({posX,posY,anchoRecuadro,altoRecuadro},textosMezclados[fila*3+columna],tamañoFuente,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
            btnS.Draw();
        }
    }

    int rectInferiorX=(int)(ancho/2 -0.25*anchoRecuadro);
    int rectInferiorY=(int)(alto -200);
    int anchoRectInferior=(int)(1*anchoRecuadro + 2*espacioX);
    int altoRectInferior=(int)(alto/8);
    DrawRectangle(rectInferiorX,rectInferiorY,anchoRectInferior,altoRectInferior,WHITE);

    char *textoConcatenado=game.GetTextoConcatenado();
    int anchoTextoConcatenado=MeasureText(textoConcatenado,tamañoFuente);
    float textoConcatenadoPosX=rectInferiorX+(anchoRectInferior - anchoTextoConcatenado)/2.0f;
    float textoConcatenadoPosY=rectInferiorY+(altoRectInferior - tamañoFuente)/2.0f;
    DrawTextEx(f,textoConcatenado,{textoConcatenadoPosX,textoConcatenadoPosY},tamañoFuente,0,BLACK);

    char textoVidas[20];
    sprintf(textoVidas,"Vidas: %d",game.GetVidas());
    DrawTextEx(f,textoVidas,{(float)ancho -150,20},tamañoFuente,0,RED);
}

PantallaJuego JuegoScreen::GetType() const {
    return PantallaJuego::JUEGO;
}
