#pragma once

#include "skse64/NiObjects.h"


namespace RE
{
	class NiAVObject : public NiObjectNET
	{
	public:
		enum Flag : UInt32
		{
			kFlag_SelectiveUpdate				= 1 << 1,
			kFlag_UpdatePropertyControllers		= 1 << 2,
			kFlag_SelectiveUpdateRigid			= 1 << 4,
			kFlag_OverrideSelectiveTransforms	= 1 << 7,
		};


		struct ControllerUpdateContext
		{
			enum
			{
				kDirty = 1 << 0,
			};


			float	delta;
			UInt32	flags;
		};


		virtual void		UpdateControllers(ControllerUpdateContext* ctx);	// calls controller vtbl+0x8C
		virtual void		UpdateNodeBound(ControllerUpdateContext* ctx);

		// SE: one of these 4 functions was deleted. Until figuring out which one, assuming it was Unk_26
		virtual void		ApplyTransform(NiMatrix33* mtx, NiPoint3* translate, bool postTransform);
		virtual void		SetPropertyState(NiProperty* prop);
		virtual void		Unk_25(UInt32 arg0); // SE: function 29
		//virtual void	Unk_26(UInt32 arg0);

		virtual NiAVObject*	GetObjectByName(const char** name);	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
		virtual void		SetSelectiveUpdateFlags(bool* selectiveUpdate, bool selectiveUpdateTransforms, bool* rigid);
		virtual void		UpdateDownwardPass(ControllerUpdateContext* ctx, void* unk1); // SE: changed unk to void*
		virtual void		UpdateSelectedDownwardPass(ControllerUpdateContext* ctx, void* unk1); // SE: changed unk to void*
		virtual void		UpdateRigidDownwardPass(ControllerUpdateContext* ctx, void* unk1); // SE: changed unk to void*
		virtual void		UpdateWorldBound(void);
		virtual void		UpdateWorldData(ControllerUpdateContext* ctx);	// SE: function 30 (vtable+0x180)
		virtual void		UpdateNoControllers(ControllerUpdateContext* ctx);
		virtual void		UpdateDownwardPassTempParent(NiNode* parent, ControllerUpdateContext* ctx);
		virtual void		Unk_30(void);	// calls virtual function on parent - SE: function 33

		// SE: one of these functions was deleted. Doesn't matter which, they are unks, assuming Unk_32.
		virtual void		Unk_31(UInt32 arg0); // SE: function 34
		//virtual void	Unk_32(UInt32 arg0);

		void				UpdateNode(ControllerUpdateContext* ctx)	{ CALL_MEMBER_FN(reinterpret_cast<::NiAVObject*>(this), UpdateNode)(reinterpret_cast<::NiAVObject::ControllerUpdateContext*>(ctx)); }


		NiNode*				m_parent;			// 30
		UInt32				unk038;				// 38 - New in SE, init'd to FFFFFFFF
		UInt32				pad03C;				// 3C
		NiAVObject*			unk040;				// 40
		NiTransform			m_localTransform;	// 48
		NiTransform			unkTransform;;		// 7C - SE: this one is new
		NiTransform			m_worldTransform;	// B0
		float				unkE4;				// E4
		float				unkE8;				// E8
		float				unkEC;				// EC
		float				unkF0;				// F0
		UInt32				m_flags;			// F4 - bitfield
		float				unkF8;				// F8
		UInt32				unkFC;				// FC
		float				unk100;				// 100 - New in SE? init's to 1.0
		UInt32				unk104;				// 104 - New in SE? init'd to 0
		UInt8				unk108;				// 108
		UInt8				unk109;				// 109 - bitfield
		UInt8				pad10A[6];			// 10A
	};
}
