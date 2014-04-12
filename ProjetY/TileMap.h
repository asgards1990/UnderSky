#pragma once

#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const char* texturePath, int tileWidth, int tileHeight, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height);
	bool isLoaded()const{return m_vertices.getVertexCount()!=0;}

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states)const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};