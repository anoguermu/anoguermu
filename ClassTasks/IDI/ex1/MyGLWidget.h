#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual void initializeGL ();
    virtual bool checkPosition(glm::vec3 pos);
    virtual void TerraTransform();
    virtual void paintGL ();
    virtual void PipeTransform (glm::vec3 pos);
    virtual void iniEscena ();
    virtual void SnakeHeadTransform ();
    virtual void SnakeTailTransform ();
    virtual void MarbleTransform ();
    virtual void iniCamera();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void resizeGL (int w, int h);
    virtual void mouseMoveEvent(QMouseEvent *e);
  public slots:
    virtual void animar();

  private:
    int printOglError(const char file[], int line, const char func[]);
    bool cort=false;
    QTimer timer;
    bool timeron=false;
    float canviy;
    float canvix;
};
