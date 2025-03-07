#include "ofApp2.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(255, 255, 255);
    movement = new Movement();
    dino = new Dino();
    dino->setMovement(movement); // Certifique-se de configurar o movement antes de usar


    //
    ambientOn = true;
    dirOn = true;
    pointOn = true;
    autoMove = true;
    spotOn = true;

    pointZtheta = 0;
    dirVecTheta = 0.;
    
    spotExponent = 64;
    spotCutoff = 45;
    spotTheta = 0;
    spotDirVec = ofVec3f(0, 0, 0);
    spotDirAux = ofVec3f(0, 0, 0);
    spotPosVec = ofVec3f(-gw() * 0.25, 0., gh() * 0.25);



    // Fundo inicial
    fundoPosition = ofVec3f(0, 0, -200); // Atrás da cena
    fundoScale = ofVec3f(1000, 1000, 1); // Preenche a tela
    fundoMaterialID = 16; // ID do material a ser aplicado ao fundo

}

//--------------------------------------------------------------
void ofApp::update() {
    ofSetWindowTitle("CG Game running @ " + ofToString(int(ofGetFrameRate())));
    movement->update();
    dino->update();
    dino->setMovement(movement);
    if (autoMove) {

        //calculo da posicao da luz pontual
        pointPos[0] = gw() * 0.5 * sin(3 * pointZtheta * PI / 180.0);
        pointPos[1] = gh() * 0.5 * cos(pointZtheta * PI / 180.0);
        pointPos[2] = 200 + 199 * cos(8 * pointZtheta * PI / 180.0);
        pointZtheta += 0.25;

        //calculo da direcao da luz direcional
        float z = gh() * 0.25 * (cos(dirVecTheta * PI / 180.) * 0.5 + 0.45);
        float y = gh() * 0.2;
        float x = gw() / 2;
        dirVec3f = ofVec3f(x, y, z) - ofVec3f(0, 0, 0);
        dirVecTheta += 3;




    }
    else {
        //luz pontual
        //centro acima
        pointPos[0] = 0;
        pointPos[1] = 0;
        pointPos[2] = 90;

        dirVec3f = ofVec3f(gw() / 2, gh() * 0.1, 0);
                //spot
        spotDir[0] = 1;
        spotDir[1] = 0;
    }

    


}


