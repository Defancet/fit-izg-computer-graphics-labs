/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * ipolasek@fit.vutbr.cz
 *
 * Popis: Rasterizator polygonalnich modelu vyuzivajici studentsky kod
 *
 * Opravy a modifikace:
 * - 
 */

#include "rasterizer.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include "io.h"
#include "base.h"
#include "globals.h"
#include "student.h"

float Vector::length() const
{ return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

Vector Vector::normalized() const
{
    const auto l{ length() };
    return l > 0.0f ? Vector{ v[0] / l, v[1] / l, v[2] / l } : *this;
}

Vector Vector::cross(const Vector& other) const
{
    return Vector{
        v[1] * other[2] - v[2] * other[1], 
        v[2] * other[0] - v[0] * other[2], 
        v[0] * other[1] - v[1] * other[0]
    };
}

float Vector::dot(const Vector& other) const
{ return v[0] * other[0] + v[1] * other[1] + v[2] * other[2]; }

Vector Vector::asPoint() const
{ return Vector{ v[0], v[1], v[2], 1.0f }; }

Vector Vector::asVector() const
{ return Vector{ v[0], v[1], v[2], 0.0f }; }

float &Vector::operator[](std::size_t idx) 
{ return v[idx]; }
float Vector::operator[](std::size_t idx) const 
{ return v[idx]; }

Vector Vector::operator-() const
{ return Vector{ -v[0], -v[1], -v[2] }; }
Vector Vector::operator-(const Vector& other) const
{ return *this + (-other); }
Vector Vector::operator+(const Vector& other) const
{ return Vector{ v[0] + other[0], v[1] + other[1], v[2] + other[2] }; }
Vector Vector::operator*(float val) const
{ return Vector{ v[0] * val, v[1] * val, v[2] * val }; }
Vector Vector::operator/(float val) const
{ return Vector{ v[0] / val, v[1] / val, v[2] / val }; }

bool Vector::operator==(const Vector &other) const
{ return v[0] == other[0] && v[1] == other[1] && v[2] == other[2]; }
bool Vector::operator!=(const Vector &other) const
{ return !(*this == other); }

void Matrix::identity()
{
    v[ 0] = 1.0f; v[ 1] = 0.0f; v[ 2] = 0.0f; v[ 3] = 0.0f; 
    v[ 4] = 0.0f; v[ 5] = 1.0f; v[ 6] = 0.0f; v[ 7] = 0.0f; 
    v[ 8] = 0.0f; v[ 9] = 0.0f; v[10] = 1.0f; v[11] = 0.0f; 
    v[12] = 0.0f; v[13] = 0.0f; v[14] = 0.0f; v[15] = 1.0f; 
}

void Matrix::lookAt(const Vector &cameraPos, const Vector &lookAt, const Vector &up)
{
    /* Zdroj: scratchapixel.com */
    identity();

    const auto forward{ (lookAt - cameraPos).normalized() };
    const auto right{
        fabs(forward.dot(up.normalized())) >= 0.9999f ? 
        (up + Vector{ 0.1f, 0.1f, 0.1f}).normalized().cross(forward).normalized() : 
        up.normalized().cross(forward).normalized()
    };
    const auto upCorrected{ forward.cross(right).normalized() };

    m[0] = right.asVector();
    m[1] = upCorrected.asVector();
    m[2] = forward.asVector();
    m[3] = {
        -right.dot(cameraPos),
        -upCorrected.dot(cameraPos),
        -forward.dot(cameraPos),
        1.0f
    };
}

void Matrix::perspective(float fov, float near, float far, float aspect)
{
    /* Zdroj: scratchapixel.com */
    identity();

    const auto s{ 1.0f / tanf(fov * 0.5f * PI / 180.0f) };

    m[0][0] = s * aspect;
    m[1][1] = s;
    m[2][2] = -far / (far - near);
    m[3][2] = (-far * near) / (far - near);
    m[2][3] = -1.0f;
    m[3][3] = 0.0f;
}

void Matrix::scale(float s)
{
    identity();

    m[0][0] = s; m[1][1] = s;
    m[2][2] = s; m[3][3] = 1.0f;
}

void Matrix::scale(const Vector &s)
{
    identity();

    m[0][0] = s[0]; m[1][1] = s[1];
    m[2][2] = s[2]; m[3][3] = 1.0f;
}

void Matrix::rotate(const Vector &r)
{
    /* Zdroj: scratchapixel.com */
    identity();

    const auto xSin{ sinf(r.x) };
    const auto xCos{ cosf(r.x) };
    const auto ySin{ sinf(r.y) };
    const auto yCos{ cosf(r.y) };
    const auto zSin{ sinf(r.z) };
    const auto zCos{ cosf(r.z) };

    m[0][0] = yCos * zCos;
    m[0][1] = xSin * ySin * zCos - xCos * zSin;
    m[0][2] = xCos * ySin * zCos + xSin * zSin;

    m[1][0] = yCos * zSin;
    m[1][1] = xSin * ySin * zSin + xCos * zCos;
    m[1][2] = xCos * ySin * zSin - xSin * zCos;

    m[2][0] = -ySin;
    m[2][1] = xSin * yCos;
    m[2][2] = xCos * yCos;
}

void Matrix::translate(const Vector &t)
{
    identity();

    m[3] = t.asPoint();
}

void Matrix::scaleRotateTranslate(const Vector &s, const Vector &r, const Vector &t)
{
    Matrix sm{ }; sm.scale(s);
    Matrix rm{ }; rm.rotate(r);
    Matrix tm{ }; tm.translate(t);

    const auto srt{ sm.multiply(rm.multiply(tm)) };
    m[0] = srt[0]; m[1] = srt[1];
    m[2] = srt[2]; m[3] = srt[3];
}

Vector Matrix::multiply(const Vector& p) const 
{
    return Vector{
        p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0] + p[3] * m[3][0], 
        p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1] + p[3] * m[3][1], 
        p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] + p[3] * m[3][2], 
        p[0] * m[0][3] + p[1] * m[1][3] + p[2] * m[2][3] + p[3] * m[3][3], 
    };
    /*
    return Vector{
        m[0][0] * p[0] + m[0][1] * p[1] + m[0][2] * p[2] + m[0][3] * p[3], 
        m[1][0] * p[0] + m[1][1] * p[1] + m[1][2] * p[2] + m[1][3] * p[3], 
        m[2][0] * p[0] + m[2][1] * p[1] + m[2][2] * p[2] + m[2][3] * p[3], 
        m[3][0] * p[0] + m[3][1] * p[1] + m[3][2] * p[2] + m[3][3] * p[3], 
    };
    */
}

