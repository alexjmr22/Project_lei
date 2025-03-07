#pragma once

#include "ofMain.h"
#include "cg_game_movement2.h"
#include "Materiais2.h"

class ofApp : public ofBaseApp {
public:
	int stateCam = 0;
	float theta = 60;
	float alpha = 100;
	float beta = 1000;
	float distancia_cam = gh() * 0.5 / tanf(theta * 0.5 * PI / 180.0);
	void draw_lines();
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void luzes();
	void game_mode();

	bool game_m = true;
	Movement* movement;
	Dino* dino;

	int lightState;


	bool ambientOn;
	bool dirOn;
	bool pointOn;
	bool autoMove;

	GLfloat ambientLight[4];

	GLfloat dirVec[4];
	GLfloat dirAmb[4];
	GLfloat dirDif[4];
	GLfloat dirSpec[4];
	ofVec3f dirVec3f;
	GLfloat dirVecTheta;


	GLfloat pointPos[4];
	GLfloat pointAmb[4];
	GLfloat pointDif[4];
	GLfloat pointSpec[4];
	GLfloat pointZtheta;



	ofImage back;


	GLfloat spotPos[4];
	GLfloat spotDir[3];
	ofVec3f spotDirVec;
	ofVec3f spotDirAux;
	ofVec3f spotPosVec;
	GLfloat spotAmb[4];
	GLfloat spotDif[4];
	GLfloat spotSpecular[4];
	GLfloat spotExponent;
	GLfloat spotCutoff;
	GLfloat spotTheta;
	GLfloat spotAtC, spotAtL, spotAtQ;
	bool spotOn;


	//materiais
	int mat;
	bool customMat;
	GLint customMatCoef;

	GLfloat customMatAmb[4];
	GLfloat customMatDif[4];
	GLfloat customMatSpec[4];


	ofVec3f fundoPosition;
	ofVec3f fundoScale;
	int fundoMaterialID; 

};
