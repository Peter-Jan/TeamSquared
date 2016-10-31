#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"

const double YsPi=3.1415927;


void DrawGroundLattice(void)
{
    glColor3ub(0,0,255);
    glBegin(GL_LINES);
    
    for(int i=-1000; i<=1000; i+=20)
    {
        glVertex3i(i,0,-1000);
        glVertex3i(i,0,+1000);
        
        glVertex3i(-1000,0,i);
        glVertex3i(+1000,0,i);
    }
    
    glEnd();
}


class CameraObject
{
public:
    double x,y,z;
    double h,p,b;
    
    double fov,nearZ,farZ;
    
    CameraObject();
    void Initialize(void);
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);
    
    void GetForwardVector(double &vx,double &vy,double &vz);
};

CameraObject::CameraObject()
{
    Initialize();
}

void CameraObject::Initialize(void)
{
    x=0;
    y=0;
    z=0;
    h=0;
    p=0;
    b=0;
    
    fov=YsPi/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void CameraObject::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;
    
    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/YsPi,aspect,nearZ,farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-b*180.0/YsPi,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/YsPi,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-h*180.0/YsPi,0.0,1.0,0.0);  // Rotation about Y axis
    glTranslated(-x,-y,-z);
}

void CameraObject::GetForwardVector(double &vx,double &vy,double &vz)
{
    vx=-cos(p)*sin(h);
    vy= sin(p);
    vz=-cos(p)*cos(h);
}

int main(void)
{
    int terminate=0;
    double py,pvy;
    double g = -.03;
    double vx,vy,vz;
    vx=0;
    vy=0;
    vz=0;
    CameraObject camera;
    py = 30;
    pvy = 0;
    camera.y=py;
    camera.nearZ=1.0;
    camera.farZ=1000.0;
    
    FsOpenWindow(16,16,800,600,1);
    
    while(0==terminate)
    {
        FsPollDevice();
        
        int wid,hei;
        FsGetWindowSize(wid,hei);
        
        int key=FsInkey();
        switch(key)
        {
            case FSKEY_ESC:
                terminate=1;
                break;
        }

        
        
        if(0!=FsGetKeyState(FSKEY_LEFT))
        {
            camera.h+=YsPi/240.0;
        }
        if(0!=FsGetKeyState(FSKEY_RIGHT))
        {
            camera.h-=YsPi/240.0;
        }
        if(0!=FsGetKeyState(FSKEY_UP))
        {
            camera.p+=YsPi/240.0;
        }
        if(0!=FsGetKeyState(FSKEY_DOWN))
        {
            camera.p-=YsPi/240.0;
        }
        if(py == 30)
        {
            if(0!=FsGetKeyState(FSKEY_SPACE))
            {
                pvy = 2;
            }
        }
        if(0!=FsGetKeyState(FSKEY_F))
        {
            double vx,vy,vz;
            camera.GetForwardVector(vx,vy,vz);
            camera.x+=vx/1;
            camera.z+=vz/1;
        }
        if(0!=FsGetKeyState(FSKEY_B))
        {
            double vx,vy,vz;
            camera.GetForwardVector(vx,vy,vz);
            camera.x-=vx/4.0;
            camera.z-=vz/4.0;
        }
        if(pvy!=0)
        {
            py = py + pvy;
            pvy+=g;
        }
        if(py<30)
        {
            py = 30;
            pvy = 0;
        }
        camera.y = py;
        
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        
        glViewport(0,0,wid,hei);
        
        // Set up 3D drawing
        camera.SetUpCameraProjection();
        camera.SetUpCameraTransformation();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        
        // 3D drawing from here
        DrawGroundLattice();
        
        
        // Set up 2D drawing
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,(float)wid-1,(float)hei-1,0,-1,1);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glDisable(GL_DEPTH_TEST);
        
        // 2D drawing from here
        
        
        FsSwapBuffers();
        FsSleep(10);
    }
    
    return 0;
}


