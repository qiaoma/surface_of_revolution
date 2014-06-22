/*
Name: Qiao Ma
CIN: 302599315
*/

#include <windows.h>
#include <gl/Gl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <vector>

#include "mesh.h"

using namespace std;

// constructor
Mesh:: Mesh() 
{
	numVerts=0; 
	ready2draw=0;
	numFaces=0; 
	norm=NULL; 
	face=NULL; 	
}

// destructor
Mesh:: 	~Mesh() 
{								
	if(numVerts !=0) {
		delete[] pt;
		delete[] norm;
		delete[] face;
	} 

	if (numberVertex != 0) {
		delete[] ptArray;
		delete[] normArray;
		delete[] faceArray;
	}
}

// use OpenGL to draw this mesh in solid object mode
void Mesh:: drawSolid() // use OpenGL to draw this mesh
{  
	if(ready2draw) 
	{
		for(int f = 0; f < numFaces; f++) // draw each face
		{
			//glColor3f(f / (float)10, f / (float)10, f / (float)10);
			glColor3f(f / (float)(numFaces + 10), f / (float)(numFaces + 10), f / (float)(numFaces + 10));
			glBegin(GL_POLYGON);
			  for(int v = 0; v < face[f].nVerts; v++) // for each one..
			  {
				  int in = face[f].vert[v].normIndex ; // index of this normal
				  int iv =  face[f].vert[v].vertIndex ; // index of this vertex
				  glNormal3f(norm[in].x, norm[in].y, norm[in].z);
				  glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
			  }
			glEnd();
		}
	}
}


// use OpenGL to draw this mesh in wireframe mode
void Mesh:: drawWireframe() 
{  
	if(ready2draw) {
		for(int f = 0; f < numFaces; f++) // draw each face
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_LINE_LOOP);
			  for(int v = 0; v < face[f].nVerts; v++) // for each one..
			  {
				  int iv =  face[f].vert[v].vertIndex ; // index of this vertex
				  glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
			  }
			glEnd();
		}
	}
}

void Mesh::initPrism(int N)
{
	numVerts = N * 2;
	numNormals = N + 2;
	numFaces = N + 2;

	pt = new Point3[numVerts];
	norm = new Vector3[numNormals];
	face = new Face[numFaces];
}


void Mesh::createPrism(int N, Point3 *p, float length)
{
	// initialize the prism
	initPrism(N);

	// create the vertex list
	for (int i = 0; i < N; i++){
		pt[i] = p[i];
	}
	for (int i = N; i < numVerts; i++){
		pt[i].x = p[i - N].x;
		pt[i].y = p[i - N].y;
		pt[i].z = p[i - N].z + length;

	}
	// create side faces

	for (int i = 0; i < N; i++){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + N;
		face[i].vert[2].vertIndex = (i + 1) % N + N;
		face[i].vert[3].vertIndex = (i + 1) % N;
	}

	// create the base and cap faces
	//base
	face[N].nVerts = N;
	face[N].vert = new VertexID[face[N].nVerts];
	for (int i = 0; i < N; i++){
		face[N].vert[i].vertIndex = i;
	}

	//cap
	face[N + 1].nVerts = N;
	face[N + 1].vert = new VertexID[face[N + 1].nVerts];
	for (int i = 0; i < N; i++){
		face[N + 1].vert[i].vertIndex = i + N;
	}
	// when all is done, ready2draw=1;

	ready2draw = 1;
}

void Mesh:: initMash(int N)
{
	numVerts = (N + 1) * 36;
	numNormals = N * 36;
	numFaces = N * 36;

	pt = new Point3[numVerts];
	norm = new Vector3[numNormals];
	face = new Face[numFaces];
} 


