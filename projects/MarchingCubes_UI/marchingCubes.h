#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <qopengl.h>
#include <QObject>
#include <QVector>
#include <QVector3D>

class MarchingCubes : public QObject
{
    Q_OBJECT

public:
    MarchingCubes(QObject* parent = nullptr);

    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }

public slots:
    void setPathAndCalculate(const QStringList& list);

signals:
    void marchingCubesFinished(const QVector<GLfloat>& data, int count);

private:
    void add(const QVector3D &v, const QVector3D &n);
    float interpol(int sourad1, int sourad2, float val1, float val2, int prah);

    QVector<GLfloat> m_data;
    int m_count = 0;
};

#endif // MARCHINGCUBES_H
