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

#include "vpvl2/vpvl2.h"
#include "vpvl2/internal/util.h"

#include "vpvl2/pmx/Bone.h"
#include "vpvl2/pmx/Material.h"
#include "vpvl2/pmx/Morph.h"
#include "vpvl2/pmx/RigidBody.h"
#include "vpvl2/pmx/Vertex.h"

namespace
{

using namespace vpvl2::VPVL2_VERSION_NS;

#pragma pack(push, 1)

struct MorphUnit {
    uint8 category;
    uint8 type;
    int32 size;
};

struct VertexMorph {
    float32 position[3];
};

struct UVMorph {
    float32 position[4];
};

struct BoneMorph {
    float32 position[3];
    float32 rotation[4];
};

struct MaterialMorph {
    uint8_t operation;
    float32 diffuse[4];
    float32 specular[3];
    float32 shininess;
    float32 ambient[3];
    float32 edgeColor[4];
    float32 edgeSize;
    float32 textureWeight[4];
    float32 sphereTextureWeight[4];
    float32 toonTextureWeight[4];
};

struct GroupMorph {
    float32 weight;
};

struct FlipMorph {
    float32 weight;
};

struct ImpulseMorph {
    uint8 isLocal;
    float32 velocity[3];
    float32 torque[3];
};

#pragma pack(pop)

}

namespace vpvl2
{
namespace VPVL2_VERSION_NS
{
namespace pmx
{

struct Morph::PrivateContext {
    PrivateContext(Model *modelRef)
        : parentModelRef(modelRef),
          namePtr(0),
          englishNamePtr(0),
          weight(0),
          internalWeight(0),
          category(kBase),
          type(kUnknownMorph),
          index(-1),
          hasParent(false),
          dirty(false)
    {
    }
    ~PrivateContext() {
        vertices.releaseAll();
        uvs.releaseAll();
        bones.releaseAll();
        materials.releaseAll();
        groups.releaseAll();
        flips.releaseAll();
        impulses.releaseAll();
        internal::deleteObject(namePtr);
        internal::deleteObject(englishNamePtr);
        parentModelRef = 0;
        weight = 0;
        internalWeight = 0;
        category = kBase;
        type = kUnknownMorph;
        index = -1;
        hasParent = false;
        dirty = false;
    }

    static bool loadBones(const Array<pmx::Bone *> &bones, Morph *morph) {
        const int nMorphBones = morph->m_context->bones.count();
        const int nbones = bones.count();
        for (int32 i = 0; i < nMorphBones; i++) {
            Bone *bone = morph->m_context->bones[i];
            int boneIndex = bone->index;
            if (boneIndex >= 0) {
                if (boneIndex >= nbones) {
                    VPVL2_LOG(WARNING, "Invalid PMX bone morph: index=" << i << " bone=" << boneIndex);
                    return false;
                }
                else {
                    bone->bone = bones[boneIndex];
                }
            }
        }
        return true;
    }
    static bool loadGroups(const Array<Morph *> &morphs, Morph *morph) {
        const int nMorphGroups = morph->m_context->groups.count();
        const int nmorphs = morphs.count();
        for (int32 i = 0; i < nMorphGroups; i++) {
            Group *group = morph->m_context->groups[i];
            int groupIndex = group->index;
            if (groupIndex >= 0) {
                if (groupIndex >= nmorphs) {
                    VPVL2_LOG(WARNING, "Invalid PMX group morph: index=" << i << " group=" << groupIndex);
                    return false;
                }
                else {
                    Morph *morph = morphs[groupIndex];
                    group->morph = morph;
                    morph->m_context->hasParent = true;
                }
            }
        }
        return true;
    }
    static bool loadMaterials(const Array<pmx::Material *> &materials, Morph *morph) {
        const int nMorphMaterials = morph->m_context->materials.count();
        const int nmaterials = materials.count();
        for (int i = 0; i < nMorphMaterials; i++) {
            Material *material = morph->m_context->materials[i];
            int materialIndex = material->index;
            if (materialIndex >= 0) {
                if (materialIndex >= nmaterials) {
                    VPVL2_LOG(WARNING, "Invalid PMX material morph: index=" << i << " material=" << materialIndex);
                    return false;
                }
                else {
                    material->materials->append(materials[materialIndex]);
                }
            }
            else {
                const int nmaterials = materials.count();
                for (int j = 0; j < nmaterials; j++) {
                    pmx::Material *m = materials[j];
                    material->materials->append(m);
                }
            }
        }
        return true;
    }
    static bool loadUVs(const Array<pmx::Vertex *> &vertices, int offset, Morph *morph) {
        const int nMorphUVs = morph->m_context->uvs.count();
        const int nvertices = vertices.count();
        for (int i = 0; i < nMorphUVs; i++) {
            UV *uv = morph->m_context->uvs[i];
            int vertexIndex = uv->index;
            if (vertexIndex >= 0) {
                if (vertexIndex >= nvertices) {
                    VPVL2_LOG(WARNING, "Invalid PMX UV vertex morph: index=" << i << " vertex=" << vertexIndex);
                    return false;
                }
                else {
                    uv->vertex = vertices[vertexIndex];
                    uv->offset = offset;
                }
            }
        }
        return true;
    }
    static bool loadVertices(const Array<pmx::Vertex *> &vertices, Morph *morph) {
        const int nMorphVertices = morph->m_context->vertices.count();
        const int nvertices = vertices.count();
        for (int i = 0; i < nMorphVertices; i++) {
            Vertex *vertex = morph->m_context->vertices[i];
            int vertexIndex = vertex->index;
            if (vertexIndex >= 0) {
                if (vertexIndex >= nvertices) {
                    VPVL2_LOG(WARNING, "Invalid PMX vertex morph: index=" << i << " vertex=" << vertexIndex);
                    return false;
                }
                else {
                    vertex->vertex = vertices[vertexIndex];
                }
            }
        }
        return true;
    }
    static bool loadFlips(const Array<Morph *> &morphs, Morph *morph) {
        const int nMorphFlips = morph->m_context->impulses.count();
        const int nflips = morphs.count();
        for (int i = 0; i < nMorphFlips; i++) {
            Flip *flip = morph->m_context->flips[i];
            int flipIndex = flip->index;
            if (flipIndex >= 0) {
                if (flipIndex >= nflips) {
                    VPVL2_LOG(WARNING, "Invalid flip morph: index=" << i << " morph=" << flipIndex);
                    return false;
                }
                else {
                    flip->morph = morphs[flipIndex];
                }
            }
        }
        return true;
    }
    static bool loadImpulses(const Array<RigidBody *> &rigidBodies, Morph *morph) {
        const int nMorphImpulses = morph->m_context->impulses.count();
        const int nbodies = rigidBodies.count();
        for (int i = 0; i < nMorphImpulses; i++) {
            Impulse *impulse = morph->m_context->impulses[i];
            int rigidBodyIndex = impulse->index;
            if (rigidBodyIndex >= 0) {
                if (rigidBodyIndex >= nbodies) {
                    VPVL2_LOG(WARNING, "Invalid impluse morph: index=" << i << " body=" << rigidBodyIndex);
                    return false;
                }
                else {
                    impulse->rigidBody = rigidBodies[rigidBodyIndex];
                }
            }
        }
        return true;
    }

