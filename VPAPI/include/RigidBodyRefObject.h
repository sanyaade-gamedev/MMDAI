/**

 Copyright (c) 2010-2014  hkrn

 All rights reserved.

 Redistribution and use in source and binary forms, with or
 without modification, are permitted provided that the following
 conditions are met:

 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided
   with the distribution.
 - Neither the name of the MMDAI project team nor the names of
   its contributors may be used to endorse or promote products
   derived from this software without specific prior written
   permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef RIGIDBODYREFOBJECT_H
#define RIGIDBODYREFOBJECT_H

#include <QJsonValue>
#include <QObject>
#include <QUuid>
#include <QVector3D>
#include <vpvl2/IRigidBody.h>

class BoneRefObject;
class ModelProxy;

class RigidBodyRefObject : public QObject
{
    Q_OBJECT
    Q_ENUMS(ObjectType)
    Q_ENUMS(ShapeType)
    Q_PROPERTY(ModelProxy *parentModel READ parentModel CONSTANT FINAL)
    Q_PROPERTY(QUuid uuid READ uuid CONSTANT FINAL)
    Q_PROPERTY(int index READ index CONSTANT FINAL)
    Q_PROPERTY(BoneRefObject *parentBone READ parentBone WRITE setParentBone NOTIFY parentBoneChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)
    Q_PROPERTY(QVector3D degreeRotation READ degreeRotation WRITE setDegreeRotation NOTIFY rotationChanged FINAL)
    Q_PROPERTY(ObjectType objectType READ objectType WRITE setObjectType NOTIFY objectTypeChanged FINAL)
    Q_PROPERTY(ShapeType shapeType READ shapeType WRITE setShapeType NOTIFY shapeTypeChanged FINAL)
    Q_PROPERTY(qreal mass READ mass WRITE setMass NOTIFY massChanged FINAL)
    Q_PROPERTY(qreal linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged FINAL)
    Q_PROPERTY(qreal angularDamping READ angularDamping WRITE setAngularDamping NOTIFY angularDampingChanged FINAL)
    Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY frictionChanged FINAL)
    Q_PROPERTY(qreal restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged FINAL)
    Q_PROPERTY(quint16 collisionGroupMask READ collisionGroupMask WRITE setCollisionGroupMask NOTIFY collisionGroupMaskChanged FINAL)
    Q_PROPERTY(quint8 collisionGroupID READ collisionGroupID WRITE setCollisionGroupID NOTIFY collisionGroupIDChanged FINAL)
    Q_PROPERTY(bool dirty READ isDirty NOTIFY dirtyChanged FINAL)

public:
    enum ObjectType {
        StaticObject  = vpvl2::IRigidBody::kStaticObject,
        DynamicObject = vpvl2::IRigidBody::kDynamicObject,
        AlignedObject = vpvl2::IRigidBody::kAlignedObject
    };
    enum ShapeType {
        SphereShape  = vpvl2::IRigidBody::kSphereShape,
        BoxShape     = vpvl2::IRigidBody::kBoxShape,
        CapsureShape = vpvl2::IRigidBody::kCapsureShape
    };

    RigidBodyRefObject(ModelProxy *parentModelRef,
                       vpvl2::IRigidBody *rigidBodyRef,
                       const QUuid &uuid);
    ~RigidBodyRefObject();

    Q_INVOKABLE QJsonValue toJson() const;

    vpvl2::IRigidBody *data() const;
    ModelProxy *parentModel() const;
    QUuid uuid() const;
    int index() const;
    BoneRefObject *parentBone() const;
    void setParentBone(BoneRefObject *value);
    QString name() const;
    void setName(const QString &value);
    QVector3D position() const;
    void setPosition(const QVector3D &value);
    QVector3D rotation() const;
    void setRotation(const QVector3D &value);
    QVector3D degreeRotation() const;
    void setDegreeRotation(const QVector3D &value);
    ObjectType objectType() const;
    void setObjectType(const ObjectType &value);
    ShapeType shapeType() const;
    void setShapeType(const ShapeType &value);
    qreal mass() const;
    void setMass(const qreal &value);
    qreal linearDamping() const;
    void setLinearDamping(const qreal &value);
    qreal angularDamping() const;
    void setAngularDamping(const qreal &value);
    qreal friction() const;
    void setFriction(const qreal &value);
    qreal restitution() const;
    void setRestitution(const qreal &value);
    quint16 collisionGroupMask() const;
    void setCollisionGroupMask(const quint16 &value);
    quint8 collisionGroupID() const;
    void setCollisionGroupID(const quint8 &value);
    bool isDirty() const;
    void setDirty(bool value);

signals:
    void parentBoneChanged();
    void nameChanged();
    void positionChanged();
    void rotationChanged();
    void objectTypeChanged();
    void shapeTypeChanged();
    void massChanged();
    void linearDampingChanged();
    void angularDampingChanged();
    void frictionChanged();
    void restitutionChanged();
    void collisionGroupMaskChanged();
    void collisionGroupIDChanged();
    void dirtyChanged();

private:
    ModelProxy *m_parentModelRef;
    vpvl2::IRigidBody *m_rigidBodyRef;
    const QUuid m_uuid;
    bool m_dirty;
};

#endif
