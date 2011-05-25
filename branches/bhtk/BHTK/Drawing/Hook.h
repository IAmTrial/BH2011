#pragma once
#define EXPORTING
#include <list>
#include <Windows.h>

#include "../Exports.h"

namespace Drawing {
	// HookGroups allow use of the basic hooks(Line,Text,Box,Frame)
	//	in more advanced hooks(Input,UI,Button) that require
	//	multiple basic hooks.
	class Hook;
	class HookGroup {
		public:
			std::list<Hook*> Hooks;
			virtual unsigned int GetX() = 0;
			virtual unsigned int GetY() = 0;
			virtual unsigned int GetXSize() = 0;
			virtual unsigned int GetYSize() = 0;
			virtual bool IsActive() = 0;
	};

	enum HookVisibility {InGame,OutOfGame,Automap,Perm,Group};
	enum BoxTrans {BTThreeFourths, BTOneHalf, BTOneFourth, BTWhite, BTBlack, BTNormal, BTScreen, BTHighlight, BTFull};
	enum {None=0, Center=1, Right=2, Top=4};

	typedef std::list<Hook*> HookList;
	typedef std::list<Hook*>::iterator HookIterator;
	typedef bool (__cdecl *OnClick)(bool,Hook*,void*);

	class Hook {
		private:
			static HookList Hooks;//Holds a list of every basic hook used.
			HookVisibility visibility;//When we should show the hook.
			unsigned int x, y, z;//Hooks screen coordinates and the z-order.
			CRITICAL_SECTION crit;//Critical Section so we don't have race conditions.
			bool active;//Boolean to hold if we should draw the hook or not.
			int alignment;//Holds what type of alignment(if any) we should use.
			HookGroup* group;//Holds the group this hook is associated with.
			OnClick left;//Click callback handler for left clicking
			void* leftVoid;//Holds data to give back to the callback for things like knowing your proper class.
			OnClick right;//Click callback handler for right clicking
			void* rightVoid;//Holds data to give back to the callback for things like knowing your proper class.
		public:
			//Two Hook Initializations; one for basic hooks, one for grouped hooks.
			EXPORT Hook(HookVisibility visibility, unsigned int x, unsigned int y);
			EXPORT Hook(HookGroup* group, unsigned int x, unsigned int y);
			//~Hook();

			//Critical Section Helpers.
			EXPORT void Lock();
			EXPORT void Unlock();

			//Returns the x position of where the hook will be drawn.
			EXPORT unsigned int GetX();

			//Returns the base x position not calculating in groups or alignment.
			EXPORT unsigned int GetBaseX();

			//Sets the base x position, or offset from groups x position.
			EXPORT void SetBaseX(unsigned int xPos);

			//Returns the width of the hook, determine by super-class.
			EXPORT virtual unsigned int GetXSize() = 0;


			//Returns the y position of where the hook will be drawn.
			EXPORT unsigned int GetY();

			//Returns the base y position not calculating in groups or alignment.
			EXPORT unsigned int GetBaseY();

			//Sets the base y position, or offset from groups y position.
			EXPORT void SetBaseY(unsigned int yPos);

			//Returns the height of the hook, determine by super-class.
			EXPORT virtual unsigned int GetYSize() = 0;


			//Returns when the hook will be drawn compared to other hooks.
			EXPORT int GetZOrder();

			//Sets when the hook will be drawn compared to the other hooks.
			EXPORT void SetZOrder(int zPos);


			//Returns when the hook will be visible
			EXPORT HookVisibility GetVisibility();

			//Sets when the hook will be visible
			EXPORT void SetVisibility(HookVisibility newVisibility);


			//Returns if we are drawing the hook currently.
			EXPORT bool IsActive();

			//Sets if we should be drawing the hook.
			EXPORT void SetActive(bool newActive);


			//Returns how we are going to align the hook.
			EXPORT int GetAlignment();

			//Sets how we are to align the hook.
			EXPORT void SetAlignment(int newAlign);

			
			//Returns the hook's group.
			EXPORT HookGroup* GetGroup();

			//Sets the hook's group.
			EXPORT void SetGroup(HookGroup* newGroup);

			//Returns the callback handler for left clicks
			EXPORT OnClick GetLeftClickHandler();

			//Return the callback void handler for left clicks
			EXPORT void* GetLeftClickVoid();

			//Set the callback for left clicks
			EXPORT void SetLeftCallback(OnClick leftHandler, void* voidVar);


			//Returns the callback handler for right clicks
			EXPORT OnClick GetRightClickHandler();

			//Return the callback void handler for right clicks
			EXPORT void* GetRightClickVoid();

			//Set the callback for right clicks
			EXPORT void SetRightCallback(OnClick rightHandler, void* voidVar);

			//Determine if the given x/y set is within the hooks drawing area.
			EXPORT bool InRange(unsigned int x, unsigned int y);

			//This is the function in super-class we actually draw the function.
			virtual void OnDraw() = 0;

			//Function gets called when someone clicks, return true to block the click.
			virtual bool OnLeftClick(bool up, unsigned int x, unsigned int y) { return false; };
			virtual bool OnRightClick(bool up, unsigned int x, unsigned int y) { return false; };

			//Function gets called when someone types, return true to block the input.
			virtual bool OnKey(bool up, BYTE key, LPARAM lParam) { return false; };


			//Static function to draw all the hooks with the given visibility.
			static void Draw(HookVisibility type);

			//Static function to check if we interacted with any hooks.
			static bool LeftClick(bool up, unsigned int x, unsigned int y);
			static bool RightClick(bool up, unsigned int x, unsigned int y);
			static bool KeyClick(bool bUp, BYTE bKey, LPARAM lParam);

			//Misc Hook Functions needed
			EXPORT static unsigned int GetScreenHeight();
			EXPORT static unsigned int GetScreenWidth();
			EXPORT static void ScreenToAutomap(POINT* ptPos, int x, int y);
			EXPORT static void AutomapToScreen(POINT* ptPos, int x, int y);

	};
};