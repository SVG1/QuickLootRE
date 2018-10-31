#pragma once

#include "skse64/ScaleformLoader.h"  // GFxLoader

#include "RE/GFxMovieView.h"  // RE::GFxMovieView::ScaleModeType


namespace RE
{
	class GFxStateBag;
	class IMenu;


	class GFxLoader
	{
	public:
		UInt32				unk00;				// 00
		GFxStateBag*		stateBag;			// 04
		UInt32				unk08;				// 08
		UInt32				unk0C;				// 0C
		BSScaleformImageLoader*	imageLoader;	// 10

		inline static GFxLoader* GetSingleton()	{ return reinterpret_cast<GFxLoader*>(::GFxLoader::GetSingleton()); }

		inline bool LoadMovie(IMenu* menu, GFxMovieView*& viewOut, const char* swfName, GFxMovieView::ScaleModeType mode, float backGroundAlpha)
		{
			::GFxMovieView* view = reinterpret_cast<::GFxMovieView*>(viewOut);
			bool result = CALL_MEMBER_FN(reinterpret_cast<::GFxLoader*>(this), LoadMovie)(reinterpret_cast<::IMenu*>(menu), &view, swfName, mode, backGroundAlpha);
			viewOut = reinterpret_cast<GFxMovieView*>(view);
			return result;
		}
	};
}
