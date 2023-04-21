/******************************************************************************
 * Vektor (dynamicke pole) libovolnych prvku
 * 
 * $Id: vector.c 44 2011-02-09 10:06:04Z spanel $
 */

#include "vector.h"

//#include <memory.h>


/*****************************************************************************/

S_Vector * vecCreateEmpty(int elemSize)
{
    S_Vector * vec = (S_Vector *)malloc(sizeof(S_Vector));
    IZG_CHECK(vec, "Cannot allocate enough memory");

    vecInit(vec, elemSize);
    return vec;
}

/*****************************************************************************/

S_Vector * vecCreate(int size, int elemSize)
{
    S_Vector * vec = (S_Vector *)malloc(sizeof(S_Vector));
    IZG_CHECK(vec, "Cannot allocate enough memory");

    vecInit(vec, elemSize);
    vecResize(vec, size);
    return vec;
}

/*****************************************************************************/

void vecRelease(S_Vector ** ppVec)
{
	if( ppVec && *ppVec )
    {
        vecClear(*ppVec);
        free(*ppVec);
        *ppVec = NULL;
    }
}

/*****************************************************************************/

void vecInit(S_Vector *pVec, int elemSize)
{
    IZG_ASSERT(pVec && elemSize > 0);

    pVec->size = pVec->reserved = 0;
    pVec->elemSize = elemSize;
    pVec->data = NULL;
}

/*****************************************************************************/

void vecResize(S_Vector *pVec, int size)
{
    IZG_ASSERT(pVec && size >= 0);

    if( size > pVec->reserved || size < (pVec->reserved >> 1) )
    {
        char *p = (char *)realloc(pVec->data, size * pVec->elemSize);
        IZG_CHECK(p, "Cannot allocate enough memory");
        pVec->data = p;
        pVec->reserved = size;
    }

   pVec->size = size;
}

/*****************************************************************************/

void vecClear(S_Vector *pVec)
{
    IZG_ASSERT(pVec);

    pVec->size = pVec->reserved = 0;
    pVec->elemSize = 1;
    free(pVec->data);
    pVec->data = NULL;
}

/*****************************************************************************/

void vecClean(S_Vector *pVec)
{
	IZG_ASSERT(pVec);
	if(pVec->data != NULL)
	{
		free(pVec->data);
		pVec->data = NULL;
		pVec->reserved = 0;
		pVec->size = 0;
	}
}

/*****************************************************************************/

int vecPushBack(S_Vector *pVec, void *pElem)
{
    static const int blockSize = 512;

    IZG_ASSERT(pVec && pElem);

    if( pVec->size >= pVec->reserved )
    {
        char *p = (char *)realloc(pVec->data, (pVec->size + blockSize) * pVec->elemSize);
        IZG_CHECK(p, "Cannot allocate enough memory");
        pVec->data = p;
        pVec->reserved = pVec->size + blockSize;
    }

    MEMCOPY(pVec->data + pVec->size * pVec->elemSize, pElem, pVec->elemSize);
    ++pVec->size;

    return (pVec->size - 1);
}

/*****************************************************************************/

S_Vector * vecDeepCopy(const S_Vector *pVec) {
  IZG_ASSERT(pVec);
  S_Vector *copy = vecCreate(pVec->size, pVec->elemSize);
  MEMCOPY(copy->data, pVec->data, pVec->size*pVec->elemSize);
  return copy;
}

/*****************************************************************************/
/*****************************************************************************/