Matrix Matrix::multiply(const Matrix& o) const
{
    const auto& m1{ m };
    const auto& m2{ o.m };
    return Matrix{
        m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0], 
        m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1], 
        m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2], 
        m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3], 

        m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0], 
        m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1], 
        m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2], 
        m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3], 

        m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0], 
        m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1], 
        m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2], 
        m1[2][0] * m2[0][3] + m1[2][1] * m2[1][3] + m1[2][2] * m2[2][3] + m1[2][3] * m2[3][3], 

        m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0], 
        m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1], 
        m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2], 
        m1[3][0] * m2[0][3] + m1[3][1] * m2[1][3] + m1[3][2] * m2[2][3] + m1[3][3] * m2[3][3]
    };
}

Vector &Matrix::operator[](std::size_t idx)
{ return m[idx]; }
Vector Matrix::operator[](std::size_t idx) const
{ return m[idx]; }

/* Provede projekci vstupni pozice dle aktualne nastavenych parametru projektoru */
Vector Projector::projectVector(const Vector& position) const
{
    /* Modelova matice pouzita pro skalovani, rotaci a translaci */
    Matrix model{ };
    model.scaleRotateTranslate(scale, rotate, translate);

    /* Pohledova matice nastavena na pozici kamery */
    Matrix view{ };
    if (camera.position == camera.lookAt)
    { view.lookAt(camera.position, Vector{ 0.0f, 0.0f, -1.0f }, Vector{ 0.0f, 1.0f, 0.0f }); }
    else
    { view.lookAt(camera.position, camera.lookAt, Vector{ 0.0f, 1.0f, 0.0f }); }

    /* Projekcni matice podle vlastnosti kamery */
    Matrix projection{ };
    projection.perspective(camera.fov, camera.pNear, camera.pFar, static_cast<float>(gHeight) / gWidth);

    /* Kombinace matic m * v * p -> MVP */
    Matrix mvp{ model.multiply(view.multiply(projection)) };

    /* Project the point and transform back into point by perspective division */
    const auto projectedPosition{ mvp.multiply(position.asPoint()) };
    const auto projectedPoint{
        abs(projectedPosition.w) > std::numeric_limits<float>::epsilon() ? 
        projectedPosition / projectedPosition.w : projectedPosition.asPoint()
    };

    return Vector{
        std::max<float>(0.0f, std::min<float>(gWidth - 1.0f, 
            (projectedPoint.x + 1.0f) * 0.5f * gWidth)), 
        std::max<float>(0.0f, std::min<float>(gHeight - 1.0f, 
            (1.0f - (projectedPoint.y + 1.0f) * 0.5f) * gHeight)), 
        0.0f, 1.0f
    };
}

