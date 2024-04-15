#include <SFML/Graphics.hpp>
#include <sstream>

// Constantes do tamanho da tela
int SCREEN_WIDTH{960};
int SCREEN_HEIGHT{540};

int FAKE_SCREEN_WIDTH{1920};
int FAKE_SCREEN_HEIGHT{1080};

// usar o namespace do sfml pra facilitar a escrita do código
using namespace sf;

int main(int argc, char const *argv[])
{
  // Setar a HUD
  int score{0};
  Text scoreText;
  Text messageText;

  Font font;
  font.loadFromFile("fonts/KOMIKAP_.ttf");

  scoreText.setFont(font);
  messageText.setFont(font);

  messageText.setString("Press enter to start!");
  scoreText.setString("Score = 0");

  messageText.setCharacterSize(75);
  scoreText.setCharacterSize(100);

  messageText.setFillColor(Color::White);
  scoreText.setFillColor(Color::White);

  // Posicionar o texto??
  FloatRect textRect = messageText.getLocalBounds(); // pegar a scale, position e transform do texto
  messageText.setOrigin(
      textRect.left + textRect.width / 2.0f,
      textRect.top + textRect.height / 2.0f);                                   // Setar a origem ou pivot do texto pro centro
  messageText.setPosition(FAKE_SCREEN_WIDTH / 2.0f, FAKE_SCREEN_HEIGHT / 2.0f); // Posicionar o texto no centro
  scoreText.setPosition(20, 20);                                                // Setar posição da pontuação

  // Criar um objeto VideoMode
  VideoMode vm(SCREEN_WIDTH, SCREEN_HEIGHT);

  // Criar um tela com o VideoMode, titulo "Timber!!!" e estilo default ( modo janela )
  RenderWindow window(vm, "Timber!!!", Style::Default);
  // seta a largura fake pra não quebrar as imagens
  View view(sf::FloatRect(0, 0, FAKE_SCREEN_WIDTH, FAKE_SCREEN_HEIGHT));
  window.setView(view);

  // Criar uma textura e salvar na gpu
  Texture textureBackground;
  // carrega a textura do arquivo
  textureBackground.loadFromFile("graphics/background.png");
  // cria o sprite
  Sprite spriteBackground;
  // conecta a textura na sprite
  spriteBackground.setTexture(textureBackground);
  // seta a posição da sprite pra 0 0
  spriteBackground.setPosition(0, 0);

  // Criar a arvore
  Texture textureTree;
  textureTree.loadFromFile("graphics/tree.png");
  Sprite spriteTree;
  spriteTree.setTexture(textureTree);
  spriteTree.setPosition(810, 0);

  // cria a abelha
  Texture textureBee;
  textureBee.loadFromFile("graphics/bee.png");
  Sprite spriteBee;
  spriteBee.setTexture(textureBee);
  spriteBee.setPosition(0, 800);
  bool beeActive{false};
  float beeSpeed{0.0f};

  // cria as nuvens
  Texture textureCloud;
  textureCloud.loadFromFile("graphics/cloud.png");
  Sprite spriteCloud1;
  Sprite spriteCloud2;
  Sprite spriteCloud3;
  spriteCloud1.setTexture(textureCloud);
  spriteCloud2.setTexture(textureCloud);
  spriteCloud3.setTexture(textureCloud);
  spriteCloud1.setPosition(0, 0);
  spriteCloud2.setPosition(0, 250);
  spriteCloud3.setPosition(0, 500);
  bool cloud1Active{false};
  bool cloud2Active{false};
  bool cloud3Active{false};
  float cloud1Speed{0.0f};
  float cloud2Speed{0.0f};
  float cloud3Speed{0.0f};

  // cria um relogio?
  Clock clock;

  // criar barra
  RectangleShape timeBar;
  float timeBarStartWidth{400};
  float timeBarHeight{80};
  timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
  timeBar.setFillColor(Color::Red);
  timeBar.setPosition((FAKE_SCREEN_WIDTH / 2) - timeBarStartWidth / 2, 980);

  Time gameTimeTotal;
  float timeRemaining{6.0f};
  float timeBarWidthPerSecond{timeBarStartWidth / timeRemaining};

  // variavel de pause
  bool paused{true};

  while (window.isOpen())
  {
    // remover o pause caso aperte enter
    if (Keyboard::isKeyPressed(Keyboard::Enter) && paused)
    {
      paused = false;

      // Reseta o jogo
      score = 0;
      timeRemaining = 6;
    }

    // criar um objeto evento
    Event event;
    // pega os eventos da janela
    while (window.pollEvent(event))
    {
      // se o evento for para do tipo Closed
      if (event.type == Event::Closed)
      {
        // fecha a janela
        window.close();
      }
    }
    // Caso eu aperte a tecla ESC
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
      // fecha a tela
      window.close();
    }

    if (!paused)
    {

      // Mensurar o tempo?
      Time dt = clock.restart();

      // diminuir o tempo restante
      timeRemaining -= dt.asSeconds();

      // setar largura da barra de acordo com o tempo restante
      timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

      if (timeRemaining <= 0.0f)
      {
        // pausar o game
        paused = true;
        // mudar o texto
        messageText.setString("Out of time!!");
        // reposicionar o texto baseado no novo texto
        FloatRect textRect = messageText.getLocalBounds();
        messageText.setOrigin(textRect.left +
                                  textRect.width / 2.0f,
                              textRect.top +
                                  textRect.height / 2.0f);
        messageText.setPosition(FAKE_SCREEN_WIDTH / 2.0f, FAKE_SCREEN_HEIGHT / 2.0f);
      }

      // configurar a abelha
      if (!beeActive)
      {
        // gera uma velocidade aleatoria para a abelha
        srand(int(time(0)));             // gerar um seed para o gerador de numero aleatorio
        beeSpeed = (rand() % 200) + 200; // gerar um numero aleatorio entre 200 e 399 para a velocidade da abelha

        srand((int)time(0) * 10);            // gerar um seed dnv
        float height = (rand() % 500) + 500; // gerar uma altura aleatoria para a abelha entre 500 e 999
        spriteBee.setPosition(2000, height); // colocar a abelha pra fora da tela e na altura gerada
        beeActive = true;                    // ativar a abelha
      }
      else // caso a abelha esteja ativa, mover ela
      {
        spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
        if (spriteBee.getPosition().x < -100)
        {
          beeActive = false;
        }
      }

      // configurando as nuvens
      if (!cloud1Active)
      {
        srand((int)time(0) * 10);
        cloud1Speed = (rand() % 200);
        srand((int)time(0) * 10);
        float height = (rand() % 150);
        spriteCloud1.setPosition(-200, height);
        cloud1Active = true;
      }
      else
      {
        spriteCloud1.setPosition(
            spriteCloud1.getPosition().x +
                (cloud1Speed * dt.asSeconds()),
            spriteCloud1.getPosition().y);
        if (spriteCloud1.getPosition().x > 1920)
        {
          cloud1Active = false;
        }
      }
      // Nuvem 2
      if (!cloud2Active)
      {
        srand((int)time(0) * 20);
        cloud2Speed = (rand() % 200);
        srand((int)time(0) * 20);
        float height = (rand() % 300) - 150;
        spriteCloud2.setPosition(-200, height);
        cloud2Active = true;
      }
      else
      {
        spriteCloud2.setPosition(
            spriteCloud2.getPosition().x +
                (cloud2Speed * dt.asSeconds()),
            spriteCloud2.getPosition().y);
        if (spriteCloud2.getPosition().x > 1920)
        {
          cloud2Active = false;
        }
      }
      // Nuvem 3
      if (!cloud3Active)
      {
        srand((int)time(0) * 30);
        cloud3Speed = (rand() % 200);
        srand((int)time(0) * 30);
        float height = (rand() % 300) - 150;
        spriteCloud3.setPosition(-200, height);
        cloud3Active = true;
      }
      else
      {
        spriteCloud3.setPosition(
            spriteCloud3.getPosition().x +
                (cloud3Speed * dt.asSeconds()),
            spriteCloud3.getPosition().y);
        if (spriteCloud3.getPosition().x > 1920)
        {
          cloud3Active = false;
        }
      }

      // Setar texto do score
      std::stringstream ss;
      ss << "Score = " << score;
      scoreText.setString(ss.str());
    }

    /*
    ***********************************************************
    DRAW
    Aqui é onde vou desenhar as coisas
    depois de limpar a tela e antes de mostrar as coisas
    ***********************************************************
    */

    // Limpa as coisas na tela do ultimo frame
    window.clear();

    // desenha o background
    window.draw(spriteBackground);

    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);

    // desenha a arvore
    window.draw(spriteTree);

    window.draw(spriteBee);

    window.draw(scoreText);

    window.draw(timeBar);

    if (paused)
    {
      window.draw(messageText);
    }

    // Mostrar as coisas do frame atual na tela
    window.display();
  }

  return 0;
}
