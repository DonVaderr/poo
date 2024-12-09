#include "raylib.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

// Enumeración para identificar las pantallas
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

// Declaración adelantada de la clase Game
class Game;

// Clase base abstracta para las pantallas
class Screen {
public:
    virtual ~Screen() {}
    virtual void Update(Game &game) = 0;
    virtual void Draw(Game &game) = 0;
    virtual PantallaJuego GetType() const = 0;
};

// Clase para representar una pareja palabra-imagen
class ParejaPalabraImagen {
private:
    std::string palabra;
    std::string silabas[9];
    Texture2D imagen;
public:
    ParejaPalabraImagen() {}
    ParejaPalabraImagen(const char* p, const char* s[9], Texture2D img) : palabra(p), imagen(img) {
        for (int i = 0; i < 9; i++) silabas[i] = s[i];
    }
    const std::string &GetPalabra() const { return palabra; }
    const std::string &GetSilaba(int i) const { return silabas[i]; }
    Texture2D GetImagen() const { return imagen; }
};

// Funciones utilitarias
static void mezclarArreglo(int *arreglo, int tamaño) {
    for (int i = tamaño - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

static void seleccionarPalabrasEImagenesAleatorias(ParejaPalabraImagen *arregloFuente, int tamañoFuente, ParejaPalabraImagen *arregloObjetivo, int tamañoObjetivo) {
    int *indices = (int *)malloc(tamañoFuente * sizeof(int));
    for (int i = 0; i < tamañoFuente; i++) indices[i] = i;
    mezclarArreglo(indices, tamañoFuente);
    for (int i = 0; i < tamañoObjetivo; i++) arregloObjetivo[i] = arregloFuente[indices[i]];
    free(indices);
}

// Clase Button para encapsular la lógica de un botón
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
    Button(Rectangle r, const std::string &t, int fs, Color nc, Color hc, Color cc, Color tc, Sound hs, Sound cs, bool se, Font f)
        : rect(r), text(t), fontSize(fs), normalColor(nc), hoverColor(hc), clickColor(cc), textColor(tc),
          hoverSound(hs), clickSound(cs), soundEffectsEnabled(se), font(f) {}

    void Draw() {
        Vector2 mousePosition = GetMousePosition();
        Color currentColor = normalColor;
        int borderWidth = 2;

        if (CheckCollisionPointRec(mousePosition, rect)) {
            if (!hoverSoundPlayed && soundEffectsEnabled) {
                PlaySound(hoverSound);
                hoverSoundPlayed = true;
            }
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                currentColor = clickColor;
                borderWidth = 5;
            } else {
                currentColor = hoverColor;
                borderWidth = 3;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && soundEffectsEnabled) {
                PlaySound(clickSound);
            }
        } else {
            hoverSoundPlayed = false;
        }

        DrawRectangleRec(rect, currentColor);
        DrawRectangleLinesEx(rect, borderWidth, DARKGRAY);
        int textWidth = MeasureText(text.c_str(), fontSize);
        float textX = rect.x + (rect.width - textWidth) / 2;
        float textY = rect.y + (rect.height - fontSize) / 2;
        DrawTextEx(font, text.c_str(), Vector2{textX, textY}, fontSize, 0, textColor);
    }

    bool IsClicked() const {
        return IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rect);
    }

    void SetText(const std::string &t) { text = t; }
};

// Clase principal Game: mantiene el estado global, recursos, y permite cambiar de pantalla
class Game {
private:
    // Recursos
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

    // Variables de estado global
    bool musicaFondoEnabled = true;
    bool efectosSonidoEnabled = true;

