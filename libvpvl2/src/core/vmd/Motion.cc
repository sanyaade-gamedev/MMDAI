/**

 Copyright (c) 2009-2011  Nagoya Institute of Technology
                          Department of Computer Science
               2010-2014  hkrn

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

#include "vpvl2/vpvl2.h"
#include "vpvl2/internal/MotionHelper.h"

#include "vpvl2/vmd/BoneAnimation.h"
#include "vpvl2/vmd/BoneKeyframe.h"
#include "vpvl2/vmd/CameraAnimation.h"
#include "vpvl2/vmd/CameraKeyframe.h"
#include "vpvl2/vmd/LightAnimation.h"
#include "vpvl2/vmd/LightKeyframe.h"
#include "vpvl2/vmd/ModelAnimation.h"
#include "vpvl2/vmd/ModelKeyframe.h"
#include "vpvl2/vmd/MorphAnimation.h"
#include "vpvl2/vmd/MorphKeyframe.h"
#include "vpvl2/vmd/Motion.h"
#include "vpvl2/vmd/ProjectAnimation.h"
#include "vpvl2/vmd/ProjectKeyframe.h"

namespace vpvl2
{
namespace VPVL2_VERSION_NS
{
namespace vmd
{

static const float64 kFPS = 30.0f;

struct Motion::PrivateContext {
    PrivateContext(IModel *modelRef, IEncoding *encodingRef)
        : motionPtr(0),
          parentSceneRef(0),
          parentModelRef(modelRef),
          encodingRef(encodingRef),
          name(0),
          boneMotion(encodingRef),
          morphMotion(encodingRef),
          modelMotion(modelRef, encodingRef),
          error(kNoError),
          active(true)
    {
        type2animationRefs.insert(IKeyframe::kBoneKeyframe, &boneMotion);
        type2animationRefs.insert(IKeyframe::kCameraKeyframe, &cameraMotion);
        type2animationRefs.insert(IKeyframe::kLightKeyframe, &lightMotion);
        type2animationRefs.insert(IKeyframe::kMorphKeyframe, &morphMotion);
        type2animationRefs.insert(IKeyframe::kModelKeyframe, &modelMotion);
        type2animationRefs.insert(IKeyframe::kProjectKeyframe, &projectMotion);
    }
    ~PrivateContext() {
        release();
    }

    void parseHeader(const Motion::DataInfo &info) {
        name = encodingRef->toString(info.namePtr, IString::kShiftJIS, kNameSize);
    }
    void parseBoneKeyframes(const Motion::DataInfo &info) {
        boneMotion.read(info.boneKeyframePtr, info.boneKeyframeCount);
        boneMotion.setParentModelRef(parentModelRef);
    }
    void parseMorphKeyframes(const Motion::DataInfo &info) {
        morphMotion.read(info.morphKeyframePtr, info.morphKeyframeCount);
        morphMotion.setParentModelRef(parentModelRef);
    }
    void parseCameraKeyframes(const Motion::DataInfo &info) {
        cameraMotion.read(info.cameraKeyframePtr, info.cameraKeyframeCount);
        if (!cameraMotion.findKeyframe(0)) {
            CameraKeyframe *keyframe = new CameraKeyframe();
            keyframe->setTimeIndex(0);
            keyframe->setAngle(kZeroV3);
            keyframe->setDistance(50.0f);
            keyframe->setFov(27.0f);
            keyframe->setLookAt(Vector3(0.0f, 10.0f, 0.0f));
            keyframe->setDefaultInterpolationParameter();
            cameraMotion.addKeyframe(keyframe);
        }
    }
    void parseLightKeyframes(const Motion::DataInfo &info) {
        lightMotion.read(info.lightKeyframePtr, info.lightKeyframeCount);
        if (!lightMotion.findKeyframe(0)) {
            LightKeyframe *keyframe = new LightKeyframe();
            keyframe->setTimeIndex(0);
            keyframe->setColor(Vector3(0.6f, 0.6f, 0.6f));
            keyframe->setDirection(Vector3(-1.0f, -0.5f, -1.0f));
            lightMotion.addKeyframe(keyframe);
        }
    }
    void parseSelfShadowKeyframes(const Motion::DataInfo &info) {
        projectMotion.read(info.selfShadowKeyframePtr, info.selfShadowKeyframeCount);
    }
    void parseModelKeyframes(const Motion::DataInfo &info) {
        modelMotion.read(info.modelKeyframePtr, info.modelKeyframeCount);
        if (!modelMotion.findKeyframe(0)) {
            ModelKeyframe *keyframe = new ModelKeyframe(encodingRef);
            keyframe->setTimeIndex(0);
            keyframe->setVisible(true);
            modelMotion.addKeyframe(keyframe);
        }
    }
    void release() {
        /* retain model reference */
        internal::deleteObject(name);
        internal::deleteObject(motionPtr);
        parentSceneRef = 0;
        motionPtr = 0;
        error = kNoError;
        active = false;
    }

    IMotion *motionPtr;
    Scene *parentSceneRef;
    IModel *parentModelRef;
    IEncoding *encodingRef;
    IString *name;
    Motion::DataInfo dataInfo;
    BoneAnimation boneMotion;
    CameraAnimation cameraMotion;
    MorphAnimation morphMotion;
    LightAnimation lightMotion;
    ModelAnimation modelMotion;
    ProjectAnimation projectMotion;
    Hash<HashInt, BaseAnimation *> type2animationRefs;
    Error error;
    bool active;
};

