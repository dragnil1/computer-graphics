#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<vector>
#include<iostream>


#include <windows.h>
#include <GL/glut.h>
#include "1705096_classes.h"
#include "bitmap_image.hpp"

using namespace std;

#define maxA 30
#define maxR 30
#define pi (2 * acos(0.0))
const double MAXN = 1e9;
const double viewAngle = 100.0;
const double aspectRatio = 1.0;
const double nearPlane = 1.0;
const double farPlane = 1000.0;
double windowHeight = 500;
double windowWidth = 500;
double imageHeight = 500;
double imageWidth = 500;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int level_of_recursion;
int pixels_along_both_dimensions;
int number_of_objects;
int number_of_point_lights;
int number_of_spot_lights;

vector<Object*> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

//bitmap_image image;

int imageSub = 11;


point pos, u, r, l;
double A = 30, R = 0;

double degreeToRadians(double degree)
{
	return degree * acos(-1) / 180.0;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

point pointMultiply(point p, double mul)
{
    p.x *= mul;
    p.y *= mul;
    p.z *= mul;
    return p;
}

point pointSub(point p, point q)
{
    p.x -= q.x;
    p.y -= q.y;
    p.z -= q.z;
    return p;
}

point pointAdd(point p, point q)
{
    p.x += q.x;
    p.y += q.y;
    p.z += q.z;
    return p;
}

void capture();

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        point t;
		case '1':
            t = l;
			l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointAdd(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
			break;
        case '2':
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointSub(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '3':
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointSub(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '4':
            t = l;
            l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointAdd(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '5':
            t = u;
            u = pointAdd(pointMultiply(u, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointSub(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '6':
            t = u;
            u = pointSub(pointMultiply(u, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointAdd(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
		case '0':
			capture();
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            pos = pointSub(pos, pointMultiply(l, 2));
			break;
		case GLUT_KEY_UP:		// up arrow key
            pos = pointAdd(pos, pointMultiply(l, 2));
			break;

		case GLUT_KEY_RIGHT:
			//pos.x += 2*r.x;
            //pos.y += 2*r.y;
            //pos.z += 2*r.z;
            pos = pointAdd(pos, pointMultiply(r, 2));
			break;
		case GLUT_KEY_LEFT:
			//pos.x -= 2*r.x;
            //pos.y -= 2*r.y;
            //pos.z -= 2*r.z;
            pos = pointSub(pos, pointMultiply(r, 2));
			break;

		case GLUT_KEY_PAGE_UP:
            //pos.x += 2*u.x;
            //pos.y += 2*u.y;
            //pos.z += 2*u.z;
            pos = pointAdd(pos, pointMultiply(u, 2));
			break;
		case GLUT_KEY_PAGE_DOWN:
            //pos.x -= 2*u.x;
            //pos.y -= 2*u.y;
            //pos.z -= 2*u.z;
            pos = pointSub(pos, pointMultiply(u, 2));
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
            if(R + 1 <= maxR)
            {
                R++;
            }
            if(A - 1 >= 0)
            {
                A--;
            }
			break;
		case GLUT_KEY_END:
            if(R - 1 >= 0)
            {
                R--;
            }
            if(A + 1 <= maxA)
            {
                A++;
            }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(pos.x, pos.y, pos.z,  pos.x + l.x, pos.y + l.y, pos.z + l.z,   u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    
    
	//Sphere sphere;
	//sphere.setColor(1, 0, 0);
	//sphere.setRadius(30);
	//sphere.setSlices(24);
	//sphere.setStacks(30);
	//sphere.draw();
	//Triangle triangle;
	//triangle.setColor(1, 0, 0);
	//triangle.setPoint1(0, 0, 0);
	//triangle.setPoint2(0, 30, 0);
	//triangle.setPoint3(30, 0, 0);
	//triangle.draw();
	//glColor3f(1, 1, 1);
	//drawSquare(30);
	//Floor floor(1000, 20);
	//floor.setColor(Color(0, 0, 0));
	//floor.draw();

	//cout << objects.size() << endl;

	for(auto o : objects)
	{
		o->draw();
	}
	for(auto pl : pointLights)
	{
		pl.draw();
	}
	for(auto sl : spotLights)
	{
		sl.draw();
	}

    ///drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	///drawSphere(30,24,20);

    //draw8Spheres();
    //draw4Cylinder();
    //drawCube();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}


void loadData()
{
	ifstream indata;
	indata.open("scenetest.txt");
	if (!indata)
    {
        cerr << "Error! file could not be opened" << endl;
        exit(1);
    }
	indata >> level_of_recursion;
	indata >> pixels_along_both_dimensions;
	imageHeight = imageWidth = pixels_along_both_dimensions;
	Object *temp = new Floor(1000, 20);
	temp->setColor(Color(0, 0, 0));
	temp->setCoEfficients(.6, .6, .6, .6);
	temp->setShine(10);
	objects.push_back(temp);
	indata >> number_of_objects;
	while(number_of_objects--)
	{
		string obj;
		indata >> obj;
		if(obj == "sphere")
		{
			double cx, cy, cz;
			indata >> cx >> cy >> cz;
			double radius;
			indata >> radius;
			Object *temp = new Sphere(Vector3D(cx, cy, cz), radius);
			double r, g, b;
			indata >> r >> g >> b;
			temp->setColor(Color(r, g, b));
			double coef_ambient, coef_diffuse, coef_specular, coef_reflection;
			indata >> coef_ambient >> coef_diffuse >> coef_specular >> coef_reflection;
			temp->setCoEfficients(coef_ambient, coef_diffuse, coef_specular, coef_reflection);
			double shininess;
			indata >> shininess;
			temp->setShine(shininess);
			objects.push_back(temp);
		}
		else if(obj == "triangle")
		{
			double x1, y1, z1;
			indata >> x1 >> y1 >> z1;
			double x2, y2, z2;
			indata >> x2 >> y2 >> z2;
			double x3, y3, z3;
			indata >> x3 >> y3 >> z3;
			Object *temp = new Triangle(Vector3D(x1, y1, z1), Vector3D(x2, y2, z2), Vector3D(x3, y3, z3));
			double r, g, b;
			indata >> r >> g >> b;
			temp->setColor(r, g, b);
			double coef_ambient, coef_diffuse, coef_specular, coef_reflection;
			indata >> coef_ambient >> coef_diffuse >> coef_specular >> coef_reflection;
			temp->setCoEfficients(coef_ambient, coef_diffuse, coef_specular, coef_reflection);
			double shininess;
			indata >> shininess;
			temp->setShine(shininess);
			objects.push_back(temp);
		}
		else if(obj == "general")
		{
			double A, B, C, D, E, F, G, H, I, J;
			indata >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
			Object *temp = new GeneralQuadraticEquation(A, B, C, D, E, F, G, H, I, J);
			double rx, ry, rz;
			double length, width, height;
			indata >> rx >> ry >> rz >> length >> width >> height;
			temp->setReferencePoint(Vector3D(rx, ry, rz));
			temp->setLength(length);
			temp->setWidth(width);
			temp->setHeight(height);
			double r, g, b;
			indata >> r >> g >> b;
			temp->setColor(r, g, b);
			double coef_ambient, coef_diffuse, coef_specular, coef_reflection;
			indata >> coef_ambient >> coef_diffuse >> coef_specular >> coef_reflection;
			temp->setCoEfficients(coef_ambient, coef_diffuse, coef_specular, coef_reflection);
			double shininess;
			indata >> shininess;
			temp->setShine(shininess);
			objects.push_back(temp);
		}
	}
	indata >> number_of_point_lights;
	while(number_of_point_lights--)
	{
		double px, py, pz;
		indata >> px >> py >> pz;
		double r, g, b;
		indata >> r >> g >> b;
		PointLight temp = PointLight(Vector3D(px, py, pz), r, g, b);
		pointLights.push_back(temp);
	}
	indata >> number_of_spot_lights;
	while(number_of_spot_lights--)
	{
		double px, py, pz;
		indata >> px >> py >> pz;
		double r, g, b;
		indata >> r >> g >> b;
		double dx, dy, dz;
		indata >> dx >> dy >> dz;
		double cutoff;
		indata >> cutoff;
		SpotLight temp = SpotLight(Vector3D(px, py, pz), r, g, b, Vector3D(dx, dy, dz), cutoff);
		spotLights.push_back(temp);
	}
	indata.close();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(viewAngle,	aspectRatio,	nearPlane,	farPlane);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

    //initiate camera parameters
    u.x = 0, u.y = 0, u.z = 1;
    r.x = -(1/sqrt(2)), r.y = 1/sqrt(2), r.z = 0;
    l.x = -(1/sqrt(2)), l.y = -(1/sqrt(2)), l.z = 0;
    pos.x = 100, pos.y = 100, pos.z = 100;
	loadData();
}

void capture()
{
	//image.setwidth_height(imageWidth, imageHeight, true);
	bitmap_image image(imageWidth, imageHeight);
	for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }
	double planeDistance = (windowWidth/2.0)/tan(degreeToRadians(viewAngle/2.0));
	Vector3D topLeft = Vector3D(pos) + Vector3D(l)*planeDistance - Vector3D(r)*(windowWidth/2.0) + 
						Vector3D(u)*(windowHeight/2.0);
	double du = (double)windowWidth/imageWidth;
	double dv = (double)windowHeight/imageHeight;
	topLeft = topLeft + Vector3D(r)*(du/2.0) - Vector3D(u)*(dv/2.0);
	Vector3D curPixel = topLeft;
	int nearest;
	double t, tMin;
	for(int i = 0; i < imageWidth; i++)
	{
		for(int j = 0; j < imageHeight; j++)
		{
			curPixel = topLeft + Vector3D(r)*(i*du) - Vector3D(u)*(j*dv);
			Ray ray(Vector3D(pos), (curPixel - Vector3D(pos)).normalize());
			Color dummyColor = Color(0, 0, 0);
			Color color = Color(0, 0, 0);
			Object* o_n;
			t = INF;
			bool updated = false;
			for(auto o : objects)
			{
				double tTemp = o->intersect(ray, dummyColor, 0);
				if(tTemp > 0 && tTemp < t)
				{
					t = tTemp;
					o_n = o;
					updated = true;
				}
			}
			if(updated)
			{
				tMin = o_n->intersect(ray, color, 1);
				image.set_pixel(i, j, (int) round(color.r*255.0), (int) round(color.g*255.0), (int) round(color.b*255.0));
			}
		}
	}
	image.save_image("Output_" + to_string(imageSub) + ".bmp");
	cout << "Image Saved" << endl;
	imageSub++;
}

void unLoadData()
{
	for(auto o : objects)
	{
		delete o;
	}
	objects.clear();
	pointLights.clear();
	spotLights.clear();
}

int main(int argc, char **argv){
	//loadData();
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(250, 250);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);
	glutMainLoop();		//The main loop of OpenGL
	unLoadData();
	return 0;
}
