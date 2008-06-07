#include "Event_Manager.h"
#include "HImage.h"
#include "Init.h"
#include "Local.h"
#include "SGP.h"
#include "Screens.h"
#include "Strategic_Movement_Costs.h"
#include "Sys_Globals.h"
#include "VObject.h"
#include "RenderWorld.h"
#include "MouseSystem.h"
#include "WorldDef.h"
#include "Animation_Data.h"
#include "Overhead.h"
#include "Font_Control.h"
#include "Timer_Control.h"
#include "Radar_Screen.h"
#include "Render_Dirty.h"
#include "Sound_Control.h"
#include "Lighting.h"
#include "Cursor_Control.h"
#include "Music_Control.h"
#include "Video.h"
#include "MapScreen.h"
#include "Interface_Items.h"
#include "Strategic.h"
#include "Dialogue_Control.h"
#include "Text.h"
#include "Laptop.h"
#include "NPC.h"
#include "MercTextBox.h"
#include "Tile_Cache.h"
#include "StrategicMap.h"
#include "Map_Information.h"
#include "Exit_Grids.h"
#include "Summary_Info.h"
#include "GameSettings.h"
#include "Game_Init.h"
#include "Strategic_Movement.h"
#include "Vehicles.h"
#include "Shading.h"
#include "VSurface.h"

#if defined JA2BETAVERSION && defined JA2EDITOR
#	include "EditScreen.h"
#	include "JAScreens.h"
#endif


// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

#ifdef JA2BETAVERSION
extern	BOOLEAN	gfUseConsecutiveQuickSaveSlots;
#endif


ScreenID InitializeJA2(void)
try
{
	gfWorldLoaded = FALSE;

	// Load external text
	LoadAllExternalText();

	gsRenderCenterX = 805;
	gsRenderCenterY = 805;

	// Init animation system
	InitAnimationSystem();

	// Init lighting system
	InitLightingSystem();

	// Init dialog queue system
	InitalizeDialogueControl();

	InitStrategicEngine();

	//needs to be called here to init the SectorInfo struct
	InitStrategicMovementCosts( );

	// Init tactical engine
	InitTacticalEngine();

	// Init timer system
	//Moved to the splash screen code.
	//InitializeJA2Clock( );

	// INit shade tables
	BuildShadeTable( );

	// INit intensity tables
	BuildIntensityTable( );

	// Init Event Manager
	InitializeEventManager();

	// Initailize World
	InitializeWorld();

	InitTileCache( );

	InitMercPopupBox( );

	// Set global volume
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR

	//UNCOMMENT NEXT LINE TO ALLOW FORCE UPDATES...
	//LoadGlobalSummary();
	if( gfMustForceUpdateAllMaps )
	{
		ApologizeOverrideAndForceUpdateEverything();
	}
	#endif
#endif

#ifdef JA2BETAVERSION
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if ( strcmp( gzCommandLine, "-DOMAPS" ) == 0 )
	{
		return( MAPUTILITY_SCREEN );
	}
#endif

#ifdef JA2BETAVERSION
	//This allows the QuickSave Slots to be autoincremented, ie everytime the user saves, there will be a new quick save file
	if (strcasecmp(gzCommandLine, "-quicksave") == 0)
	{
		gfUseConsecutiveQuickSaveSlots = TRUE;
	}
#endif

#if defined JA2BETAVERSION && defined JA2EDITOR
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if( !strcmp( gzCommandLine, "-EDITORAUTO" ) )
	{
		DebugMsg(TOPIC_JA2EDITOR, DBG_LEVEL_1, "Beginning JA2 using -EDITORAUTO commandline argument...");
		//For editor purposes, need to know the default map file.
		strcpy(g_filename, "none");
		//also set the sector
		gWorldSectorX = 0;
		gWorldSectorY = 0;
		gfAutoLoadA9 = TRUE;
		gfIntendOnEnteringEditor = TRUE;
		gGameOptions.fGunNut = TRUE;
		return( GAME_SCREEN );
	}
	if ( strcmp( gzCommandLine, "-EDITOR" ) == 0 )
	{
		DebugMsg(TOPIC_JA2EDITOR, DBG_LEVEL_1, "Beginning JA2 using -EDITOR commandline argument...");
		//For editor purposes, need to know the default map file.
		strcpy(g_filename, "none");
		//also set the sector
		gWorldSectorX = 0;
		gWorldSectorY = 0;
		gfAutoLoadA9 = FALSE;
		gfIntendOnEnteringEditor = TRUE;
		gGameOptions.fGunNut = TRUE;
		return( GAME_SCREEN );
	}
#endif

	return( INIT_SCREEN );
}
catch (...) { return ERROR_SCREEN; }


void ShutdownJA2(void)
{
  UINT32 uiIndex;

	FRAME_BUFFER->Fill(Get16BPPColor(FROMRGB(0, 0, 0)));
	InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	RefreshScreen();

	ShutdownStrategicLayer();

	// remove temp files built by laptop
	ClearOutTempLaptopFiles( );

	// Shutdown queue system
	ShutdownDialogueControl();

  // Shutdown Screens
  for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
  {
		void (*const shutdown)(void) = GameScreens[uiIndex].ShutdownScreen;
		if (shutdown != NULL) shutdown();
  }


	ShutdownLightingSystem();

	CursorDatabaseClear();

	ShutdownTacticalEngine( );

	// Shutdown Overhead
	ShutdownOverhead( );

	DeInitAnimationSystem();

	DeinitializeWorld( );

	DeleteTileCache( );

	ShutdownJA2Clock( );

	ShutdownFonts();

	ShutdownJA2Sound( );

	ShutdownEventManager( );

	// Unload any text box images!
	RemoveTextMercPopupImages( );

	ClearOutVehicleList();
}
