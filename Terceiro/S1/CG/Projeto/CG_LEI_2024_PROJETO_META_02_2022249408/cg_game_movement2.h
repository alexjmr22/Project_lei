#pragma once
#include "ofMain.h"
#include "cg_extras2.h"
#include "cg_drawing_extras2.h"
#include "cg_cam_extras2.h"
#include "cg_game_particle2.h"
#include <ctime>
#include "dino2.h"
#include "Materiais2.h"


class Movement {
    Dino* dino;
public:
    Movement();
    ~Movement();
    void setup();
    void resetPhysics();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    float avatarSize;
    int state;
    int numStates;
    bool running = false;
    int score = 0;                
    float gameSpeed;        
    int speedIncreaseThreshold = 200; // Aumenta a velocidade a cada 100 pontos
    int case_number;
    void linearMovement();
    ofVec3f avatarPos;
    ofVec3f avatarVelDir;
    float avatarVelMod=7;
    ofVec3f avatarAccelDir;
    float avatarAccelMod;
    int cameraMode;


    ofImage image1;
    ofImage image2;
    ofImage image3;


    ofTexture texture1;
    ofTexture texture2;
    ofTexture texture3;
};