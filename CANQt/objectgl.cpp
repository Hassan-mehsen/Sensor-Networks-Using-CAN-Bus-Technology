#include "objectgl.h"

#define L1  0.9
#define L2  0.4
#define L3  0.2

ObjectOpenGL::ObjectOpenGL(QWidget *parent) :  QGLWidget(parent)
{
    // Initialiser les couleurs
    BackGround_Color = QColor::fromRgb(255, 255, 255);
    Axis_X_Color = QColor::fromRgb(255, 87, 51, 180); // #FF5733 (rouge corail) pour l'axe X
    Axis_Y_Color = QColor::fromRgb(0, 128, 0, 180);   // #008000 (vert) pour l'axe Y
    Axis_Z_Color = QColor::fromRgb(51, 153, 255, 180); // #3399FF (bleu clair) pour l'axe Z

    // Définir les valeurs initiales des angles pour une vue dynamique
    angle_x = 30; // Angle X initial
    angle_y = 30; // Angle Y initial
    angle_z = 0;  // Angle Z initial

    // Démarrer l'affichage dans la vue isométrique
    IsometricView();
}



/*
 *************************************************************************************************/

ObjectOpenGL::~ObjectOpenGL()
{
    makeCurrent();
}

/*
 *************************************************************************************************/

void ObjectOpenGL::initializeGL()
{
    qglClearColor(BackGround_Color);                   // Set backGround color
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);                           // Depth buffer enabled (Hide invisible items)
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // For transparency
    glColorMaterial(GL_FRONT,GL_DIFFUSE);
    glEnable(GL_NORMALIZE);
}

/*
 *************************************************************************************************
void ObjectOpenGL::paintGL(  )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set the lights
    GLfloat LightAmbient[]={0.4f,0.4f,0.4f,1.0f};
    GLfloat LightDiffuse[]={0.8f,0.8f,0.8f,1.0f};
    glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
    int LightPos[4]={0,0,10,1};
    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);

    // Move the display according to the current orientation
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    glDisable(GL_LIGHTING);

    // Invert the Y-axis for an orthonormal frame
    glScalef(1,-1,1);

    // Draw Frame
    Draw_Frame();

    // Start display of the items
    glPushMatrix();                // The following properties are only for the object

    // Zoom according to the view's parameters
    glScalef(Zoom,Zoom,Zoom);      // ReZoom the object

    // Light independant (color is constant)
    glLineWidth(5.0);

    Draw_Box();
    glEnable(GL_LIGHTING);   // Re enable the light
    // End of the object
    glPopMatrix();

    // Update the view
    glViewport(0, 0,WindowSize.width(), WindowSize.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat Ratio=(GLfloat)WindowSize.width()/(GLfloat)WindowSize.height();
    glOrtho((-0.5+dx)*Ratio,
            ( 0.5+dx)*Ratio ,
            +0.5+dy,
            -0.5+dy,
            -1500.0, 1500.0);
    glMatrixMode(GL_MODELVIEW);
}
*/
/*
 *************************************************************************************************/