const uint8 *Motion::kSignature = reinterpret_cast<const uint8 *>("Vocaloid Motion Data 0002");

Motion::Motion(IModel *modelRef, IEncoding *encodingRef)
    : m_context(new PrivateContext(modelRef, encodingRef))
{
}

Motion::~Motion()
{
    internal::deleteObject(m_context);
}

bool Motion::preparse(const uint8 *data, vsize size, DataInfo &info)
{
    vsize rest = size;
    // Header(30) + Name(20)
    if (!data || kSignatureSize + kNameSize > rest) {
        VPVL2_LOG(WARNING, "Data is null or MVD header not satisfied: " << size);
        m_context->error = kInvalidHeaderError;
        return false;
    }

    uint8 *ptr = const_cast<uint8 *>(data);
    info.basePtr = ptr;
    VPVL2_VLOG(1, "VMDBasePtr: ptr=" << static_cast<const void*>(ptr) << " size=" << size);

    // Check the signature is valid
    if (std::memcmp(ptr, kSignature, sizeof(kSignature) - 1) != 0) {
        VPVL2_LOG(WARNING, "Invalid VMD signature detected: " << static_cast<const void*>(ptr));
        m_context->error = kInvalidSignatureError;
        return false;
    }
    ptr += kSignatureSize;
    info.namePtr = ptr;
    ptr += kNameSize;
    rest -= kSignatureSize + kNameSize;
    VPVL2_VLOG(1, "VMDNamePtr: ptr=" << static_cast<const void *>(info.namePtr) << " size=" << kNameSize << " rest=" << rest);

    // Bone key frame
    int32 nBoneKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nBoneKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD bone keyframe size detected: " << static_cast<const void*>(ptr) << " size=" << nBoneKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidBoneKeyframesSizeError;
        return false;
    }
    info.boneKeyframePtr = ptr;
    if (!internal::validateSize(ptr, BoneKeyframe::strideSize(), nBoneKeyframes, rest)) {
        VPVL2_LOG(WARNING, "Invalid VMD bone keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nBoneKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidBoneKeyframesError;
        return false;
    }
    info.boneKeyframeCount = nBoneKeyframes;
    VPVL2_VLOG(1, "VMDBoneKeyframes: ptr=" << static_cast<const void *>(info.boneKeyframePtr) << " size=" << nBoneKeyframes << " rest=" << rest);

    // Morph key frame
    int32 nMorphKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nMorphKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD morph keyframe size detected: " << static_cast<const void*>(ptr) << " size=" << nMorphKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidMorphKeyframesSizeError;
        return false;
    }
    info.morphKeyframePtr = ptr;
    if (!internal::validateSize(ptr, MorphKeyframe::strideSize(), nMorphKeyframes, rest)) {
        VPVL2_LOG(WARNING, "Invalid VMD morph keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nMorphKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidMorphKeyframesError;
        return false;
    }
    info.morphKeyframeCount = nMorphKeyframes;
    VPVL2_VLOG(1, "VMDMorphKeyframes: ptr=" << static_cast<const void *>(info.morphKeyframePtr) << " size=" << nMorphKeyframes << " rest=" << rest);

    // Camera key frame
    int32 nCameraKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nCameraKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD camera keyframe size detected: " << static_cast<const void*>(ptr) << " size=" << nCameraKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidCameraKeyframesSizeError;
        return false;
    }
    info.cameraKeyframePtr = ptr;

    vsize cameraKeyframeStrideSize = CameraKeyframe::strideSize();
    if (!internal::validateSize(ptr, cameraKeyframeStrideSize, nCameraKeyframes, rest)) {
        VPVL2_LOG(WARNING, "Invalid VMD camera keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nCameraKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidCameraKeyframesError;
        return false;
    }
    info.cameraKeyframeCount = nCameraKeyframes;
    VPVL2_VLOG(1, "VMDCameraKeyframes: ptr=" << static_cast<const void *>(info.cameraKeyframePtr) << " size=" << nCameraKeyframes << " rest=" << rest);

    // workaround for no camera keyframe
    if (nCameraKeyframes == 0 && rest == cameraKeyframeStrideSize + sizeof(nCameraKeyframes)) {
        internal::validateSize(ptr, cameraKeyframeStrideSize, 1, rest);
        return true;
    }

    // Light key frame
    int32 nLightKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nLightKeyframes)) {
        m_context->error = kVMDInvalidLightKeyframesSizeError;
        return false;
    }
    info.lightKeyframePtr = ptr;
    if (!internal::validateSize(ptr, LightKeyframe::strideSize(), nLightKeyframes, rest)) {
        VPVL2_LOG(WARNING, "Invalid VMD light keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nLightKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidCameraKeyframesError;
        return false;
    }
    info.lightKeyframeCount = nLightKeyframes;
    VPVL2_VLOG(1, "VMDLightKeyframes: ptr=" << static_cast<const void *>(info.lightKeyframePtr) << " size=" << nLightKeyframes << " rest=" << rest);
    if (rest == 0) {
        return true;
    }

    int32 nSelfShadowKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nSelfShadowKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD self shadow keyframe size detected: " << static_cast<const void*>(ptr) << " size=" << nSelfShadowKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidShadowKeyframesSizeError;
        return false;
    }
    info.selfShadowKeyframeCount = nSelfShadowKeyframes;
    if (rest == 0) {
        return true;
    }
    if (!internal::validateSize(ptr, ProjectKeyframe::strideSize(), nSelfShadowKeyframes, rest)) {
        VPVL2_LOG(WARNING, "Invalid VMD self shadow keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nSelfShadowKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidShadowKeyframesError;
        return false;
    }
    info.selfShadowKeyframePtr = ptr;
    VPVL2_VLOG(1, "VMDSelfShadowKeyframes: ptr=" << static_cast<const void *>(info.selfShadowKeyframePtr) << " size=" << nSelfShadowKeyframes << " rest=" << rest);

    int32 nModelKeyframes = 0;
    if (!internal::getTyped<int32>(ptr, rest, nModelKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD model keyframe size detected: " << static_cast<const void*>(ptr) << " size=" << nModelKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidModelKeyframesSizeError;
        return false;
    }
    info.modelKeyframePtr = ptr;
    if (!ModelKeyframe::preparse(ptr, rest, nModelKeyframes)) {
        VPVL2_LOG(WARNING, "Invalid VMD model keyframes detected: " << static_cast<const void*>(ptr) << " size=" << nModelKeyframes << " rest=" << rest);
        m_context->error = kVMDInvalidModelKeyframesError;
        return false;
    }
    info.modelKeyframeCount = nModelKeyframes;
    VPVL2_VLOG(1, "VMDModelKeyframes: ptr=" << static_cast<const void *>(info.modelKeyframePtr) << " size=" << nModelKeyframes << " rest=" << rest);

    return rest == 0;
}

bool Motion::load(const uint8 *data, vsize size)
{
    DataInfo info;
    internal::zerofill(&info, sizeof(info));
    if (preparse(data, size, info)) {
        m_context->release();
        m_context->parseHeader(info);
        m_context->parseBoneKeyframes(info);
        m_context->parseMorphKeyframes(info);
        m_context->parseCameraKeyframes(info);
        m_context->parseLightKeyframes(info);
        m_context->parseSelfShadowKeyframes(info);
        m_context->parseModelKeyframes(info);
        return true;
    }
    return false;
}

void Motion::save(uint8 *data) const
{
    internal::writeBytes(kSignature, kSignatureSize, data);
    internal::writeStringAsByteArray(m_context->name, m_context->encodingRef, IString::kShiftJIS, kNameSize, data);
    int32 nBoneKeyframes = m_context->boneMotion.countKeyframes();
    internal::writeBytes(&nBoneKeyframes, sizeof(nBoneKeyframes), data);
    for (int32 i = 0; i < nBoneKeyframes; i++) {
        BoneKeyframe *keyframe = m_context->boneMotion.findKeyframeAt(i);
        keyframe->write(data);
        data += BoneKeyframe::strideSize();
    }
    int32 nMorphKeyframes = m_context->morphMotion.countKeyframes();
    internal::writeBytes(&nMorphKeyframes, sizeof(nMorphKeyframes), data);
    for (int32 i = 0; i < nMorphKeyframes; i++) {
        MorphKeyframe *keyframe = m_context->morphMotion.findKeyframeAt(i);
        keyframe->write(data);
        data += MorphKeyframe::strideSize();
    }
    int32 nCameraKeyframes = m_context->cameraMotion.countKeyframes();
    internal::writeBytes(&nCameraKeyframes, sizeof(nCameraKeyframes), data);
    for (int32 i = 0; i < nCameraKeyframes; i++) {
        CameraKeyframe *keyframe = m_context->cameraMotion.findKeyframeAt(i);
        keyframe->write(data);
        data += CameraKeyframe::strideSize();
    }
    int32 nLightKeyframes = m_context->lightMotion.countKeyframes();
    internal::writeBytes(&nLightKeyframes, sizeof(nLightKeyframes), data);
    for (int32 i = 0; i < nLightKeyframes; i++) {
        LightKeyframe *keyframe = m_context->lightMotion.findKeyframeAt(i);
        keyframe->write(data);
        data += LightKeyframe::strideSize();
    }
    int32 emptyShadowKeyframes = 0;
    internal::writeBytes(&emptyShadowKeyframes, sizeof(emptyShadowKeyframes), data);
    int32 nModelKeyframes = m_context->modelMotion.countKeyframes();
    if (nModelKeyframes > 0) {
        internal::writeBytes(&nModelKeyframes, sizeof(nModelKeyframes), data);
        for (int32 i = 0; i < nModelKeyframes; i++) {
            ModelKeyframe *keyframe = m_context->modelMotion.findKeyframeAt(i);
            keyframe->write(data);
            data += keyframe->estimateSize();
        }
    }
}

vsize Motion::estimateSize() const
{
    /*
     * header[30]
     * name[20]
     * bone size
     * morph size
     * camera size
     * light size
     * selfshadow size
     * model size
     */
    return kSignatureSize + kNameSize + sizeof(int32) * 6
            + m_context->boneMotion.countKeyframes() * BoneKeyframe::strideSize()
            + m_context->morphMotion.countKeyframes() * MorphKeyframe::strideSize()
            + m_context->cameraMotion.countKeyframes() * CameraKeyframe::strideSize()
            + m_context->lightMotion.countKeyframes() * LightKeyframe::strideSize()
            + m_context->projectMotion.countKeyframes() * ProjectKeyframe::strideSize()
            + m_context->modelMotion.estimateSize();
}

void Motion::setParentSceneRef(Scene *value)
{
    m_context->parentSceneRef = value;
}

void Motion::setParentModelRef(IModel *value)
{
    m_context->boneMotion.setParentModelRef(value);
    m_context->morphMotion.setParentModelRef(value);
    m_context->modelMotion.setParentModelRef(value);
    m_context->parentModelRef = value;
    if (value) {
        if (const IString *name = value->name(IEncoding::kDefaultLanguage)) {
            internal::setString(name, m_context->name);
        }
    }
}

void Motion::seekSeconds(const float64 &seconds)
{
    seekTimeIndex(uint64(seconds * kFPS));
}

void Motion::seekSceneSeconds(const float64 &seconds, Scene *scene)
{
    seekSceneTimeIndex(uint64(seconds * kFPS), scene);
}

void Motion::seekTimeIndex(const IKeyframe::TimeIndex &timeIndex)
{
    m_context->boneMotion.seek(timeIndex);
    m_context->morphMotion.seek(timeIndex);
    m_context->modelMotion.seek(timeIndex);
    m_context->active = durationTimeIndex() > timeIndex;
}

void Motion::seekSceneTimeIndex(const IKeyframe::TimeIndex &timeIndex, Scene *scene)
{
    if (m_context->cameraMotion.countKeyframes() > 0) {
        m_context->cameraMotion.seek(timeIndex);
        ICamera *camera = scene->cameraRef();
        camera->setLookAt(m_context->cameraMotion.position());
        camera->setAngle(m_context->cameraMotion.angle());
        camera->setFov(m_context->cameraMotion.fovy());
        camera->setDistance(m_context->cameraMotion.distance());
    }
    if (m_context->lightMotion.countKeyframes() > 0) {
        m_context->lightMotion.seek(timeIndex);
        ILight *light = scene->lightRef();
        light->setColor(m_context->lightMotion.color());
        light->setDirection(m_context->lightMotion.direction());
    }
    if (m_context->projectMotion.countKeyframes() > 0) {
        m_context->projectMotion.seek(timeIndex);
        if (IShadowMap *shadowMap = scene->shadowMapRef()) {
            shadowMap->setDistance(m_context->projectMotion.shadowDistance());
        }
    }
}

void Motion::advance(const IKeyframe::TimeIndex &deltaTimeIndex)
{
    if (deltaTimeIndex == 0) {
        m_context->boneMotion.advance(deltaTimeIndex);
        m_context->morphMotion.advance(deltaTimeIndex);
    }
    else if (m_context->active) {
        // The motion is active and continue to advance
        m_context->boneMotion.advance(deltaTimeIndex);
        m_context->morphMotion.advance(deltaTimeIndex);
        if (isReachedTo(durationTimeIndex())) {
            m_context->active = false;
        }
    }
}

void Motion::advanceScene(const IKeyframe::TimeIndex &deltaTimeIndex, Scene *scene)
{
    if (m_context->cameraMotion.countKeyframes() > 0) {
        m_context->cameraMotion.advance(deltaTimeIndex);
        ICamera *camera = scene->cameraRef();
        camera->setLookAt(m_context->cameraMotion.position());
        camera->setAngle(m_context->cameraMotion.angle());
        camera->setFov(m_context->cameraMotion.fovy());
        camera->setDistance(m_context->cameraMotion.distance());
    }
    if (m_context->lightMotion.countKeyframes() > 0) {
        m_context->lightMotion.advance(deltaTimeIndex);
        ILight *light = scene->lightRef();
        light->setColor(m_context->lightMotion.color());
        light->setDirection(m_context->lightMotion.direction());
    }
}

void Motion::reload()
{
    /* rebuild internal keyframe nodes */
    m_context->boneMotion.setParentModelRef(m_context->parentModelRef);
    m_context->morphMotion.setParentModelRef(m_context->parentModelRef);
    reset();
}

void Motion::reset()
{
    m_context->boneMotion.seek(0.0f);
    m_context->morphMotion.seek(0.0f);
    m_context->boneMotion.reset();
    m_context->morphMotion.reset();
    m_context->projectMotion.reset();
    m_context->active = true;
}

float64 Motion::durationSeconds() const
{
    return durationTimeIndex() * kFPS;
}

IKeyframe::TimeIndex Motion::durationTimeIndex() const
{
    IKeyframe::TimeIndex duration(0);
    btSetMax(duration, m_context->boneMotion.duration());
    btSetMax(duration, m_context->cameraMotion.duration());
    btSetMax(duration, m_context->lightMotion.duration());
    btSetMax(duration, m_context->morphMotion.duration());
    btSetMax(duration, m_context->modelMotion.duration());
    btSetMax(duration, m_context->projectMotion.duration());
    return duration;
}

bool Motion::isReachedTo(const IKeyframe::TimeIndex &atEnd) const
{
    if (m_context->active) {
        return internal::MotionHelper::isReachedToDuration(m_context->boneMotion, atEnd) &&
                internal::MotionHelper::isReachedToDuration(m_context->cameraMotion, atEnd) &&
                internal::MotionHelper::isReachedToDuration(m_context->lightMotion, atEnd) &&
                internal::MotionHelper::isReachedToDuration(m_context->morphMotion, atEnd) &&
                internal::MotionHelper::isReachedToDuration(m_context->modelMotion, atEnd) &&
                internal::MotionHelper::isReachedToDuration(m_context->projectMotion, atEnd);
    }
    return true;
}

bool Motion::isNullFrameEnabled() const
{
    return m_context->boneMotion.isNullFrameEnabled() && m_context->morphMotion.isNullFrameEnabled();
}

void Motion::setNullFrameEnable(bool value)
{
    m_context->boneMotion.setNullFrameEnable(value);
    m_context->morphMotion.setNullFrameEnable(value);
}

IBoneKeyframe *Motion::createBoneKeyframe()
{
    return new BoneKeyframe(m_context->encodingRef);
}

ICameraKeyframe *Motion::createCameraKeyframe()
{
    return new CameraKeyframe();
}

IEffectKeyframe *Motion::createEffectKeyframe()
{
    VPVL2_LOG(WARNING, "Effect keyframe is not supported in VMD format");
    return 0;
}

ILightKeyframe *Motion::createLightKeyframe()
{
    return new LightKeyframe();
}

IModelKeyframe *Motion::createModelKeyframe()
{
    return new ModelKeyframe(m_context->encodingRef);
}

IMorphKeyframe *Motion::createMorphKeyframe()
{
    return new MorphKeyframe(m_context->encodingRef);
}

IProjectKeyframe *Motion::createProjectKeyframe()
{
    VPVL2_LOG(WARNING, "Project keyframe is not supported in VMD format");
    return 0;
}

void Motion::addKeyframe(IKeyframe *value)
{
    if (!value || value->layerIndex() != 0) {
        return;
    }
    if (BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(value->type())) {
        BaseAnimation *animation = *animationPtr;
        animation->addKeyframe(value);
    }
}

void Motion::replaceKeyframe(IKeyframe *value, bool alsoDelete)
{
    if (!value) {
        VPVL2_LOG(WARNING, "null keyframe cannot be replaced");
        return;
    }
    IKeyframe *keyframeToDelete = 0;
    switch (value->type()) {
    case IKeyframe::kBoneKeyframe: {
        keyframeToDelete = m_context->boneMotion.findKeyframe(value->timeIndex(), value->name());
        if (keyframeToDelete) {
            m_context->boneMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->boneMotion.addKeyframe(value);
        break;
    }
    case IKeyframe::kCameraKeyframe: {
        keyframeToDelete = m_context->cameraMotion.findKeyframe(value->timeIndex());
        if (keyframeToDelete) {
            m_context->cameraMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->cameraMotion.addKeyframe(value);
        break;
    }
    case IKeyframe::kLightKeyframe: {
        keyframeToDelete = m_context->lightMotion.findKeyframe(value->timeIndex());
        if (keyframeToDelete) {
            m_context->lightMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->lightMotion.addKeyframe(value);
        break;
    }
    case IKeyframe::kMorphKeyframe: {
        keyframeToDelete = m_context->morphMotion.findKeyframe(value->timeIndex(), value->name());
        if (keyframeToDelete) {
            m_context->morphMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->morphMotion.addKeyframe(value);
        break;
    }
    case IKeyframe::kModelKeyframe: {
        keyframeToDelete = m_context->modelMotion.findKeyframe(value->timeIndex());
        if (keyframeToDelete) {
            m_context->morphMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->morphMotion.addKeyframe(value);
        break;
    }
    case IKeyframe::kProjectKeyframe: {
        keyframeToDelete = m_context->projectMotion.findKeyframe(value->timeIndex());
        if (keyframeToDelete) {
            m_context->projectMotion.removeKeyframe(keyframeToDelete);
        }
        m_context->projectMotion.addKeyframe(value);
        break;
    }
    default:
        VPVL2_LOG(WARNING, "Invalid keyframe type: " << value->type());
        break;
    }
    if (alsoDelete) {
        internal::deleteObject(keyframeToDelete);
    }
}

int Motion::countKeyframes(IKeyframe::Type value) const
{
    int count = 0;
    if (const BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(value)) {
        const BaseAnimation *animation = *animationPtr;
        count = animation->countKeyframes();
    }
    return count;
}

void Motion::getKeyframeRefs(const IKeyframe::TimeIndex &timeIndex,
                             const IKeyframe::LayerIndex &layerIndex,
                             IKeyframe::Type type,
                             Array<IKeyframe *> &keyframes)
{
    if (layerIndex != -1 && layerIndex != 0) {
        return;
    }
    if (const BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(type)) {
        const BaseAnimation *animation = *animationPtr;
        animation->getKeyframes(timeIndex, keyframes);
    }
}

IKeyframe::LayerIndex Motion::countLayers(const IString * /* name */,
                                          IKeyframe::Type /* type */) const
{
    return 1;
}

IBoneKeyframe *Motion::findBoneKeyframeRef(const IKeyframe::TimeIndex &timeIndex,
                                           const IString *name,
                                           const IKeyframe::LayerIndex &layerIndex) const
{
    return layerIndex == 0 ? m_context->boneMotion.findKeyframe(timeIndex, name) : 0;
}

IBoneKeyframe *Motion::findBoneKeyframeRefAt(int index) const
{
    return m_context->boneMotion.findKeyframeAt(index);
}

ICameraKeyframe *Motion::findCameraKeyframeRef(const IKeyframe::TimeIndex &timeIndex,
                                               const IKeyframe::LayerIndex &layerIndex) const
{
    return layerIndex == 0 ? m_context->cameraMotion.findKeyframe(timeIndex) : 0;
}

ICameraKeyframe *Motion::findCameraKeyframeRefAt(int index) const
{
    return m_context->cameraMotion.findKeyframeAt(index);
}

IEffectKeyframe *Motion::findEffectKeyframeRef(const IKeyframe::TimeIndex & /* timeIndex */,
                                               const IString * /* name */,
                                               const IKeyframe::LayerIndex & /* layerIndex */) const
{
    VPVL2_LOG(WARNING, "Effect keyframe is not supported in VMD motion.");
    return 0;
}

IEffectKeyframe *Motion::findEffectKeyframeRefAt(int /* index */) const
{
    VPVL2_LOG(WARNING, "Effect keyframe is not supported in VMD motion");
    return 0;
}

ILightKeyframe *Motion::findLightKeyframeRef(const IKeyframe::TimeIndex &timeIndex,
                                             const IKeyframe::LayerIndex &layerIndex) const
{
    return layerIndex == 0 ? m_context->lightMotion.findKeyframe(timeIndex) : 0;
}

ILightKeyframe *Motion::findLightKeyframeRefAt(int index) const
{
    return m_context->lightMotion.findKeyframeAt(index);
}

IModelKeyframe *Motion::findModelKeyframeRef(const IKeyframe::TimeIndex & /* timeIndex */,
                                             const IKeyframe::LayerIndex & /* layerIndex */) const
{
    return 0;
}

IModelKeyframe *Motion::findModelKeyframeRefAt(int /* index */) const
{
    return 0;
}

IMorphKeyframe *Motion::findMorphKeyframeRef(const IKeyframe::TimeIndex &timeIndex,
                                             const IString *name,
                                             const IKeyframe::LayerIndex &layerIndex) const
{
    return layerIndex == 0 ? m_context->morphMotion.findKeyframe(timeIndex, name) : 0;
}

IMorphKeyframe *Motion::findMorphKeyframeRefAt(int index) const
{
    return m_context->morphMotion.findKeyframeAt(index);
}

IProjectKeyframe *Motion::findProjectKeyframeRef(const IKeyframe::TimeIndex & /* timeIndex */,
                                                 const IKeyframe::LayerIndex & /* layerIndex */) const
{
    return 0;
}

IProjectKeyframe *Motion::findProjectKeyframeRefAt(int /* index */) const
{
    return 0;
}

void Motion::removeKeyframe(IKeyframe *value)
{
    /* prevent deleting a null keyframe and timeIndex() of the keyframe is zero */
    if (!value || value->timeIndex() == 0) {
        VPVL2_LOG(WARNING, "null keyframe or keyframe timeIndex is 0 cannot be removed");
        return;
    }
    IKeyframe::Type type = value->type();
    if (BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(value->type())) {
        BaseAnimation *animation = *animationPtr;
        animation->removeKeyframe(value);
        update(type);
    }
}

void Motion::deleteKeyframe(IKeyframe *&value)
{
    /* prevent deleting a null keyframe and timeIndex() of the keyframe is zero */
    if (!value || value->timeIndex() == 0) {
        VPVL2_LOG(WARNING, "null keyframe or keyframe timeIndex is 0 cannot be deleted");
        return;
    }
    IKeyframe::Type type = value->type();
    if (BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(value->type())) {
        BaseAnimation *animation = *animationPtr;
        animation->deleteKeyframe(value);
        update(type);
        value = 0;
    }
}

void Motion::update(IKeyframe::Type type)
{
    switch (type) {
    case IKeyframe::kBoneKeyframe:
        m_context->boneMotion.setParentModelRef(m_context->parentModelRef);
        break;
    case IKeyframe::kCameraKeyframe:
        m_context->cameraMotion.update();
        break;
    case IKeyframe::kLightKeyframe:
        m_context->lightMotion.update();
        break;
    case IKeyframe::kMorphKeyframe:
        m_context->morphMotion.setParentModelRef(m_context->parentModelRef);
        break;
    case IKeyframe::kProjectKeyframe:
        m_context->projectMotion.update();
        break;
    default:
        VPVL2_LOG(WARNING, "Invalid keyframe type: " << type);
        break;
    }
}

IMotion *Motion::clone() const
{
    IMotion *dest = m_context->motionPtr = new Motion(m_context->parentModelRef, m_context->encodingRef);
    const int nbkeyframes = m_context->boneMotion.countKeyframes();
    for (int i = 0; i < nbkeyframes; i++) {
        BoneKeyframe *keyframe = m_context->boneMotion.findKeyframeAt(i);
        dest->addKeyframe(keyframe->clone());
    }
    const int nckeyframes = m_context->cameraMotion.countKeyframes();
    for (int i = 0; i < nckeyframes; i++) {
        CameraKeyframe *keyframe = m_context->cameraMotion.findKeyframeAt(i);
        dest->addKeyframe(keyframe->clone());
    }
    const int nlkeyframes = m_context->lightMotion.countKeyframes();
    for (int i = 0; i < nlkeyframes; i++) {
        LightKeyframe *keyframe = m_context->lightMotion.findKeyframeAt(i);
        dest->addKeyframe(keyframe->clone());
    }
    /*
    const int nmdkeyframes = m_context->morphMotion.countKeyframes();
    for (int i = 0; i < nmkeyframes; i++) {
        ModelKeyframe *keyframe = m_context->morphMotion.keyframeAt(i);
        dest->addKeyframe(keyframe->clone());
    }
    */
    const int nmkeyframes = m_context->morphMotion.countKeyframes();
    for (int i = 0; i < nmkeyframes; i++) {
        MorphKeyframe *keyframe = m_context->morphMotion.findKeyframeAt(i);
        dest->addKeyframe(keyframe->clone());
    }
    m_context->motionPtr = 0;
    return dest;
}

void Motion::getAllKeyframeRefs(Array<IKeyframe *> &value, IKeyframe::Type type)
{
    if (const BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(type)) {
        const BaseAnimation *animation = *animationPtr;
        animation->getAllKeyframes(value);
    }
}

void Motion::setAllKeyframes(const Array<IKeyframe *> &value, IKeyframe::Type type)
{
    if (BaseAnimation *const *animationPtr = m_context->type2animationRefs.find(type)) {
        BaseAnimation *animation = *animationPtr;
        animation->setAllKeyframes(value, type);
        update(type);
    }
}

const IString *Motion::name() const
{
    return m_context->name;
}

Scene *Motion::parentSceneRef() const
{
    return m_context->parentSceneRef;
}

IModel *Motion::parentModelRef() const
{
    return m_context->parentModelRef;
}

Motion::Error Motion::error() const
{
    return m_context->error;
}

const BoneAnimation &Motion::boneAnimation() const
{
    return m_context->boneMotion;
}

const CameraAnimation &Motion::cameraAnimation() const
{
    return m_context->cameraMotion;
}

const MorphAnimation &Motion::morphAnimation() const
{
    return m_context->morphMotion;
}

const LightAnimation &Motion::lightAnimation() const
{
    return m_context->lightMotion;
}

const Motion::DataInfo &Motion::result() const
{
    return m_context->dataInfo;
}

BoneAnimation *Motion::mutableBoneAnimation()
{
    return &m_context->boneMotion;
}

CameraAnimation *Motion::mutableCameraAnimation()
{
    return &m_context->cameraMotion;
}

MorphAnimation *Motion::mutableMorphAnimation()
{
    return &m_context->morphMotion;
}

LightAnimation *Motion::mutableLightAnimation()
{
    return &m_context->lightMotion;
}

bool Motion::isActive() const
{
    return m_context->active;
}

IMotion::FormatType Motion::type() const
{
    return kVMDFormat;
}

} /* namespace vmd */
} /* namespace VPVL2_VERSION_NS */
} /* namespace vpvl2 */
