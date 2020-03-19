#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <time.h>

using namespace rapidjson;

int main() {

    FILE* fp = fopen("resources/colors.json", "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document d;
    d.ParseStream(is);

    Value& s = (d["6"])["code"];
    std::cout << s["g"].GetInt() << std::endl;

    int highest = 6;
    int lowest = 0;
    int range = highest - lowest + 1;

    srand(static_cast<int>(time(0)));

    for(int i = 0; i < 7; i++){
        int color = rand() % 7;
        const char * color_char = std::to_string(color).c_str();
        std::cout << "Color: " << d[color_char]["color"].GetString() << std::endl;
        std::cout << "R: " << d[color_char]["code"]["r"].GetInt() << std::endl;
        std::cout << "G: " << d[color_char]["code"]["g"].GetInt() << std::endl;
        std::cout << "B: " << d[color_char]["code"]["b"].GetInt() << std::endl;

        /*
        std::cout << "Color: " << (d[std::to_string(v1)])["color"].GetString() << std::endl;
        std::cout << "R: " << (d[std::to_string(v1)])["code"]["r"].GetInt() << std::endl;
        std::cout << "G: " << (d[std::to_string(v1)])["code"]["g"].GetInt() << std::endl;
        std::cout << "B: " << (d[std::to_string(v1)])["code"]["b"].GetInt() << std::endl;
        */
    }

    fclose(fp);
    /*
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
    */
}
