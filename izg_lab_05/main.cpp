/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG Lab 04
 * ihulik@fit.vutbr.cz
 *
 *
 *
 * *) Ovladani programu:
 *      "Leve tlacitko mysi"		- Zadavani controlnich bodu
 *      "Esc"						- ukonceni programu
 *
 * Opravy a modifikace:
 * isvoboda@fit.vutbr.cz	- Lab01 to Lab04
 * 							- Prepracovan system zpracovani udalosti
 * 							- Clear uzivatelskeho bufferu - frame_buffer
 */

/******************************************************************************
******************************************************************************
 * Includes
 */

#ifdef _WIN32
    #include <windows.h>
#endif

/* knihovna SDL + system */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <cstring>
#include <iostream>

/* nase veci... */
#include "color.h"
#include "student.h"
#include "globals.h"
#include "vector.h"
#include "main.h"

/******************************************************************************
 ******************************************************************************
 * Globalni konstanty a promenne
 */

/* titulek hlavniho okna */
const char     * PROGRAM_TITLE  = "IZG Lab 04 Curves";

/* defaultni velikost okna */
const int          DEFAULT_WIDTH    = 800;
const int          DEFAULT_HEIGHT   = 800;

/* kreslici buffer knihovny SDL */
SDL_Window         * screen         = NULL;
SDL_Renderer	   * renderer	    = NULL;
SDL_Surface		   * screen_surface = NULL;

/* kreslici buffer IZG cviceni */
S_RGBA				* frame_buffer	= NULL;

/* pomocna promenna pro ukonceni aplikace */
int                 quit            = 0;
int					width			= 800;
int					height			= 800;

/* aktivni bod */
Point2d				*active_point	= NULL;
const int			snipeDistance	= 10;

/* Kvalita generovane krivky - pocet segmentu - usecek, ze kterych je vykreslena */
int					quality			= 100;
int                 delay           = 1;	// ms

//Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2i_vecGetPtr((pVec), (i)))

/*Initialization has to be done in the scope of function (Limitation of C), see the main function.*/
S_Vector			* control_points_up	  = NULL;
S_Vector			* curve_points_up     = NULL;
S_Vector			* control_points_down = NULL;
S_Vector			* curve_points_down   = NULL;

/**
 * Funkce vlozi pixel na pozici bodu [x;y]. Je nutne hlidat frame_buffer, pokud
 * je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 * @param x Souradnice bodu x
 * @param y Souradnice bodu y
 * @param color Barva bodu
 */
void putPixel(int x, int y, const S_RGBA *color)
{
	if (x > 0 && y > 0 && x < width && y < height)
			frame_buffer[y * width + x] = *color;
}

void putPixelPoint2d(const Point2d *p, const S_RGBA *color)
{
	putPixel(ROUND(p->x), ROUND(p->y), color);
}

/**
 * Funkce getLine kresli usecku mezi dvema body [x1,y1] a [x2,y2]
 * pomoci Bressenhamova algoritmu.
 * @param x1 pocatecni souradnice x
 * @param y1 pocatecni souradnice y
 * @param x2 koncova souradnice x
 * @param y2 koncova souradnice y
 * @param out_points body usecky
 */