void Mesh:: createMash(int N, Point3 *p)
{	

	// initialize the mash
	initMash(N);

	// create the vertex list
	
	for (int i = 0; i < N; i++)
	{
		pt[i] = p[i];
	}
	for (int i = N; i < numVerts; i++)
	{
		pt[i].x = pt[i - N].x*cos(2 * PI * 10 / 360) + pt[i - N].z*sin(2 * PI * 10 / 360);
		pt[i].y = pt[i - N].y;
		pt[i].z = pt[i - N].x*(-sin(2 * PI * 10 / 360)) + pt[i - N].z*cos(2 * PI * 10 / 360);
	}
	
	// create side faces
	for (int i = 0; i < numFaces; i++){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		//CW
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + N;
		face[i].vert[2].vertIndex = (i + 1) % numVerts + N;
		face[i].vert[3].vertIndex = (i + 1) % numVerts;
		
		//CCW
		/*
		face[i].vert[0].vertIndex = (i + 1) % numVerts;
		face[i].vert[1].vertIndex = (i + 1) % numVerts + N;
		face[i].vert[2].vertIndex = i + N;
		face[i].vert[3].vertIndex = i;
		*/
	}

	// create the normal list
	for (int i = 0; i < numNormals; i++){

		for (int j = 0; j < 4; j++){
			face[i].vert[j].normIndex = i;
			
		}
		
		norm[i].x = (pt[i].y - pt[(i + N)].y) * (pt[(i)].z + pt[i + N].z) +
					(pt[i + N].y - pt[(i + 1) % numVerts + N].y ) * (pt[i + N].z + pt[(i + 1) % numVerts + N].z) +
					(pt[(i + 1) % numVerts + N].y - pt[(i + 1) % numVerts].y) * (pt[(i + 1) % numVerts + N].z + pt[(i + 1) % numVerts].z) +
					(pt[(i + 1) % numVerts].y - pt[i].y) * (pt[(i + 1) % numVerts].z + pt[i].z);

		norm[i].y = (pt[i].z - pt[(i + N)].z) * (pt[(i)].x + pt[i + N].x) +
					(pt[i + N].z - pt[(i + 1) % numVerts + N].z) * (pt[i + N].x + pt[(i + 1) % numVerts + N].x) +
					(pt[(i + 1) % numVerts + N].z - pt[(i + 1) % numVerts].z) * (pt[(i + 1) % numVerts + N].x + pt[(i + 1) % numVerts].x) +
					(pt[(i + 1) % numVerts].z - pt[i].z) * (pt[(i + 1) % numVerts].x + pt[i].x);

		norm[i].z = (pt[i].x - pt[(i + N)].x) * (pt[(i)].y + pt[i + N].y) +
					(pt[i + N].x - pt[(i + 1) % numVerts + N].x) * (pt[i + N].y + pt[(i + 1) % numVerts + N].y) +
					(pt[(i + 1) % numVerts + N].x - pt[(i + 1) % numVerts].x) * (pt[(i + 1) % numVerts + N].y + pt[(i + 1) % numVerts].y) +
					(pt[(i + 1) % numVerts].x - pt[i].x) * (pt[(i + 1) % numVerts].y + pt[i].y);

		std::cout << "norm " << i << ": " << norm[i].x << " " << norm[i].y << " " << norm[i].z << std::endl;
	}

	ready2draw = 1;
}

void Mesh::initArray(char* filename){
	ifstream readFile;
	string line;

	readFile.open(filename);
	if (readFile.is_open()){
		cout << "Loading data..." << endl;

		while (!readFile.eof()){			
			getline(readFile, line);
			if (line.find("v ") == 0){
				//vertVector.push_back(p3);
				numberVertex++;
			}
			else if (line.find("vn ") == 0){
				numberNormals++;
			}
			else if (line.find("f ") == 0){
				numberFaces++;
			}
		}
		readFile.close();
	}
	else{
		cout << "Unable to open file!" << endl;
	}

	ptArray = new Point3[numberVertex];
	faceArray = new Face[numberFaces];
	normArray = new Vector3[numberNormals];
}

