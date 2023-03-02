#include "marchingCubes.h"
#include <qmath.h>
#include <QImage>
#include <QDir>
#include <QDebug>
#include "MCTable.h"

MarchingCubes::MarchingCubes(QObject* parent)
    : QObject(parent)
{

}

void MarchingCubes::setPathAndCalculate(const QStringList &list, int threshold)
{
    m_data.clear();
    m_count = 0;

    // Cube vertexes
    // Pixel values
    float bod1, bod2, bod3, bod4, bod5, bod6, bod7, bod8;
    // Binary pixel values based on thrsh 0/1
    int bod1_log, bod2_log, bod3_log, bod4_log, bod5_log, bod6_log, bod7_log, bod8_log;
    // Vertex position
    int sour1_x, sour1_y, sour1_z, sour2_x, sour2_y, sour2_z, sour3_x, sour3_y, sour3_z,
        sour4_x, sour4_y, sour4_z, sour5_x, sour5_y, sour5_z, sour6_x, sour6_y, sour6_z,
        sour7_x, sour7_y, sour7_z, sour8_x, sour8_y, sour8_z;

    // Additional var
    int pripad;
    int hrana;

    // Interpolated edge position
    float vertsx[12] = {};
    float vertsy[12] = {};
    float vertsz[12] = {};

    // Loading images in cycle (load 2 images at the same time)
    for (int k = 0; k < list.count() - 1; k++) {
        QImage obrazek;
        QImage obrazek1;

        // Load images and check validity
        if (!obrazek.load(list.at(k))) {
            qWarning() << "image" << list.at(k) << " not found";
            return;
        }
        if (!obrazek1.load(list.at(k + 1))) {
            qWarning() << "image " << list.at(k + 1) << " not found";
            return;
        }
        if (obrazek.size() != obrazek1.size())
            return;

        // Iterate throw pixels of 2 images
        QSize velikost = obrazek.size();
        for (int i = 0; i < (int)velikost.width() - 1; i++) {
            for (int j = 0; j < (int)velikost.height() - 1; j++) {

                // Vertex coordinates
                sour1_x = i + 1;        sour1_y = j + 1;        sour1_z = k;
                sour2_x = i;            sour2_y = j + 1;        sour2_z = k;
                sour3_x = i;            sour3_y = j;            sour3_z = k;
                sour4_x = i + 1;        sour4_y = j;            sour4_z = k;
                sour5_x = i+ 1;         sour5_y = j +1;         sour5_z = k + 1;
                sour6_x = i;            sour6_y = j + 1;        sour6_z = k + 1;
                sour7_x = i ;           sour7_y = j;            sour7_z = k + 1;
                sour8_x = i + 1;        sour8_y = j;            sour8_z = k + 1;

                // Vertex values
                bod1 = qGray(obrazek.pixel(sour1_x, sour1_y));
                bod2 = qGray(obrazek.pixel(sour2_x, sour2_y));
                bod3 = qGray(obrazek.pixel(sour3_x, sour3_y));
                bod4 = qGray(obrazek.pixel(sour4_x, sour4_y));
                bod5 = qGray(obrazek1.pixel(sour5_x, sour5_y));
                bod6 = qGray(obrazek1.pixel(sour6_x, sour6_y));
                bod7 = qGray(obrazek1.pixel(sour7_x, sour7_y));
                bod8 = qGray(obrazek1.pixel(sour8_x, sour8_y));

                // Logical binding of values
                bod1_log = (bod1 > threshold);
                bod2_log = (bod2 > threshold);
                bod3_log = (bod3 > threshold);
                bod4_log = (bod4 > threshold);
                bod5_log = (bod5 > threshold);
                bod6_log = (bod6 > threshold);
                bod7_log = (bod7 > threshold);
                bod8_log = (bod8 > threshold);

                // Edge combination
                pripad = bod1_log * 1 + bod2_log * 2 + bod3_log * 4 + bod4_log * 8 + bod5_log * 16 + bod6_log * 32 + bod7_log * 64 + bod8_log * 128;
                hrana = edgeTable[pripad];

                // Skip cases when all pixels are 0/1
                if (!edgeTable[hrana]) continue;

                // Linear interpolation of edges
                if (hrana & 1) {
                    vertsx[0] = interpol(sour1_x, sour2_x, bod1, bod2, threshold);
                    vertsy[0] = interpol(sour1_y, sour2_y, bod1, bod2, threshold);
                    vertsz[0] = interpol(sour1_z, sour2_z, bod1, bod2, threshold);
                }
                if (hrana & 2) {
                    vertsx[1] = interpol(sour2_x, sour3_x, bod2, bod3, threshold);
                    vertsy[1] = interpol(sour2_y, sour3_y, bod2, bod3, threshold);
                    vertsz[1] = interpol(sour2_z, sour3_z, bod2, bod3, threshold);
                }
                if (hrana & 4) {
                    vertsx[2] = interpol(sour3_x, sour4_x, bod3, bod4, threshold);
                    vertsy[2] = interpol(sour3_y, sour4_y, bod3, bod4, threshold);
                    vertsz[2] = interpol(sour3_z, sour4_z, bod3, bod4, threshold);
                }
                if (hrana & 8) {
                    vertsx[3] = interpol(sour4_x, sour1_x, bod4, bod1, threshold);
                    vertsy[3] = interpol(sour4_y, sour1_y, bod4, bod1, threshold);
                    vertsz[3] = interpol(sour4_z, sour1_z, bod4, bod1, threshold);
                }
                if (hrana & 16) {
                    vertsx[4] = interpol(sour5_x, sour6_x, bod5, bod6, threshold);
                    vertsy[4] = interpol(sour5_y, sour6_y, bod5, bod6, threshold);
                    vertsz[4] = interpol(sour5_z, sour6_z, bod5, bod6, threshold);
                }
                if (hrana & 32) {
                    vertsx[5] = interpol(sour6_x, sour7_x, bod6, bod7, threshold);
                    vertsy[5] = interpol(sour6_y, sour7_y, bod6, bod7, threshold);
                    vertsz[5] = interpol(sour6_z, sour7_z, bod6, bod7, threshold);
                }
                if (hrana & 64) {
                    vertsx[6] = interpol(sour7_x, sour8_x, bod7, bod8, threshold);
                    vertsy[6] = interpol(sour7_y, sour8_y, bod7, bod8, threshold);
                    vertsz[6] = interpol(sour7_z, sour8_z, bod7, bod8, threshold);
                }
                if (hrana & 128) {
                    vertsx[7] = interpol(sour8_x, sour5_x, bod8, bod5, threshold);
                    vertsy[7] = interpol(sour8_y, sour5_y, bod8, bod5, threshold);
                    vertsz[7] = interpol(sour8_z, sour5_z, bod8, bod5, threshold);
                }
                if (hrana & 256) {
                    vertsx[8] = interpol(sour1_x, sour5_x, bod1, bod5, threshold);
                    vertsy[8] = interpol(sour1_y, sour5_y, bod1, bod5, threshold);
                    vertsz[8] = interpol(sour1_z, sour5_z, bod1, bod5, threshold);
                }
                if (hrana & 512) {
                    vertsx[9] = interpol(sour2_x, sour6_x, bod2, bod6, threshold);
                    vertsy[9] = interpol(sour2_y, sour6_y, bod2, bod6, threshold);
                    vertsz[9] = interpol(sour2_z, sour6_z, bod2, bod6, threshold);
                }
                if (hrana & 1024) {
                    vertsx[10] = interpol(sour3_x, sour7_x, bod3, bod7, threshold);
                    vertsy[10] = interpol(sour3_y, sour7_y, bod3, bod7, threshold);
                    vertsz[10] = interpol(sour3_z, sour7_z, bod3, bod7, threshold);
                }
                if (hrana & 2048) {
                    vertsx[11] = interpol(sour4_x, sour8_x, bod4, bod8, threshold);
                    vertsy[11] = interpol(sour4_y, sour8_y, bod4, bod8, threshold);
                    vertsz[11] = interpol(sour4_z, sour8_z, bod4, bod8, threshold);
                }

                // Calculates vertexes of possible triangles (based on triTable, max 5 triangles)
                for (int n = 0; triTable[pripad][n] != -1; n += 3) {
                    //1. vrchol
                    QVector3D first(vertsx[triTable[pripad][n + 2]] / velikost.width(),
                                    vertsy[triTable[pripad][n + 2]] / velikost.height(),
                                    vertsz[triTable[pripad][n + 2]] / (float)list.count());

                    //2. vrchol
                    QVector3D second(vertsx[triTable[pripad][n + 1]] / velikost.width(),
                                     vertsy[triTable[pripad][n + 1]] / velikost.height(),
                                     vertsz[triTable[pripad][n + 1]] / (float)list.count() );

                    //3. vrchol
                    QVector3D third(vertsx[triTable[pripad][n]] / velikost.width(),
                                    vertsy[triTable[pripad][n]] / velikost.height(),
                                    vertsz[triTable[pripad][n]] / (float)list.count());

                    // Normal vectors (for illumination)
                    QVector3D normal = QVector3D::normal(first, second);

                    // Add data to buffer
                    add(first, normal);
                    add(second, normal);
                    add(third, normal);
                }
            }
        }
    }

    // Send data outside
    emit marchingCubesFinished(m_data, m_count);
}

void MarchingCubes::add(const QVector3D &v, const QVector3D &n)
{
    m_data.append(v.x());
    m_data.append(v.y());
    m_data.append(v.z());
    m_data.append(n.x());
    m_data.append(n.y());
    m_data.append(n.z());
    m_count += 6;
}

float MarchingCubes::interpol(int sourad1, int sourad2, float val1, float val2, int threshold)
{
    float p;
    if (val1 != val2)
        p = (float)sourad1 + ((float)sourad2 - (float)sourad1) / (val2 - val1) * ((float)threshold - val1);
    else
        p = (float)sourad1;
    return p;
}
