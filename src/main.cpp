#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


typedef struct
{
    const char *palabra;
    const char *silabas[9];
    Texture2D imagen;
} ParejaPalabraImagen;

// Función para mezclar un arreglo de enteros
void mezclarArreglo(int *arreglo, int tamaño)
{
    for (int i = tamaño - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

// Función para seleccionar palabras e imágenes aleatorias de un arreglo
void seleccionarPalabrasEImagenesAleatorias(ParejaPalabraImagen *arregloFuente, int tamañoFuente, ParejaPalabraImagen *arregloObjetivo, int tamañoObjetivo)
{
    int *indices = (int *)malloc(tamañoFuente * sizeof(int));
    for (int i = 0; i < tamañoFuente; i++)
    {
        indices[i] = i;
    }

    mezclarArreglo(indices, tamañoFuente);

    for (int i = 0; i < tamañoObjetivo; i++)
    {
        arregloObjetivo[i] = arregloFuente[indices[i]];
    }

    free(indices);
}

// Función para dibujar un botón con efectos y sonidos
void DrawButton(Rectangle rect, const char *text, int fontSize, Color normalColor, Color hoverColor, Color clickColor, Color textColor, Sound hoverSound, Sound clickSound, bool soundEffectsEnabled, Font font)
{
    // Font font = LoadFont("./res/fonts/Medium.ttf");

    Vector2 mousePosition = GetMousePosition();
    Color currentColor = normalColor;
    int borderWidth = 2;
    static bool hoverSoundPlayed = false;

    if (CheckCollisionPointRec(mousePosition, rect))
    {
        if (!hoverSoundPlayed && soundEffectsEnabled)
        {
            PlaySound(hoverSound);
            hoverSoundPlayed = true;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            currentColor = clickColor;
            borderWidth = 5;
        }
        else
        {
            currentColor = hoverColor;
            borderWidth = 3;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && soundEffectsEnabled)
        {
            PlaySound(clickSound);
        }
    }
    else
    {
        hoverSoundPlayed = false;
    }

    DrawRectangleRec(rect, currentColor);
    DrawRectangleLinesEx(rect, borderWidth, DARKGRAY);

    int textWidth = MeasureText(text, fontSize);
    float textX = rect.x + (rect.width - textWidth) / 2;
    float textY = rect.y + (rect.height - fontSize) / 2;

    DrawTextEx(font, text, Vector2{textX, textY}, fontSize, 0, textColor);
}

int main(void)
{
    // Inicialización
    const int anchoPantallaInicial = 1600;
    const int altoPantallaInicial = 900;

    InitWindow(anchoPantallaInicial, altoPantallaInicial, "Write & Fun");
    SetWindowState(FLAG_FULLSCREEN_MODE);

    // Inicializar dispositivo de audio
    InitAudioDevice();

    // Cargar música y sonidos
    Music musicaFondo = LoadMusicStream("./res/music/fondo.mp3");
    Sound hoverSound = LoadSound("path/to/your/hover/sound.wav");
    Sound clickSound = LoadSound("path/to/your/click/sound.wav");
    Sound aciertoSound = LoadSound("./res/music/correct.mp3");
    Sound errorSound = LoadSound("./res/music/fail.mp3");

    // Cargar el font
    Font font = LoadFont("./res/fonts/crepes.ttf");

    // Reproducir música
    PlayMusicStream(musicaFondo);

    // Cargar la imagen de fondo
    Texture2D fondoMenu = LoadTexture("./res/images/back_menu.png");
    Texture2D fondoJuego = LoadTexture("./res/images/back_juego.png");
    Texture2D fondoNiveles = LoadTexture("./res/images/back_niveles.png");
    Texture2D fondoOpciones = LoadTexture("./res/images/back_opciones.png");
    Texture2D fondoConfirmar = LoadTexture("./res/images/back_confirmar.png");
    Texture2D fondoGanaste = LoadTexture("./res/images/back_ganaste.png");
    Texture2D fondoPerdiste = LoadTexture("./res/images/back_perdiste.png");

    SetTargetFPS(60);  // Establece el límite de FPS
    srand(time(NULL)); // Inicializa la semilla para números aleatorios

    // Definir estado del juego
    PantallaJuego pantallaActual = MENU;

    // Variables para controlar el sonido
    bool musicaFondoEnabled = true;
    bool efectosSonidoEnabled = true;

    // Arreglos de palabras e imágenes por nivel de dificultad
    ParejaPalabraImagen palabrasFacil[] = {
        {"casa", {"ca", "sa", "lo", "es", "za", "er", "la", "ta", "he"}, LoadTexture("./res/images/casa.png")},
        {"diente", {"dien", "te", "ka", "lo", "pe", "su", "me", "na", "ri"}, LoadTexture("./res/images/diente.png")},
        {"fuego", {"fue", "go", "fa", "lu", "ra", "mo", "ne", "zi", "bo"}, LoadTexture("./res/images/fuego.png")},
        {"lapiz", {"la", "piz", "so", "ta", "nu", "ke", "vi", "lo", "zu"}, LoadTexture("./res/images/lapiz.png")},
        {"libro", {"li", "bro", "ca", "sa", "ne", "mo", "ta", "ye", "le"}, LoadTexture("./res/images/libro.png")},
        {"lluvia", {"llu", "via", "pa", "re", "zi", "no", "lu", "mi", "sa"}, LoadTexture("./res/images/lluvia.png")},
        {"mesa", {"me", "sa", "ma", "es", "ta", "xi", "zu", "la", "lo"}, LoadTexture("./res/images/mesa.png")},
        {"nube", {"nu", "be", "ti", "fu", "ro", "me", "za", "li", "pu"}, LoadTexture("./res/images/nube.png")},
        {"rueda", {"rue", "da", "sa", "li", "pa", "mu", "ta", "ro", "zu"}, LoadTexture("./res/images/rueda.png")},
        {"taza", {"ta", "za", "ma", "se", "lo", "xa", "ni", "fu", "ti"}, LoadTexture("./res/images/taza.png")}};

    ParejaPalabraImagen palabrasMedio[] = {
        {"viajeros", {"via", "je", "ros", "se", "lo", "za", "ni", "fu", "ti"}, LoadTexture("./res/images/viajeros.png")},
        {"platano", {"pla", "ta", "no", "li", "pe", "mu", "ma", "ro", "zu"}, LoadTexture("./res/images/platano.png")},
        {"girasol", {"gi", "ra", "sol", "fu", "ro", "me", "za", "li", "pu"}, LoadTexture("./res/images/girasol.png")},
        {"tomate", {"to", "ma", "te", "es", "ta", "xi", "zu", "la", "lo"}, LoadTexture("./res/images/tomate.png")},
        {"zapato", {"za", "pa", "to", "re", "zi", "no", "lu", "mi", "sa"}, LoadTexture("./res/images/zapato.png")},
        {"ventana", {"ven", "ta", "na", "ha", "nu", "ke", "vi", "lo", "zu"}, LoadTexture("./res/images/ventana.png")},
        {"naranja", {"na", "ran", "ja", "lo", "pe", "su", "me", "ta", "ri"}, LoadTexture("./res/images/naranja.png")},
        {"paraguas", {"pa", "ra", "guas", "lu", "fa", "mo", "ne", "zi", "bo"}, LoadTexture("./res/images/paraguas.png")},
        {"ballena", {"ba", "lle", "na", "es", "za", "er", "la", "ta", "he"}, LoadTexture("./res/images/ballena.png")},
        {"gusano", {"gu", "sa", "no", "lo", "pe", "su", "me", "na", "ri"}, LoadTexture("./res/images/gusano.png")}};

    ParejaPalabraImagen palabrasDificil[] = {
        {"zanahoria", {"za", "na", "ho", "ria", "re", "zi", "no", "lu", "mi"}, LoadTexture("./res/images/zanahoria.png")},
        {"dinosaurio", {"di", "no", "sau", "rio", "li", "pe", "mu", "ma", "ro"}, LoadTexture("./res/images/dinosaurio.png")},
        {"elefante", {"e", "le", "fan", "te", "fu", "ro", "me", "za", "li"}, LoadTexture("./res/images/elefante.png")},
        {"camiseta", {"ca", "mi", "se", "ta", "es", "ta", "xi", "zu", "la"}, LoadTexture("./res/images/camiseta.png")},
        {"bicicleta", {"bi", "ci", "cle", "ta", "ha", "nu", "ke", "vi", "lo"}, LoadTexture("./res/images/bicicleta.png")},
        {"mariposa", {"ma", "ri", "po", "sa", "lo", "pe", "su", "me", "ta"}, LoadTexture("./res/images/mariposa.png")},
        {"cocodrilo", {"co", "co", "dri", "lo", "lu", "fa", "mo", "ne", "zi"}, LoadTexture("./res/images/cocodrilo.png")},
        {"pelicula", {"pe", "li", "cu", "la", "es", "za", "er", "la", "ta"}, LoadTexture("./res/images/pelicula.png")},
        {"telefono", {"te", "le", "fo", "no", "nu", "ke", "vi", "lo", "zu"}, LoadTexture("./res/images/telefono.png")},
        {"pantalones", {"pan", "ta", "lo", "nes", "se", "lo", "za", "ni", "fu"}, LoadTexture("./res/images/pantalones.png")}};

    const int numPalabrasFacil = sizeof(palabrasFacil) / sizeof(palabrasFacil[0]);
    const int numPalabrasMedio = sizeof(palabrasMedio) / sizeof(palabrasMedio[0]);
    const int numPalabrasDificil = sizeof(palabrasDificil) / sizeof(palabrasDificil[0]);

    ParejaPalabraImagen palabrasSeleccionadas[10];

    // Variables de juego
    int palabraActual = 0;
    ParejaPalabraImagen palabraActualPareja;
    const char *textos[9]; // Arreglo para las sílabas

    // Mezclar posiciones
    int posiciones[9];
    for (int i = 0; i < 9; i++)
    {
        posiciones[i] = i;
    }

    const char *textosMezclados[9];

    // Texto concatenado y contador de selecciones
    char textoConcatenado[20] = "";
    int contadorSeleccion = 0;
    int maxSelecciones = 3;

    // Contador de vidas
    int vidas = 5;

    // Contador de aciertos
    int aciertos = 0;

    // Ángulo de rotación y dirección
    float anguloRotacion = 0.0f;
    bool rotandoDerecha = true;

    // Variables para temporización
    bool mostrandoResultado = false;
    float temporizadorResultado = 0.0f;
    const float tiempoMostrarResultado = 2.0f;

    // Bucle principal
    while (!WindowShouldClose())
    {
        // Obtener el tamaño actual de la ventana
        float anchoPantalla = GetScreenWidth();
        float altoPantalla = GetScreenHeight();

        // Calcular posiciones y tamaños basados en el tamaño actual de la ventana
        int tamañoFuente = 10 + anchoPantalla / 80;

        // Tamaño de los recuadros
        float anchoRecuadro = anchoPantalla / 8;
        float altoRecuadro = altoPantalla / 8;

        // Espacio entre recuadros
        int espacioX = anchoRecuadro + anchoPantalla / 50;
        int espacioY = altoRecuadro + altoPantalla / 50;

        // Coordenadas iniciales
        int inicioX = anchoPantalla / 2 - 0.25 * espacioX;
        int inicioY = altoPantalla / 4;

        // Tamaño del rectángulo grande para la imagen
        int anchoRectGrande = anchoPantalla / 3;
        int altoRectGrande = altoPantalla - 175;

        // Coordenadas y tamaño del rectángulo inferior
        int rectInferiorX = anchoPantalla / 2 - 0.25 * anchoRecuadro;
        int rectInferiorY = altoPantalla - 200;
        int anchoRectInferior = 1 * anchoRecuadro + 2 * espacioX;
        int altoRectInferior = altoPantalla / 8;

        if (pantallaActual == MENU || pantallaActual == SELECCION_NIVEL || pantallaActual == OPCIONES || pantallaActual == SALIR)
        {
            if (musicaFondoEnabled)
            {
                // Actualiza la música en cada frame
                UpdateMusicStream(musicaFondo);
                if (!IsMusicStreamPlaying(musicaFondo))
                {
                    PlayMusicStream(musicaFondo); // Reproducir nuevamente si no está reproduciendo
                }
            }
            else
            {
                StopMusicStream(musicaFondo);
            }
        }
        else
        {
            StopMusicStream(musicaFondo); // Detener la música si no estamos en las pantallas especificadas
        }

        if (pantallaActual == MENU)
        {
            // Procesar entrada del usuario en el menú
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 posicionMouse = GetMousePosition();
                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 50, 200, 50}))
                {
                    pantallaActual = SELECCION_NIVEL;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 200, 50}))
                {
                    pantallaActual = OPCIONES;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 50}))
                {
                    pantallaActual = SALIR;
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            // Dibujar la imagen de fondo redimensionada
            float ratioAncho = (float)anchoPantalla / fondoMenu.width;
            float ratioAlto = (float)altoPantalla / fondoMenu.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRec = {0.0f, 0.0f, (float)fondoMenu.width, (float)fondoMenu.height};
            Rectangle destRec = {0, 0, fondoMenu.width * ratio, fondoMenu.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoMenu, sourceRec, destRec, origin, 0.0f, WHITE);

            float anchoPantallaFloat = anchoPantalla;
            float altoPantallaFloat = altoPantalla;

            DrawTextEx(font, "Write & Fun", Vector2{anchoPantallaFloat / 2 - MeasureText("Write & Fun", 40) / 2, altoPantallaFloat / 4}, tamañoFuente + 50, 0, BLACK);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 50, 200, 50}, "JUGAR", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 200, 50}, "OPCIONES", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 50}, "SALIR", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            EndDrawing();
        }

        if (pantallaActual == SELECCION_NIVEL)
        {
            // Procesar entrada del usuario en la selección de nivel
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 posicionMouse = GetMousePosition();
                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 100, 200, 50}))
                {
                    // Nivel fácil
                    seleccionarPalabrasEImagenesAleatorias(palabrasFacil, numPalabrasFacil, palabrasSeleccionadas, 10);
                    palabraActual = 0;
                    palabraActualPareja = palabrasSeleccionadas[palabraActual];
                    maxSelecciones = 2; // Establecer el límite de selecciones
                    for (int i = 0; i < 9; i++)
                        textos[i] = palabraActualPareja.silabas[i];
                    mezclarArreglo(posiciones, 9);
                    for (int i = 0; i < 9; i++)
                        textosMezclados[i] = textos[posiciones[i]];
                    pantallaActual = JUEGO;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 25, 200, 50}))
                {
                    // Nivel medio
                    seleccionarPalabrasEImagenesAleatorias(palabrasMedio, numPalabrasMedio, palabrasSeleccionadas, 10);
                    palabraActual = 0;
                    palabraActualPareja = palabrasSeleccionadas[palabraActual];
                    maxSelecciones = 3; // Establecer el límite de selecciones
                    for (int i = 0; i < 9; i++)
                        textos[i] = palabraActualPareja.silabas[i];
                    mezclarArreglo(posiciones, 9);
                    for (int i = 0; i < 9; i++)
                        textosMezclados[i] = textos[posiciones[i]];
                    pantallaActual = JUEGO;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 50}))
                {
                    // Nivel difícil
                    seleccionarPalabrasEImagenesAleatorias(palabrasDificil, numPalabrasDificil, palabrasSeleccionadas, 10);
                    palabraActual = 0;
                    palabraActualPareja = palabrasSeleccionadas[palabraActual];
                    maxSelecciones = 4; // Establecer el límite de selecciones
                    for (int i = 0; i < 9; i++)
                        textos[i] = palabraActualPareja.silabas[i];
                    mezclarArreglo(posiciones, 9);
                    for (int i = 0; i < 9; i++)
                        textosMezclados[i] = textos[posiciones[i]];
                    pantallaActual = JUEGO;
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            // Dibujar la imagen de fondo redimensionada
            float ratioAncho = (float)anchoPantalla / fondoNiveles.width;
            float ratioAlto = (float)altoPantalla / fondoNiveles.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRec = {0.0f, 0.0f, (float)fondoNiveles.width, (float)fondoNiveles.height};
            Rectangle destRec = {0, 0, fondoNiveles.width * ratio, fondoNiveles.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoNiveles, sourceRec, destRec, origin, 0.0f, WHITE);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 100, 200, 50}, "Facil", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 25, 200, 50}, "Medio", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 50}, "Dificil", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            EndDrawing();
        }
        if (pantallaActual == JUEGO)
        {
            // Actualizar ángulo de rotación
            if (rotandoDerecha)
            {
                anguloRotacion += 0.50f;
                if (anguloRotacion >= 10.0f)
                {
                    rotandoDerecha = false;
                }
            }
            else
            {
                anguloRotacion -= 0.50f;
                if (anguloRotacion <= -10.0f)
                {
                    rotandoDerecha = true;
                }
            }

            // Procesar entrada del usuario en el juego
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && contadorSeleccion < maxSelecciones && vidas > 0 && !mostrandoResultado)
            {
                Vector2 posicionMouse = GetMousePosition();
                for (int fila = 0; fila < 3; fila++)
                {
                    for (int columna = 0; columna < 3; columna++)
                    {
                        float posX = inicioX + columna * espacioX;
                        float posY = inicioY + fila * espacioY;
                        if (CheckCollisionPointRec(posicionMouse, (Rectangle){posX, posY, anchoRecuadro, altoRecuadro}))
                        {
                            strncat(textoConcatenado, textosMezclados[fila * 3 + columna], sizeof(textoConcatenado) - strlen(textoConcatenado) - 1);
                            contadorSeleccion++;
                        }
                    }
                }
            }

            // Verificar si se alcanzó el número máximo de selecciones
            if (contadorSeleccion == maxSelecciones && !mostrandoResultado)
            {
                int coincidenciaEncontrada = strcmp(textoConcatenado, palabraActualPareja.palabra) == 0;

                if (coincidenciaEncontrada)
                {
                    aciertos++;
                    if (efectosSonidoEnabled)
                        PlaySound(aciertoSound); // Reproducir sonido de acierto
                    palabraActual++;
                }
                else
                {
                    vidas--;
                    if (efectosSonidoEnabled)
                        PlaySound(errorSound); // Reproducir sonido de error
                    if (vidas <= 0)
                    {
                        pantallaActual = JUEGO_TERMINADO;
                    }
                }

                mostrandoResultado = true;
                temporizadorResultado = 0.0f;
            }

            if (mostrandoResultado)
            {
                temporizadorResultado += GetFrameTime();
                if (temporizadorResultado >= tiempoMostrarResultado)
                {
                    mostrandoResultado = false;
                    temporizadorResultado = 0.0f;
                    contadorSeleccion = 0;
                    textoConcatenado[0] = '\0';

                    if (aciertos >= 10)
                    {
                        pantallaActual = FELICITACIONES;
                    }
                    else
                    {
                        if (palabraActual < 10)
                        {
                            palabraActualPareja = palabrasSeleccionadas[palabraActual];
                            for (int i = 0; i < 9; i++)
                                textos[i] = palabraActualPareja.silabas[i];
                            mezclarArreglo(posiciones, 9);
                            for (int i = 0; i < 9; i++)
                                textosMezclados[i] = textos[posiciones[i]];
                        }
                        else
                        {
                            pantallaActual = SELECCION_NIVEL;
                        }
                    }
                }
            }

            // Procesar entrada del usuario para el botón de regreso al menú
            Vector2 posicionMouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(posicionMouse, (Rectangle){10, 30, 50, 50}))
            {
                pantallaActual = CONFIRMAR_SALIDA;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            float ratioAncho = (float)anchoPantalla / fondoJuego.width;
            float ratioAlto = (float)altoPantalla / fondoJuego.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRecPro = {0.0f, 0.0f, (float)fondoJuego.width, (float)fondoJuego.height};
            Rectangle destRecPro = {0, 0, fondoJuego.width * ratio, fondoJuego.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoJuego, sourceRecPro, destRecPro, origin, 0.0f, WHITE);

            // Dibujar el botón de regreso al menú con esquinas redondeadas
            DrawButton((Rectangle){10, 30, 50, 50}, "<-", 30, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            // Dibujar la barra de progreso
            DrawRectangle(0, 0, anchoPantalla * aciertos / 10, 20, GREEN);

            // Dibujar el rectángulo grande a la izquierda y mostrar la imagen correspondiente
            // DrawRectangle(50, 100, anchoRectGrande, altoRectGrande, WHITE);

            // Calcular la posición y tamaño de la imagen para centrarla en el rectángulo
            float imagenX = 50 + anchoRectGrande / 2;
            float imagenY = 50 + altoRectGrande / 2;
            Rectangle sourceRec = {0.0f, 0.0f, (float)palabraActualPareja.imagen.width, (float)palabraActualPareja.imagen.height};
            Rectangle destRec = {imagenX, imagenY, (float)palabraActualPareja.imagen.width, (float)palabraActualPareja.imagen.height};
            Vector2 origen = {(float)palabraActualPareja.imagen.width / 2, (float)palabraActualPareja.imagen.height / 2};

            DrawTexturePro(palabraActualPareja.imagen, sourceRec, destRec, origen, anguloRotacion, WHITE);

            // Dibujar múltiples textos en una cuadrícula 3x3 usando bucles for anidados
            for (int fila = 0; fila < 3; fila++)
            {
                for (int columna = 0; columna < 3; columna++)
                {
                    float posX = inicioX + columna * espacioX;
                    float posY = inicioY + fila * espacioY;
                    Rectangle rec = {posX, posY, anchoRecuadro, altoRecuadro};

                    // Dibujar el botón de la cuadrícula
                    DrawButton(rec, textosMezclados[fila * 3 + columna], tamañoFuente, WHITE, GRAY, DARKGRAY, BLACK, hoverSound, clickSound, efectosSonidoEnabled, font);
                }
            }

            // Dibujar el rectángulo inferior
            DrawRectangle(rectInferiorX, rectInferiorY, anchoRectInferior, altoRectInferior, WHITE);

            // Dibujar el texto concatenado en el rectángulo inferior y centrarlo
            int anchoTextoConcatenado = MeasureText(textoConcatenado, tamañoFuente);
            float textoConcatenadoPosX = rectInferiorX + (anchoRectInferior - anchoTextoConcatenado) / 2;
            float textoConcatenadoPosY = rectInferiorY + (altoRectInferior - tamañoFuente) / 2;
            DrawTextEx(font, textoConcatenado, Vector2{textoConcatenadoPosX, textoConcatenadoPosY}, tamañoFuente, 0, BLACK);

            // Mostrar las vidas restantes
            char textoVidas[20];
            sprintf(textoVidas, "Vidas: %d", vidas);

            float anchoPantallaFloat = anchoPantalla;
            DrawTextEx(font, textoVidas, Vector2{anchoPantallaFloat - 150, 20}, tamañoFuente, 0, RED);

            EndDrawing();
        }

        if (pantallaActual == CONFIRMAR_SALIDA)
        {
            // Procesar entrada del usuario en la pantalla de confirmación de salida
            Vector2 posicionMouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 80, 40}))
                {
                    pantallaActual = MENU;
                    // Resetear variables del juego
                    contadorSeleccion = 0;
                    textoConcatenado[0] = '\0';
                    vidas = 5;
                    aciertos = 0;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 + 20, altoPantalla / 2, 80, 40}))
                {
                    pantallaActual = JUEGO;
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            float ratioAncho = (float)anchoPantalla / fondoConfirmar.width;
            float ratioAlto = (float)altoPantalla / fondoConfirmar.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRecPro = {0.0f, 0.0f, (float)fondoConfirmar.width, (float)fondoConfirmar.height};
            Rectangle destRecPro = {0, 0, fondoConfirmar.width * ratio, fondoConfirmar.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoConfirmar, sourceRecPro, destRecPro, origin, 0.0f, WHITE);

            // Dibujar botones de confirmación
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 80, 40}, "Si", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 + 20, altoPantalla / 2, 80, 40}, "No", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            EndDrawing();
        }

        if (pantallaActual == FELICITACIONES)
        {
            // Procesar entrada del usuario en la pantalla de felicitaciones
            Vector2 posicionMouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 4, 150, 50}))
            {
                pantallaActual = MENU;
                // Resetear variables del juego
                contadorSeleccion = 0;
                textoConcatenado[0] = '\0';
                vidas = 5;
                aciertos = 0;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            float ratioAncho = (float)anchoPantalla / fondoGanaste.width;
            float ratioAlto = (float)altoPantalla / fondoGanaste.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRecPro = {0.0f, 0.0f, (float)fondoGanaste.width, (float)fondoGanaste.height};
            Rectangle destRecPro = {0, 0, fondoGanaste.width * ratio, fondoGanaste.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoGanaste, sourceRecPro, destRecPro, origin, 0.0f, WHITE);

            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 4, 150, 50}, "Menu", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            EndDrawing();
        }

        if (pantallaActual == JUEGO_TERMINADO)
        {
            // Procesar entrada del usuario en la pantalla de juego terminado
            Vector2 posicionMouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 150, 50}))
            {
                pantallaActual = MENU;
                // Resetear variables del juego
                contadorSeleccion = 0;
                textoConcatenado[0] = '\0';
                vidas = 5;
                aciertos = 0;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            float ratioAncho = (float)anchoPantalla / fondoPerdiste.width;
            float ratioAlto = (float)altoPantalla / fondoPerdiste.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRecPro = {0.0f, 0.0f, (float)fondoPerdiste.width, (float)fondoPerdiste.height};
            Rectangle destRecPro = {0, 0, fondoPerdiste.width * ratio, fondoPerdiste.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoPerdiste, sourceRecPro, destRecPro, origin, 0.0f, WHITE);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2, 150, 50}, "Menu", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);

            EndDrawing();
        }

        if (pantallaActual == OPCIONES)
        {
            // Procesar entrada del usuario para el botón de regreso al menú
            Vector2 posicionMouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(posicionMouse, (Rectangle){10, 30, 50, 50}))
                {
                    pantallaActual = MENU;
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 50, 200, 50}))
                {
                    musicaFondoEnabled = !musicaFondoEnabled; // Toggle música de fondo
                }

                if (CheckCollisionPointRec(posicionMouse, (Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 50}))
                {
                    efectosSonidoEnabled = !efectosSonidoEnabled; // Toggle efectos de sonido
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Dibujar la imagen de fondo redimensionada
            float ratioAncho = (float)anchoPantalla / fondoOpciones.width;
            float ratioAlto = (float)altoPantalla / fondoOpciones.height;
            float ratio = (ratioAncho > ratioAlto) ? ratioAncho : ratioAlto;

            Rectangle sourceRec = {0.0f, 0.0f, (float)fondoOpciones.width, (float)fondoOpciones.height};
            Rectangle destRec = {0, 0, fondoOpciones.width * ratio, fondoOpciones.height * ratio};
            Vector2 origin = {0, 0};

            DrawTexturePro(fondoOpciones, sourceRec, destRec, origin, 0.0f, WHITE);

            DrawButton((Rectangle){10, 30, 50, 50}, "<-", 30, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 - 50, 300, 50}, musicaFondoEnabled ? "Desactivar Musica" : "Activar Musica", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            DrawButton((Rectangle){anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 300, 50}, efectosSonidoEnabled ? "Desactivar Efectos" : "Activar Efectos", tamañoFuente, WHITE, GRAY, DARKGRAY, DARKGRAY, hoverSound, clickSound, efectosSonidoEnabled, font);
            EndDrawing();
        }

        if (pantallaActual == SALIR)
        {
            CloseWindow();
        }
    }

    // Desinicialización
    for (int i = 0; i < numPalabrasFacil; i++)
        UnloadTexture(palabrasFacil[i].imagen);
    for (int i = 0; i < numPalabrasMedio; i++)
        UnloadTexture(palabrasMedio[i].imagen);
    for (int i = 0; i < numPalabrasDificil; i++)
        UnloadTexture(palabrasDificil[i].imagen);

    // Descargar música y sonidos, y cerrar dispositivo de audio
    UnloadMusicStream(musicaFondo);
    UnloadSound(hoverSound);
    UnloadSound(clickSound);
    UnloadSound(aciertoSound);
    UnloadSound(errorSound);
    UnloadTexture(fondoMenu); // Descargar la textura del fondo del menú
    CloseAudioDevice();
    CloseWindow();

    return 0;
}