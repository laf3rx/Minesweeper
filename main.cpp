#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
using namespace sf;

int main()
{
	srand(time(0));
	RenderWindow app(VideoMode(400, 400), "Minesweeper");
	int width = 32;
	int gridLogic[12][12];
	int gridView[12][12];
	int mines_found = 0;
	bool end_game = false;
	int N = 12;
	int mines = N;
	int M = N;
	int flags = N;
	SoundBuffer buffer, buffer1, buffer2;
	buffer.loadFromFile("audio/boom.ogg");
	buffer1.loadFromFile("audio/pick.ogg");
	buffer2.loadFromFile("audio/win.ogg");
	Sound sound;
	Texture texture;
	texture.loadFromFile("images/image.jpg");
	Sprite s(texture);
	
	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++)
		{
			gridView[i][j] = 10;
			if ((rand() % 5 == 0)&&(M >0)) {
				gridLogic[i][j] = 9;
				M--;
			}
			else gridLogic[i][j] = 0;
		}
	}
	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			int n = 0;
			if (gridLogic[i][j] == 9) continue;
			if (gridLogic[i + 1][j] == 9) {n++;}
			if (gridLogic[i][j + 1] == 9) {n++;}
			if (gridLogic[i - 1][j] == 9) {n++;}
			if (gridLogic[i][j - 1] == 9) {n++;}
			if (gridLogic[i + 1][j + 1] == 9) {n++;}
			if (gridLogic[i - 1][j - 1] == 9) {n++;}
			if (gridLogic[i - 1][j + 1] == 9) {n++;}
			if (gridLogic[i + 1][j - 1] == 9) {n++;}
			gridLogic[i][j] = n;
		}
	while (app.isOpen())
	{
		Event e;

		while (app.waitEvent(e))
		{	
			if (end_game) {
				RenderWindow end(VideoMode(360, 60), "Game Over!");
				Font font;
				font.loadFromFile("C:\\Windows\\Fonts\\Calibrib.ttf");

				while (end.isOpen())
				{
					Text text("", font, 25);                                   
					Text record("", font, 25);
					if (mines_found != mines) { text.setString("The game is over! You lost!"); }           


					record.setString("Bombs found " + std::to_string(mines_found) + " from " + std::to_string(mines) + "!");                        

					text.setFillColor(Color::Black);
					text.setPosition(Vector2f(10, 5));

					record.setFillColor(Color::Black);
					record.setPosition(Vector2f(10, 25));

					end.draw(text);
					end.draw(record);
					Event a;
					while (end.pollEvent(a))
					{
						if (a.type == Event::Closed) { exit(0); };

					}
					end.clear(Color::White);
					end.draw(text);
					end.draw(record);
					end.display();
				}
			}
			Vector2i pos = Mouse::getPosition(app);
			int x = pos.x / width;
			int y = pos.y / width;

			if (e.type == Event::Closed)
				app.close();

			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{

					
					if (gridLogic[x][y] == 9) {
						
						for (int i = 1; i <= 10; i++) {
							for (int j = 1; j <= 10; j++)
							{
								gridView[i][j] = gridLogic[i][j];
								
							}
						}
						sound.setBuffer(buffer);
						sound.play();
						end_game = true;				
					}
					
					else {
						//printf("x: %d, y: %d\n", x, y);
						if (gridView[x][y] == 10) {
							sound.setBuffer(buffer1);
							sound.play();
							int random = 1 + rand() % 2;                                
							for (int s = x - random; s <= x + random; s++)             
							{
								for (int d = y - random; d <= y + random; d++)
								{
									int n = 1 + rand() % 4;                            
									if (n == 1 && gridLogic[s][d] != 9) gridView[s][d] = gridLogic[s][d];      
									n = 0;
								}
							}
						}
						gridView[x][y] = gridLogic[x][y];
					}

				}
				
				//printf("mines_found: %d\n", mines_found);
				if (mines_found == mines)
				{
					for (int i = 1; i <= 10; i++) {
						for (int j = 1; j <= 10; j++)
						{

							gridView[i][j] = gridLogic[i][j];
						}
					}
					sound.setBuffer(buffer2);
					sound.play();
					RenderWindow end(VideoMode(360, 60), "Game Over!");             
					Font font;                                                      
					font.loadFromFile("C:\\Windows\\Fonts\\Calibrib.ttf");         

					while (end.isOpen())
					{
						Text text("", font, 25);                                    
						Text record("", font, 25);
						text.setString("The game is over! You win!");
						

						record.setString("Bombs found " + std::to_string(mines_found) + " from " + std::to_string(mines) + "!");                       

						text.setFillColor(Color::Black);
						text.setPosition(Vector2f(10, 5));

						record.setFillColor(Color::Black);
						record.setPosition(Vector2f(10, 25));

						end.draw(text);
						end.draw(record);
						Event a;
						while (end.pollEvent(a))
						{
							if (a.type == Event::Closed) end.close();
						}
						end.clear(Color::White);
						end.draw(text);
						end.draw(record);
						end.display();
					}
				}	
				if ((e.key.code == Mouse::Right) && (flags > 0) &&(gridView[x][y] == 10)) {
					gridView[x][y] = 11;
					sound.setBuffer(buffer1);
					sound.play();
					if (gridLogic[x][y] == 9) {
						mines_found++;
					}
					flags--;
				}
				else {
					if ((e.key.code == Mouse::Right) && (gridView[x][y] == 11)) {
						sound.setBuffer(buffer1);
						sound.play();
						flags++;
						gridView[x][y] = 10;
						if (gridLogic[x][y] == 9) {
							mines_found--;
						}
					}
				}
			}
			
			app.clear(Color(92, 108, 135));

			for (int i = 1; i <= 10; i++)
				for (int j = 1; j <= 10; j++)
				{
					s.setTextureRect(IntRect(gridView[i][j] * width, 0, width, width));
					s.setPosition(i * width, j * width);
					app.draw(s);

				}
			app.display();
		}
	}
	return 0;
}
