#pragma once
#include "CollisionVolume.h"

namespace NCL {
    class CapsuleVolume : public CollisionVolume
    {
    public:
        CapsuleVolume(float halfHeight, float radius) {
            this->halfHeight    = halfHeight;
            this->radius        = radius;
            this->type          = VolumeType::Capsule;
        };
        ~CapsuleVolume() {

        }
        float GetRadius() const {
            return radius;
        }

        float GetHalfHeight() const {
            return halfHeight;
        }

        void SetHalfHeight(float newHeight) {
            halfHeight = newHeight;
        }

        void SetRadius(float newRadius) {
            radius = newRadius;
        }

        void PrintDebugInfo(int& currLine, float lineSpacing) const override;


    protected:
        float radius;
        float halfHeight;
    };
}

