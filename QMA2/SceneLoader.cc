#include "SceneLoader.h"
#include "VPDFile.h"
#include "util.h"

#include <vpvl/vpvl.h>
#include <vpvl/gl/Renderer.h>

SceneLoader::SceneLoader(vpvl::gl::Renderer *renderer)
    : m_renderer(renderer),
      m_camera(0)
{
}

SceneLoader::~SceneLoader()
{
    delete m_camera;
    m_camera = 0;
    foreach (vpvl::VMDMotion *motion, m_motions) {
        vpvl::PMDModel *model = m_motions.key(motion);
        model->removeMotion(motion);
        delete motion;
    }
    foreach (vpvl::PMDModel *model, m_models) {
        m_renderer->unloadModel(model);
        delete model;
    }
    m_models.clear();
    m_motions.clear();
    m_assets.clear();
    m_modelLocation.clear();
    m_motionLocation.clear();
}

SceneLoader *SceneLoader::clone(vpvl::gl::Renderer *renderer)
{
    SceneLoader *loader = new SceneLoader(renderer);
    loader->m_modelLocation = m_modelLocation;
    loader->m_motionLocation = m_motionLocation;
    QFileInfo info;
    foreach (const QString &location, m_modelLocation) {
        info.setFile(location);
        vpvl::VMDMotion *nullMotion = 0;
        vpvl::PMDModel *model = loader->loadModel(info.fileName(), info.dir(), nullMotion);
        if (model) {
            vpvl::PMDModel *oldModel = m_modelLocation.key(location);
            vpvl::VMDMotion *modelMotion = m_motions.value(oldModel);
            const QString &path = m_motionLocation.value(modelMotion);
            if (!path.isEmpty()) {
                loader->loadModelMotion(path, model);
            }
            else {
                // loader->setModelMotion(modelMotion->clone(), model, QString());
                loader->setModelMotion(nullMotion, model, QString());
            }
        }
    }
    return loader;
}

bool SceneLoader::deleteModel(vpvl::PMDModel *model)
{
    const QString &key = m_models.key(model);
    if (!key.isNull()) {
        m_renderer->unloadModel(model);
        m_renderer->scene()->removeModel(model);
        m_models.remove(key);
        delete model;
        m_renderer->setSelectedModel(0);
        return true;
    }
    return false;
}

vpvl::PMDModel *SceneLoader::findModel(const QString &name) const
{
    return m_models.value(name);
}

vpvl::VMDMotion *SceneLoader::findModelMotion(vpvl::PMDModel *model) const
{
    return m_motions.value(model);
}

vpvl::XModel *SceneLoader::loadAsset(const QString &baseName, const QDir &dir)
{
    QFile file(dir.absoluteFilePath(baseName));
    vpvl::XModel *model = 0;
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        model = new vpvl::XModel();
        if (model->load(reinterpret_cast<const uint8_t *>(data.constData()), data.size())) {
            QString key = baseName;
            if (m_assets.contains(key)) {
                int i = 0;
                while (true) {
                    QString tmpKey = QString("%1%2").arg(key).arg(i);
                    if (!m_assets.contains(tmpKey))
                        key = tmpKey;
                    i++;
                }
            }
            m_renderer->loadAsset(model, std::string(dir.absolutePath().toUtf8()));
            m_assets[key] = model;
        }
        else {
            delete model;
            model = 0;
        }
    }
    return model;
}

vpvl::VMDMotion *SceneLoader::loadCameraMotion(const QString &path)
{
    QFile file(path);
    vpvl::VMDMotion *motion = 0;
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        motion = new vpvl::VMDMotion();
        if (motion->load(reinterpret_cast<const uint8_t *>(data.constData()), data.size())) {
            delete m_camera;
            m_camera = motion;
            m_renderer->scene()->setCameraMotion(motion);
        }
        else {
            delete motion;
            motion = 0;
        }
    }
    return motion;
}

vpvl::PMDModel *SceneLoader::loadModel(const QString &baseName, const QDir &dir, vpvl::VMDMotion *&nullMotion)
{
    const QString &path = dir.absoluteFilePath(baseName);
    QFile file(path);
    vpvl::PMDModel *model = 0;
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        model = new vpvl::PMDModel();
        if (model->load(reinterpret_cast<const uint8_t *>(data.constData()), data.size())) {
            m_renderer->loadModel(model, std::string(dir.absolutePath().toUtf8()));
            m_renderer->scene()->addModel(model);
            QString key = internal::toQString(model);
            qDebug() << key << baseName;
            if (m_models.contains(key)) {
                int i = 0;
                while (true) {
                    QString tmpKey = QString("%1%2").arg(key).arg(i);
                    if (!m_models.contains(tmpKey))
                        key = tmpKey;
                    i++;
                }
            }
            nullMotion = new vpvl::VMDMotion();
            nullMotion->setEnableSmooth(false);
            model->addMotion(nullMotion);
            insertModel(model, key, path);
            insertMotion(nullMotion, model, QString());
            // force to render an added model
            m_renderer->scene()->seek(0.0f);
        }
        else {
            delete model;
            model = 0;
        }
    }
    return model;
}

vpvl::VMDMotion *SceneLoader::loadModelMotion(const QString &path)
{
    QFile file(path);
    vpvl::VMDMotion *motion = 0;
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        motion = new vpvl::VMDMotion();
        if (!motion->load(reinterpret_cast<const uint8_t *>(data.constData()), data.size())) {
            delete motion;
            motion = 0;
        }
    }
    return motion;
}

vpvl::VMDMotion *SceneLoader::loadModelMotion(const QString &path, QList<vpvl::PMDModel *> &models)
{
    vpvl::VMDMotion *motion = loadModelMotion(path);
    if (motion) {
        foreach (vpvl::PMDModel *model, m_models) {
            setModelMotion(motion, model, path);
            models.append(model);
        }
    }
    return motion;
}

vpvl::VMDMotion *SceneLoader::loadModelMotion(const QString &path, vpvl::PMDModel *model)
{
    vpvl::VMDMotion *motion = loadModelMotion(path);
    if (motion)
        setModelMotion(motion, model, path);
    return motion;
}

VPDFile *SceneLoader::loadPose(const QString &path, vpvl::PMDModel * /* model */)
{
    QFile file(path);
    VPDFile *pose = 0;
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        pose = new VPDFile();
        if (pose->load(stream)) {
            // pose->makePose(model);
        }
        else {
            delete pose;
            pose = 0;
        }
    }
    return pose;
}

void SceneLoader::setModelMotion(vpvl::VMDMotion *motion, vpvl::PMDModel *model, const QString &location)
{
    motion->setEnableSmooth(false);
    model->addMotion(motion);
    if (m_motions.contains(model)) {
        vpvl::VMDMotion *oldMotion = m_motions.value(model);
        model->removeMotion(oldMotion);
        delete oldMotion;
    }
    insertMotion(motion, model, location);
}

void SceneLoader::insertModel(vpvl::PMDModel *model, const QString &name, const QString &location)
{
    m_models.insert(name, model);
    m_modelLocation.insert(model, location);
}

void SceneLoader::insertMotion(vpvl::VMDMotion *motion, vpvl::PMDModel *model, const QString &location)
{
    m_motions.insert(model, motion);
    m_motionLocation.insert(motion, location);
}
