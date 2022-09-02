#pragma once
#include <vector>
#include <string>

#include "../MAIN.h"

namespace Model {

	class MMD {
	public:
		MMD(const char* fileName);
		~MMD();

		void operator=(const MMD& right);

		void SetAnimSpeed(float speed);
		void SetAnimTime(float time);
		void SetLoopTime(float startTime, float endTime);
		int SetAttachIndex(int index);
		int SetRotate(VECTOR rotate);
		int SetScale(VECTOR scale);
		int SetPos(VECTOR pos);

		int GetAnimNum();
		int GetAttachIndex();

		int Draw(bool isAnimation);

		float GetTotalTime(int index);
	private:
		bool IsAnimLoop;
		bool IsVMDLoop;

		int AnimNum;
		int AttachIndex;
		int ModelHandle;

		float PlaySpeed;
		float PlayTime;
		float LoopBeginTime;
		float LoopEndTime;

		std::string FileName;

		std::vector<float> TotalTime;
	};

}