    // Palabras por nivel
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
    Game() {
        InitWindow(1600,900,"Write & Fun");
        SetWindowState(FLAG_FULLSCREEN_MODE);
        InitAudioDevice();
        srand((unsigned int)time(NULL));
        SetTargetFPS(60);

        musicaFondo = LoadMusicStream("./res/music/fondo.mp3");
        hoverSound = LoadSound("ruta/a/tu/sonido/hover.wav");
        clickSound = LoadSound("./res/music/click.wav");
        aciertoSound = LoadSound("./res/music/correct.mp3");
        errorSound = LoadSound("./res/music/fail.mp3");
        font = LoadFont("./res/fonts/crepes.ttf");

        fondoMenu = LoadTexture("./res/images/back_menu.png");
        fondoNiveles = LoadTexture("./res/images/back_niveles.png");
        fondoJuego = LoadTexture("./res/images/back_juego.png");
        fondoOpciones = LoadTexture("./res/images/back_opciones.png");
        fondoConfirmar = LoadTexture("./res/images/back_confirmar.png");
        fondoGanaste = LoadTexture("./res/images/back_ganaste.png");
        fondoPerdiste = LoadTexture("./res/images/back_perdiste.png");

        PlayMusicStream(musicaFondo);

        // Inicializar palabras (Fácil)
        {
            const char* f1[9]={"ca","sa","lo","es","za","er","la","ta","he"};
            palabrasFacil[0]=ParejaPalabraImagen("casa",f1,LoadTexture("./res/images/casa.png"));

            const char* f2[9]={"dien","te","ka","lo","pe","su","me","na","ri"};
            palabrasFacil[1]=ParejaPalabraImagen("diente",f2,LoadTexture("./res/images/diente.png"));

            const char* f3[9]={"fue","go","fa","lu","ra","mo","ne","zi","bo"};
            palabrasFacil[2]=ParejaPalabraImagen("fuego",f3,LoadTexture("./res/images/fuego.png"));

            const char* f4[9]={"la","piz","so","ta","nu","ke","vi","lo","zu"};
            palabrasFacil[3]=ParejaPalabraImagen("lapiz",f4,LoadTexture("./res/images/lapiz.png"));

            const char* f5[9]={"li","bro","ca","sa","ne","mo","ta","ye","le"};
            palabrasFacil[4]=ParejaPalabraImagen("libro",f5,LoadTexture("./res/images/libro.png"));

            const char* f6[9]={"llu","via","pa","re","zi","no","lu","mi","sa"};
            palabrasFacil[5]=ParejaPalabraImagen("lluvia",f6,LoadTexture("./res/images/lluvia.png"));

            const char* f7[9]={"me","sa","ma","es","ta","xi","zu","la","lo"};
            palabrasFacil[6]=ParejaPalabraImagen("mesa",f7,LoadTexture("./res/images/mesa.png"));

            const char* f8[9]={"nu","be","ti","fu","ro","me","za","li","pu"};
            palabrasFacil[7]=ParejaPalabraImagen("nube",f8,LoadTexture("./res/images/nube.png"));

            const char* f9[9]={"rue","da","sa","li","pa","mu","ta","ro","zu"};
            palabrasFacil[8]=ParejaPalabraImagen("rueda",f9,LoadTexture("./res/images/rueda.png"));

            const char* f10[9]={"ta","za","ma","se","lo","xa","ni","fu","ti"};
            palabrasFacil[9]=ParejaPalabraImagen("taza",f10,LoadTexture("./res/images/taza.png"));
            numPalabrasFacil=10;
        }

        // Medio
        {
            const char* m1[9]={"via","je","ros","se","lo","za","ni","fu","ti"};
            palabrasMedio[0]=ParejaPalabraImagen("viajeros",m1,LoadTexture("./res/images/viajeros.png"));

            const char* m2[9]={"pla","ta","no","li","pe","mu","ma","ro","zu"};
            palabrasMedio[1]=ParejaPalabraImagen("platano",m2,LoadTexture("./res/images/platano.png"));

            const char* m3[9]={"gi","ra","sol","fu","ro","me","za","li","pu"};
            palabrasMedio[2]=ParejaPalabraImagen("girasol",m3,LoadTexture("./res/images/girasol.png"));

            const char* m4[9]={"to","ma","te","es","ta","xi","zu","la","lo"};
            palabrasMedio[3]=ParejaPalabraImagen("tomate",m4,LoadTexture("./res/images/tomate.png"));

            const char* m5[9]={"za","pa","to","re","zi","no","lu","mi","sa"};
            palabrasMedio[4]=ParejaPalabraImagen("zapato",m5,LoadTexture("./res/images/zapato.png"));

            const char* m6[9]={"ven","ta","na","ha","nu","ke","vi","lo","zu"};
            palabrasMedio[5]=ParejaPalabraImagen("ventana",m6,LoadTexture("./res/images/ventana.png"));

            const char* m7[9]={"na","ran","ja","lo","pe","su","me","ta","ri"};
            palabrasMedio[6]=ParejaPalabraImagen("naranja",m7,LoadTexture("./res/images/naranja.png"));

            const char* m8[9]={"pa","ra","guas","lu","fa","mo","ne","zi","bo"};
            palabrasMedio[7]=ParejaPalabraImagen("paraguas",m8,LoadTexture("./res/images/paraguas.png"));

            const char* m9[9]={"ba","lle","na","es","za","er","la","ta","he"};
            palabrasMedio[8]=ParejaPalabraImagen("ballena",m9,LoadTexture("./res/images/ballena.png"));

            const char* m10[9]={"gu","sa","no","lo","pe","su","me","na","ri"};
            palabrasMedio[9]=ParejaPalabraImagen("gusano",m10,LoadTexture("./res/images/gusano.png"));
            numPalabrasMedio=10;
        }

        // Difícil
        {
            const char* d1[9]={"za","na","ho","ria","re","zi","no","lu","mi"};
            palabrasDificil[0]=ParejaPalabraImagen("zanahoria",d1,LoadTexture("./res/images/zanahoria.png"));

            const char* d2[9]={"di","no","sau","rio","li","pe","mu","ma","ro"};
            palabrasDificil[1]=ParejaPalabraImagen("dinosaurio",d2,LoadTexture("./res/images/dinosaurio.png"));

            const char* d3[9]={"e","le","fan","te","fu","ro","me","za","li"};
            palabrasDificil[2]=ParejaPalabraImagen("elefante",d3,LoadTexture("./res/images/elefante.png"));

            const char* d4[9]={"ca","mi","se","ta","es","ta","xi","zu","la"};
            palabrasDificil[3]=ParejaPalabraImagen("camiseta",d4,LoadTexture("./res/images/camiseta.png"));

            const char* d5[9]={"bi","ci","cle","ta","ha","nu","ke","vi","lo"};
            palabrasDificil[4]=ParejaPalabraImagen("bicicleta",d5,LoadTexture("./res/images/bicicleta.png"));

            const char* d6[9]={"ma","ri","po","sa","lo","pe","su","me","ta"};
            palabrasDificil[5]=ParejaPalabraImagen("mariposa",d6,LoadTexture("./res/images/mariposa.png"));

            const char* d7[9]={"co","co","dri","lo","lu","fa","mo","ne","zi"};
            palabrasDificil[6]=ParejaPalabraImagen("cocodrilo",d7,LoadTexture("./res/images/cocodrilo.png"));

            const char* d8[9]={"pe","li","cu","la","es","za","er","la","ta"};
            palabrasDificil[7]=ParejaPalabraImagen("pelicula",d8,LoadTexture("./res/images/pelicula.png"));

            const char* d9[9]={"te","le","fo","no","nu","ke","vi","lo","zu"};
            palabrasDificil[8]=ParejaPalabraImagen("telefono",d9,LoadTexture("./res/images/telefono.png"));

            const char* d10[9]={"pan","ta","lo","nes","se","lo","za","ni","fu"};
            palabrasDificil[9]=ParejaPalabraImagen("pantalones",d10,LoadTexture("./res/images/pantalones.png"));
            numPalabrasDificil=10;
        }

        for (int i = 0; i < 9; i++) posiciones[i] = i;
        textoConcatenado[0] = '\0';

        ChangeScreen(PantallaJuego::MENU);
    }