void ofApp::luzes() {
    // Ativa o uso de iluminação e normalização
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    

    // Configuração da luz ambiente global
    if (ambientOn) {
        ambientLight[0] = 0.3; ambientLight[1] = 0.3; ambientLight[2] = 0.3; ambientLight[3] = 1.0;
    } else {
        ambientLight[0] = 0.0; ambientLight[1] = 0.0; ambientLight[2] = 0.0; ambientLight[3] = 1.0;
    }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // Configuração da luz direcional (GL_LIGHT0)
    dirVec[0] = dirVec3f.x;
    dirVec[1] = dirVec3f.y;
    dirVec[2] = dirVec3f.z;
    dirVec[3] = 0; // Indica vetor de direção

    dirAmb[0] = 0.1; dirAmb[1] = 0.1; dirAmb[2] = 0.1; dirAmb[3] = 1.0;
    dirDif[0] = 0.8; dirDif[1] = 0.8; dirDif[2] = 0.8; dirDif[3] = 1.0;
    dirSpec[0] = 0.5; dirSpec[1] = 0.5; dirSpec[2] = 0.5; dirSpec[3] = 1.0;

    glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);

    if (dirOn) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }

    // Configuração da luz pontual (GL_LIGHT1)
    pointPos[0] = gw() * 0.5; // Centro horizontal
    pointPos[1] = gh() * 0.25; // Centro vertical
    pointPos[2] = 150;        // Altura
    pointPos[3] = 1;          // Indica ponto de luz

    pointAmb[0] = 1; pointAmb[1] =1; pointAmb[2] = 1; pointAmb[3] = 1.0;
    pointDif[0] = 1; pointDif[1] = 1; pointDif[2] =1; pointDif[3] = 1.0;
    pointSpec[0] = 1.0; pointSpec[1] = 1.0; pointSpec[2] = 1.0; pointSpec[3] = 1.0;

    glLightfv(GL_LIGHT1, GL_POSITION, pointPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);

    if (pointOn) {
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    //posicao  -------------------------------------------------------------
    spotPos[0] = dino->avatarPos.x;
    spotPos[1] = gh()/2;
    spotPos[2] = 0;
    spotPos[3] = 1.;


    //direcao
    spotDir[0] = 1;
    spotDir[1] = 0;
    spotDir[2] = 1;

    //ambiente
    spotAmb[0] = 1.;//R
    spotAmb[1] = 1.;//G
    spotAmb[2] = 1.;//B
    spotAmb[3] = 1.;//constante

    //difusa
    spotDif[0] = 1.;//R
    spotDif[1] = 1.;//G
    spotDif[2] = 1.;//B
    spotDif[3] = 1.;//constante

    //specular
    spotSpecular[0] = 1.;//R
    spotSpecular[1] = 1.;//G
    spotSpecular[2] = 1.;//B
    spotSpecular[3] = 1.;//constante


    if (spotOn) {
        glEnable(GL_LIGHT2);
    }
    else {
        glDisable(GL_LIGHT2);
    }

    glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);

    glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);

    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);

    //atenua��o
    spotAtC = 1.;
    spotAtL = 0.;
    spotAtQ = 0.;
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, spotAtC);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, spotAtL);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, spotAtQ);



    
    // Desenha representações visuais das luzes
    glDisable(GL_LIGHTING); // Desativa a iluminação para os objetos de debug
    if (pointOn) {
        glColor3f(1,1, 0); // Vermelho para a luz pontual
        glPushMatrix();
        glTranslatef(pointPos[0], pointPos[1], pointPos[2]);
        glScalef(30, 30, 30);
        cube_unit();
        glPopMatrix();
    }

    if (dirOn) {
        glColor3f(0, 1, 0); // Verde para luz direcional
        glPushMatrix();
        glEnd();
        glPopMatrix();
    }
    if (spotOn) {
        glColor3f(1,1, 1);
        glPushMatrix();
        glTranslatef(spotPos[0], spotPos[1], spotPos[2]);
        glScalef(30, 30, 30);
        cube_unit();
        glPopMatrix();
    }
    glEnable(GL_LIGHTING); // Reativa a iluminação
}





//--------------------------------------------------------------
void ofApp::draw() {
   if (game_m) {game_mode(); }
   else{
    ofSetColor(255); 
    ofPushMatrix();
    ofTranslate(0, 0, -10); 
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofPopMatrix();
    luzes();


    // Configuração da câmera
    switch (stateCam) {
    case 0:
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        break;
    case 1:
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(theta, alpha, beta);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        lookat(-gw() * 0.55, gh() * 0.55, distancia_cam * 3 / 4, 0, 0, 0, 0, 1, 0);

        break;
    case 2:
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(theta, alpha, beta);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        lookat(dino->avatarPos.x + dino->avatarSize * 0.5, dino->avatarPos.y + dino->avatarSize, 0, gw() / 2, 0, 0, 0, 1, 0);

        break;
    default:
        break;
    }

    draw_lines();

    if (!dino->colide) {
        loadMaterial(16);
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Score: " + ofToString(movement->score), -gw() / 2 + 49, -gh() / 2 + 200);
        ofDrawBitmapString("Speed: " + ofToString(movement->gameSpeed), -gw() / 2 + 49, -gh() / 2 + 250);


        movement->draw();

        dino->draw();
    

    }
    else {
        // Quando ocorre colisão
        stateCam = 0; // Reseta para visão 2D

        // Fundo preto
        glPushMatrix();
        glColor3f(0, 0, 0);
        glScalef(10000, 10000, 1);
        loadMaterial(2);
        cube_unit();
        glPopMatrix();
        glPushMatrix();
        loadMaterial(16);
        glScalef(0.01f, 0.01f, 0.01f);
        glColor3f(1.0f, 1.0f, 1.0f);
        ofDrawBitmapString("Press 1 to play again", -8000, -2000);
        ofDrawBitmapString("Press r change setings", -8000, -4000);
        glPopMatrix();
    }
    }
}

