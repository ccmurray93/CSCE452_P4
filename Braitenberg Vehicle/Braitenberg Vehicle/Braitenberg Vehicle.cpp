#include "stdafx.h"
#include "Point.h"
#include "Matrix.h"
#include "Vehicle.h"
#include "LightSource.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

static const int MOVE_VEHICLE_DELAY = 10;


vector<Vehicle> vehicles;
vector<LightSource> lightSources;


int kType = 0;

bool placingLights = true;
bool placingVehicles = false;
bool simStarted = false;

void drawVehicles() {
	for (int i = 0; i < vehicles.size(); ++i) {
		Point c = vehicles[i].center;
		int w = vehicles[i].width / 2;
		int h = vehicles[i].height / 2;
		double rotate_deg = (vehicles[i].rotation / (2 * PI)) * 360;
		glPushMatrix();
		glTranslated(c.x, c.y, 0);
		glRotated(rotate_deg, 0, 0, 1);
		glTranslated(-1 * c.x, -1 * c.y, 0);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2f(c.x - w, c.y - h);
			glVertex2f(c.x + w, c.y - h);
			glVertex2f(c.x + w, c.y + h);
			glVertex2f(c.x - w, c.y + h);
		glEnd();

		// Draw Right Wheel
		int wWidth = 8;
		int wHeight = 16;

		int ww = wWidth / 2;
		int wh = wHeight / 2;

		int xOffset = c.x - w - ww;
		int yOffset = c.y - h;

		glBegin(GL_QUADS);
			if (vehicles[i].kType == VEHICLE_K_STRAIGHT) glColor3f(0.0f, 1.0f, 1.0f);
			else glColor3f(1.0f, 0.0f, 1.0f);
			glVertex2f(xOffset - ww, yOffset - wh);
			glVertex2f(xOffset + ww, yOffset - wh);
			glVertex2f(xOffset + ww, yOffset + wh);
			glVertex2f(xOffset - ww, yOffset + wh);
		glEnd();

		// Draw Left Wheel
		xOffset = c.x + w + ww;
		yOffset = c.y - h;

		glBegin(GL_QUADS);
			if (vehicles[i].kType == VEHICLE_K_STRAIGHT) glColor3f(0.0f, 1.0f, 1.0f);
			else glColor3f(1.0f, 0.0f, 1.0f);
			glVertex2f(xOffset - ww, yOffset - wh);
			glVertex2f(xOffset + ww, yOffset - wh);
			glVertex2f(xOffset + ww, yOffset + wh);
			glVertex2f(xOffset - ww, yOffset + wh);
		glEnd();
		//glTranslated(c.x, c.y, 0);
		glPopMatrix();
	}
}

void drawLightSources() {
	for (int i = 0; i < lightSources.size(); ++i) {
		Point c = lightSources[i].center;
		int w = lightSources[i].width / 2;
		int h = lightSources[i].height / 2;
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex2f(c.x - w, c.y - h);
			glVertex2f(c.x + w, c.y - h);
			glVertex2f(c.x + w, c.y + h);
			glVertex2f(c.x - w, c.y + h);
		glEnd();
	}
}

void display(void)
{
	// should not be necessary but some GPUs aren't displaying anything until a clear call.
	glClear(GL_COLOR_BUFFER_BIT);

	drawVehicles();
	drawLightSources();

	glFlush();
}

void moveVehicles(int value) {
	for (int i = 0; i < vehicles.size(); ++i) {
		vehicles[i].update(lightSources);
	}
	glutPostRedisplay();
	glutTimerFunc(MOVE_VEHICLE_DELAY, moveVehicles, 0);
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- GLUT CALLBACK FUNCTIONS */
/*--------------------------------------------------------------------------*/

void mouseClick(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		//if (placingLights) {
			if (button == GLUT_LEFT_BUTTON) lightSources.push_back(LightSource(Point(x, y)));
		//}
		//else if (placingVehicles) {
			if (button == GLUT_RIGHT_BUTTON) {
				if (kType) vehicles.push_back(Vehicle(Point(x,y), VEHICLE_K_STRAIGHT));
				else vehicles.push_back(Vehicle(Point(x,y), VEHICLE_K_CROSSED));
			}
		//}
		//printf("#Lights = %d, #Vehicles = %d\n", lightSources.size(), vehicles.size());
		glutPostRedisplay();
	}
}

void mouseMove(int x, int y) {

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case ' ':
			//if (placingLights) {
			// 	placingLights = false;
			// 	placingVehicles = true;
			// }
			// else if (placingVehicles) {
				placingVehicles = false;
				if (!simStarted) {
					glutTimerFunc(MOVE_VEHICLE_DELAY, moveVehicles, 0);
					simStarted = true;
				}
			//}
			break;
		case 'a':
			kType = !kType;
			break;
	}
}





/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- OPENGL INITIALIZATION FUNCTIONS */
/*--------------------------------------------------------------------------*/
void init(void)
{
	gluOrtho2D(0, ImageW - 1, ImageH - 1, 0);

}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ImageW, ImageH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Braitenberg Vehicles");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	//glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
