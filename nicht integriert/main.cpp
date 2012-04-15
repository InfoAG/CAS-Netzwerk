#include <QApplication>
#include "matrix.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    matrix Matrix;
    Matrix.show();

    return app.exec();
}
