#include "init.h"

#include <windows.h>

#include "lau/lau.h"
#include "util.h"
#include "sigscan/sigscan.h"
#include "classes/structures.h"
#include "classes/panelwrapper.h"
#include "classes/matsystemsurface.h"
#include "vtable.h"
#include "hooks.h"

using namespace util;

EngineVersion version = GARRYSMOD;

unsigned long font;

void csgolua::init() {
	version = GetEngineVersion(GetAppID());

	structs.client = GetInterface<CClient *>("client.dll", "VClient");

	hook::Init_Hooks();

	structs.trace = GetInterface<CEngineTrace *>("engine.dll", "EngineTraceClient");
	structs.engine = GetInterface<CEngineClient *>("engine.dll", "VEngineClient");
	structs.entity_list = GetInterface<CClientEntityList *>("client.dll", "VClientEntityList");

	structs.prediction = GetInterface<CPrediction *>("client.dll", "VClientPrediction");

	structs.game_movement = GetInterface<CGameMovement *>("client.dll", "GameMovement");

	structs.globals = **(CGlobalVarsBase ***)(getvfunc(structs.client, 11) + (version == CSGO ? 0xA : 5));

	structs.model_info = GetInterface<CModelInfo *>("engine.dll", "VModelInfoClient");
	structs.render = GetInterface<CStudioRenderContext *>("studiorender.dll", "VStudioRender");
	structs.framework = GetInterface<CToolFrameworkInternal *>("engine.dll", "VENGINETOOLFRAMEWORK");
	structs.panel = GetInterface<VPanelWrapper *>("vgui2.dll", "VGUI_Panel");
	structs.surface = GetInterface<CMatSystemSurface *>("vguimatsurface.dll", "VGUI_Surface");
	structs.debug = GetInterface<CIVDebugOverlay *>("engine.dll", "VDebugOverlay");
	structs.materials = GetInterface<CMaterialSystem *>("materialsystem.dll", "VMaterialSystem");
	structs.engine_vgui = GetInterface<CEngineVGui *>("engine.dll", "VEngineVGui");
	structs.event_manager = GetInterface<CGameEventManager *>("engine.dll", "GAMEEVENTSMANAGER");
	structs.sprops = GetInterface<CPhysicsSurfaceProps *>("vphysics.dll", "VPhysicsSurfaceProps");
	structs.tools = GetInterface<CClientTools *>("client.dll", "VCLIENTTOOLS");
	structs.local = GetInterface<CLocalize *>("localize.dll", "Localize_");
	if (!structs.local) MessageBoxA(0, "nope", ":(", 0);

	//can be found with an xref to scripts/vgui_screens.txt

	if (version == GARRYSMOD)
	{
		structs.client_mode = **(ClientModeShared ***)((char *)(SigScan(
			"\xE8????"
			"\xA3????" // here
			"\xE8????"
			"\x8B\x10"
			"\x8B\xC8"
			"\x8B\x02"
			"\x68????"
			"\xFF\xD0"
			"\xC3",
			GetModuleHandleA("client.dll")
			)) + 0x6);
	}
	else if (version == CSGO)
	{
		auto addr = SigScan(
			"\xA1????"
			"\xC7\x05????????" // first dword of ?
			"\xA8\x01"
			"\x75?"
			"\x83\xC8\x01"
			"\xA3????"
			"\xE8????"
			"\x68????"
			"\xE8????"
			"\x83\xC4\x04"
			"\xA1????"
			"\xB9????"
			"\x68????"
			"\xFF\x10"
			"\xC3",
			GetModuleHandleA("client.dll")
			);
		structs.client_mode = **(ClientModeShared ***)((char *)(addr)+0x7);
	}

	structs.surface->SetFontGlyphSet(structs.surface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);

	structs.L = new Lau();

	structs.L->Init();
}