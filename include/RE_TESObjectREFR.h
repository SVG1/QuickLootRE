#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameBSExtraData.h"  // BaseExtraList
#include "skse64/GameExtraData.h"  // ExtraOwnership
#include "skse64/GameFormComponents.h"  // ActorWeightModel
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameObjects.h"  // TESNPC
#include "skse64/NiExtraData.h"  // BSFaceGenAnimationData
#include "skse64/NiNodes.h"  // BSFaceGenNiNode, NiNode
#include "skse64/NiTypes.h"  // NiPoint3
#include "skse64/GameReferences.h"  // TESObjectREFR


namespace RE
{
	class TESObjectREFR : public ::TESForm
	{
	public:
		enum { kTypeID = kFormType_Reference };


		enum RemoveType : UInt32
		{
			kRemoveType_Remove,
			kRemoveType_Steal,
			kRemoveType_Trade,
			kRemoveType_Drop,
			kRemoveType_Take,
			kRemoveType_Unk05
		};


		virtual void					Unk_39(void);
		virtual void					Unk_3A(void);
		virtual void					Unk_3B(void);
		virtual void					Unk_3C(void);
		virtual void					Unk_3D(void);
		virtual void					Unk_3E(void);
		virtual void					Unk_3F(void);
		virtual void					Unk_40(void);
		virtual void					Unk_41(void);
		virtual void					Unk_42(void);
		virtual void					Unk_43(void);
		virtual void					Unk_44(void);
		virtual void					Unk_45(void);
		virtual void					Unk_46(void);
		virtual void					Unk_47(void);
		virtual void					Unk_48(void);
		virtual void					Unk_49(void);
		virtual void					Unk_4A(void);
		virtual void					Unk_4B(void);
		virtual void					Unk_4C(void);
		virtual void					Unk_4D(void);
		virtual void					Unk_4E(void);
		virtual void					Unk_4F(void);
		virtual void					Unk_50(void);
		virtual void					GetStartingPos(::NiPoint3 * pos);
		virtual void					Unk_52(void);
		virtual void					Unk_53(void);
		virtual void					Unk_54(void);
		virtual void					Unk_55(void);
		virtual void					RemoveItem(UInt32* droppedItemHandle, ::TESForm* akItem, UInt32 aiCount, RemoveType mode, ::BaseExtraList* extraList, ::TESObjectREFR* moveToRef, UInt32 unk7, UInt32 unk8);
		virtual void					Unk_57(void);
		virtual void					Unk_58(void);
		virtual void					Unk_59(void);
		virtual void					Unk_5A(void);
		virtual void					GetMarkerPosition(::NiPoint3 * pos);
		virtual void					Unk_5C(void);
		virtual void					Unk_5D(void);
		virtual void					Unk_5E(void);
		virtual void					Unk_5F(void);
		virtual void					Unk_60(void);
		virtual BSFaceGenNiNode*		GetFaceGenNiNode(void);
		virtual void					Unk_62(void);
		virtual BSFaceGenAnimationData*	GetFaceGenAnimationData(void);
		virtual void					Unk_64(void);
		virtual void					Unk_65(void);
		virtual void					Unk_66(void);
		virtual void					Unk_67(void);
		virtual void					Unk_68(void);
		virtual void					Unk_69(void);
		virtual void					Unk_6A(void);
		virtual void					Unk_6B(void);
		virtual void					Unk_6C(void);
		virtual void					SetNiNode(::NiNode * root, UInt32 unk1);  // NULL, 1?
		virtual void					Unk_6E(void);
		virtual NiNode*					GetNiRootNode(UInt32 firstPerson);
		virtual NiNode*					GetNiNode(void);  // Root of the skeleton (Get3D)
		virtual void					Unk_71(void);
		virtual void					Unk_72(void);
		virtual void					Unk_73(void);
		virtual void					Unk_74(void);
		virtual void					Unk_75(void);
		virtual void					Unk_76(void);
		virtual void					Unk_77(void);
		virtual void					Unk_78(void);
		virtual void					Unk_79(void);
		virtual void					Unk_7A(void);
		virtual void					Unk_7B(void);
		virtual void					Unk_7C(void);
		virtual void					Unk_7D(void);
		virtual ActorWeightModel*		GetWeightModel(UInt32 weightModel);  // 0 Small 1 Large
		virtual void					Unk_7F(void);
		virtual void					Unk_80(void);
		virtual void					Unk_81(void);
		virtual void					Unk_82(void);
		virtual void					Unk_83(void);
		virtual void					Unk_84(void);
		virtual void					Unk_85(void);
		virtual void					Unk_86(void);
		virtual void					Unk_87(void);
		virtual void					Unk_88(void);
		virtual void					Unk_89(void);
		virtual void					ResetInventory(bool unk);
		virtual void					Unk_8B(void);
		virtual void					Unk_8C(void);
		virtual void					Unk_8D(void);
		virtual void					Unk_8E(void);
		virtual void					Unk_8F(void);
		virtual void					Unk_90(void);
		virtual void					Unk_91(void);
		virtual void					Unk_92(void);
		virtual void					Unk_93(void);
		virtual void					Unk_94(void);
		virtual void					Unk_95(void);
		virtual void					Unk_96(void);
		virtual void					Unk_97(void);
		virtual void					Unk_98(void);
		virtual bool					IsDead(UInt8 unk1);  // unk1 = 1 for Actors
		virtual void					Unk_9A(void);
		virtual void					Unk_9B(void);


		struct LoadedState
		{
			UInt8	todo[0x68];	// 00
			NiNode*	node;		// 68
			// ... probably more
		};


		// parents
		BSHandleRefObject						handleRefObject;	// 20
		BSTEventSink <BSAnimationGraphEvent>	animGraphEventSink;	// 30
		IAnimationGraphManagerHolder			animGraphHolder;	// 38

		// members
		TESForm*								baseForm;			// 40
		NiPoint3								rot;				// 48
		NiPoint3								pos;				// 54

		TESObjectCELL*							parentCell;			// 60
		LoadedState*							loadedState;		// 68
		BaseExtraList							extraData;			// 70
		UInt64									unk88;				// 88 - New in SE
		UInt16									unk90;				// 90 - init'd to 100
		UInt8									unk92;				// 92
		UInt8									unk93;				// 93
		UInt32									pad94;				// 94


		UInt32									CreateRefHandle(void);
		TESNPC*									GetActorOwner(void) const;


		MEMBER_FN_PREFIX(TESObjectREFR);
	};
	STATIC_ASSERT(offsetof(TESObjectREFR, extraData) == 0x70);
	STATIC_ASSERT(offsetof(TESObjectREFR, loadedState) == 0x68);
	STATIC_ASSERT(offsetof(TESObjectREFR::LoadedState, node) == 0x68);


	UInt32 TESObjectREFR::CreateRefHandle(void)
	{
		return reinterpret_cast<::TESObjectREFR*>(this)->CreateRefHandle();
	}


	TESNPC* TESObjectREFR::GetActorOwner(void) const
	{
		ExtraOwnership* exOwnership = extraData.GetByType<ExtraOwnership>();
		if (exOwnership && exOwnership->owner && exOwnership->owner->Is(FormType::Character)) {
			return (TESNPC*)exOwnership->owner;
		}
		return nullptr;
	}
};
