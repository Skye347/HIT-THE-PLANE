简单打飞机
===============================

真的很简单
-------------------------------
并没有写多少= =因为只在课上写所以只完成了基本功能= =只有700多行

功能
-------------------------------
上下左右键移动，空格开炮
--BUG：目前发现左上无法同时移动 原因未知
有关卡设置、根据分数提升关卡
--可设置玩家子弹移动速度、间隔时间、电脑飞机移动速度、出现间隔、生命值、子弹速度与时间间隔
玩家血量30，被电脑飞机撞上扣10血，被子弹击中扣1血

环境
-------------------------------
Windows10
Visual Studio 2015 RC (vc++14)
SFML 2.3

### main()代码
int main()
{
	sf::RenderWindow window(sf::VideoMode(480, 800), "SFML works!",sf::Style::Close);
//load field
	Area field;
//load field end

//load shapes
	Plane mainPlane;
	mainPlane.addToArea(&field);
	field.PlaneAdd(&mainPlane);
	//init position
	mainPlane.setPlane(1);
//load shapes end

//load backgroud
	sf::Texture backgroud;
	if (!backgroud.loadFromFile("background1.png"))
		std::cout << "error" << std::endl;
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroud);
//

//load fonts and text
	sf::Font font;
	if (!font.loadFromFile("consola.ttf")) {
		std::cout << "load error" << std::endl;
	}
	sf::Text LifeText;
	sf::Text ScoreText;
	sf::Text levelText;
	LifeText.setFont(font);
	ScoreText.setFont(font);
	levelText.setFont(font);
	std::ostringstream life;
	std::ostringstream score;
	std::ostringstream level;
	LifeText.setCharacterSize(24);
	LifeText.setColor(sf::Color::Black);
	//LifeText.setPosition(200, 200);
	ScoreText.setCharacterSize(24);
	ScoreText.setColor(sf::Color::Black);
	ScoreText.setPosition(LifeText.getPosition().x, LifeText.getPosition().y+30);
	levelText.setCharacterSize(24);
	levelText.setColor(sf::Color::Black);
	levelText.setPosition(LifeText.getPosition().x, LifeText.getPosition().y + 60);

//
//load audio
	sf::SoundBuffer background_audio_buffer;
	//if (!background_audio_buffer.loadFromFile("test_b.flac")){
	//	std::cout << "load test_b.flac failed" << std::endl;
	//}
	//sf::Sound background_audio;
	//background_audio.setBuffer(background_audio_buffer);
	//background_audio.play();
//load audio end

//run
	window.setFramerateLimit(300);
	HANDLE backThread = CreateThread(NULL, 0, &thread2, &field, NULL, NULL);
	HANDLE levelThread = CreateThread(NULL, 0, &levelThread_f, &field, NULL, NULL);
	while (window.isOpen())
	{
		sf::Event event;
		//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			//move
			if (event.type == sf::Event::KeyPressed){
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
					mainPlane.moveXStart(LEFT);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
					mainPlane.moveXStart(RIGHT);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
					mainPlane.moveYStart(UP);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
					mainPlane.moveYStart(DOWN);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
					mainPlane.fire();
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Space) {
					mainPlane.fireStop();
				}
				if (event.key.code == sf::Keyboard::Right) {
					mainPlane.moveXStop();
				}
				if (event.key.code == sf::Keyboard::Left) {
					mainPlane.moveXStop();
				}
				if (event.key.code == sf::Keyboard::Up) {
					mainPlane.moveYStop();
				}
				if (event.key.code == sf::Keyboard::Down) {
					mainPlane.moveYStop();
				}
			}
			//move end
		}
		window.clear();
		window.draw(backgroundSprite);
		if (field.gameStatus == 1) {
			field.bulletUpdate(&window, &field);
			field.PlaneUpdate(&window);
			life.clear();
			life.str("");
			score.str("");
			score.clear();
			level.str("");
			level.clear();
			life << "Life:" << mainPlane.Life;
			LifeText.setString(life.str());
			score << "Score:" << mainPlane.score;
			ScoreText.setString(score.str());
			level << "Level:" << field.Level;
			levelText.setString(level.str());
			window.draw(levelText);
			window.draw(LifeText);
			window.draw(ScoreText);
		}
		else if (field.gameStatus == 0) {
			std::cout << "game over" << std::endl;
			sf::Sprite Over;
			sf::Texture OverTexture;
			OverTexture.loadFromFile("gameover.png");
			Over.setTexture(OverTexture);
			window.draw(Over);
		}
		window.display();
	}
//run end
	TerminateThread(backThread,1);
	TerminateThread(levelThread, 1);
	return 0;
}
