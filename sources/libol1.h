

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                      LIB OCTREE LOCALISATION V1.55                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Description:         Octree for mesh localization                       */
/*    Author:              Loic MARECHAL                                      */
/*    Creation date:       mar 16 2012                                        */
/*    Last modification:   dec 13 2018                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public defines : type of elements and 32/64 bits int                       */
/*----------------------------------------------------------------------------*/

enum TypTag {  LolTypVer, LolTypEdg, LolTypTri, LolTypQad,
               LolTypTet, LolTypPyr, LolTypPri, LolTypHex, LolNmbTyp };


/*----------------------------------------------------------------------------*/
/* Public procedures                                                          */
/*----------------------------------------------------------------------------*/

int64_t LolNewOctree       (int, double *, double *, int, int *, int *,
                            int, int *, int *, int, int *, int *,
                            int, int *, int *, int, int *, int *,
                            int, int *, int *, int, int *, int *);
size_t  LolFreeOctree      (int64_t);
int     LolGetBoundingBox  (int64_t , int , int, int *, double [3], double [3]);
int     LolGetNearest      (int64_t, int, double *, double *,
                            double, int (void *, int), void * );
int     LolIntersectSurface(int64_t, double *, double *, double *,
                            double, int (void *, int), void * );
int     LolProjectVertex   (int64_t, double *, int, int, double *);


/*----------------------------------------------------------------------------*/
/* Fortran 77 API                                                             */
/*----------------------------------------------------------------------------*/

#if defined(F77_NO_UNDER_SCORE)
#define call(x) x
#else
#define call(x) x ## _
#endif
