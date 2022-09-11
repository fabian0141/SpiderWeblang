#ifndef WIDGET_H
#define WIDGET_H

#include <QColor>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include"object.h"
#include"interpreter.h"

class Widget
{
public:
    Widget(QList<Object *> arguments, Interpreter* interpreter);
    void draw(int, int);

    void mousePress(int, int);
    void mouseMove(int, int);
    void mouseRelease(int, int);

    QList<Object *> arguments;
    Interpreter* interpreter;

    QString id;
    int x = 0,y = 0, z = 0;
    int width = 20,height = 20;
    QColor* backgroundColor = new QColor("#FFFFFFFF");
    QString backgroundImage = "";
    LoxCallable* onClick = NULL;
    LoxCallable* onHover = NULL;
    LoxCallable* onLeave = NULL;
    QString text = "";
    QColor* textColor = new QColor("#FFFFFFFF");
    double textX = 0;
    double textY = 0;
    double borderWidth = 0;
    QColor* borderColor = new QColor("#FFFFFFFF");
    double borderRadius = 0;
    double shadowWidth = 0;
    QColor* shadowColor = new QColor("#FFFFFFFF");
    double shadowX = 0;
    double shadowY = 0;

private:
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    QColor color;
};

#endif // WIDGET_H
