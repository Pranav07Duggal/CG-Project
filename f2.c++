// Filename: main.cpp

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WIDTH = 1280, HEIGHT = 720;
const float STRIP_LENGTH = 60.0f;
const float STRIP_WIDTH = 4.0f;

enum State { IDLE, TAKEOFF_START, TAKEOFF_ASCEND, FLIGHT, TARGET_HIT, LANDING_PREP, LANDING, DONE };
State state = IDLE;

float jetX = -STRIP_LENGTH / 2.0f + 1.0f;
float jetY = 0.0f;
float jetZ = 0.0f;
float jetAngle = 0.0f;
float speed = 0.05f;

float camX = jetX, camY = 2.0f, camZ = 15.0f;
bool followJet = false;

bool missileFired = false;
float missileX, missileY;
const float missileSpeed = 0.5f;

float enemyX = 15.0f, enemyY = 10.0f;
bool enemyAlive = false;
bool wasdEnabled = false;

bool keyW, keyA, keyS, keyD;
std::vector<float> vertices, normals, texcoords;
std::vector<unsigned int> indices;
GLuint textureColor;

bool loadModel(const std::string &path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << err << std::endl; return false;
    }
    for (auto &shape: shapes) {
        for (auto &idx: shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3*idx.vertex_index]);
            vertices.push_back(attrib.vertices[3*idx.vertex_index+1]);
            vertices.push_back(attrib.vertices[3*idx.vertex_index+2]);
            if (!attrib.normals.empty()) {
                normals.push_back(attrib.normals[3*idx.normal_index]);
                normals.push_back(attrib.normals[3*idx.normal_index+1]);
                normals.push_back(attrib.normals[3*idx.normal_index+2]);
            }
            if (!attrib.texcoords.empty()) {
                texcoords.push_back(attrib.texcoords[2*idx.texcoord_index]);
                texcoords.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
            }
            indices.push_back(indices.size());
        }
    }
    return true;
}

