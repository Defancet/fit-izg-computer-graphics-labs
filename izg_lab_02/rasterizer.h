/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * ipolasek@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Rasterizator polygonalnich modelu vyuzivajici studentsky kod
 *
 * Opravy a modifikace:
 * - 
 */

#include <cstdlib>
#include <string>
#include <vector>

#include "color.h"

#ifndef Rasterizer_H
#define Rasterizer_H

/* Konstanta PI, neboli Ludolfovo cislo */
static constexpr auto PI{ 3.14159265358f };
/* 2 PI */
static constexpr auto PI_2{ 2.0f * PI };

/* Pomocna struktura reprezentujici bod v prostoru homogennich koordinatu. */
struct alignas(float[4]) Vector 
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
    };

    /* Inicializace vektoru ze zadanych hodnot */
    Vector(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f, float vw = 0.0f) : 
        x{ vx }, y{ vy }, z{ vz }, w{ vw }
    { }

    /* Vypocitej delku tohoto vektoru ve 3D */
    float length() const;

    /* Vypocitej normalizovanou verzi tohoto vektoru ve 3D */
    Vector normalized() const;

    /* Vypocitej cross-product this x other ve 3D */
    Vector cross(const Vector& other) const;

    /* Vypocitej dot-product this . other ve 3D */
    float dot(const Vector& other) const;

    /* Ziskej tento vektor ve forme bodu (w=1) */
    Vector asPoint() const;

    /* Ziskej tento vektor ve forme vektoru (w=0) */
    Vector asVector() const;

    /* Pomocny operator pro indexovani vektoru */
    float &operator[](std::size_t idx);
    /* Pomocny operator pro indexovani vektoru */
    float operator[](std::size_t idx) const;

    /* Operator pro negaci vektoru ve 3D */
    Vector operator-() const;
    /* Operator pro odecitani vektoru ve 3D */
    Vector operator-(const Vector &other) const;
    /* Operator pro scitani vektoru ve 3D */
    Vector operator+(const Vector &other) const;
    /* Operator pro nasobeni vektoru konstantou ve 3D */
    Vector operator*(float val) const;
    /* Operator pro deleni vektoru konstantou ve 3D */
    Vector operator/(float val) const;

    /* Operator pro porovnani dvou vektoru ve 3D na rovnost (pozor na float!) */
    bool operator==(const Vector &other) const;
    /* Operator pro porovnani dvou vektoru ve 3D na nerovnost (pozor na float!) */
    bool operator!=(const Vector &other) const;
}; // struct Vector

/* Matice 4x4 */
struct alignas(float[4]) Matrix
{
    union
    {
        Vector m[4];
        float v[16]{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    };

    /* Inicializace matice ze zadanych hodnot */
    Matrix(const Vector &v1 = { }, const Vector &v2 = { }, 
        const Vector &v3 = { }, const Vector &v4 = { }) :
        m{ v1, v2, v3, v4 }
    { }

    /* Inicializace matice ze zadanych hodnot */
    Matrix(float v11, float v12, float v13, float v14, 
        float v21, float v22, float v23, float v24, 
        float v31, float v32, float v33, float v34, 
        float v41, float v42, float v43, float v44) : 
        v{ v11, v12, v13, v14, 
        v21, v22, v23, v24, 
        v31, v32, v33, v34, 
        v41, v42, v43, v44}
    { }

    /* Nastav matici na hodnotu identity */
    void identity();

    /* Nastav matici jako pohledovou matici pro danou kameru */
    void lookAt(const Vector &cameraPos, const Vector &lookAt, const Vector &up);

    /* Nastav matici jako projekcni matici pro danou kameru */
    void perspective(float fov, float near, float far, float aspect);

    /* Nastav matici jako skalovaci matici pro danou velikost */
    void scale(float s);

    /* Nastav matici jako skalovaci matici pro danou velikost */
    void scale(const Vector &s);

    /* Nastav matici jako rotacni matici pro dane euler rotacni uhly */
    void rotate(const Vector &r);

    /* Nastav matici jako translacni matici pro dany posuv */
    void translate(const Vector &t);

    /* Nastav matici jako skalovani->rotaci->translaci */
    void scaleRotateTranslate(const Vector &s, const Vector &r, const Vector &t);

    /* Nasobebni radkoveho vektoru a matice p * this */
    Vector multiply(const Vector& p) const;

