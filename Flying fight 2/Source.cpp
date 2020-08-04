#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<sstream>



using namespace sf;
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player { // класс Игрока
public:
	float x, y, w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	int dir = 0; //направление (direction) движения игрока

	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт
};

class updator : public Player {
public:
	virtual void move(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		case 4: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}
		x += dx * time;
		y += dy * time;
		if (x < 0) {
			x = 0;
		}
		if (x > 1280 - 171) {
			x = 1280 - 171;
		}
		if (y < 0) {
			y = 0;
		}
		if (y > 720 - 147) {
			y = 720 - 147;
		}
		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
	}
	virtual void move(float time, int type) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{

		switch (type)
		{
		case 0: x += 0 * time; y += 0.1 * time; break;
		case 1:x += 0 * time;
			y += 0.2 * time;
			if (y > 720) {
				y = -200;
				x = w + rand() % 1280 - w;
			}
			break;
		case 3: x += 0 * time; y -= 0.5 * time; break;
		}

		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.

	}

};

class plane : public updator {
public:
	plane(String F, float X, float Y, float W, float H) {  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту

		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная 	image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой

		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
	}
};

int main()
{

	int score = 0;
	RenderWindow window(sf::VideoMode(1280, 720), "Fire in the sky");
	Font font;//шрифт 
	font.loadFromFile("times-new-roman.TTF");
	Text text("", font, 30);

	SoundBuffer backmusic;
	if (!backmusic.loadFromFile("sound/backmusic.wav"))
		return -1;
	Sound sound0;
	sound0.setBuffer(backmusic);
	sound0.setVolume(20);
	sound0.play();

	SoundBuffer back;
	if (!back.loadFromFile("sound/back.wav"))
		return -1;
	Sound sound;
	sound.setBuffer(back);
	sound.setVolume(50);


	SoundBuffer mshot;
	if (!mshot.loadFromFile("sound/shot.wav"))
		return -1;
	Sound sound2;
	sound2.setBuffer(mshot);

	SoundBuffer mboom;
	if (!mboom.loadFromFile("sound/boom.wav"))
		return -1;
	Sound sound3;
	sound3.setBuffer(mboom);


	text.setFillColor(Color::Yellow);
	bool play = true;
	bool target = false;
	bool start = false;

	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;
	Clock clock2;
	srand(time(0));

	plane starter("start.png", 0, 0, 1280, 720);
	plane b1("back1.png", 0, 0, 1280, 723);
	plane b2("back2.png", 0, -720, 1280, 723);
	plane p("player.png", 600, 500, 171, 147);
	plane boom("boom.png", -172, -132, 172, 132);
	plane shot("shot.png", 0, 0, 136, 17);
	plane game_over("game_over.png", 0, 0, 1280, 720);

	plane p1("1.png", rand() % 1280 - 203, -600, 203, 165);
	plane p2("2.png", rand() % 1280 - 203, -200, 203, 166);
	plane p3("3.png", rand() % 1280 - 243, -400, 243, 124);
	plane p4("4.png", rand() % 1280 - 172, -800, 172, 147);
	plane p5("5.png", rand() % 1280 - 232, -1000, 232, 124);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			sound.play();
			start = true;
		}
		if (!start) {
			window.draw(starter.sprite);
			clock.restart();
		}
		if (start) {

			float time = clock.getElapsedTime().asMicroseconds();
			float time2 = clock2.getElapsedTime().asSeconds();

			clock.restart();
			time = time / 800;
			CurrentFrame += 0.05 * time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			p.sprite.setTextureRect(IntRect(171 * int(CurrentFrame), 0, 171, 147));


			p1.sprite.setTextureRect(IntRect(203 * int(CurrentFrame), 0, 203, 165));
			p2.sprite.setTextureRect(IntRect(203 * int(CurrentFrame), 0, 203, 166));
			p3.sprite.setTextureRect(IntRect(243 * int(CurrentFrame), 0, 243, 124));
			p4.sprite.setTextureRect(IntRect(172 * int(CurrentFrame), 0, 172, 147));
			p5.sprite.setTextureRect(IntRect(232 * int(CurrentFrame), 0, 232, 124));




			if (b1.y > 720) b1.y = -720;
			if (b2.y > 720) b2.y = -720;

			if ((Keyboard::isKeyPressed(Keyboard::Escape))) {
				sound.play();
				sound0.play();
				play = true;
				score = 0;
				p1.x = p1.w + rand() % 1280 - p1.w;
				p1.y = -200;
				p2.x = p2.w + rand() % 1280 - p2.w;
				p2.y = -200;
				p3.x = p3.w + rand() % 1280 - p3.w;
				p3.y = -200;
				p4.x = p4.w + rand() % 1280 - p4.w;
				p4.y = -200;
				p5.x = p5.w + rand() % 1280 - p5.w;
				p5.y = -200;

			}
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.2;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
			}
			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.2;//направление вправо, см выше
			}
			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.1;//направление вниз, см выше

			}
			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 2; p.speed = 0.15;//направление вверх, см выше
			}
			if (Keyboard::isKeyPressed(Keyboard::Space) && shot.y < 0) { //если нажата клавиша стрелка влево или англ буква А
				sound2.play();
				shot.x = p.x + 20;
				shot.y = p.y + 15;
			}




			if (play) {
				b1.move(time, 0);
				b2.move(time, 0);
				p.move(time);
				p1.move(time, 1);
				p2.move(time, 1);
				p3.move(time, 1);
				p4.move(time, 1);
				p5.move(time, 1);
				shot.move(time, 3);
			}

			window.draw(b1.sprite);
			window.draw(b2.sprite);

			window.draw(p.sprite);//рисуем спрайт объекта p класса player
			window.draw(p1.sprite);
			window.draw(p2.sprite);
			window.draw(p3.sprite);
			window.draw(p4.sprite);
			window.draw(p5.sprite);
			window.draw(shot.sprite);
			if (!play) {
				boom.sprite.setPosition(p.x, p.y);
				window.draw(boom.sprite);
				window.draw(game_over.sprite);

			}


			if ((p.x > p1.x&& p.x < p1.x + p1.w || p.x + p.w>p1.x&& p.x + p.w < p1.x + p1.w) && (p.y > p1.y&& p.y < p1.y + p1.h || p.y + p.h>p1.y&& p.y + p.h < p1.y + p1.h)) {
				sound3.play();
				play = false;
				p1.x = p1.w + rand() % 1280 - p1.w;
				p1.y = -200;

			}
			if ((p.x > p2.x&& p.x < p2.x + p2.w || p.x + p.w>p2.x&& p.x + p.w < p2.x + p2.w) && (p.y > p2.y&& p.y < p2.y + p2.h || p.y + p.h>p2.y&& p.y + p.h < p2.y + p2.h)) {
				sound3.play();
				play = false;
				p2.x = p2.w + rand() % 1280 - p2.w;
				p2.y = -200;
			}
			if ((p.x > p3.x&& p.x < p3.x + p3.w || p.x + p.w>p3.x&& p.x + p.w < p3.x + p3.w) && (p.y > p3.y&& p.y < p3.y + p3.h || p.y + p.h>p3.y&& p.y + p.h < p3.y + p3.h)) {
				sound3.play();
				play = false;
				p3.x = p3.w + rand() % 1280 - p3.w;
				p3.y = -200;
			}
			if ((p.x > p4.x&& p.x < p4.x + p4.w || p.x + p.w>p4.x&& p.x + p.w < p4.x + p4.w) && (p.y > p4.y&& p.y < p4.y + p4.h || p.y + p.h>p4.y&& p.y + p.h < p4.y + p4.h)) {
				sound3.play();
				play = false;
				p4.x = p4.w + rand() % 1280 - p4.w;
				p4.y = -200;
			}
			if ((p.x > p5.x&& p.x < p5.x + p5.w || p.x + p.w>p5.x&& p.x + p.w < p5.x + p5.w) && (p.y > p5.y&& p.y < p5.y + p5.h || p.y + p.h>p5.y&& p.y + p.h < p5.y + p5.h)) {
				sound3.play();
				play = false;
				p5.x = p5.w + rand() % 1280 - p5.w;
				p5.y = -200;
			}

			if ((shot.x > p1.x&& shot.x < p1.x + p1.w || shot.x + shot.w>p1.x&& shot.x + shot.w < p1.x + p1.w) && (shot.y > p1.y&& shot.y < p1.y + p1.h || shot.y + shot.h>p1.y&& shot.y + shot.h < p1.y + p1.h)) {
				score++;
				sound3.play();
				boom.x = p1.x;
				boom.y = p1.y + 20;

				target = true;
				shot.y = -100;
				p1.x = p1.w + rand() % 1280 - p1.w;
				p1.y = -200;

			}
			if ((shot.x > p2.x&& shot.x < p2.x + p2.w || shot.x + shot.w>p2.x&& shot.x + shot.w < p2.x + p2.w) && (shot.y > p2.y&& shot.y < p2.y + p2.h || shot.y + shot.h>p2.y&& shot.y + shot.h < p2.y + p2.h)) {
				score++;
				sound3.play();
				boom.x = p2.x;
				boom.y = p2.y + 20;
				target = true;
				shot.y = -100;
				p2.x = p2.w + rand() % 1280 - p2.w;
				p2.y = -200;

			}
			if ((shot.x > p3.x&& shot.x < p3.x + p3.w || shot.x + shot.w>p3.x&& shot.x + shot.w < p3.x + p3.w) && (shot.y > p3.y&& shot.y < p3.y + p3.h || shot.y + shot.h>p3.y&& shot.y + shot.h < p3.y + p3.h)) {
				score++;
				sound3.play();
				boom.x = p3.x;
				boom.y = p3.y + 20;
				target = true;
				shot.y = -100;
				p3.x = p3.w + rand() % 1280 - p3.w;
				p3.y = -200;

			}
			if ((shot.x > p4.x&& shot.x < p4.x + p4.w || shot.x + shot.w>p4.x&& shot.x + shot.w < p4.x + p4.w) && (shot.y > p4.y&& shot.y < p4.y + p4.h || shot.y + shot.h>p4.y&& shot.y + shot.h < p4.y + p4.h)) {
				sound3.play();
				score++;
				target = true;
				boom.x = p4.x;
				boom.y = p4.y + 20;

				shot.y = -100;
				p4.x = p4.w + rand() % 1280 - p4.w;
				p4.y = -200;

			}
			if ((shot.x > p5.x&& shot.x < p5.x + p5.w || shot.x + shot.w>p5.x&& shot.x + shot.w < p5.x + p5.w) && (shot.y > p5.y&& shot.y < p5.y + p5.h || shot.y + shot.h>p5.y&& shot.y + shot.h < p5.y + p5.h)) {
				score++;
				sound3.play();
				target = true;
				boom.x = p5.x;
				boom.y = p5.y + 20;

				shot.x = -1000;
				shot.y = -1000;
				p5.x = p5.w + rand() % 1280 - p5.w;
				p5.y = -200;

			}

			if (target) {
				boom.sprite.setPosition(boom.x, boom.y);
				window.draw(boom.sprite);
				if (time2 > 0.1)
					target = false;
			}
			if (!target) clock2.restart();

			std::ostringstream playerScoreString;    // объявили переменную
			playerScoreString << score;		//занесли в нее число очков, то есть формируем строку
			text.setString("Score:" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
			text.setPosition(10, 10);

			window.draw(text);//рисую этот текст
		}


		window.display();
	}
	return 0;
}