GLuint loadTexture(const char *file) {
    int w,h,n;
    unsigned char *data = stbi_load(file,&w,&h,&n,0);
    if(!data){ std::cerr<<"Failed: "<<file<<std::endl; return 0; }
    GLuint tex; glGenTextures(1,&tex); glBindTexture(GL_TEXTURE_2D,tex);
    GLenum fmt=(n==4?GL_RGBA:GL_RGB);
    glTexImage2D(GL_TEXTURE_2D,0,fmt,w,h,0,fmt,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

void drawGroundAndStrip(float yOffset = 0) {
    // Green ground
    glColor3f(0.1f, 0.8f, 0.1f);  // Bright green
    glBegin(GL_QUADS);
    glVertex3f(-100, yOffset, -100);
    glVertex3f(100, yOffset, -100);
    glVertex3f(100, yOffset, 100);
    glVertex3f(-100, yOffset, 100);
    glEnd();

    // Black airstrip
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(-STRIP_LENGTH / 2, yOffset + 0.01f, -STRIP_WIDTH / 2);
    glVertex3f(STRIP_LENGTH / 2, yOffset + 0.01f, -STRIP_WIDTH / 2);
    glVertex3f(STRIP_LENGTH / 2, yOffset + 0.01f, STRIP_WIDTH / 2);
    glVertex3f(-STRIP_LENGTH / 2, yOffset + 0.01f, STRIP_WIDTH / 2);
    glEnd();

    // White stripes
    glColor3f(1, 1, 1);
    float stripeW = STRIP_LENGTH / 20;
    for (int i = 0; i < 20; i += 2) {
        float x0 = -STRIP_LENGTH / 2 + i * stripeW;
        glBegin(GL_QUADS);
        glVertex3f(x0, yOffset + 0.02f, -0.2f);
        glVertex3f(x0 + stripeW, yOffset + 0.02f, -0.2f);
        glVertex3f(x0 + stripeW, yOffset + 0.02f, 0.2f);
        glVertex3f(x0, yOffset + 0.02f, 0.2f);
        glEnd();
    }
}

void updateCamera() {
    if (followJet) {
        camX = jetX;
        camY = jetY + 2.0f;
        camZ = jetZ + 15.0f;
    }
}
void drawModel() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureColor);
    glBegin(GL_TRIANGLES);
    for(size_t i=0;i<indices.size();++i){
      if(!texcoords.empty()) glTexCoord2f(texcoords[2*i],texcoords[2*i+1]);
      if(!normals.empty())   glNormal3f(normals[3*i],normals[3*i+1],normals[3*i+2]);
      glVertex3f(vertices[3*i],vertices[3*i+1],vertices[3*i+2]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void drawJet() {
    glPushMatrix();
    glTranslatef(jetX, jetY+1, jetZ);
    glRotatef(-90, 1, 0, 0);
    glRotatef(-jetAngle,0,1,0);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.7f, 0.7f, 1.0f);
    drawModel();
    glPopMatrix();
}

void drawEnemy() {
    if (!enemyAlive && state == FLIGHT) {
        enemyAlive = true;
        enemyX = jetX + 15;
        enemyY = jetY + 10;
    }
    if (enemyAlive) {
        glPushMatrix();
        glTranslatef(enemyX, enemyY, 0);
        glColor3f(1, 0, 0);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.5,0.5,0.5);
        drawModel();
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, jetX, jetY, jetZ, 0, 1, 0);

    drawGroundAndStrip();  // main airstrip
    if (state == LANDING || state == DONE)
        drawGroundAndStrip(-1);  // second strip

    drawJet();
    drawEnemy();

    if (missileFired) {
        missileX += missileSpeed;
        if (missileX >= enemyX - 1) {
            enemyAlive = false;
            missileFired = false;
            state = TARGET_HIT;
        }
        glColor3f(1, 0.5, 0);
        glPushMatrix();
        glTranslatef(missileX, missileY, 0);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void update(int) {
    switch (state) {
        case TAKEOFF_START:
            jetX += speed;
            speed += 0.0008f;
            if (jetX >= STRIP_LENGTH / 2 - 5) {
                state = TAKEOFF_ASCEND;
            }
            break;

        case TAKEOFF_ASCEND:
            jetAngle += 1.5f;
            jetX += 0.1f;
            jetY += 0.15f;
            if (jetAngle >= 60) {
                jetAngle -= 1.5f;
                jetAngle = 0;
                followJet = true;
                wasdEnabled = true;
                state = FLIGHT;            
            }
            break;

        case LANDING_PREP:
            jetY -= 0.05f;
            if (jetY <= 0.1f) {
                jetY = 0.1f;
                state = DONE;
            }
            break;

        default: break;
    }

    if (wasdEnabled) {
        if (keyW) jetY += 0.1f;
        if (keyS) jetY -= 0.1f;
        if (keyA) jetX -= 0.1f;
        if (keyD) jetX += 0.1f;
    }

    updateCamera();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboardDown(unsigned char key, int, int) {
    switch (key) {
        case 't': if (state == IDLE) { state = TAKEOFF_START; speed = 0.05f; } break;
        case 'l': if (state == TARGET_HIT) { state = LANDING_PREP; } break;
        case 'w': keyW = true; break;
        case 'a': keyA = true; break;
        case 's': keyS = true; break;
        case 'd': keyD = true; break;
        case ' ': if (state == FLIGHT && !missileFired) {
            missileFired = true;
            missileX = jetX;
            missileY = jetY + 1;
        } break;
    }
}

void keyboardUp(unsigned char key, int, int) {
    if (key == 'w') keyW = false;
    if (key == 'a') keyA = false;
    if (key == 's') keyS = false;
    if (key == 'd') keyD = false;
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 200);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Jet Takeoff and Landing Simulation");

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
    loadModel("model/F-2.obj");
    textureColor = loadTexture("model/textures/F-2_Color.png");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
