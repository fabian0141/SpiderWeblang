#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "webpage/tokentype.h"
#include "interpreter.h"
#include "widget.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class Webpage : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Webpage(QWidget *parent = 0);
    ~Webpage();
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setClearColor(const QColor &color);

    static void error(int line, QString message);
    QString getImports(QString path, QString file);
    QString deleteSpaces(QString content);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    QColor clearColor;
    void runFile(QString path);
    void run(QString source);
    static void report(int line, QString, QString message);
    QMap<QString,Widget*>* widgets;
    Interpreter* interpreter;
    int width,height;
    QString prjPath = "/home/fabian/Spider/lib/";
};

#endif // WEBPAGE_H
