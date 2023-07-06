#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <time.h>
using namespace std;
int MAX_RESCURSION = 0;
const int MAX_RESCURSION_ACTUAL = 6;
const int MAX_RESCURSION_EXPO =pow(2,MAX_RESCURSION_ACTUAL-1);
struct Point{
   double x, y, z;
};
int count1 = 0;
int FPS = 0;
int FPS_LAST_TIME = time(NULL);
int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = WINDOW_HEIGHT;
float TRIANGLE_SIDE = WINDOW_WIDTH;
uint64_t timeSinceEpochMicrosec() {
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}



void drawLines(Point ini, Point final) {
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex3d(ini.x/WINDOW_WIDTH, ini.y/WINDOW_HEIGHT,ini.z/WINDOW_HEIGHT);
    glVertex3d(final.x/WINDOW_WIDTH, final.y/WINDOW_HEIGHT,final.z/WINDOW_HEIGHT);
    glEnd();
}


void drawSquare(Point ver1, Point ver2, Point ver3, Point ver4) {
    glColor3f(1,1,1);
    glPointSize(3.0);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3d(ver1.x/WINDOW_WIDTH, ver1.y/WINDOW_HEIGHT,ver1.z/WINDOW_HEIGHT);
    glVertex3d(ver2.x/WINDOW_WIDTH, ver2.y/WINDOW_HEIGHT,ver2.z/WINDOW_HEIGHT);
    glVertex3d(ver3.x/WINDOW_WIDTH, ver3.y/WINDOW_HEIGHT,ver3.z/WINDOW_HEIGHT);
    glVertex3d(ver4.x/WINDOW_WIDTH, ver4.y/WINDOW_HEIGHT,ver4.z/WINDOW_HEIGHT);
    glEnd();
}

void drawTriangle(Point ver1, Point ver2, Point ver3) {

    glBegin(GL_POLYGON);
    glColor3f(0,0,1);
    glVertex3d(ver1.x/WINDOW_WIDTH, ver1.y/WINDOW_HEIGHT,ver1.z/WINDOW_HEIGHT);
        glColor3f(0,1,0);
    glVertex3d(ver2.x/WINDOW_WIDTH, ver2.y/WINDOW_HEIGHT,ver2.z/WINDOW_HEIGHT);
        glColor3f(1,0,0);
    glVertex3d(ver3.x/WINDOW_WIDTH, ver3.y/WINDOW_HEIGHT,ver3.z/WINDOW_HEIGHT);
    glEnd();
}

void drawSier(float side, float centerX = 0, float centerY = 0,float centerZ = 0, int count = 0){
    if(count == MAX_RESCURSION){
        Point vertex1 = {centerX,centerY + (0.43301270189)*side, centerZ};
        Point vertex2 = {centerX - side/2,centerY - (0.43301270189)*side,centerZ+side/2};
        Point vertex3 = {centerX + side/2,centerY - (0.43301270189)*side,centerZ+side/2};
        Point vertex4 = {centerX - side/2,centerY - (0.43301270189)*side,centerZ-side/2};
        Point vertex5= {centerX + side/2,centerY - (0.43301270189)*side,centerZ-side/2};

        drawSquare(vertex2,vertex3,vertex5,vertex4);
        drawTriangle(vertex2,vertex3,vertex1);
        drawTriangle(vertex2,vertex4,vertex1);
        drawTriangle(vertex4,vertex5,vertex1);
        drawTriangle(vertex5,vertex3,vertex1);


        drawLines(vertex2,vertex1);
        drawLines(vertex3,vertex1);
        drawLines(vertex4,vertex1);
        drawLines(vertex5,vertex1);
        return;
    }


    count++;
    drawSier(side/2, centerX + side/4, centerY - (0.43301270189)*side + (0.43301270189/2)*side, centerZ + side/2 - side/4,count);
    drawSier(side/2, centerX + side/4, centerY - (0.43301270189)*side + (0.43301270189/2)*side, centerZ - side/2 + side/4,count);
    drawSier(side/2, centerX - side/4, centerY - (0.43301270189)*side + (0.43301270189/2)*side, centerZ + side/2 - side/4,count);
    drawSier(side/2, centerX - side/4, centerY - (0.43301270189)*side + (0.43301270189/2)*side, centerZ - side/2 + side/4,count);
    drawSier(side/2, centerX, centerY + (0.43301270189)*side - (0.43301270189/2)*side, centerZ,count);
}


