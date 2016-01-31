//Using SDL and standard IO

#include "Engine/Scene.h"
#include "Engine/Engine.h"
#include "Engine/RTexture.h"
#include "SceneExamples.h"
#include "SceneFont.h"
#include "menuscene.h"
#include "level.h"
#include <stdio.h>
#include <string>


bool loadMedia() {
	//Loading success flag 
	bool success = true; 

	EngineInst->loadGlobalFont(GlobalFontPath, 18);
	//EngineInst->showFPS(true);
	
	success &= EngineInst->loadResources(globalTextures, globalTexturesSize);
//	gScene = new MainScene();
	// uncomment this to instead get scene with first preview of map
	if (!EngineInst->loadAudioResources(globalAudios, globalAudiosSize))
		success = false;

	globalAudios[BG_MUSIC].res.music->setVolume(0.2f);
	globalAudios[BG_MUSIC].res.music->play();
	return success;
}

int main( int argc, char* args[] )
{
	printf("argc %d\n", argc);

	int world = 0;
	int level = 0;

	if (argc == 3) {
		world = atoi(args[1]);
		level = atoi(args[2]);
	}

	MenuScene *scene = new MenuScene(world, level);

	if( !Engine::Create() )
	{
		printf( "Failed to initialize!\n" );
		PAUSE();
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
			PAUSE();
		}
		else
		{	
			EngineInst->setNextScene(scene);
			EngineInst->mainLoop();
		}
	}

	EngineInst->unLoadResources(globalTextures, globalTexturesSize);
	EngineInst->unLoadGlobalFont();
	Engine::Destroy();

	delete scene;
	return 0;
}
