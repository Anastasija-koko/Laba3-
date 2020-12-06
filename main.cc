/*
 * main.cc
 *
 *  Created on: 12 окт. 2020 г.
 *      Author: unyuu
 */

/* Задание:
 * 1. Используя функции SDL (SDL_RenderDraw... и SDL_RenderFill...) нарисовать:
 * 		- поверхность земли
 * 		- траву
 * 		- дом с крышей, окном, дверью и трубой
 * 		- на заднем плане - забор (опционально)
 *
 * 2. Реализовать рисование окружности с использованием алгоритма Брезенхэма
 *
 * 3. Нарисовать у домика круглое окно на чердаке.
 *
 *
 * Пакет с SDL2 для MSYS2:
 * 		mingw-w64-x86_64-SDL2		- 64-битный
 * 		mingw-w64-i686-SDL2			- 32-битный
 */


#include <iostream>

#include <memory>
/*
 * std::shared_ptr - разделяемый указатель
 * std::unique_ptr - уникальный указатель
 * std::weak_ptr - "слабый" указатель
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>

#include <cmath>
constexpr double Pi = acos(-1.);

constexpr int WIDTH = 1500, HEIGHT = 720;

constexpr int foxW = 348 / 2, foxH = 348 / 2;//киса
int foxX0 = 20, foxY0 = HEIGHT - foxH;

int superX0 = 0;
int superDX = 6;

int wait = -20;//для танцующей лисы
void DrawCircle(SDL_Renderer * ren, int cx, int cy, int r)
{
	// тут алгоритм Брезенхэма
	int x = 0;
	int y = r;
	int delta = 1 - 2 * r;
	int error = 0;
	while (y >= 0){
		SDL_RenderDrawPoint(ren, cx + x, cy + y);
		SDL_RenderDrawPoint(ren, cx + x, cy - y);
		SDL_RenderDrawPoint(ren, cx - x, cy + y);
		SDL_RenderDrawPoint(ren, cx - x, cy - y);
		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
			delta += 2* ++x +1;
		else if ((delta > 0) && (error > 0))
			delta -= 2* --y + 1;
		else
			delta += 2 * (++x - --y);
	}
}
void DrawGrass(SDL_Renderer * ren, int number, int x, int y, int height[]){
	SDL_SetRenderDrawColor(ren, 64, 92, 52, 255);
	for (int i = 0; i < number; i++){
		for (int j = 0; j < height[i]; j++){
			SDL_RenderDrawLine(ren, x + height[i] / 2 - j, y + j, x + height[i] / 2 + j, y + j);
		}
		x += height[i];
	}
}

int main(int, char **){
	std::cout << "Начало работы программы" << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);

	auto window = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow("Это окно нашего приложения",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					WIDTH, HEIGHT,
					SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);
	if (window == nullptr) {
		std::cerr << "Ошибка при создании окна: " <<
				SDL_GetError() << std::endl;
		return 1;
	}

	auto ren = std::shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(window.get(), -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDL_DestroyRenderer);
	if (ren == nullptr) {
		std::cerr << "Ошибка при создании рендерера: " <<
				SDL_GetError() << std::endl;
		return 1;
	}


	auto foxImage = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox.png"), SDL_DestroyTexture);
	if (foxImage == nullptr){
		std::cerr << "Не могу загрузить картинку: " <<
				SDL_GetError() <<std::endl;
		return 1;
	}

	auto danceImage1 = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox_dance1.png"), SDL_DestroyTexture);
	if (danceImage1 == nullptr){
			std::cerr << "Не могу загрузить картинку: " <<
					SDL_GetError() <<std::endl;
			return 1;
		}
	auto danceImage2 = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox_dance2.png"), SDL_DestroyTexture);
	if (danceImage2 == nullptr){
			std::cerr << "Не могу загрузить картинку: " <<
					SDL_GetError() <<std::endl;
			return 1;
		}

	auto supermanImage1 = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox_superman1.png"), SDL_DestroyTexture);
	if (supermanImage1 == nullptr){
		std::cerr << "Не могу загрузить картинку: " <<
				SDL_GetError() <<std::endl;
		return 1;
	}
	auto supermanImage2 = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox_superman2.png"), SDL_DestroyTexture);
	if (supermanImage2 == nullptr){
		std::cerr << "Не могу загрузить картинку: " <<
				SDL_GetError() <<std::endl;
		return 1;
	}
	auto foxHome = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "fox1.png"), SDL_DestroyTexture);
	if (foxHome == nullptr){
		std::cerr << "Не могу загрузить картинку: " <<
				SDL_GetError() <<std::endl;
		return 1;
	}
	auto treeImage = std::shared_ptr <SDL_Texture>(IMG_LoadTexture(ren.get(), "tree.png"), SDL_DestroyTexture);
	if (treeImage == nullptr){
		std::cerr << "Не могу загрузить картинку: " <<
				SDL_GetError() <<std::endl;
		return 1;
	}

	auto keys = SDL_GetKeyboardState(nullptr);// ключи
	SDL_Event event;
	bool finish = false;
	// Основной цикл программы
	while (not finish) {
		// Цикл обработки событий
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) // Если окно закрыли...
				finish = true;
		}


				SDL_SetRenderDrawColor(ren.get(), 117, 234, 252, 255);
				SDL_RenderClear(ren.get());

				SDL_Rect trava { 0, 330, WIDTH, HEIGHT-330};
				SDL_SetRenderDrawColor(ren.get(), 134, 193, 86, 255);
				SDL_RenderFillRect(ren.get(), &trava);

				//забор
				constexpr int zabW = 30;
				constexpr int zabH = 150;
				constexpr int zabY = HEIGHT / 2 -zabH / 2;
				constexpr int dist = zabW + 60;
				SDL_SetRenderDrawColor(ren.get(), 169, 136, 49, 255);
				for (int i = 0; i < WIDTH; i += dist){
					SDL_Rect zabRect { i, zabY, zabW, zabH};
					SDL_RenderFillRect(ren.get(), &zabRect);
				}

				constexpr int domW = 600;
				constexpr int domH = 250;
				SDL_Rect domRect { WIDTH/2-domW/2, HEIGHT/2 - domH / 2, domW, domH};
				SDL_SetRenderDrawColor(ren.get(), 220, 161, 108, 255);
				SDL_RenderFillRect(ren.get(), &domRect);
				//сваи
				constexpr int floorW = 680;
				constexpr int floorH = 50;
				SDL_Rect floorRect { WIDTH/2-floorW/2, HEIGHT/2 + domH / 2, floorW, floorH};
				SDL_SetRenderDrawColor(ren.get(), 224, 182, 132, 255);
				SDL_RenderFillRect(ren.get(), &floorRect);
				constexpr int svaiW = 40;
				constexpr int svaiH = 50;
				SDL_Rect svaiRect1 { WIDTH/2-floorW/2 + 20, HEIGHT/2 + domH / 2 + floorH, svaiW, svaiH};
				SDL_RenderFillRect(ren.get(), &svaiRect1);
				SDL_Rect svaiRect2 { WIDTH/2+floorW/2 - 20 - svaiW, HEIGHT/2 + domH / 2 + floorH, svaiW, svaiH};
				SDL_RenderFillRect(ren.get(), &svaiRect2);

				constexpr int roof1 = 390;
				SDL_SetRenderDrawColor(ren.get(),184, 68, 22, 255);
				for (int i = 290; i< roof1; i += 3){
					SDL_RenderDrawLine(ren.get(), WIDTH/2-i, HEIGHT/2 - domH / 2 - roof1 + i,
												WIDTH/2+i,  HEIGHT/2- domH / 2 - roof1 + i);
					SDL_RenderDrawLine(ren.get(), WIDTH/2-i, HEIGHT/2 - domH / 2 - roof1 + i+1,
																WIDTH/2+i,  HEIGHT/2- domH / 2 - roof1 + i+1);
					SDL_RenderDrawLine(ren.get(), WIDTH/2-i, HEIGHT/2 - domH / 2 - roof1 + i+2,
																	WIDTH/2+i,  HEIGHT/2- domH / 2 - roof1 + i+2);
				}
				constexpr int roof2 = 390;
				for (int i = 0, j = 0; i< roof2; i += 3, j++){
					SDL_RenderDrawLine(ren.get(), WIDTH/2-i, HEIGHT/2 - domH / 2 - 100 - roof2/3 + j,
												WIDTH/2+i,  HEIGHT/2- domH / 2 - 100 - roof2/3+ j);
				}

				SDL_SetRenderDrawColor(ren.get(), 184, 197, 209, 255);//круглое окно
				constexpr int circle = 30;
				constexpr int stripe = 10;
				for (int i = circle; i > 0; i--)
					DrawCircle(ren.get(), WIDTH/2, HEIGHT/2 - domH / 2 - 100 - roof2/3/2, i);
				SDL_SetRenderDrawColor(ren.get(), 115, 34, 7, 255);
				for (int i = circle + stripe, j = 0; i > circle; i--, j++){
					DrawCircle(ren.get(), WIDTH/2, HEIGHT/2 - domH / 2 - 100 - roof2/3/2, i);
					SDL_RenderDrawLine(ren.get(),WIDTH/2 - stripe/2 + j, HEIGHT/2 - domH / 2 - 100 - roof2/3/2 - circle,
												WIDTH/2 - stripe/2 +j, HEIGHT/2 - domH / 2 - 100 - roof2/3/2 + circle);
					SDL_RenderDrawLine(ren.get(),WIDTH/2 - circle, HEIGHT/2 - domH / 2 - 100 - roof2/3/2 - stripe/2 +j,
											WIDTH/2 + circle, HEIGHT/2 - domH / 2 - 100 - roof2/3/2 - stripe/2 + j);
				}

				//огромные двери
				constexpr int doorsW = domW - 70;
				constexpr int doorsH = domH - 60;
				constexpr int doorsX = WIDTH/2-domW/2 + 70/2;
				constexpr int doorsY = HEIGHT / 2 - domH/2 + 60/2;
				SDL_SetRenderDrawColor(ren.get(), 223, 187, 148, 255);
				SDL_Rect doorsRect{ doorsX, doorsY, doorsW, doorsH};
				SDL_RenderFillRect(ren.get(), &doorsRect);
				SDL_SetRenderDrawColor(ren.get(), 218, 137, 66, 255);
				constexpr int stripeDoors = 8;
				int step1 = doorsW / 5;
				int step2 = doorsH / 5;
				for (int i = 0 ; i < stripeDoors; i ++){
					SDL_RenderDrawLine(ren.get(), doorsX - stripeDoors, doorsY - stripeDoors + i,
												doorsX + doorsW, doorsY - stripeDoors + i);
					SDL_RenderDrawLine(ren.get(), doorsX - stripeDoors, doorsY - stripeDoors + doorsH + i,
												doorsX + doorsW, doorsY - stripeDoors + doorsH + i);
					SDL_RenderDrawLine(ren.get(), doorsX - stripeDoors + i, doorsY - stripeDoors,
												doorsX - stripeDoors + i, doorsY + doorsH);
					SDL_RenderDrawLine(ren.get(), doorsX - stripeDoors + doorsW + i, doorsY - stripeDoors,
												doorsX - stripeDoors + doorsW +i, doorsY + doorsH);
				}
				int stripe1X0 = doorsX + doorsW / 5;
				int stripe2X0 = doorsX + doorsW / 10;
				int stripe1Y0 = doorsY + doorsH / 5;
				for (int i = 0; i < 5; i++){
					if (i < 4)
						for (int j = 0; j < stripeDoors; j++){
							SDL_RenderDrawLine(ren.get(),stripe1X0 + j, doorsY,
														stripe1X0 + j, doorsY + doorsH);
						}
					for (int j = 0; j < stripeDoors/2; j++){
						SDL_RenderDrawLine(ren.get(),stripe2X0 + j, doorsY,
													stripe2X0 + j, doorsY + doorsH);
						SDL_RenderDrawLine(ren.get(),doorsX, stripe1Y0 + j,
													doorsX + doorsW, stripe1Y0 + j);
					}
					stripe1X0 += step1;
					stripe2X0 += step1;
					stripe1Y0 += step2;
				}

				SDL_SetRenderDrawColor(ren.get(),115, 34, 7, 255);
				stripe1X0 = WIDTH / 2 - domW / 2 -20;
				step1 = step1 - 36;
				for (int i = 0; i < 10; i++){
					for (int j = 0; j < stripeDoors + 3; j++)
						SDL_RenderDrawLine(ren.get(),stripe1X0 + j, HEIGHT / 2 - domH / 2,
													stripe1X0 + j, HEIGHT / 2 + domH / 2 + 5);
					stripe1X0 += step1;
				}

				constexpr int danceW = 300, danceH = 300;
				constexpr int danceX = WIDTH - 300, danceY = HEIGHT - 500;

				SDL_Rect danceRect{danceX, danceY, danceW, danceH};
				if(wait < 0){
					wait ++;
					SDL_RenderCopy(ren.get(), danceImage2.get(),	nullptr, &danceRect);
				}
				else if (wait < 20){
					wait++;
					SDL_RenderCopy(ren.get(), danceImage1.get(),	nullptr, &danceRect);
				}
				else wait = -20;

				constexpr int treeW = 1876 * 0.4, treeH = 1536 * 0.4;
				constexpr int treeX = -90, treeY = 0;
				SDL_Rect treeRect{treeX, treeY, treeW, treeH};
				SDL_RenderCopy(ren.get(), treeImage.get(),	nullptr, &treeRect);

				constexpr int superW = 800 / 4, superH = 725 / 4;
				superX0 += superDX;
				if(superX0 >= WIDTH - superW or superX0 < 0){
					superDX = -superDX;

				}
				SDL_Rect superRect{superX0, 10, superW, superH};
				if (superDX > 0) SDL_RenderCopy(ren.get(), supermanImage2.get(),	nullptr, &superRect);
				else SDL_RenderCopy(ren.get(), supermanImage1.get(),	nullptr, &superRect);

				constexpr int fox1W = 564 * 0.3, fox1H = 603 * 0.3;
				SDL_Rect fox1Rect{WIDTH / 2 + 100, HEIGHT / 2 + domH / 2,fox1W, fox1H};
				if (foxX0 > 600 and foxX0 < 1100) SDL_RenderCopy(ren.get(), foxHome.get(),	nullptr, &fox1Rect);

				constexpr int leftBorderW = WIDTH/2-domW/2-foxH/2;
				constexpr int rightBorderW = WIDTH/2+domW/2;
				constexpr int zaborBorder = HEIGHT/2;
				constexpr int upB = HEIGHT/2;
				constexpr int pace = 8;
				SDL_Rect foxRect{foxX0, foxY0,foxW, foxH};
				SDL_RenderCopy(ren.get(), foxImage.get(), nullptr, &foxRect);
				if ((keys[SDL_SCANCODE_RIGHT] and ((foxX0 <= WIDTH - foxW and foxX0 >= rightBorderW)
						or foxX0 <= leftBorderW
						or (foxX0 <= WIDTH - foxW and foxY0 >= upB)))){
					foxX0 += pace;
				}
				if (keys[SDL_SCANCODE_LEFT]and ((foxX0 >= 0 and foxX0 <= leftBorderW)
						or foxX0 >= rightBorderW
						or (foxX0 >= 0 and foxY0 >= upB)
						or (foxY0 >= upB+pace and foxX0 >= treeX + treeW - 200))){
					foxX0 -= pace;
				}
				if (keys[SDL_SCANCODE_UP] and ((foxY0 >= upB+pace and foxX0 >= treeX + treeW - 200)
						or (foxX0 <= leftBorderW and foxY0 >= treeY + treeH - 150)
						or (foxX0 >= rightBorderW and foxY0 >= zaborBorder))){
					foxY0 -= pace;
				}
				if (keys[SDL_SCANCODE_DOWN] and
						(foxY0 <= HEIGHT-foxH*.66)){
					foxY0 += pace;
				}


				/*
				 * Двойная буферизация:
				 * 1 буфер (передний) - буфер, содержимое которого отображается.
				 * 2 буфер (задний) - буфер, в котором осуществляется построение изображения.
				 *
				 */
				SDL_RenderPresent(ren.get());
			}


	std::cout << "Конец работы программы" << std::endl;
	return 0;

}
