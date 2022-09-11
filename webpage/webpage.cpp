#include "webpage.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include "webpage/scanner.h"
#include "webpage/token.h"
#include "parser.h"
#include "resolver.h"

Webpage::Webpage(QWidget * parent) : QOpenGLWidget(parent),
    clearColor(Qt::black)
{
    widgets = new QMap<QString,Widget*>();
    interpreter = new Interpreter(widgets);
}

void Webpage::runFile(QString path){
    QString sCode;

    sCode += getImports(path,sCode);

    run(sCode);
}

QString Webpage::getImports(QString path, QString sCode){
    QString content;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        content += stream.readAll();
    }
    file.close();
    content = deleteSpaces(content);
    int startCode = 0;
    if(content.startsWith("import")) {
        startCode = content.indexOf(';');
        QStringList importPaths = content.mid(6, startCode - 6).replace(" ", "").split(",");
        for (int i = 0; i < importPaths.size(); i++) {
            sCode += getImports(prjPath + importPaths[i],sCode);
        }

        startCode++;
    }


    sCode += content.mid(startCode);
    return sCode;
}

QString Webpage::deleteSpaces(QString content) {
    while(content.at(0) == ' ')
        content = content.mid(1);
    return content;
}

void Webpage::run(QString source){
    Scanner scanner(source);
    QList<Token> tokens = scanner.scanTokens();

    Parser* parser = new Parser(tokens);
    QList<Stmt*> statements = parser->parse();

    // Stop if there was a syntax error.
    //if (hadError)
    //    return;

    Resolver* resolver = new Resolver(interpreter);
    resolver->resolve(&statements);

    //if (hadError)
    //    return;

    interpreter->interpret(statements);

}

void Webpage::error(int line, QString message) {
    Webpage::report(line, QString(""), message);
}

void Webpage::report(int line, QString where, QString message) {
    qDebug() << "[line " << line << "] Error" << where << ": " << message;
}

Webpage::~Webpage()
{
    makeCurrent();
    doneCurrent();
}

QSize Webpage::minimumSizeHint() const
{
    return QSize(500, 400);
}

QSize Webpage::sizeHint() const
{
    return QSize(2000, 2000);
}


void Webpage::setClearColor(const QColor &color)
{
    clearColor = color;

}

void Webpage::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);

    QString path("/home/fabian/Spider/lib/script");

    runFile(path);
}

void Webpage::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (Widget * widget : widgets->values()) {
        widget->draw(width,height);
    }

}

void Webpage::resizeGL(int width, int height)
{
    glViewport(0,0, width, height);
    this->width = width;
    this->height = height;
}

void Webpage::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        int x = event->pos().x();
        int y = event->pos().y();

        for (Widget * widget : widgets->values()) {
            widget->mousePress(x,y);
        }
    }
}

void Webpage::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

    }
}

void Webpage::mouseReleaseEvent(QMouseEvent *event)
{

}
