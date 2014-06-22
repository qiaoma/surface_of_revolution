/*
Name: Qiao Ma
CIN: 302599315
*/

#include "Point3.h"
#include "Vector3.h"

#define PI 3.1415

//################# VertexID ###################
class VertexID{
	public:
		int vertIndex; // index of this vert in the vertex list
		int normIndex; // index of this vertex's normal
};


//#################### Face ##################
class Face{
  public:
	int nVerts; // number of vertices in this face
	VertexID * vert; // the list of vertex and normal indices
	Face(){nVerts = 0; vert=NULL; } // constructor
	~Face(){delete[] vert; nVerts = 0;} // destructor
};


//###################### Mesh #######################
class Mesh{
   private:
	int numVerts;	// number of vertices in the mesh
	Point3* pt;	      // array of 3D vertices
	int numNormals;   // number of normal vectors for the mesh
	Vector3 *norm;    // array of normals 
	int numFaces; 	// number of faces in the mesh
	Face* face;	      // array of face data
	
	Point3 * ptArray;
	Face * faceArray;
	Vector3 * normArray;
	int numberVertex = 1, numberFaces = 1, numberNormals = 1;

public:
	int ready2draw;	
	Mesh() ;	
	~Mesh() ; 
	
	void initPrism(int noVerts);
	void createPrism(int noVerts, Point3 *p, float length);
	void initMash(int noVerts);
	void createMash(int noVerts, Point3 *p);
	void drawWireframe();
	void drawSolid();
	void Mesh::initArray(char* filename);
	void Mesh::readOBJ(char* filename);
	void Mesh::saveMesh2obj(char* filename);
	void Mesh::drawSolidFromFile();
};	
