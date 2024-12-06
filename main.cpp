#include <any>
#include <iostream>

#include "Wrapper.h"
#include "Engine.h"
#include "TextEditor.h"

class Subject {
public:
    int x;
    int y;
    Subject(int x, int y) : x(x), y(y) {}

    int f3 (int x1, int y1) {
        std::cout << "x = " << x << " x1 = " << x1 << " y = " << y << " y1 = " << y1 << std::endl;
        x += x1;
        y += y1;
        std::cout << x << ' ' << y << std::endl;
        return x + y;
    }

    void f4 (int x1, int y1) {
        std::cout << "x = " << x << " x1 = " << x1 << " y = " << y << std::endl;
        //return 11;
    }
};

//int main()
//{
//    Subject subj = Subject(1, 2);
//    Wrapper wrapper(&subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}});
//    Engine engine;
//    engine.registerCommand(&wrapper, "command1");
//    engine.execute("command1", {{"arg1", 4}, {"arg2", 5}});
//}

int main()
{
    TextEditor editor("../ex.txt");
    auto res = editor.findPhrase(std::string("Bye"));
    Wrapper wrapper2(&editor, &TextEditor::findPhrase, {{"oldWord", std::string("Bye")}});
    Engine engine;
    engine.registerCommand(&wrapper2, "findPhrase");
    engine.execute("findPhrase", {{"oldWord", std::string("Bye")}});


//    Wrapper wrapper3(&editor, &TextEditor::toUpperSentences, {}); //TODO: wrap function without arguments
//    engine.registerCommand(&wrapper3, "toUpperSentences");
//    engine.execute("toUpperSentences", {});

    Wrapper wrapper(&editor, &TextEditor::replaceWord, {{"oldWord", std::string("Bye")}, {"newWord", std::string("Hi")}});
    engine.registerCommand(&wrapper, "replaceWord");
    engine.execute("replaceWord", {{"oldWord", std::string("Bye")}, {"newWord", std::string("Hi")}});

}
