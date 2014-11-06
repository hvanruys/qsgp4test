#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QVector3D>
#include <qmatrix4x4.h>

#include "qtle.h"
#include "qsgp4.h"
#include "qsgp4date.h"
#include "qeci.h"
#include "qtopocentric.h"
#include "qgeodetic.h"
#include "qobserver.h"
#include "qsgp4utilities.h"
#include "Vectors.h"
#include "Matrices.h"

#include <iostream>



void doList()
{
    QFile filein("SGP4-VER.TLE");
    if (!filein.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "no inputfile !";

    QFile fileout("out.txt");
    if (!fileout.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error open out.txt";

    QString qstr1, qstr2;
    QTextStream in(&filein);
    QTextStream out(&fileout);
    double tsince = 0.0;
    double startmfe, stopmfe, deltamin;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if( line.mid(0,1) == "1")
            qstr1 = line.mid(0, 69);

        if( line.mid(0,1) == "2")
        {
            QString str = line.mid(70);
            QStringList list = str.split( ' ', QString::SkipEmptyParts);
            startmfe = list.at(0).toDouble();
            stopmfe = list.at(1).toDouble();
            deltamin = list.at(2).toDouble();

            qstr2 = line.mid(0, 69);
            QTle tle( "Test", qstr1, qstr2, QTle::wgs72 );
            QSgp4 sgp4(tle);

            QEci eci;
            out << tle.satnum << " xx\n";
            if (startmfe != 0.0)
            {
                sgp4.getPosition( 0.0, eci );
                Vector3 pos = eci.GetPos();
                Vector3 vel = eci.GetVel();

                if(sgp4.m_tle.error == 0)
                {
                    out << QString(" %1").arg(0.0, 16, 'f', 8) << " " << QString("%1").arg(pos.x, 16, 'f', 8) << " " << QString("%1").arg(pos.y, 16, 'f', 8) << " " << QString("%1").arg(pos.z, 16, 'f', 8) << " " <<
                    QString("%1").arg(vel.x, 12, 'f', 9) << " " << QString("%1").arg(vel.y, 12, 'f', 9) << " " << QString("%1").arg(vel.z, 12, 'f', 9) << "\n";
                }

            }

            for ( tsince = startmfe; tsince <= stopmfe; tsince += deltamin )
            {
                sgp4.getPosition( tsince, eci );
                Vector3 pos = eci.GetPos();
                Vector3 vel = eci.GetVel();

                if(sgp4.m_tle.error == 0)
                {
                    out << QString(" %1").arg(tsince, 16, 'f', 8) << " " << QString("%1").arg(pos.x, 16, 'f', 8) << " " << QString("%1").arg(pos.y, 16, 'f', 8) << " " << QString("%1").arg(pos.z, 16, 'f', 8) << " " <<
                    QString("%1").arg(vel.x, 12, 'f', 9) << " " << QString("%1").arg(vel.y, 12, 'f', 9) << " " << QString("%1").arg(vel.z, 12, 'f', 9) << "\n";
                }
                else
                {
                    out << sgp4.m_tle.errormsg << "\n";
                    break;
                }
            }

        }
    }

    fileout.flush();
    filein.close();
    fileout.close();
    qDebug() << "Finished";

}


int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);

    doList();

    return app.exec();
}

