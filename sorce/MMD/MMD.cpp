
#include "MMD.h"

namespace Model {
	MMD::MMD(const char* fileName = nullptr)
		:IsAnimLoop(true),
		IsVMDLoop(false),
		AnimNum(-1),
		AttachIndex(-1),
		ModelHandle(-1),
		PlaySpeed(0.5f),
		PlayTime(0.0f),
		LoopBeginTime(0.0f),
		LoopEndTime(0.0f)
	{
		FileName = fileName;

		if (fileName) {
			ModelHandle = MV1LoadModel(fileName);
			AnimNum = MV1GetAnimNum(ModelHandle);
			for (int i = 0; i < AnimNum; i++) {
				int handle = MV1AttachAnim(ModelHandle, i, -1, TRUE);
				TotalTime.push_back(MV1GetAttachAnimTotalTime(ModelHandle, handle));
				MV1DetachAnim(ModelHandle, handle);
			}
			AttachIndex = MV1AttachAnim(ModelHandle, 0, -1, TRUE);
		}
	}
	MMD::~MMD()
	{
		MV1DeleteModel(ModelHandle);
	}
	void MMD::operator=(const MMD& right)
	{
		this->IsAnimLoop	= right.IsAnimLoop;
		this->IsVMDLoop		= right.IsVMDLoop;
		this->AnimNum		= right.AnimNum;
		this->AttachIndex	= right.AttachIndex;
		this->PlaySpeed		= right.PlaySpeed;
		this->LoopBeginTime = right.LoopBeginTime;
		this->LoopEndTime	= right.LoopEndTime;
		this->FileName		= right.FileName;
		this->TotalTime		= right.TotalTime;
		this->PlayTime		= 0.0f;
		this->ModelHandle	= MV1DuplicateModel(this->ModelHandle);
	}
	void MMD::SetAnimSpeed(float speed)
	{
		PlaySpeed = speed;
	}
	void MMD::SetAnimTime(float time)
	{
		PlayTime = time;
	}
	void MMD::SetLoopTime(float startTime, float endTime)
	{
		if (startTime < TotalTime[AttachIndex] && endTime < TotalTime[AttachIndex]) {
			if (startTime < endTime) {
				LoopBeginTime = startTime;
				LoopEndTime = endTime;
			}
		}
	}
	int MMD::SetAttachIndex(int index)
	{
		int result = -1;
		int attach = MV1AttachAnim(ModelHandle, index, -1, FALSE);

		if (attach != -1) {
			result = MV1DetachAnim(ModelHandle, AttachIndex);
			AttachIndex = attach;
		}

		return result;
	}
	int MMD::SetRotate(const VECTOR rotate)
	{
		return MV1SetRotationXYZ(ModelHandle, rotate);
	}
	int MMD::SetScale(VECTOR scale)
	{
		return MV1SetScale(ModelHandle, scale);
	}
	int MMD::SetPos(VECTOR pos)
	{
		return MV1SetPosition(ModelHandle, pos);
	}
	int MMD::GetAnimNum()
	{
		return AnimNum;
	}
	int MMD::GetAttachIndex()
	{
		return AttachIndex;
	}
	int MMD::Draw(bool isAnimation)
	{
		if (isAnimation) {
			PlayTime += PlaySpeed;

			if (LoopBeginTime < LoopEndTime) {
				if (PlayTime > LoopEndTime) {
					PlayTime = LoopBeginTime;
				}
			}
			else if (PlayTime > TotalTime[AttachIndex] && IsAnimLoop) {
				PlayTime = 0.0f;

				if (IsVMDLoop) {
					MV1DetachAnim(ModelHandle, AttachIndex);

					if (AttachIndex < AnimNum - 1) {
						AttachIndex++;
					}
					else {
						AttachIndex = 0;
					}
				}
			} // if (PlayTime > TotalTime[AttachIndex] && IsAnimLoop)

			MV1SetAttachAnimTime(ModelHandle, AttachIndex, PlayTime);
		} // if (IsAnim)

		return MV1DrawModel(ModelHandle);
	}
	float MMD::GetTotalTime(int index)
	{
		if (0 <= index && index < TotalTime.size()) {
			return TotalTime[index];
		}
		else {
			return 0.0f;
		}
	}
}