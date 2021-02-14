Compile:
g++ -o main -lglfw -lglew -framewoenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp -Iinclude -std=c++2a

Compile now:
g++ -o main -lglfw -lglew -framework OpenGL main.cpp Engine.cpp utils/SquareProperties.cpp utils/Input.cpp utils/Shader.cpp utils/TextureAsset.cpp utils/TextureUtils.cpp DrawingObject.cpp Texture.cpp -Iinclude -std=c++2a

Execute:
./main