//--------------------------------------------------------------
void ofApp::draw_lines() {
    loadMaterial(2);
    glViewport(0, 0, gw(), gh());
    glColor3f(0.36f, 0.25f, 0.2f);
    glLineWidth(2.0f);
    switch (stateCam) {
    case 0:
        glBegin(GL_LINES);
        glVertex2f(-gw() * .5, -dino->avatarSize * 0.5);
        glVertex2f(gw() * 0.5, -dino->avatarSize * 0.5);
        glEnd();
        break;
    case 1:
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, dino->avatarSize, dino->avatarSize * 0.5);
        filled();

        glBegin(GL_QUADS);
        glVertex2f(-gw() * 0.5 - dino->avatarSize, -dino->avatarSize * 0.5);
        glVertex2f(gw() * 0.5 + dino->avatarSize, -dino->avatarSize * 0.5);
        glVertex2f(gw() * 0.5 + dino->avatarSize, -dino->avatarSize * 3 / 2);
        glVertex2f(-gw() * 0.5 - dino->avatarSize, -dino->avatarSize * 3 / 2);
        glEnd();
        glPopMatrix();
        break;
    case 2:
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, dino->avatarSize, dino->avatarSize * 0.5);
        filled();

        glBegin(GL_QUADS);
        glVertex2f(-gw() * 0.5 - dino->avatarSize, -dino->avatarSize * 0.5);
        glVertex2f(gw() * 0.5 + dino->avatarSize, -dino->avatarSize * 0.5);
        glVertex2f(gw() * 0.5 + dino->avatarSize, -dino->avatarSize * 3 / 2);
        glVertex2f(-gw() * 0.5 - dino->avatarSize, -dino->avatarSize * 3 / 2);
        glEnd();
        glPopMatrix();
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp:: game_mode(){
    // Configura o fundo para preto

    ofBackground(0, 0, 0);
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    ofDrawBitmapString("Choose the game mode:", gw()/2-50, gh()/2);
    ofDrawBitmapString("8 - EASY!", gw() / 2 , gh() / 2 +20);
    ofDrawBitmapString("9 - HARD!", gw() / 2 , gh() / 2 +40);

    glPopMatrix();
}


//--------------------------------------------------------------
void ofApp::exit() {
    delete movement;
    delete dino;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case '1':
        setup();
        break;
    case 'r':
        setup();
        game_m = true;
        break;
    case 'v':
        stateCam++;
        if (stateCam > 2) {
            stateCam = 0;
        }
        break;


    case 'a':
        ambientOn = !ambientOn;
        break;
    case 'd':
        dirOn = !dirOn;
        break;
    case 'p':
        pointOn = !pointOn;
        break;

    case 's':
        spotOn = !spotOn;
        break;
    case 'c':
        customMat = !customMat;
        break;

    case '+':
        spotExponent++;
        if (spotExponent > 128) {
            spotExponent = 128;
        }
        break;
    case '-':
        spotExponent--;
        if (spotExponent < 0) {
            spotExponent = 0;
        }
        break;
    case 'l':
        spotCutoff++;
        if (spotCutoff > 90) {
            spotCutoff = 90;
        }
        break;
    case 'k':
        spotCutoff--;
        if (spotCutoff < 0) {
            spotCutoff = 0;
        }
        break;

    case 'q':
        autoMove = !autoMove;
        break;
    case '8':
        movement->gameSpeed = 5;
        game_m = false;
        break;
    case '9':
        movement->gameSpeed = 10;
        game_m = false;
        break;
    default:
        break;
    }

    movement->keyPressed(key);
    dino->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    movement->keyReleased(key);
    dino->keyReleased(key);
}
