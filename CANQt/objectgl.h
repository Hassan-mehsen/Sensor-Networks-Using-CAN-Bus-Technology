#ifndef OBJECTGL_H
#define OBJECTGL_H

#include <QGLWidget>
#include <QtGui>
#include <iostream>

// using namespace std;

class ObjectOpenGL : public QGLWidget
{
    Q_OBJECT

public:
    ObjectOpenGL(QWidget *parent = 0);                          // Constructor
    ~ObjectOpenGL();                                            // Destructor

    void setAngles(double anx, double any, double anz) { angle_x = anx; angle_y = any; angle_z = anz; }

public slots:
    void FrontView();                    // Standard view : front view
    void RearView();                     // Standard view : rear view
    void LeftView();                     // Standard view : left view
    void RightView();                    // Standard view : right view
    void TopView();                      // Standard view : top view
    void BottomView();                   // Standard view : bottom view
    void IsometricView();                // Standard view : isometric view

    void SetXRotation(int angle);        // Update the rotation around X
    void SetYRotation(int angle);        // Update the rotation around Y
    void SetZRotation(int angle);        // Update the rotation around Z

protected:
    void initializeGL();                     // Initialize OpenGL parameters
    void paintGL();                          // Redraw the scene
    void resizeGL(int width, int height);    // Resize the OpenGL scene
    void mousePressEvent(QMouseEvent *event); // Called on mouse press event
    void mouseMoveEvent(QMouseEvent *event);  // Called on mouse move event
    void wheelEvent(QWheelEvent *event);      // Called on mouse wheel event

signals:
    void xRotationChanged(int angle);        // Signals for new orientations
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void Draw_Axis(const QColor &color, double x, double y, double z);
    void Draw_Frame();                       // Draw the orthonormal frame
    void Draw_Box();
    void NormalizeAngle(int *angle);         // Normalize the angle between 0 and 360x16

    QColor BackGround_Color;                 // Color of the background
    QColor Axis_X_Color;                     // X axis's color
    QColor Axis_Y_Color;                     // Y axis's color
    QColor Axis_Z_Color;                     // Z axis's color
    QColor Points_Color;                     // Points color

    QSize WindowSize;                        // Size (in pixels) of the OpenGL window
    QPoint LastPos;                          // Last position of the mouse (in pixels)
    GLfloat dx;                              // X Translation (for display)
    GLfloat dy;                              // Y Translation (for display)
    GLfloat Zoom;                            // Zoom of the object
    int xRot;                                // Rotation around X
    int yRot;                                // Rotation around Y
    int zRot;                                // Rotation around Z

    // Euler angles
    double angle_x, angle_y, angle_z;
};

#endif // OBJECTGL_H
