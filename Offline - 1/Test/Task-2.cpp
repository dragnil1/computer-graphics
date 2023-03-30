#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double angle1;
double angle2;
double translate_x;
double translate_y;
double unit_x;
double unit_y;
double R;
double H;
double A;
double thres_x;

struct point
{
	double x,y,z;
};


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
			for(i=-100;i<=100;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				double temp = 0;
				if(i*10 > thres_x)
				{
					temp = (i*10 - thres_x)*sin(pi/4);
				}

				//lines parallel to Y-axis
				glVertex3f(i*10, -1000, temp);
				glVertex3f(i*10,  1000, temp);



				//lines parallel to X-axis
				glVertex3f(-1000, i*10, 0);
				glVertex3f( thres_x, i*10, 0);

				glVertex3f(thres_x, i*10, 0);
				glVertex3f(1000, i*10, (1000 - thres_x)*sin(pi/4));


			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
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

void drawWheel(double radius, double height)
{
    int i;
    int segments = 64;
    struct point points[100];
    glColor3f(0,0,1);
    //generate points
    for(i=0;i<segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw cylinder of the wheel
    for(i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i].x,points[i].y,height);
            glVertex3f(points[(i + 1)%segments].x,points[(i + 1)%segments].y,height);
            glVertex3f(points[(i + 1)%segments].x,points[(i + 1)%segments].y,0);
        }
        glEnd();
    }
    //draw aixles of the wheel
    glBegin(GL_QUADS);
    glVertex3f(points[0].x,points[0].y,0);
    glVertex3f(points[32].x,points[32].y,0);
    glVertex3f(points[32].x,points[32].y,height);
    glVertex3f(points[0].x,points[0].y,height);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(points[16].x,points[16].y,0);
    glVertex3f(points[48].x,points[48].y,0);
    glVertex3f(points[48].x,points[48].y,height);
    glVertex3f(points[16].x,points[16].y,height);
    glEnd();
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

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'w':
            angle1 += pi/12.00;
            translate_x += (30.00*pi)/12.00*unit_x;
            translate_y += (30.00*pi)/12.00*unit_y;
            break;
        case 's':
            angle1 -= pi/12.00;
            translate_x -= (30.00*pi)/12.00*unit_x;
            translate_y -= (30.00*pi)/12.00*unit_y;
            break;
        case 'a':
            angle2 += pi/12.00;
            unit_x = cos(angle2);
			unit_y = sin(angle2);
            break;
        case 'd':
            angle2 -= pi/12.00;
			unit_x = cos(angle2);
			unit_y = sin(angle2);
            break;
		case 'c':
			R /= 1.1;
			break;
		case 'x':
			R *= 1.1;
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
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
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);

    glPushMatrix();
	if(translate_x + A/2 > thres_x)
    	glTranslatef(translate_x + A/2,translate_y + A/2,R + (translate_x + A/2 - thres_x)*sin(pi/4));
	else
		glTranslatef(translate_x + A/2,translate_y + A/2,R);
    glRotatef(angle2*(180/pi), 0, 0, 1);
    glRotatef(angle1*(180/pi), 0, 1, 0);
    ///glTranslatef(0,5,0);
    glRotatef(90, 1, 0, 0);
    drawWheel(R, H);
    glPopMatrix();

	glPushMatrix();
    glTranslatef(translate_x + A/2,translate_y - A/2,R);
    glRotatef(angle2*(180/pi), 0, 0, 1);
    glRotatef(angle1*(180/pi), 0, 1, 0);
    ///glTranslatef(0,5,0);
    glRotatef(90, 1, 0, 0);
    drawWheel(R, H);
    glPopMatrix();

	glPushMatrix();
    glTranslatef(translate_x - A/2,translate_y + A/2,R);
    glRotatef(angle2*(180/pi), 0, 0, 1);
    glRotatef(angle1*(180/pi), 0, 1, 0);
    ///glTranslatef(0,5,0);
    glRotatef(90, 1, 0, 0);
    drawWheel(R, H);
    glPopMatrix();

	glPushMatrix();
    glTranslatef(translate_x - A/2,translate_y - A/2,R);
    glRotatef(angle2*(180/pi), 0, 0, 1);
    glRotatef(angle1*(180/pi), 0, 1, 0);
    ///glTranslatef(0,5,0);
    glRotatef(90, 1, 0, 0);
    drawWheel(R, H);
    glPopMatrix();

	glPushMatrix();
	glTranslatef(translate_x, translate_y, 2*R);
	//drawSquare(A);
	glPopMatrix();

	glPushMatrix();

	glColor3f(0,1,1);
	glTranslatef(translate_x,translate_y,0);
    glRotatef(angle2*(180/pi), 0, 0, 1);
	glBegin(GL_LINES);
        {
			glVertex3f(0,0,2.5*R);
			glVertex3f(A,0,2.5*R);
        }
    glEnd();
	glPopMatrix();

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
    angle1=0;
    angle2=0;
    translate_x=0;
    translate_y=5;
    unit_x = 1;
    unit_y = 0;
	R = 30;
	H = 10;
	A = 80;
	thres_x = 100;

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
