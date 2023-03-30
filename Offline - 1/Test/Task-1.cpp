#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
//#define maxA 60
//#define maxR 60

double maxA = 60.00;
double maxR = 60.00;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

point pos, u, r, l;
double A = maxA, R = 0;


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

void drawPartialCylinder(double radius, double height, int segments)
{
    int i;
    struct point points[100];
    glColor3f(0,0,1);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments/4;i++)
    {
        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i].x,points[i].y,height);
            glVertex3f(points[i + 1].x,points[i + 1].y,height);
            glVertex3f(points[i + 1].x,points[i + 1].y,0);
        }
        glEnd();
    }

}

void drawPartialSphere(double radius,int slices,int stacks, int upper)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices/4;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1, 0, 0);
		for(j=0;j<slices/4;j++)
		{
			glBegin(GL_QUADS);{
			    
                if(upper)
                {  
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                }
                else
                {
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

                }
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
        case '7':
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointSub(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            t = l;
            l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointAdd(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '8':
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointSub(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointSub(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
        case '9':
            t = l;
            l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointAdd(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            t = l;
			l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointAdd(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
			break;
        case '0':
            t = l;
            l = pointSub(pointMultiply(l, cos(pi/60)), pointMultiply(u, sin(pi/60)));
            u = pointAdd(pointMultiply(u, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            t = l;
            l = pointAdd(pointMultiply(l, cos(pi/60)), pointMultiply(r, sin(pi/60)));
            r = pointSub(pointMultiply(r, cos(pi/60)), pointMultiply(t, sin(pi/60)));
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            ///pos = pointSub(pos, pointMultiply(l, 2));
            maxA /= 1.1;
            A /= 1.1;
            maxR /= 1.1;
            R /= 1.1;
			break;
		case GLUT_KEY_UP:		// up arrow key
            ///pos = pointAdd(pos, pointMultiply(l, 2));
            maxA *= 1.1;
            A *= 1.1;
            maxR *= 1.1;
            R *= 1.1;
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

void draw8Spheres()
{
    glPushMatrix();
    glTranslatef(A, A, A);
    glRotatef(0, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-A, A, A);
    glRotatef(90, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-A, -A, A);
    glRotatef(180, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(A, -A, A);
    glRotatef(270, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(A, A, -A);
    glRotatef(0, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-A, A, -A);
    glRotatef(90, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-A, -A, -A);
    glRotatef(180, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(A, -A, -A);
    glRotatef(270, 0, 0, 1);
    drawPartialSphere(R, 24, 20, 0);
    glPopMatrix();

}

void drawUpperPart()
{
    glPushMatrix();
    glTranslated(A, A, -A);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glRotated(-90, 0, 0, 1);
    glTranslated(A, A, -A);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glRotated(-180, 0, 0, 1);
    glTranslated(A, A, -A);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glRotated(-270, 0, 0, 1);
    glTranslated(A, A, -A);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();
}

void drawMiddlePart()
{
    glPushMatrix();
    glTranslated(A, A, A);
    glRotated(-90, 0, 1, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-A, A, A);
    glRotated(-90, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-A, -A, A);
    glRotated(-180, 0, 0, 1);
    glRotated(-90, 0, 1, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(A, -A, A);
    glRotated(-90, 0, 0, 1);
    glRotated(-90, 0, 1, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();
}

void drawLowerPart()
{
    glPushMatrix();
    glTranslated(A, A, -A);
    glRotated(90, 0, 1, 0);
    glRotated(-90, -1, 0, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-A, A, -A);
    glRotated(90, 0, 1, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-A, -A, -A);
    glRotated(90, 0, 1, 0);
    glRotated(90, -1, 0, 0);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-A, -A, -A);
    glRotated(90, 0, 1, 0);
    glRotated(-90, 0, 0, 1);
    drawPartialCylinder(R, 2*A, 20);
    glPopMatrix();
}

void draw4Cylinder()
{   
    glColor3f(1,0,0);
    drawUpperPart();
    drawMiddlePart();
    drawLowerPart();
}

void drawCube()
{
    glColor3f(0, 1, 0);


    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0, 0, maxA);
    drawSquare(A);
    glPopMatrix();

   
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0, 0, -maxA);
    drawSquare(A);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, maxA);
    drawSquare(A);
    glPopMatrix();

   
    glPushMatrix();
    glTranslated(0, 0, -maxA);
    drawSquare(A);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, maxA, 0);
    glRotated(90, 1, 0, 0);
    drawSquare(A);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -maxA, 0);
    glRotated(90, 1, 0, 0);
    drawSquare(A);
    glPopMatrix();
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

    ///glColor3f(1, 1, 1);
    ///drawSquare(30);

    ///drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	///drawSphere(30,24,20);

    draw8Spheres();
    draw4Cylinder();
    drawCube();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
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
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

    //initiate camera parameters
    u.x = 0, u.y = 0, u.z = 1;
    r.x = -(1/sqrt(2)), r.y = 1/sqrt(2), r.z = 0;
    l.x = -(1/sqrt(2)), l.y = -(1/sqrt(2)), l.z = 0;
    pos.x = 100, pos.y = 100, pos.z = 0;
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
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

	return 0;
}