/* Projekce vstupniho vektoru pouzitim projektivni kamery s danymi parametry */
Vector projectVector(const Vector &position,
    const Vector &scale, const Vector &rotate, const Vector &translate,
    const Vector &cameraPosition, const Vector &cameraLookAt,
    int width, int height, float fov, float near, float far)
{
    /* Nastav projektor podle pozadavku */
    Projector p{ };
    p.camera.position = cameraPosition; p.camera.lookAt = cameraPosition;
    p.scale = scale; p.rotate = rotate; p.translate = translate;
    p.camera.width = width; p.camera.height = height;
    p.camera.fov = fov; p.camera.pNear = near; p.camera.pFar = far;

    /* Transformuj vstupni bod */
    return p.projectVector(position);
}

/* Projekce vstupniho vektoru pouzitim projektivni kamery s danymi parametry */
Vector projectVector(const Vector& position, const Projector& projector)
{ return projector.projectVector(position); }

/* Nacti OBJ soubor a uloz vertexy a trojuhelniky do bufferu */
bool loadObjFile(const std::string &filename, 
    std::vector<Vector> &vertices,
    std::vector<Triangle> &faces)
{
    /* Otevri soubor a otestuj, zda byla operace uspesna. */
    std::ifstream modelFile(filename);
    if(!modelFile.is_open()) 
    { return false; }

#define USE_REGEX_PARSER false

#if USE_REGEX_PARSER
    /* Priprav jednoduchy parser Wavefront OBJ souboru pomoci regularnich vyrazu. */
    const std::regex lineParser{
        // Typ zaznamu: 
        R"(^([a-z]) )"
        // Prvni hodnota ve formatu +-1.123e+-123: 
        R"(([+-]?[0-9]*\.[0-9]*(?:e[+-]?[0-9]+)?) )"
        // Druha hodnota ve formatu +-1.123e+-123: 
        R"(([+-]?[0-9]*\.[0-9]*(?:e[+-]?[0-9]+)?) )"
        // Treti hodnota ve formatu +-1.123e+-123: 
        R"(([+-]?[0-9]*\.[0-9]*(?:e[+-]?[0-9]+)?)$)"
    };
    std::smatch matches{ };
#endif // USE_REGEX_PARSER

    std::vector<Vector> vs{ };
    std::vector<Triangle> ts{ };

    for(std::string line; std::getline(modelFile, line); ) 
    {
#if USE_REGEX_PARSER
        if (!std::regex_match(line, matches, lineParser))
        { continue; }

        const auto recordType{ matches[1].str()[0] };
        const auto val1{ std::stof(matches[2].str()) };
        const auto val2{ std::stof(matches[3].str()) };
        const auto val3{ std::stof(matches[4].str()) };
#else // USE_REGEX_PARSER
        if (line.empty())
        { continue; }

        std::stringstream ss{ line };

        std::string recordTypeStr{ };
        float val1{ 0.0f };
        float val2{ 0.0f };
        float val3{ 0.0f };
        ss >> recordTypeStr >> val1 >> val2 >> val3;

        const auto recordType{ recordTypeStr[0] };
#endif // USE_REGEX_PARSER

        switch (recordType)
        {
            case 'v':
            { // Zaznam typu vertex.
                vs.emplace_back(Vector{ val1, val2, val3 });
                
                break;
            }
            case 'f':
            { // Zaznam typu face.
                ts.emplace_back(Triangle{
                    static_cast<std::size_t>(val1 - 1), 
                    static_cast<std::size_t>(val2 - 1), 
                    static_cast<std::size_t>(val3 - 1)
                });
                
                break;
            }
            default:
            { /* Neznamy zaznam -> preskocit */ }
        }
    }

    vertices.swap(vs);
    faces.swap(ts);

    return true;
}