void ObjectOpenGL::paintGL()
{
    // Efface le tampon de couleur et le tampon de profondeur
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configuration des lumières
    GLfloat LightAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat LightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    int LightPos[] = {0, 0, 10, 1};
    glLightiv(GL_LIGHT0, GL_POSITION, LightPos);

    // Applique les rotations selon les angles normalisés
    NormalizeAngle(&xRot);
    NormalizeAngle(&yRot);
    NormalizeAngle(&zRot);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    // Désactive les lumières pour le dessin du cadre orthonormé
    glDisable(GL_LIGHTING);

    // Inverse l'axe Y pour un cadre orthonormé correct
    glScalef(1, -1, 1);

    // Dessine le cadre orthonormé
    Draw_Frame();

    // Activation des transformations locales pour l'objet
    glPushMatrix();  // Sauvegarde l'état actuel de la matrice

    // Applique le zoom à l'objet
    glScalef(Zoom, Zoom, Zoom);

    // Dessine la boîte avec des lignes plus épaisses
    glLineWidth(5.0);
    Draw_Box();

    // Réactive les lumières pour l'objet
    glEnable(GL_LIGHTING);

    // Restaure la matrice précédente
    glPopMatrix();

    // Mise à jour du viewport
    glViewport(0, 0, WindowSize.width(), WindowSize.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calcul du ratio et projection orthographique
    GLfloat Ratio = (GLfloat)WindowSize.width() / (GLfloat)WindowSize.height();
    glOrtho((-0.5 + dx) * Ratio, (0.5 + dx) * Ratio, +0.5 + dy, -0.5 + dy, -1500.0, 1500.0);

    // Repasse en mode modèle vue pour continuer le rendu
    glMatrixMode(GL_MODELVIEW);

}

/*
 * ***********************************************************************************************

void ObjectOpenGL::Draw_Box()
{
    glPushMatrix();

    glRotated(angle_z , 0.0, 0.0, 1.0);
    glRotated(angle_y, 0.0, 1.0, 0.0);
    glRotated(-angle_x, 1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(4.,4.,4.);
    glPopMatrix();

    // Rear View
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(0,0,255,128)); // Dark Blue ; 128=Transparency
    glVertex3d(-L1 ,-L2   ,-L3);
    glVertex3d(-L1 ,L2    ,-L3);
    glVertex3d(L1  ,L2    ,0.0);
    glVertex3d(L1  ,-L2   ,0.0);
    glEnd();

    // Front View
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(51,255,0,128)); // Green
    glVertex3d(-L1 ,-L2   ,L3);
    glVertex3d(L1  ,-L2   ,0.0);
    glVertex3d(L1  ,L2    ,0.0);
    glVertex3d(-L1 ,L2    ,L3);
    glEnd();

    // Left View
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(255,255,0,128)); // yellow
    glVertex3d(-L1  ,-L2    ,L3);
    glVertex3d(-L1  ,L2     ,L3);
    glVertex3d(-L1  ,L2     ,-L3);
    glVertex3d(-L1  ,-L2    ,-L3);
    glEnd();

    // Top View
    glBegin(GL_TRIANGLES);
    qglColor(QColor::fromRgb(255,102,0,128)); // orange
    glVertex3d(-L1  ,L2    ,L3);
    glVertex3d(L1   ,L2     ,0.0);
    glVertex3d(-L1  ,L2    ,-L3);
    glEnd();


    // Bottom View
    glBegin(GL_TRIANGLES);
    qglColor(QColor::fromRgb(0,255,255,128)); // light blue
    glVertex3d(-L1  ,-L2    ,L3);
    glVertex3d(-L1  ,-L2    ,-L3);
    glVertex3d(L1   ,-L2     ,0.0);
    glEnd();

    glPopMatrix();
}
*/

void ObjectOpenGL::Draw_Box()
{
    glPushMatrix();

    // Appliquer les rotations
    glRotated(angle_z, 0.0, 0.0, 1.0);
    glRotated(angle_y, 0.0, 1.0, 0.0);
    glRotated(-angle_x, 1.0, 0.0, 0.0);

    // Rear View (vue arrière)
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(30, 30, 30, 128)); // #1E1E1E (gris très foncé), semi-transparent
    glVertex3d(-L1, -L2, -L3);
    glVertex3d(-L1, L2, -L3);
    glVertex3d(L1, L2, 0.0);
    glVertex3d(L1, -L2, 0.0);
    glEnd();

    // Front View (vue avant)
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(40, 40, 40, 128)); // #282828 (gris foncé), semi-transparent
    glVertex3d(-L1, -L2, L3);
    glVertex3d(L1, -L2, 0.0);
    glVertex3d(L1, L2, 0.0);
    glVertex3d(-L1, L2, L3);
    glEnd();

    // Left View (vue gauche)
    glBegin(GL_POLYGON);
    qglColor(QColor::fromRgb(50, 205, 50, 128)); // #32CD32 (vert lime), semi-transparent
    glVertex3d(-L1, -L2, L3);
    glVertex3d(-L1, L2, L3);
    glVertex3d(-L1, L2, -L3);
    glVertex3d(-L1, -L2, -L3);
    glEnd();

    // Top View (vue du dessus)
    glBegin(GL_TRIANGLES);
    qglColor(QColor::fromRgb(255, 165, 0, 128)); // #FFA500 (orange), semi-transparent
    glVertex3d(-L1, L2, L3);
    glVertex3d(L1, L2, 0.0);
    glVertex3d(-L1, L2, -L3);
    glEnd();

    // Bottom View (vue du dessous)
    glBegin(GL_TRIANGLES);
    qglColor(QColor::fromRgb(135, 206, 250, 128)); // #87CEFA (light sky blue), semi-transparent
    glVertex3d(-L1, -L2, L3);
    glVertex3d(-L1, -L2, -L3);
    glVertex3d(L1, -L2, 0.0);
    glEnd();

    glPopMatrix();
}


// Draw the frame (X,Y and Z axis)
/*
 *************************************************************************************************
void ObjectOpenGL::Draw_Frame()
{
    glLineWidth(10.0);
    // X-axis
    glBegin(GL_LINES);
    qglColor(Axis_X_Color);
    glVertex3d(0,0,0);
    glVertex3d(0.25, 0, 0);
    glEnd();
    // Y-axis
    glBegin(GL_LINES);
    qglColor(Axis_Y_Color);
    glVertex3d(0,0,0);
    glVertex3d(0, 0.25, 0);
    glEnd();
    // Z-axis
    glBegin(GL_LINES);
    qglColor(Axis_Z_Color);
    glVertex3d(0,0,0);
    glVertex3d(0, 0, 0.25);
    glEnd();
}
**/

void ObjectOpenGL::Draw_Axis(const QColor &color, double x, double y, double z)
{
    glBegin(GL_LINES);
    qglColor(color);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(x, y, z);
    glEnd();
}
/*
 ***********************************************************************************************/