    ~Game() {
        if (currentScreen) delete currentScreen;

        for (int i = 0; i < numPalabrasFacil; i++) UnloadTexture(palabrasFacil[i].GetImagen());
        for (int i = 0; i < numPalabrasMedio; i++) UnloadTexture(palabrasMedio[i].GetImagen());
        for (int i = 0; i < numPalabrasDificil; i++) UnloadTexture(palabrasDificil[i].GetImagen());

        UnloadMusicStream(musicaFondo);
        UnloadSound(hoverSound);
        UnloadSound(clickSound);
        UnloadSound(aciertoSound);
        UnloadSound(errorSound);

        UnloadTexture(fondoMenu);
        UnloadTexture(fondoNiveles);
        UnloadTexture(fondoJuego);
        UnloadTexture(fondoOpciones);
        UnloadTexture(fondoConfirmar);
        UnloadTexture(fondoGanaste);
        UnloadTexture(fondoPerdiste);

        CloseAudioDevice();
        CloseWindow();
    }

    void Run() {
        while (!WindowShouldClose()) {
            UpdateMusic();
            currentScreen->Update(*this);

            BeginDrawing();
            ClearBackground(RAYWHITE);
            currentScreen->Draw(*this);
            EndDrawing();

            if (GetCurrentScreenType() == PantallaJuego::SALIR) break;
        }
    }

