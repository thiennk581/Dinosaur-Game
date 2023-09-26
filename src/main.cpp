#include <iostream>
#include <SFML\Graphics.hpp>

int sizeX[] = {33, 52, 107, 148};
int sizeY[] = {68, 103, 105, 98};
int cactusY[] = {463, 446, 444, 448}; 

std::string scoreToString(int score, int highestScore)
{
    std::string a = std::to_string(score);
    std::string b = std::to_string(highestScore);
    while (a.length() < 5) a = "0" + a;
    while (b.length() < 5) b = "0" + b;
    return b + "   " + a;
}

int main() {
    int type = 0;
    int lives = 3;
    bool isLose = false;
    srand(time(NULL));
    bool spacing = false;
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Dino!", sf::Style::Default);

    //line
    sf::RectangleShape line(sf::Vector2f(1200,6));
    line.setFillColor(sf::Color::White);
    line.setPosition(1200/2,500);
    line.setOrigin(1200/2, 6/2);

    //dino
    int dinoStatus = 1;
    float frameTime = 0.1f, currentTime = 0.f;
    sf::Texture dinoTexture[3];
    dinoTexture[0].loadFromFile("../resources/Image/dino_0.png");
    dinoTexture[1].loadFromFile("../resources/Image/dino_1.png");
    dinoTexture[2].loadFromFile("../resources/Image/dino_2.png");
    sf::RectangleShape dino(sf::Vector2f(66.6,94));
    dino.setFillColor(sf::Color::White);
    dino.setPosition(40, line.getPosition().y - 50);
    dino.setOrigin(66.6/2, 94/2);
    dino.setTexture(&dinoTexture[dinoStatus],true);

    // cactus
    float cactusSpeed = 450.f;
    sf::RectangleShape cactus(sf::Vector2f(sizeX[type], sizeY[type]));
    cactus.setFillColor(sf::Color(119, 255, 144));
    cactus.setOrigin(sizeX[type] / 2, sizeY[type] / 2);
    cactus.setPosition(1300, cactusY[type]);
    sf::Texture cactusTexture[4];
    cactusTexture[0].loadFromFile("../resources/Image/cactus_0.png");
    cactusTexture[1].loadFromFile("../resources/Image/cactus_1.png");
    cactusTexture[2].loadFromFile("../resources/Image/cactus_2.png");
    cactusTexture[3].loadFromFile("../resources/Image/cactus_3.png");
    cactus.setTexture(&(cactusTexture[type]),true);

    //replay
    sf::Font font;
    font.loadFromFile("../resources/Font/arcadeclassic/ARCADECLASSIC.TTF");
    sf::Text announcement;
    announcement.setFont(font);
    announcement.setFillColor(sf::Color(144,145,145));
    announcement.setStyle(sf::Text::Bold);
    announcement.setPosition(1200/2 - 205, 700 / 2 - 120);
    announcement.setCharacterSize(70);
    announcement.setString("G A M E    O V E R");
    sf::RectangleShape replayButton(sf::Vector2f(116, 102));
    replayButton.setFillColor(sf::Color::White);
    replayButton.setPosition(1200/2, 700/2 + 40);
    replayButton.setOrigin(116/2, 102/2);
    sf::Texture textureReplay;
    textureReplay.loadFromFile("../resources/Image/replayButton.png");
    replayButton.setTexture(&textureReplay);

    //lives
    sf::RectangleShape heart(sf::Vector2f(32,32));
    heart.setPosition(50, 700/2 - 250);
    heart.setOrigin(32/2, 32/2);
    heart.setFillColor(sf::Color::Red);
    sf::Texture heartTexture;
    heartTexture.loadFromFile("../resources/Image/heart.png");
    heart.setTexture(&heartTexture);

    // score
    sf::Text scoreDisplay;
    scoreDisplay.setFont(font);
    scoreDisplay.setCharacterSize(40);
    scoreDisplay.setFillColor(sf::Color(144,145,145));
    scoreDisplay.setPosition(850, 700/2 - 277);
    scoreDisplay.setString("HI   00000   00000");
    sf::Clock timer;
    int highestScore = 0;
    float score = 1.f;

    sf::Clock clock;
    float timeInLoop = 0.f;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        if (isLose == false)
        {
            timeInLoop = clock.restart().asSeconds();
            // dino spacing
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dino.getPosition().y == line.getPosition().y - 50)
            {
                spacing = true;
            }
            if (spacing && dino.getPosition().y > line.getPosition().y - 270)
            {
                dino.move(0, timeInLoop * -400);
            }
            else 
                spacing = false;
            if (spacing == false && dino.getPosition().y < line.getPosition().y - 50)
            {
                dino.move(0, timeInLoop * 380);
            }
            else if (dino.getPosition().y > line.getPosition().y - 50)
                dino.setPosition(dino.getPosition().x, line.getPosition().y - 50);

            // cactus moving
            cactus.move(- cactusSpeed * timeInLoop, 0);
            if (int(score) % 50 == 0) {
                cactusSpeed += 7.f;
                score = score + 1.f;
            }

            // reset cactus
            if (cactus.getPosition().x < -10 - (sizeX[type] / 2)) 
            {
                type = rand()%4;
                cactus.setSize(sf::Vector2f(sizeX[type], sizeY[type]));
                cactus.setFillColor(sf::Color(119, 255, 144));
                cactus.setPosition(1300, cactusY[type] );
                cactus.setOrigin(sizeX[type] / 2, sizeY[type] / 2);
                cactus.setTexture(&cactusTexture[type], true);
                score += type + 1;
            }

            // lose
            if (cactus.getPosition().x - (sizeX[type]/2) <= dino.getPosition().x + 60/2 )    
            {
                if (cactus.getPosition().y - (sizeY[type]/2) <= dino.getPosition().y + 90/2)
                {
                    --lives;
                    if (lives != 0)
                        cactus.setPosition(1300, cactusY[type] );
                }
                if (lives == 0) isLose = true;
            }  

            // score
            score += timeInLoop * 2;
            scoreDisplay.setString("HI   " + scoreToString(int(score), highestScore));
        }
        else
        {
            // button restart
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                float mouseX = sf::Mouse::getPosition(window).x;
                float mouseY = sf::Mouse::getPosition(window).y;
                if(mouseX >= replayButton.getPosition().x - 116/2
                    && mouseX <= replayButton.getPosition().x + 116/2
                    && mouseY >= replayButton.getPosition().y - 102/2
                    && mouseY <= replayButton.getPosition().y + 102/2)
                {
                    isLose = false;
                    lives = 3;
                    cactus.setPosition(1300, cactusY[type]);
                    if (score > highestScore) highestScore = int(score);
                    score = 1.f;
                    cactusSpeed = 380.f;
                    dinoStatus = 1;
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                isLose = false;
                lives = 3;
                cactus.setPosition(1300, cactusY[type]);
                if (score > highestScore) highestScore = int(score);
                score = 1.f;
                cactusSpeed = 380.f;
                dinoStatus = 1;
            }
        }

        // dino animation
        currentTime += timeInLoop;
        if (currentTime >= frameTime)
        {
            if (dino.getPosition().y != line.getPosition().y - 50 || isLose == true) {
                dinoStatus = 0;
                dino.setTexture(&dinoTexture[dinoStatus], true);
            }
            if (dinoStatus == 0 && dino.getPosition().y == line.getPosition().y - 50)
                dinoStatus = 1;
            if (dinoStatus == 1) {
                dinoStatus = 2;
                dino.setTexture(&dinoTexture[dinoStatus], true);
            }
            else if (dinoStatus == 2) {
                dinoStatus = 1;
                dino.setTexture(&dinoTexture[dinoStatus], true);
            }
            currentTime -= frameTime;
        }
        
        window.clear(sf::Color(32,33,36));
        window.draw(line);
        window.draw(dino);
        dino.setTexture(&dinoTexture[dinoStatus]);
        window.draw(cactus);
        if (isLose) {
            window.draw(announcement);
            window.draw(replayButton);
        }
        window.draw(scoreDisplay);
        for (int i = 1; i <= lives; ++i)
        {
            heart.setPosition(50*i, 700/2 - 250);
            window.draw(heart);
        }
        window.display();
    }
}