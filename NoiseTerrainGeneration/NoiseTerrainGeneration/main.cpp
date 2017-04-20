#include <iostream>
#include <noise/noise.h>
#include <SFML/Graphics.hpp>


class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	
	void generateMap(int tileArr[], const int raw, const int col, noise::module::Perlin myModule) {
		double noiseValue;
		for (int i = 0; i < raw; i++) {
			for (int j = 0; j < col; j++) {
				noiseValue = myModule.GetValue(0.1 * i, 0.1*j, 0.1);
				if (noiseValue < -0.5)
					tileArr[i*col + j] = 2;
				else if (noiseValue >= -0.5 && noiseValue < 0)
					tileArr[i*col + j] = 0;
				else if (noiseValue >= 0 && noiseValue < 0.5)
					tileArr[i*col + j] = 1;
				else if (noiseValue >= 0.5)
					tileArr[i*col + j] = 3;

				std::cout << noiseValue << '\n';
			}

		}
	}
	void keepGeneratingMap(int tileArr[], const int raw, const int col, noise::module::Perlin myModule, int z) {
		double noiseValue;
		
		for (int i = 0; i < raw; i++) {
			for (int j = 0; j < col; j++) {
				noiseValue = myModule.GetValue(0.1 * i, 0.1*j, 0.1*z);

				if (noiseValue < -0.5)
					tileArr[i*col + j] = 2;
				else if (noiseValue >= -0.5 && noiseValue < 0)
					tileArr[i*col + j] = 0;
				else if (noiseValue >= 0 && noiseValue < 0.5)
					tileArr[i*col + j] = 1;
				else if (noiseValue >= 0.5)
					tileArr[i*col + j] = 3;

				std::cout << noiseValue << '\n';
			}

		}
	}

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
	{
		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
			for (unsigned int j = 0; j < height; ++j)
			{
				// get the current tile number
				int tileNumber = tiles[i + j * width];

				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}

		return true;
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

int main()
{
	//std::vector<std::vector<int>> tileArr;
	const int raw = 24;
	const int col = 24;
	int tileArr[raw * col];
	int seed = 0;
	sf::Clock clock;
	float elapsedTime = 0;
	//std::vector< int >tileArr;

	
	noise::module::Perlin myModule;
	/*
	std::cout << myModule.GetValue(0.5, 0.5, 0.5) << std::endl;
	system("pause");
	return 0;
	*/
	// create the window
	sf::RenderWindow window(sf::VideoMode(raw*32, col*32), "Tilemap");
	
	/*
	// define the level with an array of tile indices
	 int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
	};
	*/
	int z = 0;
	TileMap map;
	map.generateMap(tileArr, raw, col, myModule);
	
	if (!map.load("tileset.png", sf::Vector2u(32, 32), tileArr, raw, col))
		return -1;
	myModule.SetSeed(++seed);
	// create the tilemap from the level definition
	
	if (!map.load("tileset.png", sf::Vector2u(32, 32), tileArr, raw, col))
		return -1;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				map.generateMap(tileArr, raw, col, myModule);
				if (!map.load("tileset.png", sf::Vector2u(32, 32), tileArr, raw, col))
					return -1;
				myModule.SetSeed(++seed);
			}



		}
		elapsedTime = clock.getElapsedTime().asSeconds();
		if (elapsedTime > 3) {
			map.keepGeneratingMap(tileArr, raw, col, myModule, ++z);
			if (!map.load("tileset.png", sf::Vector2u(32, 32), tileArr, raw, col))
				return -1;
			clock.restart();
		}
		// draw the map
		window.clear();
		window.draw(map);
		window.display();
	}

	return 0;
}