    void ChangeScreen(PantallaJuego newScreen);

    void UpdateMusic() {
        if (musicaFondoEnabled && 
           (GetCurrentScreenType()==PantallaJuego::MENU || GetCurrentScreenType()==PantallaJuego::SELECCION_NIVEL ||
            GetCurrentScreenType()==PantallaJuego::OPCIONES || GetCurrentScreenType()==PantallaJuego::SALIR)) 
        {
            UpdateMusicStream(musicaFondo);
            if (!IsMusicStreamPlaying(musicaFondo)) PlayMusicStream(musicaFondo);
        } else {
            StopMusicStream(musicaFondo);
        }
    }

    PantallaJuego GetCurrentScreenType() const { return currentScreen->GetType(); }

    // Getters y setters globales
    bool IsMusicaEnabled() const { return musicaFondoEnabled; }
    bool IsEfectosEnabled() const { return efectosSonidoEnabled; }
    void SetMusicaEnabled(bool val) { musicaFondoEnabled = val; }
    void SetEfectosEnabled(bool val) { efectosSonidoEnabled = val; }

    Font GetFontGame() const { return font; }
    Sound GetHoverSound() const { return hoverSound; }
    Sound GetClickSound() const { return clickSound; }
    Sound GetAciertoSound() const { return aciertoSound; }
    Sound GetErrorSound() const { return errorSound; }

    Texture2D GetFondoMenu() const { return fondoMenu; }
    Texture2D GetFondoNiveles() const { return fondoNiveles; }
    Texture2D GetFondoJuego() const { return fondoJuego; }
    Texture2D GetFondoOpciones() const { return fondoOpciones; }
    Texture2D GetFondoConfirmar() const { return fondoConfirmar; }
    Texture2D GetFondoGanaste() const { return fondoGanaste; }
    Texture2D GetFondoPerdiste() const { return fondoPerdiste; }

    ParejaPalabraImagen* GetPalabrasFacil() { return palabrasFacil; }
    ParejaPalabraImagen* GetPalabrasMedio() { return palabrasMedio; }
    ParejaPalabraImagen* GetPalabrasDificil() { return palabrasDificil; }

    int GetNumFacil() const { return numPalabrasFacil; }
    int GetNumMedio() const { return numPalabrasMedio; }
    int GetNumDificil() const { return numPalabrasDificil; }

    ParejaPalabraImagen* GetPalabrasSeleccionadas() { return palabrasSeleccionadas; }

    int &GetPalabraActual() { return palabraActual; }
    ParejaPalabraImagen &GetPalabraActualPareja() { return palabraActualPareja; }

    const char** GetTextosMezclados() { return textosMezclados; }
    char* GetTextoConcatenado() { return textoConcatenado; }

    int &GetContadorSeleccion() { return contadorSeleccion; }
    int GetMaxSelecciones() const { return maxSelecciones; }
    int &GetMaxSeleccionesRef() { return maxSelecciones; }

    int &GetVidas() { return vidas; }
    int &GetAciertos() { return aciertos; }

    float &GetAnguloRotacion() { return anguloRotacion; }
    bool &GetRotandoDerecha() { return rotandoDerecha; }

    bool &GetMostrandoResultado() { return mostrandoResultado; }
    float &GetTemporizadorResultado() { return temporizadorResultado; }
    float GetTiempoMostrarResultado() const { return tiempoMostrarResultado; }

    int *GetPosiciones() { return posiciones; }
    const char **GetTextos() { return textos; }

    void ReiniciarSilabas() {
        for (int i = 0; i < 9; i++) textos[i] = palabraActualPareja.GetSilaba(i).c_str();
        mezclarArreglo(posiciones, 9);
        for (int i = 0; i < 9; i++) textosMezclados[i] = textos[posiciones[i]];
    }

    void SeleccionarNivelFacil() {
        seleccionarPalabrasEImagenesAleatorias(palabrasFacil,numPalabrasFacil,palabrasSeleccionadas,10);
        palabraActual=0;
        palabraActualPareja=palabrasSeleccionadas[palabraActual];
        maxSelecciones=2;
        ReiniciarSilabas();
        ChangeScreen(PantallaJuego::JUEGO);
    }

