#include <GL/glut.h>
#include <GL/GLU.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

void MyInit() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(0.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lightpos[] = { 2.0, -2.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 100);
	glMatrixMode(GL_MODELVIEW);
}

GLfloat  Cx = 0, Cy = 3, Cz = -3;
GLfloat d = 0;

void Face(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[]) {
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glVertex3fv(D);
	glEnd();
}

void Cube(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[]) {
	glColor3f(1, 0, 1);
	Face(V0, V1, V2, V3);
	glColor3f(1, 1, 0);
	Face(V4, V5, V6, V7);
	glColor3f(0, 0, 1);
	Face(V0, V3, V7, V4);
	glColor3f(1, 0, 0);
	Face(V1, V2, V6, V5);

	//TOP
	glColor3f(0, 1, 1);
	Face(V0, V1, V5, V4);
	glColor3f(1, 1, 0);
	Face(V3, V2, V6, V7);

}

void Cube2(GLfloat T0[], GLfloat T1[], GLfloat T2[], GLfloat T3[], GLfloat T4[], GLfloat T5[], GLfloat T6[], GLfloat T7[]) {
	glColor3f(0, 0, 1);
	Face(T0, T1, T2, T3);
	glColor3f(0, 1, 0);
	Face(T4, T5, T6, T7);
	glColor3f(0, 1, 1);
	Face(T0, T3, T7, T4);
	glColor3f(1, 0, 0);
	Face(T1, T2, T6, T5);

	//TOP
	glColor3f(0, 1, 1);
	Face(T0, T1, T5, T4);
	glColor3f(1, 1, 0);
	Face(T3, T2, T6, T7);
}

void Cube3(GLfloat T0[], GLfloat T1[], GLfloat T2[], GLfloat T3[], GLfloat T4[], GLfloat T5[], GLfloat T6[], GLfloat T7[]) {
	glColor3f(0, 0, 1);
	Face(T0, T1, T2, T3);
	glColor3f(0, 1, 0);
	Face(T4, T5, T6, T7);
	glColor3f(0, 1, 1);
	Face(T0, T3, T7, T4);
	glColor3f(1, 0, 0);
	Face(T1, T2, T6, T5);

	//TOP
	glColor3f(0, 1, 1);
	Face(T0, T1, T5, T4);
	glColor3f(1, 1, 0);
	Face(T3, T2, T6, T7);
}

void Normalize3(GLdouble* v)
{
	GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void SphereFace(int p_recurse, double p_radius, GLdouble* a,
	GLdouble* b, GLdouble* c)
{
	if (p_recurse > 1)
	{
		// Compute vectors halfway between the passed vectors 
		GLdouble d[3] = { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
		GLdouble e[3] = { b[0] + c[0], b[1] + c[1], b[2] + c[2] };
		GLdouble f[3] = { c[0] + a[0], c[1] + a[1], c[2] + a[2] };

		Normalize3(d);
		Normalize3(e);
		Normalize3(f);

		SphereFace(p_recurse - 1, p_radius, a, d, f);
		SphereFace(p_recurse - 1, p_radius, d, b, e);
		SphereFace(p_recurse - 1, p_radius, f, e, c);
		SphereFace(p_recurse - 1, p_radius, f, d, e);
	}

	glBegin(GL_TRIANGLES);
	glNormal3dv(a);
	glVertex3d(a[0] * p_radius, a[1] * p_radius, a[2] * p_radius);
	glNormal3dv(b);
	glVertex3d(b[0] * p_radius, b[1] * p_radius, b[2] * p_radius);
	glNormal3dv(c);
	glVertex3d(c[0] * p_radius, c[1] * p_radius, c[2] * p_radius);
	glEnd();
}


void Sphere(double p_radius)
{
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	//GLfloat red[] = { .8f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	GLfloat shininess[] = { 100 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLdouble a[] = { 1, 0, 0 };
	GLdouble b[] = { 0, 0, -1 };
	GLdouble c[] = { -1, 0, 0 };
	GLdouble d[] = { 0, 0, 1 };
	GLdouble e[] = { 0, 1, 0 };
	GLdouble f[] = { 0, -1, 0 };

	int recurse = 4;

	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, d, a, e);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, a, b, e);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, b, c, e);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, c, d, e);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, a, d, f);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, b, a, f);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, c, b, f);
	glColor3f(1, 0, 0);
	SphereFace(recurse, p_radius, d, c, f);
}

