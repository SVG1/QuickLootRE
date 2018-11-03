#pragma once

#include <vector>  // vector
#include <string>  // string

#include "Utility.h"


namespace QuickLootRE
{
	class ISetting
	{
	public:
		ISetting(std::string a_key) : _key(a_key) {}
		virtual ~ISetting() {}

		virtual	void				Assign(bool a_val)			= 0;
		virtual	void				Assign(int a_val)			= 0;
		virtual	void				Assign(float a_val)			= 0;
		virtual void				Dump()						= 0;
		inline const std::string&	key()				const	{ return _key; };

	protected:
		std::string	_key;
	};


	static std::vector<ISetting*> settings;


	class bSetting : public ISetting
	{
	public:
		bSetting(std::string a_key, bool a_value) : ISetting(a_key), _value(a_value) { settings.push_back(this); }
		virtual ~bSetting() {}

		inline				operator bool()		const				{ return _value; }
		inline virtual void	Assign(bool a_val)			override	{ _value = a_val; }
		inline virtual void	Assign(int a_val)			override	{ _value = a_val != 0; }
		inline virtual void	Assign(float a_val)			override	{ _value = !(a_val > -1.0 && a_val < 1.0); }
		inline virtual void Dump()						override	{ _DMESSAGE("%s: %s", _key.c_str(), boolToString(_value).c_str()); }

	protected:
		bool _value;
	};


	class iSetting : public ISetting
	{
	public:
		iSetting(std::string a_key, SInt32 a_value) : ISetting(a_key), _value(a_value) { settings.push_back(this); }
		virtual ~iSetting() {}

		inline				operator SInt32()	const				{ return _value; }
		inline virtual void	Assign(bool a_val)			override	{ _value = static_cast<int>(a_val); }
		inline virtual void	Assign(int a_val)			override	{ _value = a_val; }
		inline virtual void	Assign(float a_val)			override	{ _value = a_val; }
		inline virtual void Dump()						override	{ _DMESSAGE("%s: %i", _key.c_str(), _value); }

	protected:
		SInt32 _value;
	};


	class fSetting : public ISetting
	{
	public:
		fSetting(std::string a_key, float a_value) : ISetting(a_key), _value(a_value) { settings.push_back(this); }
		virtual ~fSetting() {}

		inline				operator float()	const				{ return _value; }
		inline virtual void	Assign(bool a_val)			override	{ _value = a_val ? 1.0 : 0.0; }
		inline virtual void	Assign(int a_val)			override	{ _value = a_val; }
		inline virtual void	Assign(float a_val)			override	{ _value = a_val; }
		inline virtual void Dump()						override	{ _DMESSAGE("%s: %i", _key.c_str(), _value); }

	protected:
		float _value;
	};


	class Settings
	{
	public:
		static void		loadSettings();


		static bSetting	disableInCombat;
		static bSetting	disableTheft;
		static bSetting	disablePickPocketing;
		static bSetting	disableIfEmpty;
		static bSetting	disableSingleLoot;
		static bSetting	playAnimations;
		static iSetting itemLimit;
		static fSetting	scale;
		static fSetting	positionX;
		static fSetting	positionY;
		static fSetting	opacity;

		static bool		isApplied;

	private:
		Settings() {}
		~Settings() {}


		static const char* FILE_NAME;
	};
}