    void SeleccionarNivelMedio() {
        seleccionarPalabrasEImagenesAleatorias(palabrasMedio,numPalabrasMedio,palabrasSeleccionadas,10);
        palabraActual=0;
        palabraActualPareja=palabrasSeleccionadas[palabraActual];
        maxSelecciones=3;
        ReiniciarSilabas();
        ChangeScreen(PantallaJuego::JUEGO);
    }

    void SeleccionarNivelDificil() {
        seleccionarPalabrasEImagenesAleatorias(palabrasDificil,numPalabrasDificil,palabrasSeleccionadas,10);
        palabraActual=0;
        palabraActualPareja=palabrasSeleccionadas[palabraActual];
        maxSelecciones=4;
        ReiniciarSilabas();
        ChangeScreen(PantallaJuego::JUEGO);
    }

    void ResetJuego() {
        contadorSeleccion=0;
        textoConcatenado[0]='\0';
        vidas=5;
        aciertos=0;
    }

    void ComprobarAcierto() {
        bool acierto=(strcmp(textoConcatenado,palabraActualPareja.GetPalabra().c_str())==0);
        if (acierto) {
            aciertos++;
            if (efectosSonidoEnabled) PlaySound(aciertoSound);
            palabraActual++;
        } else {
            vidas--;
            if (efectosSonidoEnabled) PlaySound(errorSound);
            if (vidas<=0) ChangeScreen(PantallaJuego::JUEGO_TERMINADO);
        }
        mostrandoResultado=true;
        temporizadorResultado=0.0f;
    }

    void ActualizarResultado() {
        temporizadorResultado+=GetFrameTime();
        if (temporizadorResultado>=tiempoMostrarResultado) {
            mostrandoResultado=false;
            temporizadorResultado=0.0f;
            contadorSeleccion=0;
            textoConcatenado[0]='\0';

            if (aciertos>=10) {
                ChangeScreen(PantallaJuego::FELICITACIONES);
            } else {
                if (palabraActual<10) {
                    palabraActualPareja=palabrasSeleccionadas[palabraActual];
                    ReiniciarSilabas();
                } else {
                    ChangeScreen(PantallaJuego::SELECCION_NIVEL);
                }
            }
        }
    }

    void ActualizarRotacion() {
        if (rotandoDerecha) {
            anguloRotacion+=0.50f;
            if (anguloRotacion>=10.0f) rotandoDerecha=false;
        } else {
            anguloRotacion-=0.50f;
            if (anguloRotacion<=-10.0f) rotandoDerecha=true;
        }
    }

    void SetCurrentScreen(Screen *scr) {
        if (currentScreen) delete currentScreen;
        currentScreen = scr;
    }
};

// Pantallas específicas

class MenuScreen : public Screen {
public:
    void Update(Game &game) override {
        Vector2 mousePos=GetMousePosition();
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2 -50,200,50})) {
                game.ChangeScreen(PantallaJuego::SELECCION_NIVEL);
            }
            if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2,200,50})) {
                game.ChangeScreen(PantallaJuego::OPCIONES);
            }
            if (CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2 +50,200,50})) {
                game.ChangeScreen(PantallaJuego::SALIR);
            }
        }
    }
    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoMenu();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)? ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

        DrawTextEx(f,"Write & Fun",{(float)ancho/2 - MeasureText("Write & Fun",40)/2,(float)alto/4},50,0,BLACK);

        Button btnJugar({ancho/2-100,(float)alto/2-50,200,50},"JUGAR",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        Button btnOpciones({ancho/2-100,(float)alto/2,200,50},"OPCIONES",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        Button btnSalir({ancho/2-100,(float)alto/2+50,200,50},"SALIR",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);

        btnJugar.Draw();
        btnOpciones.Draw();
        btnSalir.Draw();
    }
    PantallaJuego GetType() const override { return PantallaJuego::MENU; }
};

