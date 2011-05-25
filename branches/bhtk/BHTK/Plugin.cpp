#include "Plugin.h"
#include "PluginManager.h"
#include "BHTK.h"

Plugin::Plugin(std::wstring name, std::wstring author, double version) : name(name), author(author), version(version) {
	loaded = false;
}

Plugin::~Plugin() {
}

void Plugin::Load() {
	if (IsLoaded())
		return;

	// Hook up all the events
	
	__hook(&PluginManager::OnDraw, BHTK::pluginManager, &Plugin::OnDraw, this);
	__hook(&PluginManager::OnAutomapDraw, BHTK::pluginManager, &Plugin::OnAutomapDraw, this);
	__hook(&PluginManager::OnOOGDraw, BHTK::pluginManager, &Plugin::OnOOGDraw, this);

	//__hook(&PluginManager::OnGameJoin, BHTK::pluginManager, &Plugin::OnGameJoin, this);
	__hook(&PluginManager::OnGameExit, BHTK::pluginManager, &Plugin::OnGameExit, this);

	__hook(&PluginManager::OnLoop, BHTK::pluginManager, &Plugin::OnLoop, this);

	__hook(&PluginManager::OnLeftClick, BHTK::pluginManager, &Plugin::OnLeftClick, this);
	__hook(&PluginManager::OnRightClick, BHTK::pluginManager, &Plugin::OnRightClick, this);
	__hook(&PluginManager::OnKey, BHTK::pluginManager, &Plugin::OnKey, this);

	__hook(&PluginManager::OnChatPacketRecv, BHTK::pluginManager, &Plugin::OnChatPacketRecv, this);
	__hook(&PluginManager::OnRealmPacketRecv, BHTK::pluginManager, &Plugin::OnRealmPacketRecv, this);
	__hook(&PluginManager::OnGamePacketRecv, BHTK::pluginManager, &Plugin::OnGamePacketRecv, this);

	loaded = true;
	OnLoad();
}

void Plugin::Unload() {
	if (!IsLoaded())
		return;

	// Unhook all events
	__unhook(&PluginManager::OnDraw, BHTK::pluginManager, &Plugin::OnDraw, this);
	__unhook(&PluginManager::OnAutomapDraw, BHTK::pluginManager, &Plugin::OnAutomapDraw, this);
	__unhook(&PluginManager::OnOOGDraw, BHTK::pluginManager, &Plugin::OnOOGDraw, this);

	//__unhook(&PluginManager::OnGameJoin, BHTK::pluginManager, &Plugin::OnGameJoin, this);
	__unhook(&PluginManager::OnGameExit, BHTK::pluginManager, &Plugin::OnGameExit, this);

	__unhook(&PluginManager::OnLoop, BHTK::pluginManager, &Plugin::OnLoop, this);

	__unhook(&PluginManager::OnLeftClick, BHTK::pluginManager, &Plugin::OnLeftClick, this);
	__unhook(&PluginManager::OnRightClick, BHTK::pluginManager, &Plugin::OnRightClick, this);
	__unhook(&PluginManager::OnKey, BHTK::pluginManager, &Plugin::OnKey, this);

	__unhook(&PluginManager::OnChatPacketRecv, BHTK::pluginManager, &Plugin::OnChatPacketRecv, this);
	__unhook(&PluginManager::OnRealmPacketRecv, BHTK::pluginManager, &Plugin::OnRealmPacketRecv, this);
	__unhook(&PluginManager::OnGamePacketRecv, BHTK::pluginManager, &Plugin::OnGamePacketRecv, this);

	loaded = false;
	OnUnload();
}