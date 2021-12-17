#include <GL/glut.h>
#include<iostream>
#include <math.h>
#include <gl/glaux.h>
#pragma comment(lib, "C:/GL/Lib/GLAUX.LIB")
#pragma comment(lib, "legacy_stdio_definitions.lib")


#define PI 3.141592653
using namespace std;

// ширина и высота окна
int width = 900, height = 900;

// смещение между объектами
double shift_x = 0.3;
double shift_y = 0.0;

// режим управления положением света
int cur_light = 1;

// поворот камеры
float rotate_x = -25;
float rotate_y = 35;

// поворот первого источника света
float rotate_x1 = 0.2;
float rotate_y1 = 0.7;
float rotate_z1 = -1.0;

// поворот второго источника света
float rotate_x2 = 0.0;
float rotate_y2 = 0.2;
float rotate_z2 = -1.0;

// поворот третьего источника света
float rotate_x3 = 1.0;
float rotate_y3 = 0.7;
float rotate_z3 = 0.3;

// текстуры
GLuint TextureID[2];

// NURB
GLfloat ctrlpoints[4][4][3];
GLUnurbsObj* theNurb;

// Туман
bool enable_fog = false;



void myinit() {
    glClearColor(0.78, 0.40, 0.99, 0.0);

    // освещение
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    float ambient[4] = { 0.5, 0.5, 0.5, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // прозрачность
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // NURBS

    for (int u = 0; u < 4; u++) {
        for (int v = 0; v < 4; v++) {
            ctrlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5);
            ctrlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5);
            if ((u == 1 || u == 2) && (v == 1 || v == 2))
                ctrlpoints[u][v][2] = 3.0;
            else
                ctrlpoints[u][v][2] = -3.0;
        }
    }


    glEnable(GL_MAP2_VERTEX_3);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints[0][0][0]);

    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_AUTO_NORMAL);

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 2.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_LINE);




    AUX_RGBImageRec* bricks, * fabric;
    LPCWSTR path1 = L"C:/Users/Максим/Documents/ГУАААААП/Компьютерная графика/lab_5/bricks.bmp";
    LPCWSTR path2 = L"C:/Users/Максим/Documents/ГУАААААП/Компьютерная графика/lab_5/fabric.bmp";

    bricks = auxDIBImageLoad(path1);
    fabric = auxDIBImageLoad(path2);


    // получаем идентификаторы текстур
    glGenTextures(2, TextureID);

    // выбираем текстуру для модификации параметров
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);

    // загружаем текстуру 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bricks->sizeX,
        bricks->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
        bricks->data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (float)GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (float)GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (float)GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (float)GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // аналогично для второй текстуры

    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, fabric->sizeX,
        fabric->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
        fabric->data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (float)GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (float)GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (float)GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (float)GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // удаляем исходное изображение
    free(bricks);
    free(fabric);



    glEnable(GL_DEPTH_TEST);

}

void cube() {

    glNewList(1, GL_COMPILE);
    // BACK

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2d(0, 1);             glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2d(1, 1);             glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2d(1, 0);             glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2d(0, 0);             glVertex3f(1.0, -1.0, 1.0);

    glEnd();

    // FRONT
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2d(1, 0);             glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2d(0, 0);             glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2d(0, 1);             glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2d(1, 1);             glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2d(1, 1);             glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2d(0, 1);             glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2d(0, 0);             glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2d(1, 0);             glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2d(0, 1);             glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2d(1, 1);             glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2d(1, 0);             glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2d(0, 0);             glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2d(1, 1);             glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2d(0, 1);             glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2d(0, 0);             glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2d(1, 0);             glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    // BOTTON
    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2d(1, 0);             glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2d(0, 0);             glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2d(0, 1);             glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2d(1, 1);             glVertex3f(1.0, -1.0, -1.0);
    glEnd();
    glEndList();
}

