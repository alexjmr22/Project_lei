#include "cg_game_movement2.h"

Movement::Movement()
{
    setup();
}

Movement::~Movement()
{
}

void Movement::setup()
{
    srand(time(0));
    state = 0;
    numStates = 2;
    gameSpeed=7;
    case_number = rand() % 3;
    avatarSize = gw() * 0.1;
    resetPhysics();
    ofDisableArbTex();
    image1.load("cg7.jpg");
    image2.load("cg3.jpg");
    image3.load("cg13.jpg");


}

void Movement::resetPhysics()
{
    avatarPos = ofVec3f(gw() / 2, 0., 0.);
    avatarVelDir = ofVec3f(1., 0., 0.);
    avatarVelMod = 1.;
    avatarAccelDir = ofVec3f(1., 0, 0);
    avatarAccelMod = 0.1;

}


void Movement::update()
{
    // chat
    if (state == 0)
    {
    }
    if (state == 1)
    {
        linearMovement();
        score += 1;
        if (score % speedIncreaseThreshold == 0)
        {
            gameSpeed += 1; // Aumenta a velocidade do jogo
        }
        avatarVelMod = gameSpeed;
    }
}

void Movement::draw()
{
    if (state == 0)
    {
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Press 1 to Play", -gw() / 2 + 49, -gh() / 2 + 49);
    }
    else if (state == 1)
    {
        glColor3f(0.8, 0.8, 0.8);
        filled();
        glPushMatrix();
        glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z);
        glScalef(avatarSize, avatarSize, avatarSize);
        glEnable(GL_TEXTURE);
        image2.bind();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glColor3f(1.0, 1.0, 1.0); // Garante cor branca
        switch (case_number) {
        case 0:
            glTranslatef(0, -0.25, 0);
            loadMaterial(1);
            cube_unit();
            break;
        case 1:
            image1.bind();
            loadMaterial(14);
            parallelepiped_3_2_height();
            image1.unbind();
            break;
        case 2:
            glTranslatef(0, -0.25, 0);
            loadMaterial(16);
            image3.bind();
            parallelepiped_meio();
            image3.unbind();
            break;
        default:
            std::cout << "Erro: caso inválido" << std::endl;
            break;
        }

        image2.unbind();           // Desvincula textura
        glDisable(GL_TEXTURE);    // Desativa textura após uso
        glPopMatrix();

    }
}

void Movement::keyPressed(int key)
{
    switch (key)
    {
    case '1':
        running = true;
        state++;
        resetPhysics();
        break;
    }
    if (state > numStates)
    {
        state = 0;
    }
    if (state < 0)
    {
        state = numStates;
    }
}

void Movement::keyReleased(int key)
{
}

void Movement::linearMovement()
{
    // Faz o cubo mover-se da direita para a esquerda
    avatarPos -= avatarVelDir * avatarVelMod;
    // Verifica se atingiu a borda esquerda
    if (avatarPos.x + (avatarSize * 0.5) < -gw() * 0.5)
    {
        // Teleporta o avatar para o lado direito
        avatarPos.x = gw() * 0.5 + avatarSize * 0.5;
        case_number = rand() % 3;
    }
}


