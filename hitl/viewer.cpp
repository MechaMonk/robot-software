#include <error/error.h>
#include <cmath>
#include <box2d/box2d.h>
#include "viewer.h"
#include <thread>
#include <GL/glut.h>

std::vector<Renderable*>* renderables;

// Helper function to pass hex colors to OpenGL, as this is the most common
// format when taking color from screens
static void hexColor(int r, int g, int b)
{
    glColor3f(r / 256.f, g / 256.f, b / 256.f);
}

void RobotRenderer::render()
{
    b2Vec2 pos = robot.GetPosition();
    float angle_rad = robot.GetAngle();

    DEBUG_EVERY_N(10, "draw robot at %.3f, %.3f", pos.x, pos.y);

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.);

    glRotatef(angle_rad / M_PI * 180., 0., 0., 1.);

    glBegin(GL_QUADS);

    hexColor(0xef, 0xa0, 0x18);

    glVertex2f(-0.12, 0.12);
    glVertex2f(-0.12, -0.12);
    glVertex2f(0.12, -0.12);
    glVertex2f(0.12, 0.12);
    glEnd();

    glColor3f(0.f, 0.f, 0.f);
    glBegin(GL_LINES);
    glVertex2f(0., 0.);
    glVertex2f(0.12, 0.);
    glEnd();
    glPopMatrix();
}

void CupRenderer::render()
{
    b2Vec2 pos = cup->GetPosition();

    switch (color) {
        case CupColor::RED:
            hexColor(0xab, 0x3c, 0x37);
            break;

        case CupColor::GREEN:
            hexColor(0x30, 0x5b, 0x35);
            break;
    }

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.);

    const int sides = 10;

    glBegin(GL_POLYGON);

    for (int i = 0; i < sides; i++) {
        float angle = 2 * M_PI * (float)i / sides;
        float x = PhysicsCup::radius * cosf(angle);
        float y = PhysicsCup::radius * sinf(angle);
        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}

void TableRenderer::render()
{
    // TODO(antoinealb): Do use texture there
    glBegin(GL_QUADS);
    hexColor(0x4d, 0xd4, 0xff);
    glVertex2f(0.f, 2.f);
    glVertex2f(0.f, 0.f);
    glVertex2f(3.f, 0.f);
    glVertex2f(3.f, 2.f);
    glEnd();
}

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glOrtho(0, 3., 0, 2., -1., 1.);

    // Do a rotation around the table center, to convert OpenGL to CVRA convention 
    glRotatef(180, 0., 0., 1.);
    glTranslatef(-3., -2., 0.);

    for (Renderable* r : *renderables) {
        r->render();
    }

    glutSwapBuffers();
}

void on_timer(int /*value*/)
{
    glutPostRedisplay();
    glutTimerFunc(33, on_timer, 0);
}

void startRendering(int argc, char** argv, std::vector<Renderable*>* r)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1500, 1000);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("CVRA");

    glutDisplayFunc(display);
    glutTimerFunc(33, on_timer, 0);

    renderables = r;

    glutMainLoop();
}
