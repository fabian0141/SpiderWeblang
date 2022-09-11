#include "widget.h"

Widget::Widget(QList<Object *> arguments, Interpreter* interpreter) : arguments(arguments), interpreter(interpreter)
{
    color.setRgb(255,255,255);
    static const int coords[4][3] = { { +1, 0, 0 }, { 0, 0, 0 }, { 0, +1, 0 }, { +1, +1, 0 } };
    QVector<GLfloat> vertData;
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertData.append(0.2 * coords[j][0]);
        vertData.append(0.2 * coords[j][1]);
        vertData.append(0.2 * coords[j][2]);
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }
    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex);
        const char *vsrc =
            "#version 110\n"
            "attribute highp vec4 vertex;\n"

            "attribute mediump vec4 texCoord;\n"
            "varying mediump vec4 texc;\n"

            "uniform mediump mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    texc = texCoord;\n"
            "}\n";
        vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment);
        const char *fsrc =
            "#version 130\n"
            "varying mediump vec4 texc;\n"
            "uniform vec4 color;"
            "uniform int w;"
            "uniform int h;"
            "uniform int height;"
            "uniform int r;"
            "uniform int x;"
            "uniform int y;"
            "uniform bool foc;"

            "void main( void )"
            "{"
            //"   gl_FragColor = vec4(1.);"
            "    vec2 sc = vec2(gl_FragCoord.x,height - gl_FragCoord.y);"
            "    float b = length(max(abs(sc - vec2(x + w / 2,y + h / 2)) - vec2(w / 2 - r,h / 2 - r),vec2(0.)));"
            "    float e = length(max(abs(sc - vec2(w / 2,h / 2)) - vec2(82,22),vec2(0.)));"
            "    vec4 d = mix( vec4(0.0,0,0,0.0), vec4(0.0), smoothstep(0.0,1000.0,e));"
            "    vec4 c = mix(color, vec4(0.0), smoothstep(r - 1,r + 1,b));"
            "    if(!foc){ "
            "        gl_FragColor = c;"
            "    }else{"
            "    float g = length(max(abs(sc - vec2(x + w / 2,y + h / 2)) - vec2(w / 2 - 6,h / 2 - 6),-3.5));"
            "    vec4 f = mix(vec4(0.2,0.8,0.0,1.), c, smoothstep(3,4,g));"
            "    gl_FragColor = f;"
            "    }"
            "}";
        fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);


    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    program->link();
}

void Widget::draw(int w, int h)
{
    program->bind();
    program->setUniformValue("height", h);
    program->setUniformValue("h", height);
    program->setUniformValue("w", width);
    program->setUniformValue("r", 5);
    program->setUniformValue("color", *backgroundColor);
    program->setUniformValue("x", x);
    program->setUniformValue("y", y);
    program->setUniformValue("foc", false);

    QMatrix4x4 m;
    m.ortho(0, 0.2f * w / width,0.2f * h / height,0, -1.0f, 1.0f);
    m.translate(0.2 / width * x, 0.2 / height * y, -0.1f);

    program->setUniformValue("matrix", m);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Widget::mousePress(int xPos, int yPos)
{
    if(x < xPos && x + width > xPos && y < yPos && y + height > yPos){
        if(onClick != NULL) onClick->call(interpreter,QList<Object*>());
    }
}

void Widget::mouseMove(int x, int y)
{

}

void Widget::mouseRelease(int x, int y)
{

}

