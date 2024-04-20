#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}
void MyGLWidget::animar(){
  makeCurrent();
  LL2GLWidget::updateSnakeGame();
  update();
}
void MyGLWidget::initializeGL (){
  LL2GLWidget::initializeGL();
  connect(&timer,SIGNAL(timeout()),this,SLOT(animar()));
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}
void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  ra = float(ample)/float(alt);
  projectTransform();
}
bool MyGLWidget::checkPosition(glm::vec3 pos)
{
    bool valid = true;
    if(pos[0]>=15 or pos[0]<=-15 or pos[2]>=15 or pos[2]<=-15)valid=false;
    else if(pos==tailPos)valid=false;
    for(auto i=0; i<bodyPos.size() and valid;++i){
      if(pos==bodyPos[i])valid=false;
    }

	return valid; 
}
void MyGLWidget::TerraTransform(){
  glm::mat4 TG(1.0f);
  TG=glm::scale(TG, glm::vec3(30/10));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL (){
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);

  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);

  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  //pinta les 2 verticals
  glBindVertexArray (VAO_models[PIPE]);

    for(int i=0; i<30;++i){
        PipeTransform(glm::vec3(-15+0.5+i,0,-15+0.5));
        glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
        PipeTransform(glm::vec3(-15+0.5+i,0,+15-0.5));
        glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
        PipeTransform(glm::vec3(-15+0.5,0,-15+0.5+i));
        glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
        PipeTransform(glm::vec3(15-0.5,0,-15+0.5+i));
        glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

    }

  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::PipeTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(1,3,1));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
 void MyGLWidget::iniEscena (){

  centreEscena = glm::vec3(0,0,0);

  headPos = glm::vec3(0,0,0);
  headAngle = 0;
  bodyPos.clear();
  bodyPos.push_back(glm::vec3(-1,0,0));
  tailPos = glm::vec3(-2,0,0);
  tailAngle = 0;

  marblePos = glm::vec3(10, 0, 0);

  direction = glm::vec3(1,0,0);

}

void MyGLWidget::SnakeHeadTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, headPos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG=glm::rotate(TG,float((headAngle-90)* M_PI/180 ),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::SnakeTailTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG=glm::rotate(TG,float(tailAngle* M_PI/180 ),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::MarbleTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]*0.5));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::projectTransform (){
  if(cort){
    glm::mat4 Proj(1.0f);
    float l,r,b,t;
    l=b=+15.0f;
    r=t=-15.0f;
    if(ra>1){
      l=l*ra;
      r=r*ra;
    }else if(ra<1){
      b=b/ra;
      t=t/ra;
    }
    Proj=glm::ortho(l,r,b,t,znear,zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }else{
    float angle=asin(radiEscena/(2*radiEscena));
    if(ra<1){

      fov=2*atan(tan(angle)/ra);
    }else fov=2*angle;
    LL2GLWidget::projectTransform ();
  }
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if(DoingInteractive==ROTATE){
    canvix-=(e->x()-xClick)*factorAngleX;
    canviy-=(e->y()-yClick)*factorAngleY;
    viewTransform();
  }
  xClick=e->x();
  yClick=e->y();
  update ();
}
void MyGLWidget::viewTransform(){

  if(cort){
    glm::mat4 View(1.0f);
    View = glm::lookAt(glm::vec3(0,2*radiEscena,0), vrp, glm::vec3(0,0,1) );
     glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }else{
    glm::mat4 View(1.0f);

    float d=glm::distance(obs,vrp);
    View = glm::translate(View, glm::vec3(0,0,-d));
    View=glm::rotate(View,canviy,glm::vec3(1,0,0));
    View=glm::rotate(View,canvix,glm::vec3(0,1,0));
    View = glm::translate(View, -vrp);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }

}
void MyGLWidget::iniCamera(){
  radiEscena = sqrt(30*30 + 30*30 + 3*3)/2;
  obs = glm::vec3(0, 0, 2*radiEscena);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 0);
  fov =2* glm::asin(radiEscena/(2*radiEscena));
  znear =  radiEscena;
  zfar  = 3*radiEscena;
  canvix=0;
  canviy=M_PI/4;
  viewTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      direction= glm::vec3(0,0,-1);
      if(not timeron)LL2GLWidget::updateSnakeGame();

      break;
    }
    case Qt::Key_Down: { 
      direction= glm::vec3(0,0,+1);
      if(not timeron)LL2GLWidget::updateSnakeGame();
      break;
        }
    case Qt::Key_Left: { 
      direction= glm::vec3(-1,0,0);
      if(not timeron)LL2GLWidget::updateSnakeGame();
      break;
        }  
    case Qt::Key_Right: { 
      direction= glm::vec3(1,0,0);
      if(not timeron)LL2GLWidget::updateSnakeGame();
      break;
        }
    case Qt::Key_C: { 
      cort= not cort;
      viewTransform();
      projectTransform ();
      break;
        }           
    case Qt::Key_R: { 
        if(timeron){
          timeron=false;
          timer.stop();
        }
        canvix=0;
        canviy=M_PI/4;
        iniEscena ();
        if(cort){
          cort=false;

          projectTransform ();
        }
        viewTransform();
      break;
        }
    case Qt::Key_T: {
      if(timeron){
        timer.stop();
        timeron=false;
      }else{
        timeron=true;
        timer.start(200);
      }

      break;
    }
    default: event->ignore(); break;
  }
  update();
}