void ObjectOpenGL::Draw_Frame()
{
    glLineWidth(4.0);

    Draw_Axis(Axis_X_Color, 1.0, 0.0, 0.0);  // Axe X
    Draw_Axis(Axis_Y_Color, 0.0, 1.0, 0.0);  // Axe Y
    Draw_Axis(Axis_Z_Color, 0.0, 0.0, 1.0);  // Axe Z
}
// Standard views
/*
 *************************************************************************************************/
void ObjectOpenGL::FrontView()
{
    SetXRotation(0);
    SetYRotation(0);
    SetZRotation(0);
    Zoom=1;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::RearView()
{
    SetXRotation(0);
    SetYRotation(180*16);
    SetZRotation(0);
    Zoom=1;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::LeftView()
{
    SetXRotation(0);
    SetYRotation(90*16);
    SetZRotation(0);
    Zoom=1;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::RightView()
{
    SetXRotation(0);
    SetYRotation(-90*16);
    SetZRotation(0);
    Zoom=1;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::TopView()
{
    SetXRotation(-90*16);
    SetYRotation(0);
    SetZRotation(0);
    Zoom=0.5;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::BottomView()
{
    SetXRotation(90*16);
    SetYRotation(0);
    SetZRotation(0);
    Zoom=0.5;
    dx=dy=0;
}
/*
 *************************************************************************************************/
void ObjectOpenGL::IsometricView()
{
    SetXRotation(62*16);
    SetYRotation(0);
    SetZRotation(45*16);
    Zoom=0.5;
    dx=dy=0;
}

/*
 *************************************************************************************************/

void ObjectOpenGL::resizeGL(int width, int height)
{
    WindowSize=QSize(width,height);
}

/*
 *************************************************************************************************/
// OpenGL angles are contained in the interval [0 : 360*16]
// Normalize the angle in this interval
/*
void ObjectOpenGL::NormalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle >= 360 * 16)
        *angle -= 360 * 16;
}
*/

void ObjectOpenGL::NormalizeAngle(int *angle)
{
    const int angleLimit = 360 * 16; // Define a constant for angle limits

    // Adjust the angle if it's less than 0
    while (*angle < 0)
        *angle += angleLimit;

    // Adjust the angle if it's greater than or equal to the limit
    while (*angle >= angleLimit)
        *angle -= angleLimit;
}

/*
 *************************************************************************************************/

void ObjectOpenGL::SetXRotation(int angle)
{
    NormalizeAngle(&angle);
    if (angle != xRot)
    {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
/*
 *************************************************************************************************/
void ObjectOpenGL::SetYRotation(int angle)
{
    NormalizeAngle(&angle);
    if (angle != yRot)
    {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}
/*
 *************************************************************************************************/
void ObjectOpenGL::SetZRotation(int angle)
{
    NormalizeAngle(&angle);
    if (angle != zRot)
    {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}
/*
 *************************************************************************************************/
void ObjectOpenGL::mousePressEvent(QMouseEvent *event){
    // Right button  (Rotate)
    if(event->buttons()==Qt::RightButton)
        LastPos = event->pos();
    // Left button (Move)
    if(event->buttons()==Qt::LeftButton)
        LastPos = event->pos();
}
/*
 *************************************************************************************************/
// Wheel event -->  Change the Zoom
void ObjectOpenGL::wheelEvent(QWheelEvent *event)
{
    // Récupérer la valeur d'angle delta
    int delta = event->angleDelta().y(); // Utiliser angleDelta() pour obtenir le décalage vertical

    // Appliquer le zoom
    if (delta < 0) {
        Zoom /= 1 - (delta / 120.0) / 10.0; // Zoomer vers l'extérieur
    } else if (delta > 0) {
        Zoom *= 1 + (delta / 120.0) / 10.0; // Zoomer vers l'intérieur
    }

    // Accepter l'événement
    event->accept();
}
/*
 *************************************************************************************************/
void ObjectOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    // Left button : move
    if(event->buttons()==Qt::LeftButton)
    {
        // Compute the difference with the previous position and scale to [-0.5 ; 0.5]
        dx+= -(event->x() - LastPos.x() )/(double)WindowSize.width();
        dy+= -(event->y() - LastPos.y() )/(double)WindowSize.height();
        // Update the view according to the new position
        //        resizeGL(WindowSize.width(),WindowSize.height());
        LastPos = event->pos();
    }

    // Right button (Rotate)
    if(event->buttons()==Qt::RightButton)
    {
        // Get the difference with the previous position
        int dx_mouse = event->x() - LastPos.x();
        int dy_mouse = event->y() - LastPos.y();
        // Update the rotation
        SetXRotation(xRot - 4 * dy_mouse);
        SetYRotation(yRot + 4 * dx_mouse);

        // Memorize previous position
        LastPos = event->pos();
    }
}
/*
 *************************************************************************************************/
