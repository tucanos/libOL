

/*----------------------------------------------------------*/
/*															*/
/*				LIB OCTREE LOCALISATION V1.1				*/
/*															*/
/*----------------------------------------------------------*/
/*															*/
/*	Description:		Distance entre maillages pour Lemma	*/
/*	Author:				Loic MARECHAL						*/
/*	Creation date:		apr 29 2015							*/
/*	Last modification:	apr 30 2015							*/
/*															*/
/*----------------------------------------------------------*/


/*----------------------------------------------------------*/
/* Includes													*/
/*----------------------------------------------------------*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libmesh6.h>
#include <libol1.h>


/*----------------------------------------------------------*/
/* Defines and macros										*/
/*----------------------------------------------------------*/

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define power(a) ((a)*(a))
#define cube(a) ((a)*(a)*(a))


/*----------------------------------------------------------*/
/* Structures												*/
/*----------------------------------------------------------*/

typedef struct
{
	int NmbVer, NmbTri, MshIdx, (*tri)[3];
	long long OctIdx;
	double (*crd)[3];
}MshSct;


/*----------------------------------------------------------*/
/* Prototypes of local procedures							*/
/*----------------------------------------------------------*/

MshSct *ScaMsh(char *);
double DisMshMsh(MshSct *, MshSct *);


/*----------------------------------------------------------*/
/* Compute the distance between meshes with the liboctree	*/
/*----------------------------------------------------------*/

int main()
{
	int i;
	MshSct *msh[2];
	char *FilNam[2] = {"volume.mesh", "peau.mesh" };

	/* Read the meshes and build the octrees */

	for(i=0;i<2;i++)
	{
		if(!(msh[i] = ScaMsh(FilNam[i])))
		{
			printf("Cannot read mesh %s\n", FilNam[i]);
			exit(1);
		}

		if(!(msh[i]->OctIdx = NewOctree(msh[i]->NmbVer, msh[i]->crd[1], msh[i]->crd[2],
										msh[i]->NmbTri, msh[i]->tri[1], msh[i]->tri[2])))
		{
			printf("Cannot build octree around mesh %s\n", FilNam[i]);
			exit(1);
		}
	}

	printf("Mean distance between meshes = %g\n", DisMshMsh(msh[0], msh[1]));

	/* Clean-up memories */ 

	for(i=0;i<2;i++)
	{
		FreeOctree(msh[i]->OctIdx);
		free(msh[i]->crd);
		free(msh[i]->tri);
		free(msh[i]);
	}

	return(0);
}


/*----------------------------------------------------------*/
/* Allocate and read a mesh structure						*/
/*----------------------------------------------------------*/

MshSct *ScaMsh(char *FilNam)
{
	int ver, dim, ref;
	MshSct *msh;

	if(!(msh = calloc(1, sizeof(MshSct))))
		return(NULL);

	if(!(msh->MshIdx = GmfOpenMesh(FilNam, GmfRead, &ver, &dim)))
		return(NULL);

	 if(dim != 3)
 		return(NULL);

	if(!(msh->NmbVer = GmfStatKwd(msh->MshIdx, GmfVertices)))
		return(NULL);

	if(!(msh->crd = malloc((msh->NmbVer+1) * 3 * sizeof(double))))
		return(NULL);

	if(!(msh->NmbTri = GmfStatKwd(msh->MshIdx, GmfTriangles)))
		return(NULL);

	if(!(msh->tri = malloc((msh->NmbTri+1) * 3 * sizeof(int))))
		return(NULL);

	GmfGotoKwd(msh->MshIdx, GmfVertices);
	GmfGetBlock(msh->MshIdx, GmfVertices, \
				GmfDouble, &msh->crd[1][0], &msh->crd[2][0], \
				GmfDouble, &msh->crd[1][1], &msh->crd[2][1], \
				GmfDouble, &msh->crd[1][2], &msh->crd[2][2], \
				GmfInt, &ref, &ref);

	GmfGotoKwd(msh->MshIdx, GmfTriangles);
	GmfGetBlock(msh->MshIdx, GmfTriangles, \
				GmfInt, &msh->tri[1][0], &msh->tri[2][0], \
				GmfInt, &msh->tri[1][1], &msh->tri[2][1], \
				GmfInt, &msh->tri[1][2], &msh->tri[2][2], \
				GmfInt, &ref, &ref);

	GmfCloseMesh(msh->MshIdx);

	return(msh);
}


/*------------------------------------------------------------------------------------*/
/* Compute the minimum distance between source's vertices and destination's triangles */
/*------------------------------------------------------------------------------------*/

double DisMshMsh(MshSct *src, MshSct *dst)
{
	int i, MinTri;
	double MinDis, AvgDis=0.;

	for(i=1;i<=src->NmbVer;i++)
	{
		MinTri = GetNearest(dst->OctIdx, TypTri, src->crd[i], &MinDis, 0.);
		AvgDis += MinDis;

		if(MinDis > 0.)
			printf("vertex %d, tri %d, dis %g\n", i, MinTri, MinDis);
	}

	return(AvgDis / src->NmbTri);
}