void Torus() {
    GLfloat m1_dif[] = { 0.0, 0.0, 0.0 , 1.0 };

    GLfloat m1_amb[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat m2_amb[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat m3_amb[] = { 0.0, 0.0, 1.0, 1.0 };

    GLfloat m1_spec[] = { 0.8, 0.0, 0.0, 1.0 };
    GLfloat m2_spec[] = { 0.0, 0.8, 0.0, 1.0 };
    GLfloat m3_spec[] = { 0.0, 0.0, 0.8, 1.0 };

    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m1_dif);

    // RED
    glMaterialfv(GL_FRONT, GL_AMBIENT, m1_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m1_spec);
    glPushMatrix();
    glTranslatef(rotate_x1, rotate_y1, rotate_z1);
    glutSolidTorus(0.02, 0.05, 10, 10);
    glPopMatrix();

    // GREEN
    glMaterialfv(GL_FRONT, GL_AMBIENT, m2_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m2_spec);
    glPushMatrix();
    glTranslatef(rotate_x2, rotate_y2, rotate_z2);
    glutSolidTorus(0.02, 0.05, 10, 10);
    glPopMatrix();

    // BLUE
    glMaterialfv(GL_FRONT, GL_AMBIENT, m3_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m3_spec);
    glPushMatrix();
    glTranslatef(rotate_x3, rotate_y3, rotate_z3);
    glutSolidTorus(0.02, 0.05, 10, 10);
    glPopMatrix();
}

void light() {
    // RED
    glEnable(GL_LIGHT1);
    GLfloat light_amb_1[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_dif_1[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat light_spec_1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_rotate_1[] = { rotate_x1, rotate_y1, rotate_z1, 0.0 };

    // GREEN
    glEnable(GL_LIGHT2);
    GLfloat light_amb_2[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_dif_2[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat light_spec_2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_rotate_2[] = { rotate_x2, rotate_y2, rotate_z2, 0.0 };

    // BLUE
    glEnable(GL_LIGHT3);
    GLfloat light_amb_3[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_dif_3[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat light_spec_3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_rotate_3[] = { rotate_x3, rotate_y3, rotate_z3, 0.0 };


    // устанавливаем параметры источника света
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_dif_1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_spec_1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_rotate_1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_amb_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_dif_2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_spec_2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_rotate_2);

    glLightfv(GL_LIGHT3, GL_AMBIENT, light_amb_3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_dif_3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_spec_3);
    glLightfv(GL_LIGHT3, GL_POSITION, light_rotate_3);


    // рисуем бублики (торы)   
    Torus();
}


void display() {
    // очищаем экрна и буфер глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // сброс трансформаций
    glLoadIdentity();

    // повернём сценку
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);


    // да будет свет
    light();
    cube();

    // Туман
    if (enable_fog) {
        glEnable(GL_FOG);                       // Включает туман (GL_FOG)
        //GLfloat fogColor[4] = { 1.5 * 0.39, 1.5 * 0.20, 1.5 * 0.50, 1.0 };
        GLfloat fogColor[4] = { 1.5 * 0.39, 1.5 * 0.20, 1.5 * 0.50, 1.0 };

        glFogi(GL_FOG_MODE, GL_LINEAR);          
        glFogfv(GL_FOG_COLOR, fogColor);        
        glHint(GL_FOG_HINT, GL_NICEST);         
        glFogf(GL_FOG_START, 1.0);             
        glFogf(GL_FOG_END, 0.0);               
    }

    // Nurbs
    GLfloat m0_spec[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat m0_dif[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat m0_amb[] = { 0.3, 0.3, 0.3, 1.0 };
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m0_spec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m0_dif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, m0_amb);


    glPushMatrix();
    glTranslatef(0.0, -0.5, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    glScalef(0.2, 0.2, 0.1);
    GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3,
        &ctrlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);
    glPopMatrix();


    // Текстура
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);

    // да будет квадратик (или кубик)
    // ПЕРВЫЙ КУБИК
    // доминирующее диффузное
    GLfloat m1_spec[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat m1_dif[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat m1_amb[] = { 0.3, 0.3, 0.3, 1.0 };
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m1_spec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m1_dif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, m1_amb);

    glPushMatrix();
    glTranslatef(0.0, -0.293, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glScalef(0.12, 0.12, 0.12);
    glCallList(1);
    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, TextureID[1]);

    // ВТОРОЙ КУБИК
    GLfloat m3_spec[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat m3_dif[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat m3_amb[] = { 0.3, 0.3, 0.3, 1.0 };
    glMaterialf(GL_FRONT, GL_SHININESS, 3);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m3_spec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m3_dif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, m3_amb);


    glPushMatrix();
    glTranslatef(0.0, 0.058, 0.0);
    //glScalef(0.5, 0.5, 0.5);
    glScalef(0.18, 0.18, 0.18);
    glCallList(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.338, 0.0);
    glRotatef(45.0, 0.0, 1.0, 0.0);
    glScalef(0.1, 0.1, 0.1);
    glCallList(1);
    glPopMatrix();

    // отключение источников света
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glFlush();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {

    if (key == GLUT_KEY_RIGHT) {
        switch (cur_light)
        {
        case 1: {
            // передняя плоскость
            if ((rotate_z1 <= -0.95) && (rotate_x1 <= 0.95)) {
                rotate_x1 += 0.1;
                break;
            }
            else if ((rotate_z1 <= -0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x1 >= 0.95) && (rotate_z1 <= 0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            else if (rotate_x1 >= 0.95) {
                rotate_x1 -= 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z1 >= 0.95) && (rotate_x1 >= -0.95)) {
                rotate_x1 -= 0.1;
                break;
            }
            else if (rotate_z1 >= 0.95) {
                rotate_z1 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x1 <= -0.95) && (rotate_z1 >= -0.95)) {
                rotate_z1 -= 0.1;
                break;
            }
            else if (rotate_x1 <= -0.95) {
                rotate_x1 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y1 >= 0.95) && (rotate_x1 <= 0.95)) {
                rotate_x1 += 0.1;
                break;
            }
            else if (rotate_y1 >= 0.95) {
                rotate_y1 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y1 <= -0.95) && (rotate_x1 <= 0.95)) {
                rotate_x1 += 0.1;
                break;
            }
            else if (rotate_y1 <= -0.95) {
                rotate_y1 += 0.1;
                break;
            }

            break;
        }
        case 2: {
            // передняя плоскость
            if ((rotate_z2 <= -0.95) && (rotate_x2 <= 0.95)) {
                rotate_x2 += 0.1;
                break;
            }
            else if ((rotate_z2 <= -0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x2 >= 0.95) && (rotate_z2 <= 0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            else if (rotate_x2 >= 0.95) {
                rotate_x2 -= 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z2 >= 0.95) && (rotate_x2 >= -0.95)) {
                rotate_x2 -= 0.1;
                break;
            }
            else if (rotate_z2 >= 0.95) {
                rotate_z2 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x2 <= -0.95) && (rotate_z2 >= -0.95)) {
                rotate_z2 -= 0.1;
                break;
            }
            else if (rotate_x2 <= -0.95) {
                rotate_x2 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y2 >= 0.95) && (rotate_x2 <= 0.95)) {
                rotate_x2 += 0.1;
                break;
            }
            else if (rotate_y2 >= 0.95) {
                rotate_y2 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y2 <= -0.95) && (rotate_x2 <= 0.95)) {
                rotate_x2 += 0.1;
                break;
            }
            else if (rotate_y2 <= -0.95) {
                rotate_y2 += 0.1;
                break;
            }
            break;
        }
        case 3: {
            // передняя плоскость
            if ((rotate_z3 <= -0.95) && (rotate_x3 <= 0.95)) {
                rotate_x3 += 0.1;
                break;
            }
            else if ((rotate_z3 <= -0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x3 >= 0.95) && (rotate_z3 <= 0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            else if (rotate_x3 >= 0.95) {
                rotate_x3 -= 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z3 >= 0.95) && (rotate_x3 >= -0.95)) {
                rotate_x3 -= 0.1;
                break;
            }
            else if (rotate_z3 >= 0.95) {
                rotate_z3 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x3 <= -0.95) && (rotate_z3 >= -0.95)) {
                rotate_z3 -= 0.1;
                break;
            }
            else if (rotate_x3 <= -0.95) {
                rotate_x3 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y3 >= 0.95) && (rotate_x3 <= 0.95)) {
                rotate_x3 += 0.1;
                break;
            }
            else if (rotate_y3 >= 0.95) {
                rotate_y3 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y3 <= -0.95) && (rotate_x3 <= 0.95)) {
                rotate_x3 += 0.1;
                break;
            }
            else if (rotate_y3 <= -0.95) {
                rotate_y3 += 0.1;
                break;
            }
            break;
        }
        case 4: {
            rotate_y -= 5;
            break;
        }
        }
    }

    else if (key == GLUT_KEY_LEFT) {
        switch (cur_light)
        {
        case (1): {
            // передняя плоскость
            if ((rotate_z1 <= -0.95) && (rotate_x1 >= -0.95)) {
                rotate_x1 -= 0.1;
                break;
            }
            else if ((rotate_z1 <= -0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x1 >= 0.95) && (rotate_z1 >= -0.95)) {
                rotate_z1 -= 0.1;
                break;
            }
            else if (rotate_x1 >= 0.95) {
                rotate_x1 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z1 >= 0.95) && (rotate_x1 <= 0.95)) {
                rotate_x1 += 0.1;
                break;
            }
            else if (rotate_z1 >= 0.95) {
                rotate_z1 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x1 <= -0.95) && (rotate_z1 <= 0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            else if (rotate_x1 <= -0.95) {
                rotate_x1 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y1 >= 0.95) && (rotate_x1 >= -0.95)) {
                rotate_x1 -= 0.1;
                break;
            }
            else if (rotate_y1 >= 0.95) {
                rotate_y1 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y1 <= -0.95) && (rotate_x1 >= -0.95)) {
                rotate_x1 -= 0.1;
                break;
            }
            else if (rotate_y1 <= -0.95) {
                rotate_y1 += 0.1;
                break;
            }
            break;
        }
        case (2): {
            // передняя плоскость
            if ((rotate_z2 <= -0.95) && (rotate_x2 >= -0.95)) {
                rotate_x2 -= 0.1;
                break;
            }
            else if ((rotate_z2 <= -0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x2 >= 0.95) && (rotate_z2 >= -0.95)) {
                rotate_z2 -= 0.1;
                break;
            }
            else if (rotate_x2 >= 0.95) {
                rotate_x2 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z2 >= 0.95) && (rotate_x2 <= 0.95)) {
                rotate_x2 += 0.1;
                break;
            }
            else if (rotate_z2 >= 0.95) {
                rotate_z2 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x2 <= -0.95) && (rotate_z2 <= 0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            else if (rotate_x2 <= -0.95) {
                rotate_x2 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y2 >= 0.95) && (rotate_x2 >= -0.95)) {
                rotate_x2 -= 0.1;
                break;
            }
            else if (rotate_y2 >= 0.95) {
                rotate_y2 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y2 <= -0.95) && (rotate_x2 >= -0.95)) {
                rotate_x2 -= 0.1;
                break;
            }
            else if (rotate_y2 <= -0.95) {
                rotate_y2 += 0.1;
                break;
            }
            break;
        }
        case (3): {
            // передняя плоскость
            if ((rotate_z3 <= -0.95) && (rotate_x3 >= -0.95)) {
                rotate_x3 -= 0.1;
                break;
            }
            else if ((rotate_z3 <= -0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            // правая плоскость
            if ((rotate_x3 >= 0.95) && (rotate_z3 >= -0.95)) {
                rotate_z3 -= 0.1;
                break;
            }
            else if (rotate_x3 >= 0.95) {
                rotate_x3 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z3 >= 0.95) && (rotate_x3 <= 0.95)) {
                rotate_x3 += 0.1;
                break;
            }
            else if (rotate_z3 >= 0.95) {
                rotate_z3 -= 0.1;
                break;
            }
            // левая плоскость
            if ((rotate_x3 <= -0.95) && (rotate_z3 <= 0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            else if (rotate_x3 <= -0.95) {
                rotate_x3 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y3 >= 0.95) && (rotate_x3 >= -0.95)) {
                rotate_x3 -= 0.1;
                break;
            }
            else if (rotate_y3 >= 0.95) {
                rotate_y3 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y3 <= -0.95) && (rotate_x3 >= -0.95)) {
                rotate_x3 -= 0.1;
                break;
            }
            else if (rotate_y3 <= -0.95) {
                rotate_y3 += 0.1;
                break;
            }
            break;
        }
        case 4: {
            rotate_y += 5;
            break;
        }
        }
    }

    else if (key == GLUT_KEY_UP) {
        switch (cur_light)
        {
        case (1): {
            // передняя плоскость
            if ((rotate_z1 <= -0.95) && (rotate_y1 <= 0.95)) {
                rotate_y1 += 0.1;
                break;
            }
            else if ((rotate_z1 <= -0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y1 >= 0.95) && (rotate_z1 <= 0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            else if (rotate_x1 >= 0.95) {
                rotate_y1 -= 0.1;
                break;
            }

            // задняя плосость
            if ((rotate_z1 >= 0.95) && (rotate_y1 >= -0.95)) {
                rotate_y1 -= 0.1;
                break;
            }
            else if (rotate_z1 >= 0.95) {
                rotate_z1 -= 0.1;
                break;
            }

            // нижняя плоскость
            if ((rotate_y1 <= -0.95) && (rotate_z1 >= -0.95)) {
                rotate_z1 -= 0.1;
                break;
            }
            else if (rotate_y1 <= -0.95) {
                rotate_y1 += 0.1;
                break;
            }
            break;
        }
        case (2): {
            // передняя плоскость
            if ((rotate_z2 <= -0.95) && (rotate_y2 <= 0.95)) {
                rotate_y2 += 0.1;
                break;
            }
            else if ((rotate_z2 <= -0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y2 >= 0.95) && (rotate_z2 <= 0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            else if (rotate_x1 >= 0.95) {
                rotate_y2 -= 0.1;
                break;
            }

            // задняя плосость
            if ((rotate_z2 >= 0.95) && (rotate_y2 >= -0.95)) {
                rotate_y2 -= 0.1;
                break;
            }
            else if (rotate_z2 >= 0.95) {
                rotate_z2 -= 0.1;
                break;
            }

            // нижняя плоскость
            if ((rotate_y2 <= -0.95) && (rotate_z2 >= -0.95)) {
                rotate_z2 -= 0.1;
                break;
            }
            else if (rotate_y2 <= -0.95) {
                rotate_y2 += 0.1;
                break;
            }
            break;
        }
        case (3): {
            // передняя плоскость
            if ((rotate_z3 <= -0.95) && (rotate_y3 <= 0.95)) {
                rotate_y3 += 0.1;
                break;
            }
            else if ((rotate_z3 <= -0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y3 >= 0.95) && (rotate_z3 <= 0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            else if (rotate_x1 >= 0.95) {
                rotate_y3 -= 0.1;
                break;
            }

            // задняя плосость
            if ((rotate_z3 >= 0.95) && (rotate_y3 >= -0.95)) {
                rotate_y3 -= 0.1;
                break;
            }
            else if (rotate_z3 >= 0.95) {
                rotate_z3 -= 0.1;
                break;
            }

            // нижняя плоскость
            if ((rotate_y3 <= -0.95) && (rotate_z3 >= -0.95)) {
                rotate_z3 -= 0.1;
                break;
            }
            else if (rotate_y3 <= -0.95) {
                rotate_y3 += 0.1;
                break;
            }
            break;
        }
        case 4: {
            rotate_x += 5;
            break;
        }
        }
    }

    else if (key == GLUT_KEY_DOWN) {
        switch (cur_light)
        {
        case (1): {
            // передняя плоскость
            if ((rotate_z1 <= -0.95) && (rotate_y1 >= -0.95)) {
                rotate_y1 -= 0.1;
                break;
            }
            else if ((rotate_z1 <= -0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y1 >= 0.95) && (rotate_z1 >= -0.95)) {
                rotate_z1 -= 0.1;
                break;
            }
            else if (rotate_y1 >= 0.95) {
                rotate_y1 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z1 >= 0.95) && (rotate_y1 <= 0.95)) {
                rotate_y1 += 0.1;
                break;
            }
            else if (rotate_z1 >= 0.95) {
                rotate_z1 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y1 <= -0.95) && (rotate_z1 <= 0.95)) {
                rotate_z1 += 0.1;
                break;
            }
            else if (rotate_y1 <= -0.95) {
                rotate_y1 += 0.1;
                break;
            }
            break;
        }
        case (2): {
            // передняя плоскость
            if ((rotate_z2 <= -0.95) && (rotate_y2 >= -0.95)) {
                rotate_y2 -= 0.1;
                break;
            }
            else if ((rotate_z2 <= -0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y2 >= 0.95) && (rotate_z2 >= -0.95)) {
                rotate_z2 -= 0.1;
                break;
            }
            else if (rotate_y2 >= 0.95) {
                rotate_y2 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z2 >= 0.95) && (rotate_y2 <= 0.95)) {
                rotate_y2 += 0.1;
                break;
            }
            else if (rotate_z2 >= 0.95) {
                rotate_z2 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y2 <= -0.95) && (rotate_z2 <= 0.95)) {
                rotate_z2 += 0.1;
                break;
            }
            else if (rotate_y2 <= -0.95) {
                rotate_y2 += 0.1;
                break;
            }
            break;
        }
        case (3): {
            // передняя плоскость
            if ((rotate_z3 <= -0.95) && (rotate_y3 >= -0.95)) {
                rotate_y3 -= 0.1;
                break;
            }
            else if ((rotate_z3 <= -0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            // верхняя плоскость
            if ((rotate_y3 >= 0.95) && (rotate_z3 >= -0.95)) {
                rotate_z3 -= 0.1;
                break;
            }
            else if (rotate_y3 >= 0.95) {
                rotate_y3 += 0.1;
                break;
            }
            // задняя плосость
            if ((rotate_z3 >= 0.95) && (rotate_y3 <= 0.95)) {
                rotate_y3 += 0.1;
                break;
            }
            else if (rotate_z3 >= 0.95) {
                rotate_z3 -= 0.1;
                break;
            }
            // нижняя плоскость
            if ((rotate_y3 <= -0.95) && (rotate_z3 <= 0.95)) {
                rotate_z3 += 0.1;
                break;
            }
            else if (rotate_y3 <= -0.95) {
                rotate_y3 += 0.1;
                break;
            }
            break;
        }
        case 4: {
            rotate_x -= 5;
            break;
        }
        }
    }


    // обновление экрана
    glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1': {
        cur_light = 1;
        break;
    }

    case '2': {
        cur_light = 2;
        break;
    }

    case '3': {
        cur_light = 3;
        break;
    }
    case '4': {
        cur_light = 4;
        break;
    }
    case '5': {
        rotate_x = 0;
        rotate_y = 0;
        break;
    }
    case 'f':
    case 'F':
    case 'а':
    case 'А': {
        enable_fog = !enable_fog;
        break;
    }

    case 27: {
        exit(0);
        break;
    }
    case ' ': {
        std::cout << "x1 = " << rotate_x1 << "; y1 = " << rotate_y1 << "; z1 = " << rotate_z1 << "\n";
        std::cout << "x2 = " << rotate_x2 << "; y2 = " << rotate_y2 << "; z2 = " << rotate_z2 << "\n";
        std::cout << "x3 = " << rotate_x3 << "; y3 = " << rotate_y3 << "; z3 = " << rotate_z3 << "\n\n";
        break;
    }
    }

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, 10.0, -10.0);
    else
        glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Пусть тут будет куб");

    // инициализация
    myinit();

    // функции обратного вызова
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    // основной цикл рендеринга
    glutMainLoop();
}