class SeleccionNivelScreen : public Screen {
public:
    void Update(Game &game) override {
        Vector2 mousePos=GetMousePosition();
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
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
    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoNiveles();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

        Button btnFacil({ancho/2 -100,alto/2 -100,200,50},"Fácil",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        Button btnMedio({ancho/2 -100,alto/2 -25,200,50},"Medio",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        Button btnDificil({ancho/2 -100,alto/2 +50,200,50},"Difícil",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);

        btnFacil.Draw();
        btnMedio.Draw();
        btnDificil.Draw();
    }
    PantallaJuego GetType() const override { return PantallaJuego::SELECCION_NIVEL; }
};

class OpcionesScreen : public Screen {
public:
    void Update(Game &game) override {
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
    void Draw(Game &game) override {
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
    PantallaJuego GetType() const override { return PantallaJuego::OPCIONES; }
};

class ConfirmarSalidaScreen : public Screen {
public:
    void Update(Game &game) override {
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
    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoConfirmar();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

        Button btnSi({(float)ancho/2 -100,(float)alto/2,80,40},"Sí",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        Button btnNo({(float)ancho/2 +20,(float)alto/2,80,40},"No",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);

        btnSi.Draw();
        btnNo.Draw();
    }
    PantallaJuego GetType() const override { return PantallaJuego::CONFIRMAR_SALIDA; }
};

class FelicidadesScreen : public Screen {
public:
    void Update(Game &game) override {
        Vector2 mousePos=GetMousePosition();
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/4,150,50})) {
            game.ChangeScreen(PantallaJuego::MENU);
            game.ResetJuego();
        }
    }
    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoGanaste();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

        Button btnMenu({(float)ancho/2 -100,(float)alto/4,150,50},"Menú",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        btnMenu.Draw();
    }
    PantallaJuego GetType() const override { return PantallaJuego::FELICITACIONES; }
};

class JuegoTerminadoScreen : public Screen {
public:
    void Update(Game &game) override {
        Vector2 mousePos=GetMousePosition();
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos,{ancho/2 -100,alto/2,150,50})) {
            game.ChangeScreen(PantallaJuego::MENU);
            game.ResetJuego();
        }
    }
    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoPerdiste();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

        Button btnMenu({(float)ancho/2 -100,(float)alto/2,150,50},"Menú",30,WHITE,GRAY,DARKGRAY,BLACK,game.GetHoverSound(),game.GetClickSound(),game.IsEfectosEnabled(),f);
        btnMenu.Draw();
    }
    PantallaJuego GetType() const override { return PantallaJuego::JUEGO_TERMINADO; }
};

// Pantalla de Juego
class JuegoScreen : public Screen {
public:
    void Update(Game &game) override {
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
        int inicioX = (int)(ancho / 2 - 0.25 * espacioX);
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

    void Draw(Game &game) override {
        float ancho=GetScreenWidth();
        float alto=GetScreenHeight();
        Font f=game.GetFontGame();
        Texture2D fondo=game.GetFondoJuego();
        float ratioAncho=ancho/(float)fondo.width;
        float ratioAlto=alto/(float)fondo.height;
        float ratio=(ratioAncho>ratioAlto)?ratioAncho:ratioAlto;
        DrawTexturePro(fondo,{0,0,(float)fondo.width,(float)fondo.height},{0,0,fondo.width*ratio,fondo.height*ratio},{0,0},0,WHITE);

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

    PantallaJuego GetType() const override { return PantallaJuego::JUEGO; }
};

void Game::ChangeScreen(PantallaJuego newScreen) {
    // Elimina la pantalla actual
    if (currentScreen) { delete currentScreen; currentScreen=nullptr; }

    switch(newScreen) {
        case PantallaJuego::MENU:
            currentScreen = new MenuScreen();
            break;
        case PantallaJuego::SELECCION_NIVEL:
            currentScreen = new SeleccionNivelScreen();
            break;
        case PantallaJuego::JUEGO:
            currentScreen = new JuegoScreen();
            break;
        case PantallaJuego::OPCIONES:
            currentScreen = new OpcionesScreen();
            break;
        case PantallaJuego::CONFIRMAR_SALIDA:
            currentScreen = new ConfirmarSalidaScreen();
            break;
        case PantallaJuego::FELICITACIONES:
            currentScreen = new FelicidadesScreen();
            break;
        case PantallaJuego::JUEGO_TERMINADO:
            currentScreen = new JuegoTerminadoScreen();
            break;
        case PantallaJuego::SALIR:
            // No se crea pantalla, se cerrará el juego en el bucle principal
            break;
    }
}

int main() {
    Game game;
    game.Run();
    return 0;
}
