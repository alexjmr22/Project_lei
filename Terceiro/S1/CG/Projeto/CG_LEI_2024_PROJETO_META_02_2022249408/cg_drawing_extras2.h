#pragma once

#include "ofMain.h"
#include "cg_extras2.h"

//desenha preenchido
inline void filled() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//desenha so o wireframe
inline void wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


//desenha axis 3D
inline void axis3d() {
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
}

//função que desenha quadrado unitário 
//centrado na origem e preenchido
inline void rectFill_unit() {
	glBegin(GL_QUADS);
	glVertex3d(-0.5, -0.5, 0.);
	glVertex3d(-0.5, 0.5, 0.);
	glVertex3d(0.5, 0.5, 0.);
	glVertex3d(0.5, -0.5, 0.);
	glEnd();
}

//função que desenha malha unitária com resolução mxn
inline void malha_unit(GLint m, GLint n) {
	GLfloat x_start = -0.5;
	GLfloat y_start = -0.5;
	GLfloat x_step = 1.0 / GLfloat(m);
	GLfloat y_step = 1.0 / GLfloat(n);

	glBegin(GL_QUADS);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			glVertex2d(i * x_step + x_start, j * y_step + y_start);
			glVertex2d(i * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, j * y_step + y_start);
		}
	}
	glEnd();
}





inline void cube_unit2() {
	GLfloat p = 0.25;
	glBegin(GL_QUADS);

	//frente
	glVertex3f(-p, -p, p);
	glVertex3f(-p, p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, -p, p);

	//tras
	glVertex3f(-p, -p, -p);
	glVertex3f(p, -p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(-p, p, -p);

	//cima
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, -p, p);
	glVertex3f(p, -p, p);
	glVertex3f(p, -p, -p);

	//baixo
	glVertex3f(-p, p, p);
	glVertex3f(-p, p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(p, p, p);

	//esq
	glVertex3f(-p, -p, p);
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, p, -p);
	glVertex3f(-p, p, p);

	//dir
	glVertex3f(p, -p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, p, -p);
	glVertex3f(p, -p, -p);

	glEnd();

}




inline void parallelepiped_meio() {
	GLfloat width = 0.125f; // metade da largura original
	GLfloat height = 0.25f; // mesma altura original
	GLfloat depth = 0.25f;  // mesma profundidade original

	glBegin(GL_QUADS);

	// frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

	// trás
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

	// cima
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

	// baixo
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, -height, -depth);

	// esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

	// direita
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, depth);

	glEnd();
}

inline void parallelepiped_3_2_height() {
	GLfloat width = 0.25f;  // mesma largura original
	GLfloat height = 0.5f; // 3/2 da altura original
	GLfloat depth = 0.25f;  // mesma profundidade original

	glBegin(GL_QUADS);

	// frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

	// trás
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

	// cima
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

	// baixo
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, -height, -depth);

	// esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

	// direita
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, depth);

	glEnd();
}

inline void cube_unit() {
	GLfloat p = 0.25;

	glBegin(GL_QUADS);

	// frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, p);

	// trás
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, -p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, -p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, -p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, -p);

	// cima
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, p, p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p, p, p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, -p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, -p);

	// baixo
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p, -p, -p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, -p, -p);

	// esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p, -p, -p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p, p, -p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, p);

	// direita
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p, -p, p);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, -p);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, -p);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p, p, p);

	glEnd();
}



inline void draw_dino() {
	GLfloat depth = 0.25f; // Profundidade do modelo

	// Corpo (paralelepípedo vertical)
	glBegin(GL_QUADS);

	// Frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.5f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.5f, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.0f, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.0f, depth);

	// Traseira
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.5f, -depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.5f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.0f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.0f, -depth);

	// Lado Esquerdo
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.5f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, -0.5f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.0f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.0f, depth);

	// Lado Direito
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2f, -0.5f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.5f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.0f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2f, 0.0f, depth);

	glEnd();

	// Cabeça (paralelepípedo horizontal)
	glBegin(GL_QUADS);

	// Frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.3f, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, depth);

	// Traseira
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, -depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.3f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, -depth);

	// Superior (parte de cima da cabeça)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.3f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.3f, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.3f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, -depth);

	// Inferior (parte de baixo da cabeça)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.0f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -depth);

	// Lado Esquerdo da Cabeça
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.3f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, depth);

	// Lado Direito da Cabeça
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.4f, 0.0f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, -depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.3f, -depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f, 0.3f, depth);

	glEnd();

	// Braço (retângulo pequeno no meio do corpo)
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.15f, -0.1f, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.25f, -0.1f, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.25f, 0.0f, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.15f, 0.0f, depth);

	glEnd();
}

//funcao que desenha cubo unit�rio com malha_unit()
inline void cube_malha_unit(GLint m, GLint n) {


	//tras
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0., 0., -0.5);
	glRotated(180, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//cima
	glColor3f(0, 1, 1);
	glPushMatrix();
	glTranslated(0., -0.5, 0.);
	glRotated(90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//baixo
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0., 0.5, 0.);
	glRotated(-90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//esquerda
	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslated(-0.5, 0., 0.);
	glRotated(-90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//direita
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(0.5, 0., 0.);
	glRotated(90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//frente
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslated(0., 0., 0.5);
	malha_unit(m, n);
	glPopMatrix();


}
