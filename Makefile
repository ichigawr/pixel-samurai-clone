default:
	g++ -I src/include -L src/lib -o horizon_s_fall horizon_s_fall.res ECS/ECS.cpp AssetManager.cpp Enemies.cpp Game.cpp main.cpp Map.cpp SoundManager.cpp TextureManager.cpp Vector2D.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	./horizon_s_fall

run:
	./horizon_s_fall
