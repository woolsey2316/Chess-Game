#include "PromotionScreen.h"

PromotionScreen::PromotionScreen(std::string playerTurn)
{
  isWhite = (playerTurn == "WHITE" ? true : false);
  sf::RenderWindow promote(sf::VideoMode(300, 175), "Promote Pawn");
  piece = Run(promote);
}

std::string PromotionScreen::Run(sf::RenderWindow &promote)
{
  row = (isWhite ? 7 : 0);
  for (int j = 0; j < 4; j++) {
    texture[j].loadFromFile("images/" + board_[row][j] + ".png");
    chesspieceSprite[j].setTexture(texture[j]);
    chesspieceSprite[j].setPosition(g_squareSize*j,
                                        g_squareSize*isWhite);
  }

  bool running = true;
  while (running) {
  	mousePosition = sf::Mouse::getPosition(promote);
  	sf::Vector2f mousePosF(static_cast<float>(mousePosition.x), static_cast<float>( mousePosition.y ));
  	sf::Event e;
  	while (promote.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  promote.close();
  	  	  break;
  	    }
  	    case sf::Event::MouseButtonPressed : {
          if (e.key.code == sf::Mouse::Left) {
            for (int i = 0; i < 6; i++) {
              if (chesspieceSprite[i].getGlobalBounds().contains(mousePosF)) {
                return board_[row][i];
              }

            }
          }
        }
          break;
  	    }
      }
  for (int i = 0; i < 6; ++i) {
      promote.draw(chesspieceSprite[i]);
  }
  promote.display();

  }
  return "white_queen";
}

std::string PromotionScreen::getPromotionOption()
{
  return piece;
}
