#include <any>
#include <iostream>

#include "Wrapper.h"
#include "Engine.h"
#include "TextEditor.h"

int main()
{
    TextEditor editor("../ex.txt");
    Engine engine;

    Wrapper wrapper1(&editor, &TextEditor::addSentenceNumbering, {});
    engine.registerCommand(&wrapper1, "addSentenceNumbering");
    engine.execute("addSentenceNumbering", {});

    Wrapper wrapper2(&editor, &TextEditor::findPhrase, {{"word", std::string("Bye")}});
    engine.registerCommand(&wrapper2, "findPhrase");
    engine.execute("findPhrase", {{"word", std::string("Bye")}}); // TODO return a function result

    Wrapper wrapper3(&editor, &TextEditor::toUpperSentences, {});
    engine.registerCommand(&wrapper3, "toUpperSentences");
    engine.execute("toUpperSentences", {});

    Wrapper wrapper4(&editor, &TextEditor::toUpperWords, {});
    engine.registerCommand(&wrapper4, "toUpperWords");
    engine.execute("toUpperWords", {});

    Wrapper wrapper5(&editor, &TextEditor::replaceWord, {{"oldWord", std::string("Bye")}, {"newWord", std::string("Hi")}});
    engine.registerCommand(&wrapper5, "replaceWord");
    engine.execute("replaceWord", {{"oldWord", std::string("Bye")}, {"newWord", std::string("Hi")}});

}
