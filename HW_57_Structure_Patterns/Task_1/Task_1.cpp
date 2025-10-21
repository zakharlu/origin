#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class SimpleText
{
public:
    virtual void render(const string& text)
    {
        cout << text;
    }
};

class DecoratedText : public SimpleText
{
public:
    SimpleText* __text;
    DecoratedText(SimpleText* text) : __text(text) {}
};

class ItalicText : public DecoratedText
{
public:
    ItalicText(SimpleText* text) : DecoratedText(text) {}
    void render(const string& text) override
    {
        cout << "<i>";
        __text->render(text);
        cout << "</i>";
    }
};

class BoldText : public DecoratedText
{
public:
    BoldText(SimpleText* text) : DecoratedText(text) {}
    void render(const string& text) override
    {
        cout << "<b>";
        __text->render(text);
        cout << "</b>";
    }
};

class Paragraph : public DecoratedText
{
public:
    Paragraph(SimpleText* text) : DecoratedText(text) {}
    void render(const string& text) override
    {
        cout << "<p>";
        __text->render(text);
        cout << "</p>";
    }
};

class Reversed : public DecoratedText
{
public:
    Reversed(SimpleText* text) : DecoratedText(text) {}
    void render(const string& text) override
    {
        string reversed_text = text;
        reverse(reversed_text.begin(), reversed_text.end());
        __text->render(reversed_text);
    }
};

class Link : public DecoratedText
{
public:
    Link(SimpleText* text) : DecoratedText(text) {}
    void render(const string& href, const string& text)
    {
        cout << "<a href=" << href << ">";
        __text->render(text);
        cout << "</a>";
    }
};

int main()
{
    SimpleText text1;
    text1.render("Hello HTML");
    cout << endl;

    auto italic_text = new ItalicText(&text1);
    auto bold_text = new BoldText(&text1);
    italic_text->render("Italic HTML");
    cout << endl;
    bold_text->render("Bold HTML");
    cout << endl;

    // Новые декораторы
    auto paragraph = new Paragraph(&text1);
    paragraph->render("Hello world");
    cout << endl;

    auto reversed = new Reversed(&text1);
    reversed->render("Hello world");
    cout << endl;

    auto link = new Link(&text1);
    link->render("netology.ru", "Hello world");
    cout << endl;

    return 0;
}