void myDisplay(void){
    glEnable(GL_DEPTH_TEST);
    bool isAnimation = true;
    float scale = 1;
    float PHASE_ONE_ANIMATION = 60*2;
    float PHASE_ONE_COUNT = 0;
    float ANIMATION_RATE = 1.013;
    bool PADDING_ANIMATION = false;
    float rotation = 0;
    //int FPS_TO_SHOW = FPS;
    //uint64_t lastFrameTime = timeSinceEpochMicrosec();
    bool increaseMaxRecur = true;
    while(true){
        glClear(GL_COLOR_BUFFER_BIT);


        rotation += 0.5;
        glRotatef(rotation, 0,1,0);



        if(MAX_RESCURSION != MAX_RESCURSION_ACTUAL -1){

        }else if(PADDING_ANIMATION){
            glScalef(scale,scale,scale);

            glTranslatef(0,min(PHASE_ONE_COUNT/PHASE_ONE_ANIMATION,(float) 1)*(-TRIANGLE_SIDE*sqrt(3)/4 +(TRIANGLE_SIDE/MAX_RESCURSION_EXPO)*sqrt(3)/4 )/WINDOW_HEIGHT,0);

            if((int)rotation%90 == 0){

                glTranslatef(0,-min(PHASE_ONE_COUNT/PHASE_ONE_ANIMATION,(float) 1)*(-TRIANGLE_SIDE*sqrt(3)/4 +(TRIANGLE_SIDE/MAX_RESCURSION_EXPO)*sqrt(3)/4 )/WINDOW_HEIGHT,0);

                isAnimation=true;
                increaseMaxRecur = false;

                MAX_RESCURSION = (MAX_RESCURSION + 1)%MAX_RESCURSION_ACTUAL;
                glScalef(1/scale,1/scale,1/scale);
                scale = 1;

                PHASE_ONE_COUNT = 0;
                PADDING_ANIMATION = false;
            }
        }else{

            PHASE_ONE_COUNT++;

            isAnimation = false;

            scale = scale*ANIMATION_RATE;
            glScalef(scale,scale,scale);

            if(scale >=MAX_RESCURSION_EXPO){
                PADDING_ANIMATION = true;
            }else{
                glTranslatef(0,min(PHASE_ONE_COUNT/PHASE_ONE_ANIMATION,(float) 1)*(-TRIANGLE_SIDE*sqrt(3)/4 +(TRIANGLE_SIDE/MAX_RESCURSION_EXPO)*sqrt(3)/4 )/WINDOW_HEIGHT,0);
            }

            if(PADDING_ANIMATION){
                glTranslatef(0,min(PHASE_ONE_COUNT/PHASE_ONE_ANIMATION,(float) 1)*(-TRIANGLE_SIDE*sqrt(3)/4 +(TRIANGLE_SIDE/MAX_RESCURSION_EXPO)*sqrt(3)/4 )/WINDOW_HEIGHT,0);
            }
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawSier(TRIANGLE_SIDE);
        glFlush();

        uint64_t lastFrameTime = timeSinceEpochMicrosec() - lastFrameTime;
        if(lastFrameTime < 8000){
            usleep(8000 - lastFrameTime);
        }

        if(MAX_RESCURSION == MAX_RESCURSION_ACTUAL - 1){
            glTranslatef(0,-min(PHASE_ONE_COUNT/PHASE_ONE_ANIMATION,(float) 1)*(-TRIANGLE_SIDE*sqrt(3)/4 +(TRIANGLE_SIDE/MAX_RESCURSION_EXPO)*sqrt(3)/4 )/WINDOW_HEIGHT,0);
        }


        glRotatef(-rotation, 0,1,0);
        glScalef(1/scale,1/scale,1/scale);


        if((int)rotation%90 == 0 && isAnimation && (int) rotation!=0 && (ceil(rotation) == rotation) && increaseMaxRecur){
            MAX_RESCURSION = (MAX_RESCURSION + 1)%MAX_RESCURSION_ACTUAL;
            rotation =0;
        }else if(!increaseMaxRecur){
            increaseMaxRecur = true;
        }

        lastFrameTime = timeSinceEpochMicrosec();
    }
}


int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sierpinski Tetrahedron");
    glutDisplayFunc(myDisplay);
    glutMainLoop();

    return 0;
}
