#pragma once

enum Dll { D2CLIENT=0,D2COMMON,D2GFX,D2LANG,D2WIN,D2NET,D2GAME,D2LAUNCH,FOG,BNCLIENT, STORM, D2CMP, D2MULTI, D2MCPCLIENT};
enum PatchType { Jump=0, Call };

class IPatch {
	public:
		virtual bool Install () = 0;
		virtual bool Remove () = 0;

		virtual bool IsInstalled() = 0;
};