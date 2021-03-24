#pragma once

#include"../../Common/Vector3.h"
#include"../Audio/External/inc/fmod_common.h"

namespace NCL {
	namespace CSC8508 {

		namespace Audio {

			struct SoundListener {

				int ID;
				FMOD_VECTOR pos;
				FMOD_VECTOR vel;
				FMOD_VECTOR forward;
				FMOD_VECTOR up;


				SoundListener(int id);
				~SoundListener() {}
				void UpdateAttributes(const Maths::Vector3& position,
					const Maths::Vector3& velocity = Maths::Vector3(0, 0, 0),
					const Maths::Vector3& _forward = Maths::Vector3(0, 0, -1),
					const Maths::Vector3& _up = Maths::Vector3(0, 1, 0));


			};
		}
	}
}