#ifndef LEVEL_H
#define LEVEL_H

//#include <QtOpenGL>
//#include <QMap>
//#include <QImage>

#include "obstacle.h"
#include "vector3f.h"


/**
 * @brief
 *
 */
class Level : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param _obstacleModelsList
     * @param parent
     */
    explicit Level(QMap<GLint,Vector3f*> &_obstacleModelsList, QObject *_parent = 0);

    /**
     * @brief
     *
     * @param _id
     * @param _filename
     * @param _parent
     */
    explicit Level(GLint _id, QString _filename, QObject *_parent = 0);

    /**
     * @brief
     *
     */
    ~Level();

    /**
     * @brief
     *
     */
    GLint getId();

    /**
     * @brief
     *
     */
    QString getFilename();

    /**
     * @brief
     *
     * @param _filename
     */
    void setFilename(QString _filename);

    /**
     * @brief
     *
     */
    QString getName();

    /**
     * @brief
     *
     * @param _name
     */
    void setName(QString _name);

    /**
     * @brief
     *
     */
    bool getIsInStory();

    /**
     * @brief
     *
     */
    GLfloat getWidth();

    /**
     * @brief
     *
     */
    GLfloat getLength();

    /**
     * @brief
     *
     */
    GLfloat getGravity();

    /**
     * @brief
     *
     */
    QString getAmbientMusicFilename();

    /**
     * @brief
     *
     * @param _obstacle
     */
    void addObstacle(Obstacle *_obstacle);

    /**
     * @brief
     *
     * @param _id
     */
    void deleteObstacle(GLint _id);

    /**
     * @brief
     *
     * @param _id
     * @param newPosition
     */
    void moveObstacle(GLint _id, Vector3f *newPosition);

    /**
     * @brief
     *
     * @param filename
     */
    bool load();

    /**
     * @brief
     *
     * @param filename
     */
    bool save();

    /**
     * @brief
     *
     */
    void draw();

private:
    GLint id; /**< TODO */
    QString filename; /**< TODO */
    QObject *parent; /**< TODO */
    QString name; /**< TODO */
    bool isLoaded; /**< TODO */
    bool isInStory; /**< TODO */
    QMap<GLint,Vector3f*> obstacleModelsList; /**< TODO */
    QMap<GLint,Obstacle*> obstaclesList; /**< TODO */
    GLint width; /**< TODO */
    GLint length; /**< TODO */
    GLfloat gravity; /**< TODO */
    QString ambientMusicFilename; /**< TODO */
    QImage *background; /**< TODO */
};

#endif // LEVEL_H
