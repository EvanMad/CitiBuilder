#ifndef GUI_HPP
#define GUI_HPP

#include <vector>
#include <utility>
#include <string>

#include <SFML/Graphics.hpp>

class GuiStyle
{
public:
	sf::Color bodyCol;
	sf::Color bodyHighlightCol;
	sf::Color borderCol;
	sf::Color borderHighlightCol;
	sf::Color textCol;
	sf::Color textHighlightCol;
	sf::Font* font;

	float borderSize;

	GuiStyle(sf::Font* font, float borderSize, sf::Color bodyCol, sf::Color borderCol, sf::Color textCol, sf::Color bodyHighlightCol, sf::Color borderHighlighCol, sf::Color textHighlightCol)
	{
		this->bodyCol = bodyCol;
		this->borderCol = borderCol;
		this->textCol = textCol;
		this->bodyHighlightCol = bodyHighlightCol;
		this->borderHighlightCol = borderHighlighCol;
		this->textHighlightCol = textHighlightCol;
		this->font = font;
		this->borderSize = borderSize;
	}
	GuiStyle(){}
};

class GuiEntry
{
public:
	sf::RectangleShape shape;
	std::string message;
	sf::Text text;
	GuiEntry(const std::string& message, sf::RectangleShape shape, sf::Text text)
	{
		this->message = message;
		this->shape = shape;
		this->text = text;
	}
	GuiEntry() {}
};

class Gui : public sf::Transformable, public sf::Drawable
{
private:
	bool horizontal;
	GuiStyle style;
	sf::Vector2f dimensions;
	int padding;
public:
	std::vector<GuiEntry> entries;
	bool visible;

	Gui(sf::Vector2f dimensions, int padding, bool horizontal, GuiStyle& style, std::vector<std::pair<std::string, std::string>> entries)
	{
		visible = false;
		this->horizontal = horizontal;
		this->style = style;
		this->dimensions = dimensions;
		this->padding = padding;

		sf::RectangleShape shape;
		shape.setSize(dimensions);
		shape.setFillColor(style.bodyCol);
		shape.setOutlineThickness(-style.borderSize);
		shape.setOutlineColor(style.borderCol);

		for (auto entry : entries)
		{
			sf::Text text;
			text.setString(entry.first);
			text.setFont(*style.font);
			text.setFillColor(style.textCol);
			text.setCharacterSize(dimensions.y - style.borderSize - padding);
			this->entries.push_back(GuiEntry(entry.second, shape, text));
		}
	}

	sf::Vector2f getSize();
	int getEntry(const sf::Vector2f mousePos);
	void setEntryText(int entry, std::string text);
	void setDimensions(sf::Vector2f dimensions);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void show();
	void hide();

	void highlight(const int entry);

	std::string activate(const int entry);
	std::string activate(const sf::Vector2f mousePos);
};

#endif // GUI_HPP