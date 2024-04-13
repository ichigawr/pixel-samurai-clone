default:
	g++ -I src/include -L src/lib -o horizon_s_fall main.cpp Game.cpp TextureManager.cpp Map.cpp Vector2D.cpp Collision.cpp ECS/ECS.cpp AssetManager.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	./horizon_s_fall

run:
	./horizon_s_fall
