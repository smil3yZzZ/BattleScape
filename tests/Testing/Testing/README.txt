Compile v1.0:
g++ -o main -lglfw -lglew -framewoenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp -Iinclude -std=c++2a

Compile v1.01:
g++ -o main -lglfw -lglew -framework OpenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp utils/TextureAsset.cpp utils/TextureUtils.cpp DrawingObject.cpp Texture.cpp -Iinclude -std=c++2a

Compile v1.02:
g++ -o main -lglfw -lglew -framework OpenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp utils/TextureAsset.cpp utils/TextureUtils.cpp DrawingObject.cpp Texture.cpp camera/Camera.cpp -Iinclude -std=c++2a

Compile v1.03:
g++ -o main -lglfw -lglew -framework OpenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp utils/TextureAsset.cpp utils/TextureUtils.cpp DrawingObject.cpp Texture.cpp camera/Camera.cpp drawings/PlatformsDrawingObject.cpp drawings/WallsDrawingObject.cpp -Iinclude -std=c++2a

Execute:
./main