    /* Nasobebni matice s matici this * o */
    Matrix multiply(const Matrix& o) const;

    /* Pomocny operator pro indexovani matice */
    Vector &operator[](std::size_t idx);
    /* Pomocny operator pro indexovani matice */
    Vector operator[](std::size_t idx) const;
}; // struct Matrix

/* Informace o kamere */
struct Camera
{
    /* Pozice kamery */
    Vector position{ 0.0f, 0.0f, 5.0f };
    /* Ohnisko kamery */
    Vector lookAt{ 0.0f, 0.0f, 0.0f };

    /* Vyska vystupního rastru v pixelech */
    int width{ 800 };
    /* Sirka vystupniho rastru v pixelech */
    int height{ 600 };
    /* Uhel horizontalniho zorneho pole */
    float fov{ 60.0f };
    /* Pozice near plane */
    float pNear{ 0.001f };
    /* Pozice far plane */
    float pFar{ 10.0f };
}; // struct Camera

/* Pomocná struktura pro projekce bodu ve 3D */
struct Projector
{
    /* Informace o kamere */
    Camera camera{ };

    /* Škálování vykreslovaného objektu */
    Vector scale{ 1.0f, 1.0f, 1.0f };
    /* Rotace vykreslovaného objektu */
    Vector rotate{ 0.0f, 0.0f, 0.0f };
    /* Translace vykreslovaného objektu */
    Vector translate{ 0.0f, 0.0f, 0.0f };

    /* Provede projekci vstupni pozice dle aktualne nastavenych parametru projektoru */
    Vector projectVector(const Vector& position) const;
}; // struct Projector

/* Pomocna struktura reprezentujici trojuhelnik */
struct Triangle
{
    /* Inicializace trojuhelniku ze zadanych hodnot */
    Triangle(std::size_t i1 = 0u, std::size_t i2 = 0u, std::size_t i3 = 0u) : 
        idx1{ i1 }, idx2{ i2 }, idx3{ i3 }
    { }

    std::size_t idx1{ 0u };
    std::size_t idx2{ 0u };
    std::size_t idx3{ 0u };
}; // struct Triangle

/* Pomocna struktura obsahujici data polygonalniho modelu */
struct Model
{
    /* Seznam vertexu modelu */
    std::vector<Vector> vertices{ };
    /* Seznam trojuhelnikovych stran v modelu */
    std::vector<Triangle> faces{ };

    /* Škálování modelu */
    Vector scale{ 1.0f, 1.0f, 1.0f };
    /* Rotace modelu */
    Vector rotate{ 0.0f, 0.0f, 0.0f };
    /* Translace modelu */
    Vector translate{ 0.0f, 0.0f, 0.0f };
}; // struct Model

/* Projekce vstupniho vektoru pouzitim projektivni kamery s danymi parametry */
Vector projectVector(const Vector &position,
    const Vector &scale, const Vector &rotate, const Vector &translate,
    const Vector &cameraPosition, const Vector &cameraLookAt,
    int width, int height, float fov, float near, float far);

/* Projekce vstupniho vektoru pouzitim projektivni kamery s danymi parametry */
Vector projectVector(const Vector &position, const Projector &projector);

/* Nacti OBJ soubor a uloz vertexy a trojuhelniky do bufferu */
bool loadObjFile(const std::string &filename, 
    std::vector<Vector> &vertices,
    std::vector<Triangle> &faces);

/* Nacti model kostky do bufferu */
bool loadCube(std::vector<Vector> &vertices,
    std::vector<Triangle> &faces);

/* Vykresleni polygonalniho modelu */
void drawPolygons(const std::vector<Vector> &vertices,
    const std::vector<Triangle> &faces,
    S_RGBA lineColor, S_RGBA circleColor,
    const Projector &projector);

/* Vykresleni polygonalniho modelu */
void drawPolygons(const std::vector<Vector> &vertices,
    const std::vector<Triangle> &faces,
    S_RGBA lineColor, S_RGBA circleColor,
    const Vector& scale = { 20.0f, -20.0f, 20.0f },
    const Vector& rotate = { 0.0f, 0.0f, 0.0f },
    const Vector& translate = { 0.0f, 0.0f, 0.0f },
    const Camera& camera = { });

#endif /* Student_H */

/*****************************************************************************/
/*****************************************************************************/
