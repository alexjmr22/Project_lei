#pragma once

#include "ofMain.h"
#include "cg_extras2.h"
#include "Materiais2.h"
class Movement;

class Dino {
	Movement* movement;
public:
	Dino();
	~Dino();
	void setup();
	void update();
	void draw();
	void setMovement(Movement* m);
	void keyPressed(int key);
	void keyReleased(int key);
	void resetPhysics();
	void jump_dino();
	bool checkCollision(ofVec3f otherPos, float otherSize);
	void fall_dino();
	int cameraMode; // Controle global do modo de câmera
	bool salto = false;
	bool impulso = false;
	float jumpForce = 0.0;
	float avatarAccelMod = 0.0;
	float avatarVelMod = 0.0;
	float avatarMass = 1.0;
	float avatarSize = 50.0; // Tamanho do personagem
	ofVec3f avatarPos;
	float Fg = -0.25; // Gravidade
	int jumpImpulseFrameCounter = 0;
	int jumpImpulseNumFrames = 10;
	bool colide = false;
	bool fall = false;
	float ycam;
	bool jumping = false;
	float maxHeight = gh() * 0.5 - avatarSize * 0.5;

	ofImage imag2;

	// Variáveis para texturas
	ofTexture texture1;
	ofTexture texture2;
	ofTexture texture3;
};