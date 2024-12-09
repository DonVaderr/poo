#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <string>
#include "parejapalabraimagen.h"
#include "button.h"
#include "screens/screen.h"

// Enumeración de pantallas
enum class PantallaJuego {
    MENU,
    SELECCION_NIVEL,
    JUEGO,
    OPCIONES,
    SALIR,
    CONFIRMAR_SALIDA,
    FELICITACIONES,
    JUEGO_TERMINADO
};

class Game {
private:
    Music musicaFondo;
    Sound hoverSound;
    Sound clickSound;
    Sound aciertoSound;
    Sound errorSound;
    Font font;
    Texture2D fondoMenu;
    Texture2D fondoNiveles;
    Texture2D fondoJuego;
    Texture2D fondoOpciones;
    Texture2D fondoConfirmar;
    Texture2D fondoGanaste;
    Texture2D fondoPerdiste;

    Screen *currentScreen = nullptr;

    bool musicaFondoEnabled = true;
    bool efectosSonidoEnabled = true;

    ParejaPalabraImagen palabrasFacil[10];
    ParejaPalabraImagen palabrasMedio[10];
    ParejaPalabraImagen palabrasDificil[10];
    int numPalabrasFacil;
    int numPalabrasMedio;
    int numPalabrasDificil;

    ParejaPalabraImagen palabrasSeleccionadas[10];

    int palabraActual = 0;
    ParejaPalabraImagen palabraActualPareja;
    const char *textos[9];
    int posiciones[9];
    const char *textosMezclados[9];
    char textoConcatenado[20];
    int contadorSeleccion = 0;
    int maxSelecciones = 3;
    int vidas = 5;
    int aciertos = 0;
    float anguloRotacion = 0.0f;
    bool rotandoDerecha = true;
    bool mostrandoResultado = false;
    float temporizadorResultado = 0.0f;
    float tiempoMostrarResultado = 2.0f;

public:
    Game();
    ~Game();
    void Run();

    void ChangeScreen(PantallaJuego newScreen);
    void UpdateMusic();

    // Getters y Setters
    bool IsMusicaEnabled() const;
    bool IsEfectosEnabled() const;
    void SetMusicaEnabled(bool val);
    void SetEfectosEnabled(bool val);

    Font GetFontGame() const;
    Sound GetHoverSound() const;
    Sound GetClickSound() const;
    Sound GetAciertoSound() const;
    Sound GetErrorSound() const;

    Texture2D GetFondoMenu() const;
    Texture2D GetFondoNiveles() const;
    Texture2D GetFondoJuego() const;
    Texture2D GetFondoOpciones() const;
    Texture2D GetFondoConfirmar() const;
    Texture2D GetFondoGanaste() const;
    Texture2D GetFondoPerdiste() const;

    ParejaPalabraImagen* GetPalabrasFacil();
    ParejaPalabraImagen* GetPalabrasMedio();
    ParejaPalabraImagen* GetPalabrasDificil();

    int GetNumFacil() const;
    int GetNumMedio() const;
    int GetNumDificil() const;

    ParejaPalabraImagen* GetPalabrasSeleccionadas();
    int &GetPalabraActual();
    ParejaPalabraImagen &GetPalabraActualPareja();
    const char** GetTextosMezclados();
    char* GetTextoConcatenado();
    int &GetContadorSeleccion();
    int GetMaxSelecciones() const;
    int &GetMaxSeleccionesRef();
    int &GetVidas();
    int &GetAciertos();
    float &GetAnguloRotacion();
    bool &GetRotandoDerecha();
    bool &GetMostrandoResultado();
    float &GetTemporizadorResultado();
    float GetTiempoMostrarResultado() const;
    int *GetPosiciones();
    const char **GetTextos();

    void ReiniciarSilabas();
    void SeleccionarNivelFacil();
    void SeleccionarNivelMedio();
    void SeleccionarNivelDificil();
    void ResetJuego();
    void ComprobarAcierto();
    void ActualizarResultado();
    void ActualizarRotacion();
    void SetCurrentScreen(Screen *scr);

    PantallaJuego GetCurrentScreenType() const;
};

#endif
