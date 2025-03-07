#include "dino2.h"
#include "cg_game_movement2.h"
#include "cg_cam_extras2.h"
Dino::Dino()
{
    setup();
}

Dino::~Dino()
{
}
void Dino::setup() {
    ofSetFrameRate(60);
    resetPhysics();
    ofDisableArbTex();
    imag2.load("cg12.jpg");
}


void Dino::resetPhysics() {
    avatarPos = ofVec3f(-gh() * 0.5 + avatarSize * 0.5, 0, 0.); // Alinhar com o chão e centro do Movement
    avatarVelMod = 1.0;
    avatarSize = gw() * 0.1; // Igual ao Movement
}

void Dino::setMovement(Movement* m) {
    movement = m;
}



void Dino::update() {
    jump_dino(); // Atualiza a lógica de salto
    if (movement) {
        // Verificar colisão
        if (checkCollision(movement->avatarPos, movement->avatarSize)) {
            std::cout << "Colisão detectada com o inimigo!" << std::endl;
            resetPhysics(); 
        }
    }
    else {
        std::cerr << "Erro: movement não foi configurado!" << std::endl;
    }
}


void Dino::draw() {
    glViewport(0, 0, gw(), gh());

    glPushMatrix();
    glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z); // Posição
    glScalef(avatarSize, avatarSize, avatarSize);        // Escala


    // Ativa textura antes de desenhar
    glEnable(GL_TEXTURE);
    imag2.bind();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glColor3f(1.0, 1.0, 1.0); // Garante cor branca


    // Define material do dino
    float dinoAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    float dinoDiffuse[] = { 0.8, 0.0, 0.0, 1.0 }; 
    float dinoSpecular[] = { 0.2, 0.2, 0.2, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, dinoAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dinoDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dinoSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0);


    loadMaterial(0);
    draw_dino();                 
    imag2.unbind();           




    // Reseta o material para evitar propagação
    float neutralAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    float neutralDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    float neutralSpecular[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, neutralAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, neutralDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, neutralSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0);

    glDisable(GL_TEXTURE);    
    glPopMatrix();


}

void Dino::keyPressed(int key) {
    if (movement->running) {
        switch (key) {
        case OF_KEY_UP:
        case ' ':
            if (avatarPos.y <= 0.5 * avatarSize) {
                impulso = true;
                salto = true;
                jumpForce = 1.250;
                jumpImpulseFrameCounter = 0;
                jump_dino();
            }
            break;
        case OF_KEY_DOWN:
            if (salto) {
                if (avatarPos.y > 0.5 * avatarSize) { // Verificar se está no ar
                    impulso = false;  
                    avatarVelMod = 0; 
                    fall = true;
                    fall_dino();
                }
            }
            break;

        default:
            break;
        }
    }
}



void Dino::keyReleased(int key) {
}

void Dino::jump_dino() {
    if (impulso) {
        if (jumpImpulseFrameCounter < jumpImpulseNumFrames) {
            jumpForce = 1.250;
        }
        else {
            impulso = false; // Parar o impulso após um número definido de frames
        }
        jumpImpulseFrameCounter++;
    }
    if (salto) {
        avatarAccelMod = (jumpForce + Fg) / float(avatarMass);
        avatarVelMod += avatarAccelMod;
        avatarPos.y += avatarVelMod;
        // Limitar a altura máxima do salto
        if (avatarPos.y >= maxHeight) {
            avatarPos.y = maxHeight;
            avatarVelMod = 0.0;
            jumpForce = 0.0;
        }
        // Resetar ao tocar o chão
        float groundHeight = -gh() * 0.5 + avatarSize * 0.5;
        if (avatarPos.y <= 0) {
            avatarPos.y = 0;
            avatarVelMod = 0.0;
            jumpForce = 0.0;
            salto = false;
        }
    }
}

void Dino::fall_dino() {
    if (fall) {
        // Aplicar apenas a gravidade durante a queda
        avatarAccelMod = Fg / float(avatarMass) * 100;
        avatarVelMod += avatarAccelMod; // Aceleração reduz velocidade (queda)
        avatarPos.y -= avatarVelMod;
        float groundHeight = -gh() * 0.5 + avatarSize * 0.5;
        if (avatarPos.y <= groundHeight) {
            avatarPos.y = groundHeight; 
            avatarVelMod = 0.0;        
            jumpForce = 0.0;           
            fall = false;             
        }
    }
}

bool Dino::checkCollision(ofVec3f otherPos, float otherSize) {
    // Calcular as bordas do Dino no eixo x, y e z
    float dinoLeft = avatarPos.x - avatarSize * 0.5;
    float dinoRight = avatarPos.x;
    float dinoBottom = avatarPos.y;
    float dinoTop = avatarPos.y;
    float dinoBack = avatarPos.z - avatarSize * 0.5;
    float dinoFront = avatarPos.z + avatarSize * 0.5;
    // Calcular as bordas do objeto inimigo no eixo x, y e z
    float otherLeft = otherPos.x - avatarSize * 0.5;
    float otherRight = otherPos.x + otherSize * 0.5;
    float otherBottom = otherPos.y;
    float otherTop = otherPos.y + otherSize * 0.5;
    float otherBack = otherPos.z - otherSize * 0.5;
    float otherFront = otherPos.z + otherSize * 0.5;
    // Verificar colisão em todos os eixos (x, y, z)
    bool colidiuX = (dinoLeft <= otherRight && dinoRight >= otherLeft);
    bool colidiuY = (dinoBottom <= otherTop && dinoTop >= otherBottom);
    // Retornar true se houver colisão nos três eixos
    if (colidiuX && colidiuY) {
        colide = true;
        movement->running = false;
    }
    return colidiuX && colidiuY;
}