    void readBones(const Model::DataInfo &info, int count, uint8 *&ptr) {
        BoneMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Bone *bone = bones.append(new Morph::Bone());
            int boneIndex = internal::readSignedIndex(ptr, info.boneIndexSize);
            VPVL2_VLOG(3, "PMXBoneMorph: index=" << i << " boneIndex=" << boneIndex);
            internal::getData(ptr, morph);
            internal::setPosition(morph.position, bone->position);
            VPVL2_VLOG(3, "PMXBoneMorph: position=" << bone->position.x() << "," << bone->position.y() << "," << bone->position.z());
            internal::setRotation(morph.rotation, bone->rotation);
            VPVL2_VLOG(3, "PMXBoneMorph: rotation=" << bone->rotation.x() << "," << bone->rotation.y() << "," << bone->rotation.z());
            bone->index = boneIndex;
            ptr += sizeof(morph);
        }
    }
    void readGroups(const Model::DataInfo &info, int count, uint8 *&ptr) {
        GroupMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Group *group = groups.append(new Morph::Group());
            int morphIndex = internal::readSignedIndex(ptr, info.morphIndexSize);
            internal::getData(ptr, morph);
            VPVL2_VLOG(3, "PMXGroupMorph: index=" << i << " morphIndex=" << morphIndex << " weight=" << group->fixedWeight);
            group->fixedWeight = morph.weight;
            group->index = morphIndex;
            ptr += sizeof(morph);
        }
    }
    void readMaterials(const Model::DataInfo &info, int count, uint8 *&ptr) {
        MaterialMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Material *material = materials.append(new Morph::Material());
            int materialIndex = internal::readSignedIndex(ptr, info.materialIndexSize);
            internal::getData(ptr, morph);
            VPVL2_VLOG(3, "PMXMaterialMorph: index=" << i << " materialIndex=" << materialIndex << " operation" << int(material->operation));
            material->materials = new Array<IMaterial *>();
            material->ambient.setValue(morph.ambient[0], morph.ambient[1], morph.ambient[2]);
            VPVL2_VLOG(3, "PMXMaterialMorph: ambient=" << material->ambient.x() << "," << material->ambient.y() << "," << material->ambient.z());
            material->diffuse.setValue(morph.diffuse[0], morph.diffuse[1], morph.diffuse[2], morph.diffuse[3]);
            VPVL2_VLOG(3, "PMXMaterialMorph: diffuse=" << material->diffuse.x() << "," << material->diffuse.y() << "," << material->diffuse.z());
            material->edgeColor.setValue(morph.edgeColor[0], morph.edgeColor[1], morph.edgeColor[2], morph.edgeColor[3]);
            VPVL2_VLOG(3, "PMXMaterialMorph: edgeColor=" << material->edgeColor.x() << "," << material->edgeColor.y() << "," << material->edgeColor.z());
            material->edgeSize = morph.edgeSize;
            VPVL2_VLOG(3, "PMXMaterialMorph: edgeSize=" << material->edgeSize);
            material->index = materialIndex;
            material->operation = morph.operation;
            material->shininess = morph.shininess;
            VPVL2_VLOG(3, "PMXMaterialMorph: shininess=" << material->shininess);
            material->specular.setValue(morph.specular[0], morph.specular[1], morph.specular[2]);
            VPVL2_VLOG(3, "PMXMaterialMorph: specular=" << material->specular.x() << "," << material->specular.y() << "," << material->specular.z());
            material->sphereTextureWeight.setValue(morph.sphereTextureWeight[0], morph.sphereTextureWeight[1],
                    morph.sphereTextureWeight[2], morph.sphereTextureWeight[3]);
            VPVL2_VLOG(3, "PMXMaterialMorph: sphereTextureWeight=" << material->sphereTextureWeight.x() << ","
                       << material->sphereTextureWeight.y() << "," << material->sphereTextureWeight.z() << "," << material->sphereTextureWeight.w());
            material->textureWeight.setValue(morph.textureWeight[0], morph.textureWeight[1],
                    morph.textureWeight[2], morph.textureWeight[3]);
            VPVL2_VLOG(3, "PMXMaterialMorph: textureWeight=" << material->textureWeight.x() << ","
                       << material->textureWeight.y() << "," << material->textureWeight.z() << "," << material->textureWeight.w());
            material->toonTextureWeight.setValue(morph.toonTextureWeight[0], morph.toonTextureWeight[1],
                    morph.toonTextureWeight[2], morph.toonTextureWeight[3]);
            VPVL2_VLOG(3, "PMXMaterialMorph: toonTextureWeight=" << material->sphereTextureWeight.x() << ","
                       << material->toonTextureWeight.y() << "," << material->toonTextureWeight.z() << "," << material->toonTextureWeight.w());
            ptr += sizeof(morph);
        }
    }
    void readUVs(const Model::DataInfo &info, int count, int offset, uint8 *&ptr) {
        UVMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::UV *uv = uvs.append(new Morph::UV());
            int vertexIndex = internal::readUnsignedIndex(ptr, info.vertexIndexSize);
            VPVL2_VLOG(3, "PMXUVMorph: index=" << i << " vertexIndex=" << vertexIndex << " offset=" << offset);
            internal::getData(ptr, morph);
            uv->position.setValue(morph.position[0], morph.position[1], morph.position[2], morph.position[3]);
            VPVL2_VLOG(3, "PMXUVMorph: position=" << uv->position.x() << "," << uv->position.y() << "," << uv->position.z());
            uv->index = vertexIndex;
            uv->offset = offset;
            ptr += sizeof(morph);
        }
    }
    void readVertices(const Model::DataInfo &info, int count, uint8 *&ptr) {
        VertexMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Vertex *vertex = vertices.append(new Morph::Vertex());
            int vertexIndex = internal::readUnsignedIndex(ptr, info.vertexIndexSize);
            VPVL2_VLOG(3, "PMXVertexMorph: index=" << i << " vertexIndex=" << vertexIndex);
            internal::getData(ptr, morph);
            internal::setPosition(morph.position, vertex->position);
            VPVL2_VLOG(3, "PMXVertexMorph: position=" << vertex->position.x() << "," << vertex->position.y() << "," << vertex->position.z());
            vertex->index = vertexIndex;
            ptr += sizeof(morph);
        }
    }
    void readFlips(const Model::DataInfo &info, int count, uint8 *&ptr) {
        FlipMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Flip *flip = flips.append(new Morph::Flip());
            int morphIndex = internal::readSignedIndex(ptr, info.morphIndexSize);
            internal::getData(ptr, morph);
            VPVL2_VLOG(3, "PMXFlipMorph: index=" << i << " morphIndex=" << morphIndex << " weight=" << flip->fixedWeight);
            flip->fixedWeight = morph.weight;
            flip->index = morphIndex;
            ptr += sizeof(morph);
        }
    }
    void readImpulses(const Model::DataInfo &info, int count, uint8 *&ptr) {
        ImpulseMorph morph;
        for (int i = 0; i < count; i++) {
            Morph::Impulse *impulse = impulses.append(new Morph::Impulse());
            int rigidBodyIndex = internal::readSignedIndex(ptr, info.rigidBodyIndexSize);
            internal::getData(ptr, morph);
            impulse->isLocal = morph.isLocal != 0;
            impulse->index = rigidBodyIndex;
            VPVL2_VLOG(3, "PMXImpluseMorph: index=" << i << " rigidBodyIndex=" << rigidBodyIndex << " isLocal=" << impulse->isLocal);
            internal::setPositionRaw(morph.velocity, impulse->velocity);
            VPVL2_VLOG(3, "PMXImpluseMorph: velocity=" << impulse->velocity.x() << "," << impulse->velocity.y() << "," << impulse->velocity.z());
            internal::setPositionRaw(morph.torque, impulse->torque);
            VPVL2_VLOG(3, "PMXImpluseMorph: torque=" << impulse->torque.x() << "," << impulse->torque.y() << "," << impulse->torque.z());
            ptr += sizeof(morph);
        }
    }
    void writeBones(const Model::DataInfo &info, uint8 *&ptr) const {
        BoneMorph morph;
        const int nbones = bones.count(), boneIndexSize = int(info.boneIndexSize);
        for (int i = 0; i < nbones; i++) {
            const Morph::Bone *bone = bones[i];
            internal::getPosition(bone->position, morph.position);
            internal::getRotation(bone->rotation, morph.rotation);
            internal::writeSignedIndex(bone->index, boneIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeGroups(const Model::DataInfo &info, uint8 *&ptr) const {
        GroupMorph morph;
        const int ngroups = groups.count(), morphIndexSize = int(info.morphIndexSize);
        for (int i = 0; i < ngroups; i++) {
            const Morph::Group *group = groups[i];
            morph.weight = float32(group->fixedWeight);
            internal::writeSignedIndex(group->index, morphIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeMaterials(const Model::DataInfo &info, uint8 *&ptr) const {
        MaterialMorph morph;
        const int nmaterials = materials.count(), materialIndexSize = int(info.materialIndexSize);
        for (int i = 0; i < nmaterials; i++) {
            const Morph::Material *material = materials[i];
            internal::getColor(material->ambient, morph.ambient);
            internal::getColor(material->diffuse, morph.diffuse);
            internal::getColor(material->edgeColor, morph.edgeColor);
            morph.operation = material->operation;
            morph.shininess = material->shininess;
            morph.edgeSize = float32(material->edgeSize);
            internal::getColor(material->specular, morph.specular);
            internal::getColor(material->sphereTextureWeight, morph.sphereTextureWeight);
            internal::getColor(material->textureWeight, morph.textureWeight);
            internal::getColor(material->toonTextureWeight, morph.toonTextureWeight);
            internal::writeSignedIndex(material->index, materialIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeUVs(const Model::DataInfo &info, uint8 *&ptr) const {
        UVMorph morph;
        const int nuvs = uvs.count(), vertexIndexSize = int(info.vertexIndexSize);
        for (int i = 0; i < nuvs; i++) {
            const Morph::UV *uv = uvs[i];
            const Vector4 &position = uv->position;
            morph.position[0] = position.x();
            morph.position[1] = position.y();
            morph.position[2] = position.z();
            morph.position[3] = position.w();
            internal::writeUnsignedIndex(uv->index, vertexIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeVertices(const Model::DataInfo &info, uint8 *&ptr) const {
        VertexMorph morph;
        const int nvertices = vertices.count(), vertexIndexSize = int(info.vertexIndexSize);
        for (int i = 0; i < nvertices; i++) {
            const Morph::Vertex *vertex = vertices[i];
            internal::getPosition(vertex->position, morph.position);
            internal::writeUnsignedIndex(vertex->index, vertexIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeFlips(const Model::DataInfo &info, uint8 *&ptr) const {
        FlipMorph morph;
        const int nflips = flips.count(), morphIndexSize = int(info.morphIndexSize);
        for (int i = 0; i < nflips; i++) {
            const Morph::Flip *flip = flips[i];
            morph.weight = float32(flip->fixedWeight);
            internal::writeSignedIndex(flip->index, morphIndexSize, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }
    void writeImpulses(const Model::DataInfo &info, uint8 *&ptr) const {
        ImpulseMorph morph;
        const int nimpulses = impulses.count(), rigidBodyIndex = int(info.rigidBodyIndexSize);
        for (int i = 0; i < nimpulses; i++) {
            const Morph::Impulse *impulse = impulses[i];
            internal::getPositionRaw(impulse->velocity, morph.velocity);
            internal::getPositionRaw(impulse->torque, morph.torque);
            morph.isLocal = impulse->isLocal ? 1 : 0;
            internal::writeSignedIndex(impulse->index, rigidBodyIndex, ptr);
            internal::writeBytes(&morph, sizeof(morph), ptr);
        }
    }

    PointerArray<Vertex> vertices;
    PointerArray<UV> uvs;
    PointerArray<Bone> bones;
    PointerArray<Material> materials;
    PointerArray<Group> groups;
    PointerArray<Flip> flips;
    PointerArray<Impulse> impulses;
    Model *parentModelRef;
    IString *namePtr;
    IString *englishNamePtr;
    Array<PropertyEventListener *> eventRefs;
    IMorph::WeightPrecision weight;
    IMorph::WeightPrecision internalWeight;
    IMorph::Category category;
    IMorph::Type type;
    int index;
    bool hasParent;
    bool dirty;
};

Morph::Morph(Model *modelRef)
    : m_context(new PrivateContext(modelRef))
{
}

Morph::~Morph()
{
    internal::deleteObject(m_context);
}

bool Morph::preparse(uint8 *&ptr, vsize &rest, Model::DataInfo &info)
{
    int32 nmorphs = 0, size = 0;
    if (!internal::getTyped<int32>(ptr, rest, nmorphs)) {
        VPVL2_LOG(WARNING, "Invalid size of PMX morphs detected: size=" << nmorphs << " rest=" << rest);
        return false;
    }
    info.morphsPtr = ptr;
    MorphUnit morph;
    for (int32 i = 0; i < nmorphs; i++) {
        uint8 *namePtr;
        /* name in Japanese */
        if (!internal::getText(ptr, rest, namePtr, size)) {
            VPVL2_LOG(WARNING, "Invalid size of PMX morph name in Japanese detected: index=" << i << " size=" << size << " rest=" << rest);
            return false;
        }
        /* name in English */
        if (!internal::getText(ptr, rest, namePtr, size)) {
            VPVL2_LOG(WARNING, "Invalid size of PMX morph name in English detected: index=" << i << " size=" << size << " rest=" << rest);
            return false;
        }
        if (sizeof(MorphUnit) > rest) {
            VPVL2_LOG(WARNING, "Invalid size of PMX base morph unit detected: index=" << i << " ptr=" << static_cast<const void *>(ptr) << " rest=" << rest);
            return false;
        }
        internal::getData(ptr, morph);
        internal::drainBytes(sizeof(MorphUnit), ptr, rest);
        int nMorphsInMorph = morph.size;
        vsize extraSize;
        switch (static_cast<Type>(morph.type)) {
        case kGroupMorph:
            extraSize = info.morphIndexSize + sizeof(GroupMorph);
            break;
        case kVertexMorph:
            extraSize = info.vertexIndexSize + sizeof(VertexMorph);
            break;
        case kBoneMorph:
            extraSize = info.boneIndexSize + sizeof(BoneMorph);
            break;
        case kTexCoordMorph:
        case kUVA1Morph:
        case kUVA2Morph:
        case kUVA3Morph:
        case kUVA4Morph:
            extraSize = info.vertexIndexSize + sizeof(UVMorph);
            break;
        case kMaterialMorph:
            extraSize = info.materialIndexSize + sizeof(MaterialMorph);
            break;
        case kFlipMorph:
            if (info.version < 2.1) {
                VPVL2_LOG(WARNING, "Flip morph is not supported: index=" << i << " ptr=" << static_cast<const void *>(ptr) << " rest=" << rest);
                return false;
            }
            extraSize = info.morphIndexSize + sizeof(FlipMorph);
            break;
        case kImpulseMorph:
            if (info.version < 2.1) {
                VPVL2_LOG(WARNING, "Impulse morph is not supported: index=" << i << " ptr=" << static_cast<const void *>(ptr) << " rest=" << rest);
                return false;
            }
            extraSize = info.rigidBodyIndexSize + sizeof(ImpulseMorph);
            break;
        default:
            return false;
        }
        for (int j = 0; j < nMorphsInMorph; j++) {
            if (!internal::validateSize(ptr, extraSize, rest)) {
                VPVL2_LOG(WARNING, "Invalid size of PMX morph chunk: index=" << i << " ptr=" << static_cast<const void *>(ptr) << " size=" << extraSize << " rest=" << rest);
                return false;
            }
        }
    }
    info.morphsCount = nmorphs;
    return true;
}

bool Morph::loadMorphs(const Array<Morph *> &morphs,
                       const Array<pmx::Bone *> &bones,
                       const Array<pmx::Material *> &materials,
                       const Array<RigidBody *> &rigidBodies,
                       const Array<pmx::Vertex *> &vertices)
{
    const int nmorphs = morphs.count();
    for (int i = 0; i < nmorphs; i++) {
        Morph *morph = morphs[i];
        switch (morph->type()) {
        case kGroupMorph:
            if (!PrivateContext::loadGroups(morphs, morph)) {
                return false;
            }
            break;
        case kVertexMorph:
            if (!PrivateContext::loadVertices(vertices, morph)) {
                return false;
            }
            break;
        case kBoneMorph:
            if (!PrivateContext::loadBones(bones, morph)) {
                return false;
            }
            break;
        case kTexCoordMorph:
            if (!PrivateContext::loadUVs(vertices, 0, morph)) {
                return false;
            }
            break;
        case kUVA1Morph:
            if (!PrivateContext::loadUVs(vertices, 1, morph)) {
                return false;
            }
            break;
        case kUVA2Morph:
            if (!PrivateContext::loadUVs(vertices, 2, morph)) {
                return false;
            }
            break;
        case kUVA3Morph:
            if (!PrivateContext::loadUVs(vertices, 3, morph)) {
                return false;
            }
            break;
        case kUVA4Morph:
            if (!PrivateContext::loadUVs(vertices, 4, morph)) {
                return false;
            }
            break;
        case kMaterialMorph:
            if (!PrivateContext::loadMaterials(materials, morph)) {
                return false;
            }
            break;
        case kFlipMorph:
            if (!PrivateContext::loadFlips(morphs, morph)) {
                return false;
            }
            break;
        case kImpulseMorph:
            if (!PrivateContext::loadImpulses(rigidBodies, morph)) {
                return false;
            }
            break;
        default:
            return false;
        }
        morph->setIndex(i);
    }
    return true;
}

void Morph::writeMorphs(const Array<Morph *> &morphs, const Model::DataInfo &info, uint8 *&data)
{
    const int32 nmorphs = morphs.count();
    internal::writeBytes(&nmorphs, sizeof(nmorphs), data);
    for (int32 i = 0; i < nmorphs; i++) {
        const Morph *morph = morphs[i];
        morph->write(data, info);
    }
}

vsize Morph::estimateTotalSize(const Array<Morph *> &morphs, const Model::DataInfo &info)
{
    const int32 nmorphs = morphs.count();
    vsize size = 0;
    size += sizeof(nmorphs);
    for (int32 i = 0; i < nmorphs; i++) {
        Morph *morph = morphs[i];
        size += morph->estimateSize(info);
    }
    return size;
}

void Morph::read(const uint8 *data, const Model::DataInfo &info, vsize &size)
{
    uint8 *namePtr = 0, *ptr = const_cast<uint8 *>(data), *start = ptr;
    vsize rest = SIZE_MAX;
    int32 nNameSize = 0;
    internal::getText(ptr, rest, namePtr, nNameSize);
    IEncoding *encoding = info.encoding;
    internal::setStringDirect(encoding->toString(namePtr, nNameSize, info.codec), m_context->namePtr);
    VPVL2_VLOG(3, "PMXMorph: name=" << internal::cstr(m_context->namePtr, "(null)"));
    internal::getText(ptr, rest, namePtr, nNameSize);
    internal::setStringDirect(encoding->toString(namePtr, nNameSize, info.codec), m_context->englishNamePtr);
    VPVL2_VLOG(3, "PMXMorph: englishName=" << internal::cstr(m_context->englishNamePtr, "(null)"));
    MorphUnit unit;
    internal::getData(ptr, unit);
    m_context->category = static_cast<Category>(unit.category);
    m_context->type = static_cast<Type>(unit.type);
    VPVL2_VLOG(3, "PMXMorph: category=" << m_context->category << " type=" << m_context->type << " size=" << unit.size);
    ptr += sizeof(unit);
    switch (m_context->type) {
    case kGroupMorph:
        m_context->readGroups(info, unit.size, ptr);
        break;
    case kVertexMorph:
        m_context->readVertices(info, unit.size, ptr);
        break;
    case kBoneMorph:
        m_context->readBones(info, unit.size, ptr);
        break;
    case kTexCoordMorph:
    case kUVA1Morph:
    case kUVA2Morph:
    case kUVA3Morph:
    case kUVA4Morph:
        m_context->readUVs(info, unit.size, m_context->type - kTexCoordMorph, ptr);
        break;
    case kMaterialMorph:
        m_context->readMaterials(info, unit.size, ptr);
        break;
    case kFlipMorph:
        m_context->readFlips(info, unit.size, ptr);
        break;
    case kImpulseMorph:
        m_context->readImpulses(info, unit.size, ptr);
        break;
    default:
        VPVL2_CHECK(0); /* should not be reached here */
        break;
    }
    size = ptr - start;
}

void Morph::write(uint8 *&data, const Model::DataInfo &info) const
{
    internal::writeString(m_context->namePtr, info.encoding, info.codec, data);
    internal::writeString(m_context->englishNamePtr, info.encoding, info.codec, data);
    MorphUnit mu;
    mu.category = m_context->category;
    mu.type = m_context->type;
    switch (m_context->type) {
    case kGroupMorph:
        mu.size = m_context->groups.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeGroups(info, data);
        break;
    case kVertexMorph:
        mu.size = m_context->vertices.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeVertices(info, data);
        break;
    case kBoneMorph:
        mu.size = m_context->bones.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeBones(info, data);
        break;
    case kTexCoordMorph:
    case kUVA1Morph:
    case kUVA2Morph:
    case kUVA3Morph:
    case kUVA4Morph:
        mu.size = m_context->uvs.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeUVs(info, data);
        break;
    case kMaterialMorph:
        mu.size = m_context->materials.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeMaterials(info, data);
        break;
    case kFlipMorph:
        mu.size = m_context->flips.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeFlips(info, data);
        break;
    case kImpulseMorph:
        mu.size = m_context->impulses.count();
        internal::writeBytes(&mu, sizeof(mu), data);
        m_context->writeImpulses(info, data);
        break;
    default:
        VPVL2_CHECK(0); /* should not be reached here */
        break;
    }
}

vsize Morph::estimateSize(const Model::DataInfo &info) const
{
    vsize size = 0;
    size += internal::estimateSize(m_context->namePtr, info.encoding, info.codec);
    size += internal::estimateSize(m_context->englishNamePtr, info.encoding, info.codec);
    size += sizeof(MorphUnit);
    switch (m_context->type) {
    case kGroupMorph:
        size += m_context->groups.count() * (sizeof(GroupMorph) + info.morphIndexSize);
        break;
    case kVertexMorph:
        size += m_context->vertices.count() * (sizeof(VertexMorph) + info.vertexIndexSize);
        break;
    case kBoneMorph:
        size += m_context->bones.count() * (sizeof(BoneMorph) + info.boneIndexSize);
        break;
    case kTexCoordMorph:
    case kUVA1Morph:
    case kUVA2Morph:
    case kUVA3Morph:
    case kUVA4Morph:
        size += m_context->uvs.count() * (sizeof(UVMorph) + info.vertexIndexSize);
        break;
    case kMaterialMorph:
        size += m_context->materials.count() * (sizeof(MaterialMorph) + info.materialIndexSize);
        break;
    case kFlipMorph:
        size += m_context->flips.count() * (sizeof(FlipMorph) + info.morphIndexSize);
        break;
    case kImpulseMorph:
        size += m_context->impulses.count() * (sizeof(ImpulseMorph) + info.rigidBodyIndexSize);
        break;
    default:
        VPVL2_CHECK(0); /* should not be reached here */
        break;
    }
    return size;
}

void Morph::addEventListenerRef(PropertyEventListener *value)
{
    if (value) {
        m_context->eventRefs.remove(value);
        m_context->eventRefs.append(value);
    }
}

void Morph::removeEventListenerRef(PropertyEventListener *value)
{
    if (value) {
        m_context->eventRefs.remove(value);
    }
}

void Morph::getEventListenerRefs(Array<PropertyEventListener *> &value)
{
    value.copy(m_context->eventRefs);
}

IMorph::WeightPrecision Morph::weight() const
{
    return m_context->weight;
}

void Morph::setWeight(const IMorph::WeightPrecision &value)
{
    if (m_context->weight != value) {
        VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, weightWillChange(value, this));
        m_context->weight = value;
        m_context->dirty = true;
    }
}

void Morph::update()
{
    Type type = m_context->type;
    if (type == kVertexMorph) {
        /* force updating vertex morph because vertices alway will be reset by IModel#performUpdate */
        updateVertexMorphs(m_context->weight);
    }
    else if (type == kGroupMorph) {
        /* force updating group morph to update morph children correctly even weight is not changed (not dirty) */
        updateGroupMorphs(m_context->internalWeight, false);
    }
    else if (m_context->dirty) {
        switch (type) {
        case kBoneMorph:
            updateBoneMorphs(m_context->internalWeight);
            break;
        case kTexCoordMorph:
        case kUVA1Morph:
        case kUVA2Morph:
        case kUVA3Morph:
        case kUVA4Morph:
            updateUVMorphs(m_context->internalWeight);
            break;
        case kMaterialMorph:
            updateMaterialMorphs(m_context->internalWeight);
            break;
        case kImpulseMorph:
            updateImpluseMorphs(m_context->internalWeight);
            break;
        case kFlipMorph:
            break; /* do nothing */
        case kGroupMorph:
        case kVertexMorph:
        default:
            VPVL2_CHECK(0); /* should not be reached here */
            break;
        }
        m_context->dirty = false;
    }
}

void Morph::markDirty()
{
    m_context->dirty = true;
}

void Morph::syncWeight()
{
    if (m_context->dirty) {
        switch (m_context->type) {
        case kGroupMorph:
            updateGroupMorphs(m_context->weight, true);
            break;
        case kFlipMorph:
            updateFlipMorphs(m_context->weight);
            break;
        case kVertexMorph:
        case kBoneMorph:
        case kTexCoordMorph:
        case kUVA1Morph:
        case kUVA2Morph:
        case kUVA3Morph:
        case kUVA4Morph:
        case kMaterialMorph:
        case kImpulseMorph:
        default:
            break;
        }
        setInternalWeight(m_context->weight);
    }
}

void Morph::updateVertexMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->vertices.count();
    for (int i = 0; i < nmorphs; i++) {
        Vertex *v = m_context->vertices[i];
        if (pmx::Vertex *vertex = static_cast<pmx::Vertex *>(v->vertex)) {
            vertex->mergeMorph(v, value);
        }
    }
}

void Morph::updateBoneMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->bones.count();
    for (int i = 0; i < nmorphs; i++) {
        Bone *v = m_context->bones[i];
        if (pmx::Bone *bone = static_cast<pmx::Bone *>(v->bone)) {
            bone->mergeMorph(v, value);
        }
    }
}

void Morph::updateUVMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->uvs.count();
    for (int i = 0; i < nmorphs; i++) {
        UV *v = m_context->uvs[i];
        if (pmx::Vertex *vertex = static_cast<pmx::Vertex *>(v->vertex)) {
            vertex->mergeMorph(v, value);
        }
    }
}

void Morph::updateMaterialMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->materials.count();
    for (int i = 0; i < nmorphs; i++) {
        Material *v = m_context->materials.at(i);
        const Array<IMaterial *> *materials = v->materials;
        const int nmaterials = materials->count();
        for (int j = 0; j < nmaterials; j++) {
            pmx::Material *material = static_cast<pmx::Material *>(materials->at(j));
            material->mergeMorph(v, value);
        }
    }
}

void Morph::updateGroupMorphs(const WeightPrecision &value, bool flipOnly)
{
    const int nmorphs = m_context->groups.count();
    for (int i = 0; i < nmorphs; i++) {
        Group *v = m_context->groups[i];
        if (Morph *morph = static_cast<Morph *>(v->morph)) {
            bool isFlipMorph = morph->type() == Morph::kFlipMorph;
            if (isFlipMorph == flipOnly) {
                if (morph != this) {
                    morph->setInternalWeight(v->fixedWeight * value);
                    morph->update();
                }
            }
        }
    }
}

void Morph::updateFlipMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->flips.count();
    if (nmorphs > 0) {
        const WeightPrecision &weight = btClamped(value, WeightPrecision(0.0), WeightPrecision(1.0));
        int index = int((nmorphs + 1) * weight) - 1;
        const Flip *flip = m_context->flips.at(index);
        if (Morph *morph = static_cast<Morph *>(flip->morph)) {
            if (morph != this) {
                morph->setInternalWeight(flip->fixedWeight);
                morph->update();
            }
        }
    }
}

void Morph::updateImpluseMorphs(const WeightPrecision &value)
{
    const int nmorphs = m_context->impulses.count();
    for (int i = 0; i < nmorphs; i++) {
        Impulse *impulse = m_context->impulses.at(i);
        if (RigidBody *rigidBody = static_cast<RigidBody *>(impulse->rigidBody)) {
            rigidBody->mergeMorph(impulse, value);
        }
    }
}

const IString *Morph::name(IEncoding::LanguageType type) const
{
    switch (type) {
    case IEncoding::kDefaultLanguage:
    case IEncoding::kJapanese:
        return m_context->namePtr;
    case IEncoding::kEnglish:
        return m_context->englishNamePtr;
    default:
        return 0;
    }
}

void Morph::setName(const IString *value, IEncoding::LanguageType type)
{
    m_context->parentModelRef->removeMorph(this);
    switch (type) {
    case IEncoding::kDefaultLanguage:
    case IEncoding::kJapanese:
        if (value && !value->equals(m_context->namePtr)) {
            VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, nameWillChange(value, type, this));
            internal::setString(value, m_context->namePtr);
        }
        else if (!value && value != m_context->namePtr) {
            VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, nameWillChange(value, type, this));
            internal::deleteObject(m_context->namePtr);
        }
        break;
    case IEncoding::kEnglish:
        if (value && !value->equals(m_context->englishNamePtr)) {
            VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, nameWillChange(value, type, this));
            internal::setString(value, m_context->englishNamePtr);
        }
        else if (!value && value != m_context->englishNamePtr) {
            VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, nameWillChange(value, type, this));
            internal::deleteObject(m_context->englishNamePtr);
        }
        break;
    default:
        break;
    }
    m_context->parentModelRef->addMorphHash(this);
}

IModel *Morph::parentModelRef() const
{
    return m_context->parentModelRef;
}

IMorph::Category Morph::category() const
{
    return m_context->category;
}

IMorph::Type Morph::type() const
{
    return m_context->type;
}

int Morph::index() const
{
    return m_context->index;
}

bool Morph::hasParent() const
{
    return m_context->hasParent;
}

const Array<Morph::Bone *> &Morph::bones() const
{
    return m_context->bones;
}

const Array<Morph::Group *> &Morph::groups() const
{
    return m_context->groups;
}

const Array<Morph::Material *> &Morph::materials() const
{
    return m_context->materials;
}

const Array<Morph::UV *> &Morph::uvs() const
{
    return m_context->uvs;
}

const Array<Morph::Vertex *> &Morph::vertices() const
{
    return m_context->vertices;
}

const Array<Morph::Flip *> &Morph::flips() const
{
    return m_context->flips;
}

const Array<Morph::Impulse *> &Morph::impulses() const
{
    return m_context->impulses;
}

void Morph::addBoneMorph(Bone *value)
{
    const IBone *boneRef = value->bone;
    if (boneRef && boneRef->parentModelRef() == m_context->parentModelRef) {
        m_context->bones.append(value);
    }
}

void Morph::removeBoneMorph(Bone *value)
{
    m_context->bones.remove(value);
}

void Morph::addGroupMorph(Group *value)
{
    const IMorph *morphRef = value->morph;
    if (morphRef && morphRef->parentModelRef() == m_context->parentModelRef) {
        m_context->groups.append(value);
    }
}

void Morph::removeGroupMorph(Group *value)
{
    m_context->groups.remove(value);
}

void Morph::addMaterialMorph(Material *value)
{
    const Array<IMaterial *> *materials = value->materials;
    if (materials && materials->count() > 0 && materials->at(0)->parentModelRef() == m_context->parentModelRef) {
        m_context->materials.append(value);
    }
}

void Morph::removeMaterialMorph(Material *value)
{
    m_context->materials.remove(value);
}

void Morph::addUVMorph(UV *value)
{
    const IVertex *vertexRef = value->vertex;
    if (vertexRef && vertexRef->parentModelRef() == m_context->parentModelRef) {
        m_context->uvs.append(value);
    }
}

void Morph::removeUVMorph(UV *value)
{
    m_context->uvs.remove(value);
}

void Morph::addVertexMorph(Vertex *value)
{
    const IVertex *vertexRef = value->vertex;
    if (vertexRef && vertexRef->parentModelRef() == m_context->parentModelRef) {
        m_context->vertices.append(value);
    }
}

void Morph::removeVertexMorph(Vertex *value)
{
    m_context->vertices.remove(value);
}

void Morph::addFlipMorph(Flip *value)
{
    const IMorph *morphRef = value->morph;
    if (morphRef && morphRef->parentModelRef() == m_context->parentModelRef) {
        m_context->flips.append(value);
    }
}

void Morph::removeFlipMorph(Flip *value)
{
    m_context->flips.remove(value);
}

void Morph::addImpulseMorph(Impulse *value)
{
    const IRigidBody *rigidBodyRef = value->rigidBody;
    if (rigidBodyRef && rigidBodyRef->parentModelRef() && m_context->parentModelRef) {
        m_context->impulses.append(value);
    }
}

void Morph::removeImpulseMorph(Impulse *value)
{
    m_context->impulses.remove(value);
}

void Morph::setCategory(Category value)
{
    if (m_context->category != value) {
        VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, categoryWillChange(value, this));
        m_context->category = value;
    }
}

void Morph::setType(Type value)
{
    if (m_context->type != value) {
        VPVL2_TRIGGER_PROPERTY_EVENTS(m_context->eventRefs, typeWillChange(value, this));
        m_context->type = value;
    }
}

void Morph::setIndex(int value)
{
    m_context->index = value;
}

void Morph::setInternalWeight(const WeightPrecision &value)
{
    m_context->internalWeight = value;
    m_context->dirty = true;
}

void Morph::getBoneMorphs(Array<Bone *> &morphs) const
{
    morphs.copy(m_context->bones);
}

void Morph::getGroupMorphs(Array<Group *> &morphs) const
{
    morphs.copy(m_context->groups);
}

void Morph::getMaterialMorphs(Array<Material *> &morphs) const
{
    morphs.copy(m_context->materials);
}

void Morph::getUVMorphs(Array<UV *> &morphs) const
{
    morphs.copy(m_context->uvs);
}

void Morph::getVertexMorphs(Array<Vertex *> &morphs) const
{
    morphs.copy(m_context->vertices);
}

void Morph::getFlipMorphs(Array<Flip *> &morphs) const
{
    morphs.copy(m_context->flips);
}

void Morph::getImpulseMorphs(Array<Impulse *> &morphs) const
{
    morphs.copy(m_context->impulses);
}

} /* namespace pmx */
} /* namespace VPVL2_VERSION_NS */
} /* namespace vpvl2 */