void getLine(int x1, int y1, int x2, int y2, S_Vector *out_points)
{
	bool steep = (abs( y2 - y1 ) > abs( x2 - x1 )) ? true : false;
	if( steep )
	{
		SWAP(x1, y1);
		SWAP(x2, y2);
	}
	if( x1 > x2 )
	{
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	const int dx = x2 - x1, dy = abs(y2 - y1);
	const int P1 = 2*dy, P2 = P1 - 2*dx;
	int P = 2*dy - dx;
	int y = y1;
	int ystep = 1;
	if ( y1 > y2 ) ystep = -1;

	for (int x = x1; x <= x2; x++ )
	{
		Point2d new_pt;
		if( steep ) {
			new_pt.x = y;
			new_pt.y = x;
		} else {
			new_pt.x = x;
			new_pt.y = y;
		}
		point2d_vecPushBack(out_points, new_pt);

		if( P >= 0 )
		{
			P += P2;
			y += ystep;
		} else
			P += P1;
	}


	return;
}

void getLinePoints2d (const Point2d *a, const Point2d *b, S_Vector *out_points)
{
	getLine(ROUND(a->x), ROUND(a->y), ROUND(b->x), ROUND(b->y), out_points);
}

void getLinePoints(const S_Vector *end_points, S_Vector *out_points)
{
	for(int i = 1; i < point2d_vecSize(end_points); ++i)
	{
		getLinePoints2d(point2d_vecGetPtr(end_points, i-1), point2d_vecGetPtr(end_points, i), out_points);
	}
}

void drawLines(const S_Vector *end_points, const S_RGBA *color) {
	S_Vector *lines_points = vecCreateEmpty(sizeof(Point2d));
	getLinePoints(end_points, lines_points);
	for(int i = 0; i < point2d_vecSize(lines_points); i++) {
		Point2d *pt = point2d_vecGetPtr(lines_points, i);
		putPixel(pt->x, pt->y, color);
	}
}

int pointSnap(const Point2d *p, int range, const S_Vector *points)
{
  int index = -1;
  float min_range = FLT_MAX;

  // nalezeni nejblizsiho bodu
  for (int i = 0; i < point2d_vecSize(points); ++i)
  {
    float dx = (float)(ROUND(p->x) - ROUND(point2d_vecGetPtr(points,i)->x));
    float dy = (float)(ROUND(p->y) - ROUND(point2d_vecGetPtr(points,i)->y));
//    Point *p = (Point *)(vecGetPtr(points,i));
    float temp = dx * dx + dy * dy;

    if (temp < min_range)
    {
	    min_range = temp;
	    index = i;
	  }
  }

  // lezi bod v pozadovanem okoli?
  if (min_range < range * range)
    return index;
  else
    return -1;
}

void drawCrossPoint(const Point2d *p, int size, const S_RGBA *color)
{
  putPixelPoint2d(p, color);
  for (int i = 0; i < size; ++i)
  {
    putPixel(ROUND(p->x) - i, ROUND(p->y), color);
    putPixel(ROUND(p->x) + i, ROUND(p->y), color);
    putPixel(ROUND(p->x), ROUND(p->y) + i, color);
    putPixel(ROUND(p->x), ROUND(p->y) - i, color);
  }
}

void drawCrossPoints(const S_Vector *points, int size, const S_RGBA *color)
{
  for (int i = 0; i < point2d_vecSize(points); ++i)
  {
    drawCrossPoint(point2d_vecGetPtr(points, i), size, color);
  }
}

void drawCurvesWithConstruction(const S_Vector *ctrl_points, const S_Vector *crv_points) {
	/* Spojnice mezi kontrolnimi body */
	drawLines(ctrl_points, &COLOR_GREY);
	/* Vykreslime vsechny kontrolni body */
	drawCrossPoints(ctrl_points, 5, &COLOR_GREEN);
	/* Kreslime samotnou krivku */
	drawLines(crv_points, &COLOR_WHITE);
}

/******************************************************************************
 ******************************************************************************
 * funkce zajistujici prekresleni obsahu okna programu
 */
void onDraw(void)
{
    /* Vynulovani framebufferu, do ktereho vykreslujeme */
    memset(frame_buffer, 0 , sizeof(S_RGBA)*width*height);

	drawCurvesWithConstruction(control_points_up, curve_points_up);
	drawCurvesWithConstruction(control_points_down, curve_points_down);

    /* Test existence frame bufferu a obrazove pameti */
	IZG_ASSERT(frame_buffer && screen);

    /* Test typu pixelu */
    IZG_ASSERT(screen_surface->format->BytesPerPixel == 4);
    
	/* Test, pokud kopirujeme rozdilnou velikost frame_bufferu a rozdilne pameti, musime pamet prealokovat */
	//if (width != screen->w || height != screen->h)
	//{

	SDL_LockSurface(screen_surface);
	MEMCOPY(screen_surface->pixels, frame_buffer, sizeof(S_RGBA) * width * height);
    SDL_UnlockSurface(screen_surface);

	//Update the surface
	SDL_UpdateWindowSurface(screen);
}

void sortByX(S_Vector *points) {
	for(int n = 0; n < points->size; n++) {
		for(int i = 0; i < points->size-1; i++) {
			Point2d pt1 = *point2d_vecGetPtr(points, i);
			Point2d pt2 = *point2d_vecGetPtr(points, i+1);
			if(pt1.x > pt2.x) {
				point2d_vecSet(points, i, pt2);
				point2d_vecSet(points, i+1, pt1);
			}
		}
	}
}

void runAnimations(void) {
	
	SDL_Surface *mario = SDL_LoadBMP("../mario.bmp");
    if (mario == NULL) {
    	printf("Failed to load image: %s\n", SDL_GetError());
		return;
	}

	for(int i = 0; i < point2d_vecSize(curve_points_up); i++) {
		SDL_LockSurface(screen_surface);
		MEMCOPY(screen_surface->pixels, frame_buffer, sizeof(S_RGBA) * width * height);
		SDL_UnlockSurface(screen_surface);

		Point2d *position_up = point2d_vecGetPtr(curve_points_up, i);
		Point2d *position_down = point2d_vecGetPtr(curve_points_down, i);
		SDL_Rect dstrect;

		dstrect.x = position_up->x - mario->w/2;
		dstrect.y = position_up->y - mario->h;
		SDL_BlitSurface(mario, 0, screen_surface, &dstrect);

		dstrect.x = position_down->x - mario->w/2;
		dstrect.y = position_down->y - mario->h;
		SDL_BlitSurface(mario, 0, screen_surface, &dstrect);

		// Vymena zobrazovaneho a zapisovaneho bufferu
		SDL_UpdateWindowSurface(screen);
		SDL_Delay(delay);
	}
}


void onKeyboard(SDL_KeyboardEvent *key) {
	IZG_ASSERT(frame_buffer);
    /* vetveni podle stisknute klavesy */
    switch( key->keysym.sym )
    {
        case SDLK_a:
        	runAnimations();
            printf("Running the animation.\n");
        	break;
        default:
            break;
    }
}

/******************************************************************************
 ******************************************************************************
 * funkce tiskne napovedu na stdout
 */
void printHelpText()
{
	IZG_INFO("Application loaded - IZG LAB 04 - Curves.\n"
			 "Press 'a' to start the animation.\n"
			 "========================================================================\n\n")
}

/******************************************************************************
 ******************************************************************************
 * hlavni funkce programu
 * argc - pocet vstupnich parametru
 * argv - pole vstupnich parametru
 */

int main(int argc, char *argv[])
{
	/* Inicializace SDL knihovny */
    if( SDL_Init(SDL_INIT_VIDEO) == -1 )
    {
        IZG_SDL_ERROR("Could not initialize SDL library");
    }

	/* Alokace frame bufferu (okno + SW zapisovaci */
	screen = SDL_CreateWindow("Lab 5", SDL_WINDOWPOS_UNDEFINED, 
											SDL_WINDOWPOS_UNDEFINED, 
											width, height, 
											SDL_WINDOW_ALLOW_HIGHDPI );
	if (!(screen))
    {
		std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        SDL_Quit();
        return 1;
    }	
    screen_surface = SDL_GetWindowSurface(screen);
	
	// We must call SDL_CreateRenderer in order for draw calls to affect this window
    renderer = SDL_CreateRenderer(screen, -1, 0 );

	if (!(frame_buffer = (S_RGBA *)malloc(sizeof(S_RGBA) * width * height)))
	{
		SDL_Quit();
        return 1;
	}

	initControlPointsUp(&control_points_up, 100, 300);
	initControlPointsDown(&control_points_down, 100, 600);
	curve_points_up = vecCreateEmpty(sizeof(Point2d));
	bezierCubicsTrajectory(quality, control_points_up, curve_points_up);
	sortByX(curve_points_up);
	curve_points_down = vecCreateEmpty(sizeof(Point2d));
	bezierCubicsTrajectory(quality, control_points_down, curve_points_down);
	sortByX(curve_points_down);

	printHelpText();

	SDL_Event event;
	while(!quit)
    {
		/*Udalost se vyridi, az prijde*/
		SDL_WaitEvent(&event);
		switch( event.type )
		{
	        case SDL_KEYDOWN:
				onKeyboard(&event.key);
				break;

			/* SDL_QUIT event */
			case SDL_QUIT:
				quit = 1;
				break;

			default:
				break;
		}
		/* Provedeme preklopeni zapisovaciho frame_bufferu na obrazovku*/
		onDraw();
    }

	/* Uvolneni pameti */
	point2d_vecRelease(&control_points_up);
	point2d_vecRelease(&control_points_down);
	point2d_vecRelease(&curve_points_up);
	point2d_vecRelease(&curve_points_down);

	// SDL_FreeSurface(screen);
	free(frame_buffer);
    SDL_Quit();

	IZG_INFO("Bye bye....\n\n");
    return 0;
}


/*****************************************************************************/
/*****************************************************************************/