void Draw() {

	GLfloat V[8][3] =
	{

		//Front faces
		{-0.25,0.25,0.25},{0.25,0.25,0.25},
		{0.25,-0.25,0.25},{-0.25,-0.25,0.25},
		//Back faces
		{-0.25,0.25,-0.25},{0.25,0.25,-0.25},
		{0.25,-0.25,-0.25},{-0.25,-0.25,-0.25}
	};

	GLfloat T[8][3] =
	{
		//Front faces
		{-0.25,0.25,0.25},{0.25,0.25,0.25},
		{0.25,-0.25,0.25},{-0.25,-0.25,0.25},
		//Back faces
		{-0.25,0.25,-0.25},{0.25,0.25,-0.25},
		{0.25,-0.25,-0.25},{-0.25,-0.25,-0.25}
	};

	GLfloat Q[8][3] =
	{

		//Front faces
		{-0.25,0.25,0.25},{0.25,0.25,0.25},
		{0.25,-0.25,0.25},{-0.25,-0.25,0.25},
		//Back faces
		{-0.25,0.25,-0.25},{0.25,0.25,-0.25},
		{0.25,-0.25,-0.25},{-0.25,-0.25,-0.25}
	};

	GLfloat rV[8][3], rT[8][3], rQ[8][3], r;
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	GLfloat blue[] = { 0.5f, 0.5f, 0.1f };
	glLightfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	GLfloat lightpos[] = { 2.0, -2.0, 0.0, 1.0 };

	//Light
	glPushMatrix();
	gluLookAt(Cx, Cy, Cz, 0, -8, 3, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(d, 0, 0, 5);
	//glRotatef(d, 1, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glPopMatrix();
	Sphere(0.15);
	glPopMatrix();
	glFlush();


	//Cube
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, 0, 0, 0, 0, 1, 0);

	r = d * 3.14 / 180;

	for (i = 0; i < 8; i++) {
		rV[i][2] = V[i][2] * cos(r) - V[i][0] * sin(r);
		rV[i][0] = V[i][2] * sin(r) + V[i][0] * cos(r);
		rV[i][1] = V[i][1];

	}


	Cube(rV[0], rV[1], rV[2], rV[3], rV[4], rV[5], rV[6], rV[7]);

	//Cube2
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, 1.2, 0, 0, 0, 1, 1);

	for (i = 0; i < 8; i++) {
		rT[i][2] = T[i][2] * cos(r) - T[i][0] * sin(r);
		rT[i][0] = T[i][2] * sin(r) + T[i][0] * cos(r);
		rT[i][1] = T[i][1];

	}

	Cube2(rT[0], rT[1], rT[2], rT[3], rT[4], rT[5], rT[6], rT[7]);

	//Cube3
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, -1.2, 0, 0, 0, 1, 1);

	for (i = 0; i < 8; i++) {
		rQ[i][2] = Q[i][2] * cos(r) - Q[i][0] * sin(r);
		rQ[i][0] = Q[i][2] * sin(r) + Q[i][0] * cos(r);
		rQ[i][1] = Q[i][1];

	}

	Cube3(rQ[0], rQ[1], rQ[2], rQ[3], rQ[4], rQ[5], rQ[6], rQ[7]);

	glutSwapBuffers();
}

void keyCallback(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT: Cx += 0.5;
		glutPostRedisplay(); break;
	case GLUT_KEY_LEFT: Cx -= 0.5;
		glutPostRedisplay(); break;
	}

	switch (key) {
	case GLUT_KEY_UP: Cy += 0.5;
		glutPostRedisplay(); break;
	case GLUT_KEY_DOWN: Cy -= 0.5;
		glutPostRedisplay(); break;
	}

	switch (key) {
	case GLUT_KEY_F1: Cz += 0.5;
		glutPostRedisplay(); break;
	case GLUT_KEY_F2: Cz -= 0.5;
		glutPostRedisplay(); break;
	}
}

void Spin() {

	d += 0.1;
	if (d > 360)
		d = 0;
	glutPostRedisplay();
}


int main(int c, char* V[]) {
	glutInit(&c, V);

	glutInitWindowPosition(250, 50);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("project3");
	MyInit();
	glutDisplayFunc(Draw);
	glutSpecialFunc(keyCallback);
	glutIdleFunc(Spin);
	glutMainLoop();
	return 0;
}