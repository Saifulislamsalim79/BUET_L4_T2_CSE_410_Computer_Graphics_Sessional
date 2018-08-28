#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

//#include <GL/glut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>


#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double zoom;

struct point
{
	double x,y,z;
};

struct point p1, p2, dirVector;
double angleV, angleT;
double lenDorVector;
//double zoom;

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
		    if(zoom < 10)
            {
                zoom += 1;
            }
			break;
		case GLUT_KEY_DOWN:		// up arrow key

		    if(zoom >-10)
            {
                zoom -= 1;
            }
			break;

		case GLUT_KEY_RIGHT:
		    double tempX, tempY;
			angleV = 3;
			angleT -= 4;

			tempX = dirVector.x * cos(-angleV*pi/180) - dirVector.y * sin(-angleV*pi/180);
            tempY = dirVector.x * sin(-angleV*pi/180) + dirVector.y * cos(-angleV*pi/180);

            dirVector.x = tempX;
            dirVector.y = tempY;
			break;

		case GLUT_KEY_LEFT:
			angleV = 3;
			angleT += 4;

			tempX = dirVector.x * cos(angleV*pi/180) - dirVector.y * sin(angleV*pi/180);
            tempY = dirVector.x * sin(angleV*pi/180) + dirVector.y * cos(angleV*pi/180);

            dirVector.x = tempX;
            dirVector.y = tempY;
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

void drawBoundary(double border)
{
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    {
        glVertex3f(border, border, 0);
        glVertex3f(-border, border, 0);


        glVertex3f(-border, border, 0);
        glVertex3f(-border, -border, 0);

        glVertex3f(-border, -border, 0);
        glVertex3f(border, -border, 0);


        glVertex3f(border, -border, 0);
        glVertex3f(border, border, 0);
    }
    glEnd();
}


void drawLine(double x, double y, double z, struct point dirVector, int zoom)
{
    glBegin(GL_LINES);
        {
            glVertex3f(x, y, z);
            glVertex3f(x + 20*dirVector.x + zoom, y + 20*dirVector.y + zoom, z + 20*dirVector.z + zoom);
        }
    glEnd();
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
    for(i=0;i<=segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


/*void assignment2()
{

    //drawCircle(100,100);
    double border = 100;
    double heightTriangle = 6;
    double baseTriangle = 10;
    struct point t1, t2, t3;

    //Reflection Check
    if(p1.x >= border || p1.x <= -border)
    {
        dirVector.x = -dirVector.x;
        angleT = -angleT;
    }

    if(p1.y >= border || p1.y <= -border)
    {
        dirVector.y = -dirVector.y;
        angleT += 180;
    }

    drawBoundary(border);

    drawLine(p1.x, p1.y, p1.z, dirVector);


    t1.x = p1.x + 20*dirVector.x - 5*dirVector.y;
    t1.y = p1.y + 20*dirVector.y + 5*dirVector.x;

    t2.x = p1.x + 20*dirVector.x + 5*dirVector.y;
    t2.y = p1.y + 20*dirVector.y - 5*dirVector.x;

    t3.x = p1.x + 20*dirVector.x + 5*dirVector.x;
    t3.y = p1.y + 20*dirVector.y + 5*dirVector.y;

    t1.x *= zoom;
    t1.y *= zoom;
    t2.x *= zoom;
    t2.y *= zoom;
    t3.x *= zoom;
    t3.y *= zoom;

    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(t1.x, t1.y, 0);
        glVertex3f(t2.x, t2.y, 0);
        glVertex3f(t3.x, t3.y, 0);
    }
    glEnd();
}*/



void online()
{
    //drawCircle(100.0, 100);

    double border = 100;
    double heightTriangle = 6;
    double baseTriangle = 10;
    struct point t1, t2, t3, tmp, n;


    double l = sqrt(p1.x*p1.x + p1.y*p1.y);
    n = {-p1.x/l, -p1.y/l, p1.z/l};

    //Reflection Check
    double d = sqrt(p1.x*p1.x + p1.y*p1.y);
    if(d >= border)
    {
        tmp.x = dirVector.x - 2*(dirVector.x*n.x + dirVector.y*n.y)*n.x;
        tmp.y = dirVector.y - 2*(dirVector.x*n.x + dirVector.y*n.y)*n.y;

        //tmp.x = dirVector.x - 2*(dirVector.x*);

        //angleT = -angleT;

        dirVector.x = tmp.x;
        dirVector.y = tmp.y;
    }


    drawCircle(border,border);

    drawLine(p1.x, p1.y, p1.z, dirVector, zoom);


    t1.x = p1.x + 20*dirVector.x - 5*dirVector.y + zoom;
    t1.y = p1.y + 20*dirVector.y + 5*dirVector.x + zoom;

    t2.x = p1.x + 20*dirVector.x + 5*dirVector.y + zoom;
    t2.y = p1.y + 20*dirVector.y - 5*dirVector.x + zoom;

    t3.x = p1.x + 20*dirVector.x + 5*dirVector.x + zoom;
    t3.y = p1.y + 20*dirVector.y + 5*dirVector.y + zoom;

    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(t1.x, t1.y, 0);
        glVertex3f(t2.x, t2.y, 0);
        glVertex3f(t3.x, t3.y, 0);
    }
    glEnd();
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
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    //assignment2();

    online();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	p1.x = p1.x + .04*dirVector.x;
	p1.y = p1.y + .04*dirVector.y;

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	p1 = {0,0,0};
	dirVector = {1/sqrt(2), 1/sqrt(2),0};

	angleV = 0;
	angleT = 0;
	zoom = 0;


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
	glutInitWindowPosition(0, 0);
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