bool loadCube(std::vector<Vector> &vertices,
    std::vector<Triangle> &faces)
{
    vertices.clear();
    faces.clear();

    Vector offset{ 0.0f, 0.0f, 0.0f };
    vertices.emplace_back(Vector{ 0.5f, -0.5f, -0.5f } + offset);
    vertices.emplace_back(Vector{ 0.5f, -0.5f, 0.5f } + offset);
    vertices.emplace_back(Vector{ -0.5f, -0.5f, 0.5f } + offset);
    vertices.emplace_back(Vector{ -0.5f, -0.5f, -0.5f } + offset);

    vertices.emplace_back(Vector{ 0.5f, 0.5f, -0.5f } + offset);
    vertices.emplace_back(Vector{ 0.5f, 0.5f, 0.5f } + offset);
    vertices.emplace_back(Vector{ -0.5f, 0.5f, 0.5f } + offset);
    vertices.emplace_back(Vector{ -0.5f, 0.5f, -0.5f } + offset);

    // Spodni strana
    faces.emplace_back(Triangle{ 0u, 1u, 2u });
    faces.emplace_back(Triangle{ 0u, 2u, 3u });

    // Leva strana
    faces.emplace_back(Triangle{ 3u, 2u, 6u });
    faces.emplace_back(Triangle{ 3u, 6u, 7u });

    // Prava strana
    faces.emplace_back(Triangle{ 0u, 1u, 5u });
    faces.emplace_back(Triangle{ 0u, 5u, 4u });

    // Vrchni strana
    faces.emplace_back(Triangle{ 4u, 5u, 6u });
    faces.emplace_back(Triangle{ 4u, 6u, 7u });

    // Predni strana
    faces.emplace_back(Triangle{ 3u, 0u, 4u });
    faces.emplace_back(Triangle{ 3u, 4u, 7u });

    // Zadni strana
    faces.emplace_back(Triangle{ 2u, 1u, 5u });
    faces.emplace_back(Triangle{ 2u, 5u, 6u });

    return true;
}

/* Vykresleni polygonalniho model ve formatu OBJ */
void drawPolygons(const std::vector<Vector> &vertices, 
    const std::vector<Triangle> &faces, 
    S_RGBA lineColor, S_RGBA circleColor, 
    const Projector &projector)
{
    /*
     * Vykreslime okraje trojuhelniku pomoci rasterizace usecek
     * a trojuhelniky pomoci rasterizace kruznic.
     */

    for (const auto &triangle : faces)
    {
        const auto p1{ projectVector(vertices[triangle.idx1], projector) };
        const auto p2{ projectVector(vertices[triangle.idx2], projector) };
        const auto p3{ projectVector(vertices[triangle.idx3], projector) };

        rasterizeLine(static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y), lineColor);
        rasterizeLine(static_cast<int>(p2.x), static_cast<int>(p2.y), static_cast<int>(p3.x), static_cast<int>(p3.y), lineColor);
        rasterizeLine(static_cast<int>(p3.x), static_cast<int>(p3.y), static_cast<int>(p1.x), static_cast<int>(p1.y), lineColor);

        /*
        const auto center{ (p1 + p2 + p3) / 3.0f };
        const auto pal{ (p2 - p1).length() }; 
        const auto pbl{ (p3 - p2).length() }; 
        const auto pcl{ (p1 - p3).length() }; 
        const auto tal{ sqrtf(2.0f * pbl * pbl + 2.0f * pcl * pcl - pal * pal) };
        const auto tbl{ sqrtf(2.0f * pal * pal + 2.0f * pcl * pcl - pbl * pbl) };
        const auto tcl{ sqrtf(2.0f * pal * pal + 2.0f * pbl * pbl - pcl * pcl) };
        const auto radius{ std::min<float>(std::min<float>(tal, tbl), tcl) / 3.0f };

        drawCircle(static_cast<int>(center.x), static_cast<int>(center.y), radius, circleColor);
        */
    }
}

/* Vykresleni polygonalniho model ve formatu OBJ */
void drawPolygons(const std::vector<Vector>& vertices,
    const std::vector<Triangle>& faces,
    S_RGBA lineColor, S_RGBA circleColor,
    const Vector& scale, const Vector& rotate, const Vector& translate,
    const Camera& camera)
{
    /* Nastav projektor podle pozadavku */
    Projector p{ };
    p.camera = camera;
    p.scale = scale; p.rotate = rotate; p.translate = translate;

    return drawPolygons(vertices, faces, lineColor, circleColor, p);
}

/*****************************************************************************/
/*****************************************************************************/
