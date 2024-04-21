/******************************************************************************
 * Laborator 04 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz -
 */

#ifndef Student_H
#define Student_H

/*****************************************************************************
 * Includes
 */
#include "color.h"
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

void initControlPointsUp(S_Vector **points, int offset_x, int offset_y);
void initControlPointsDown(S_Vector **points, int offset_x, int offset_y);

void bezierCubicsTrajectory(int quality, const S_Vector *control_points, S_Vector *trajectory_points);

#ifdef __cplusplus
}
#endif

#endif /* Student_H */

/*****************************************************************************/
/*****************************************************************************/
