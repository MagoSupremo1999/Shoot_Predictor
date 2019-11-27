/* ===============================================================================================
   = Código desenvolvido para um goleiro-robô peerceber um chute e se posicionar para defendê-lo =
   = 19/11/2019                                                                                  =
   = Desenvolvido por Vinícius Alves                                                             =
   =============================================================================================== */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>

#define SEN_1 0.017452406           //Seno de 1º
#define COS_1 0.999847695           //Cosseno de 1º
#define TAN_1 0.017455064           //Tangente de 1º

int main(){
    float r = sqrt(7*7 + 9*9);                              //Distância do centro da bola ao vértice referência do chutador
    float cos_init = 7/r;                                   //Cosseno inicial da bola em relação ao chutador
    float sen_init = 9/r;                                   //Seno inicial da bola em relação ao chutador
    float tan_line = 0;                                     //Tangente inicial de alpha
    float alpha, beta;                                      //Coeficientes angular e linear, respectivamente, da reta
    float ball_x, ball_y;                                   //Coordenadas da bola
    bool activate_trajectory;                               //Variável que define se a trajetória é mostrada

    sf::RenderWindow window(sf::VideoMode(900, 600), "My window");          //Configura a janela

//======================= Desenvolvimento do layout do estádio de futebol ==========================================
    sf::RectangleShape stadium(sf::Vector2f(900, 600));                     //Configura o estádio
    stadium.setFillColor(sf::Color::Green);

    sf::RectangleShape field(sf::Vector2f(880, 580));                       //Configura a área do campo
    field.setFillColor(sf::Color::Transparent);
    field.setPosition(10, 10);
    field.setOutlineColor(sf::Color::White);
    field.setOutlineThickness(1);

    sf::RectangleShape left_area(sf::Vector2f(100, 150));                   //Configura a pequena área esquerda
    left_area.setFillColor(sf::Color::Transparent);
    left_area.setPosition(10, 225);
    left_area.setOutlineColor(sf::Color::White);
    left_area.setOutlineThickness(1);

    sf::CircleShape center_area(50);                                        //Configura a área central do campo
    center_area.setFillColor(sf::Color::Transparent);
    center_area.setPosition(400, 250);
    center_area.setOutlineColor(sf::Color::White);
    center_area.setOutlineThickness(1);

    sf::Vertex cut_field[] = {sf::Vertex(sf::Vector2f(450, 10)), sf::Vertex(sf::Vector2f(450, 590))};   //Configura a divisória do campo

    sf::RectangleShape right_area(sf::Vector2f(100, 150));                  //Configura a pequena área direita
    right_area.setFillColor(sf::Color::Transparent);
    right_area.setPosition(790, 225);
    right_area.setOutlineColor(sf::Color::White);
    right_area.setOutlineThickness(1);
//==================================================================================================================

//======================================= Adição de objetos ao campo ===============================================
    sf::RectangleShape goalkeeper(sf::Vector2f(18, 18));                //Configura o goleiro
    goalkeeper.setFillColor(sf::Color::Blue);
    goalkeeper.setPosition(25, 291);

    sf::CircleShape ball(5);                                            //Configura a bola
    ball.setFillColor(sf::Color(255, 130, 0));
    ball.setPosition(600, 295);

    sf::RectangleShape shooter(sf::Vector2f(2, 18));                    //Configura o chutador
    shooter.setFillColor(sf::Color::Black);
    shooter.setPosition(612, 291);
//==================================================================================================================

    while (window.isOpen()){                                            //Abre a janela

//========================================= Desenho das criações na janela =========================================
        window.clear();
        window.draw(stadium);                                           //Desenha o estádio
        window.draw(field);                                             //Desenha a região do campo
        window.draw(left_area);                                         //Desenha a pequena área esquerda
        window.draw(right_area);                                        //Desenha a pequena área direita
        window.draw(cut_field, 10, sf::Lines);                          //Desenha a divisória do campo dos times
        window.draw(center_area);                                       //Desenha a área central do campo
        window.draw(goalkeeper);                                        //Desenha o goleiro
        window.draw(ball);                                              //Desenha a bola
        window.draw(shooter);                                           //Desenha o chutador
//====================================================================================================================

        sf::Event rotation;
        while (window.pollEvent(rotation)) {
            float cos_up = cos_init*COS_1 - sen_init*SEN_1;             //Definição de variáveis de rotação
            float sen_up = sen_init*COS_1 + SEN_1*cos_init;
            float cos_down = cos_init*COS_1 + sen_init*SEN_1;
            float sen_down = sen_init*COS_1 - SEN_1*cos_init;
            rotation.mouseWheelScroll.wheel = sf::Mouse::VerticalWheel;

//================================================ Rotação do chutador ================================================
            if (rotation.type == sf::Event::MouseWheelScrolled) {
                goalkeeper.setPosition(25 , 291);
                goalkeeper.setFillColor(sf::Color::Blue);
                activate_trajectory = false;                
                if (rotation.mouseWheelScroll.delta < 0.0) {                    
                    shooter.rotate(1.0);                                            //Rotação horária do chutador
                    shooter.move(r*(cos_down - cos_init), -r*(sen_down - sen_init));
                    alpha = (tan_line + TAN_1)/(1 - tan_line * TAN_1);
                    cos_init = cos_down;
                    sen_init = sen_down;
                    tan_line = alpha;                    
                }
                if (rotation.mouseWheelScroll.delta > 0.0) {
                    shooter.rotate(-1.0);                                           //Rotação anti-horária do chutador
                    shooter.move(r*(cos_up - cos_init), -r*(sen_up - sen_init));
                    alpha = (tan_line - TAN_1)/(1 + tan_line * TAN_1);
                    cos_init = cos_up;
                    sen_init = sen_up;
                    tan_line = alpha;
                }
            }
//====================================================================================================================

            beta = ball_y - ball_x * alpha;
            float shooter_x = r * cos_init;                                     //Definição de variáveis de translação
            float shooter_y = r * sen_init;

//============================================== Translação do chutador ==============================================
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                goalkeeper.setPosition(25 , 291);
                goalkeeper.setFillColor(sf::Color::Blue); 
                activate_trajectory = false;
                ball_x = sf::Mouse::getPosition(window).x;
                ball_y = sf::Mouse::getPosition(window).y;
                ball.setPosition(ball_x - 5, ball_y - 5);                       //Definição da translação da bola
                shooter.setPosition(ball_x + shooter_x, ball_y - shooter_y);    //Definição da translação do chutador
            }
//====================================================================================================================

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {                
                activate_trajectory = true;
                if (225.0 < (alpha * 43 + beta) && (alpha * 43 + beta) < 375.0) {                      
                    goalkeeper.setPosition(25 , alpha * 43 + beta - 9);         //Goleiro defende a bola
                    goalkeeper.setFillColor(sf::Color::Red);                    //Goleiro fica alerta
                }            
            }
        }

        if (activate_trajectory == true) {
            sf::Vertex shoot[] = {sf::Vertex(sf::Vector2f(ball_x, ball_y)), sf::Vertex(sf::Vector2f(10, alpha * 10 + beta))};
            window.draw(shoot, 10, sf::Lines);                                  //Desenha a trajetória da bola
        }

        window.display();                                                       //Aplica os desenhos na janela

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();                                                 //Fecha a janela
            }
        }
    }
    return 0;
}