void Mesh::readOBJ(char* filename){

	initArray(filename);

	ifstream readFile;
	string line, word;
	int ptIndex = 0, normIndex = 0, faceIndex = 0;

	readFile.open(filename);
	if (readFile.is_open()){
		while (!readFile.eof()){
			getline(readFile, line);
			
			if (line.find("v ") == 0){
				stringstream ss( line.substr(2) );
			
				float vert[3];
				int i = 0;
				while ( getline(ss, word, ' ') ){
					vert[i] = stof(word);
					i++;
				}
				ptArray[ptIndex].x = vert[0];
				ptArray[ptIndex].y = vert[1];
				ptArray[ptIndex].z = vert[2];
				ptIndex++;
			}
			else if (line.find("vn ") == 0){
				stringstream ss(line.substr(3));

				float vert[3];
				int i = 0;
				while (getline(ss, word, ' ')){
					vert[i] = stof(word);
					i++;
				}
				normArray[normIndex].x = vert[0];
				normArray[normIndex].y = vert[1];
				normArray[normIndex].z = vert[2];
				normIndex++;
			}
			else if (line.find("f ") == 0){
				stringstream ss(line.substr(2));
				stringstream ss2(line.substr(2));
				int numFaceVerts = 0;	
				int num = 0;

				while (getline(ss, word, ' ')){
					numFaceVerts++;
				}

				faceArray[faceIndex].nVerts = numFaceVerts;
				faceArray[faceIndex].vert = new VertexID[numFaceVerts];

				while (getline(ss2, word, ' ')){
					
					stringstream facess(word);
					string wordPart;
					int parts[3];
					int i = 0;
					while (getline(facess, wordPart, '/')){
						parts[i] = stoi(wordPart);
						i++;
					}
					faceArray[faceIndex].vert[num].vertIndex = parts[0];
					faceArray[faceIndex].vert[num].normIndex = parts[2];
					num++;
				}

				faceIndex++;
			}					
		}
		ready2draw = 1;
		cout << "Data loading finish." << endl;
		readFile.close();
	}
	else{
		cout << "Unable to open file!" << endl;
	}
}

void Mesh::saveMesh2obj(char* filename){
	ofstream writeFile;
	writeFile.open(filename);
	if (writeFile.is_open()){
		for (int i = 0; i < numVerts; i++){
			writeFile << "v " << pt[i].x << " " << pt[i].y << " " << pt[i].z << endl;
		}
		for (int i = 0; i < numNormals; i++){
			writeFile << "vn " << norm[i].x << " " << norm[i].y << " " << norm[i].z << endl;
		}
		for (int i = 0; i < numFaces; i++){
			writeFile << "f ";
			for (int j = 0; j < face[i].nVerts; j++){
				writeFile << face[i].vert[j].vertIndex << "//" << face[i].vert[j].normIndex << " ";
			}
			writeFile << endl;
		}
		
		cout << "File write complete." << endl;
		writeFile.close();
	}
	else{
		cout << "Unable to open file!" << endl;
	}	
}

void Mesh::drawSolidFromFile() // use OpenGL to draw this mesh
{
	for (int f = 0; f <numberFaces; f++) // draw each face
	{
		//glColor3f(f / (float)10, f / (float)10, f / (float)10);
		glColor3f(f / (float)(numberFaces + 1), f / (float)(numberFaces + 1), f / (float)(numberFaces + 1));
		glBegin(GL_POLYGON);
		for (int v = 0; v < faceArray[f].nVerts; v++) // for each one..
		{
			int in = faceArray[f].vert[v].normIndex; // index of this normal
			int iv = faceArray[f].vert[v].vertIndex; // index of this vertex
			glNormal3f(normArray[in].x, normArray[in].y, normArray[in].z);
			glVertex3f(ptArray[iv].x, ptArray[iv].y, ptArray[iv].z);
		}
		glEnd();
	}
}