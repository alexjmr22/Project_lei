#Alexandre José Martins Rodrigues; André Lourenço Albuquerque
import turtle as t
import functools
import random
import math

LARGURA_JANELA = 1024
ALTURA_JANELA = 600
DEFAULT_TURTLE_SIZE = 40
DEFAULT_TURTLE_SCALE = 3
RAIO_JOGADOR = DEFAULT_TURTLE_SIZE / DEFAULT_TURTLE_SCALE
RAIO_BOLA = DEFAULT_TURTLE_SIZE / 2
PIXEIS_MOVIMENTO = 90
LADO_MAIOR_AREA = ALTURA_JANELA / 3
LADO_MENOR_AREA = 50
RAIO_MEIO_CAMPO = LADO_MAIOR_AREA / 4
START_POS_BALIZAS = ALTURA_JANELA / 4
BOLA_START_POS = (5,5)


def jogador_cima(estado_jogo, jogador): 
    if estado_jogo[jogador].ycor() < ALTURA_JANELA/2 -PIXEIS_MOVIMENTO:
        estado_jogo[jogador].goto(estado_jogo[jogador].xcor(), estado_jogo[jogador].ycor() + PIXEIS_MOVIMENTO)  

def jogador_baixo(estado_jogo, jogador):
    if estado_jogo[jogador].ycor() > -ALTURA_JANELA/2 +PIXEIS_MOVIMENTO:
        estado_jogo[jogador].goto(estado_jogo[jogador].xcor(), estado_jogo[jogador].ycor() - PIXEIS_MOVIMENTO)
    
def jogador_direita(estado_jogo, jogador):
    if estado_jogo[jogador].xcor() < LARGURA_JANELA/2 - PIXEIS_MOVIMENTO:
        estado_jogo[jogador].goto(estado_jogo[jogador].xcor()+ PIXEIS_MOVIMENTO, estado_jogo[jogador].ycor() )
def jogador_esquerda(estado_jogo, jogador):
    if estado_jogo[jogador].xcor() > -LARGURA_JANELA/2 + PIXEIS_MOVIMENTO:
        estado_jogo[jogador].goto(estado_jogo[jogador].xcor() - PIXEIS_MOVIMENTO, estado_jogo[jogador].ycor() )

def desenha_linhas_campo():
    t.speed(0)
    t.color('white')
    t.pensize(10)
    t.pu()
    t.goto(0,-RAIO_MEIO_CAMPO*2)
    t.pd()
    t.circle(RAIO_MEIO_CAMPO*2)
    t.pu()
    t.goto(0,ALTURA_JANELA/2)
    t.seth(-90)
    t.pd()
    t.fd(ALTURA_JANELA)
    t.pu()
    t.goto(LARGURA_JANELA/2,LADO_MAIOR_AREA/2)
    t.pd()
    for i in range(2):
        t.fd(LADO_MAIOR_AREA)
        t.right(90)
        t.fd(LADO_MENOR_AREA)
        t.right(90)
    t.pu()
    t.goto(-LARGURA_JANELA/2,LADO_MAIOR_AREA/2)
    t.pd()
    for i in range(2):
        t.fd(LADO_MAIOR_AREA)
        t.right(-90)
        t.fd(LADO_MENOR_AREA)
        t.right(-90)    


def criar_bola():

    bola = t.Turtle()
    bola.shape("circle")
    bola.penup()
    bola.goto(BOLA_START_POS)
    bola.color('black')
    d = random.uniform(0,2*math.pi)
    bola.dx = math.cos(d)
    bola.dy = math.sin(d)
    
    return {'objecto' : bola , 'direção_x' : bola.dx, 'direção_y' : bola.dy, 'pos_anterior': None}


def cria_jogador(x_pos_inicial, y_pos_inicial, cor):

    turtle = t.Turtle()
    turtle.penup()
    turtle.goto(x_pos_inicial, y_pos_inicial)
    turtle.color(cor)
    
    turtle.shape("circle")
    turtle.shapesize(stretch_wid=DEFAULT_TURTLE_SCALE, stretch_len=DEFAULT_TURTLE_SCALE) 
    return turtle  


def init_state():
    estado_jogo = {}
    estado_jogo['bola'] = None
    estado_jogo['jogador_vermelho'] = None
    estado_jogo['jogador_azul'] = None
    estado_jogo['var'] = {
        'bola' : [],
        'jogador_vermelho' : [],
        'jogador_azul' : [],
    }
    estado_jogo['pontuacao_jogador_vermelho'] = 0
    estado_jogo['pontuacao_jogador_azul'] = 0
    return estado_jogo

def cria_janela():
    #create a window and declare a variable called window and call the screen()
    window=t.Screen()
    window.title("Foosball Game")
    window.bgcolor("green")
    window.setup(width = LARGURA_JANELA,height = ALTURA_JANELA)
    window.tracer(0)
    return window

def cria_quadro_resultados():
    #Code for creating pen for scorecard update
    quadro=t.Turtle()
    quadro.speed(0)
    quadro.color("Blue")
    quadro.penup()
    quadro.hideturtle()
    quadro.goto(0,260)
    quadro.write("Player A: 0\t\tPlayer B: 0 ", align="center", font=('Monaco',24,"normal"))
    return quadro

def terminar_jogo(estado_jogo):
    with open('historico_resultados.csv','a+') as f:
        f.seek(0)
        a = f.read()
        if a == '':
            f.write('NJogo,JogadorVermelho,JogadorAzul\n') 
        f.seek(0)
        b = f.readlines()         
        f.write("{},{},{}\n".format(len(b),estado_jogo['pontuacao_jogador_vermelho'],estado_jogo['pontuacao_jogador_azul']))
   
    print("Adeus")
    estado_jogo['janela'].bye()
       
def setup(estado_jogo, jogar):
    janela = cria_janela()
    #Assign keys to play
    janela.listen()
    if jogar:
        janela.onkeypress(functools.partial(jogador_cima, estado_jogo, 'jogador_vermelho') ,'w')
        janela.onkeypress(functools.partial(jogador_baixo, estado_jogo, 'jogador_vermelho') ,'s')
        janela.onkeypress(functools.partial(jogador_esquerda, estado_jogo, 'jogador_vermelho') ,'a')
        janela.onkeypress(functools.partial(jogador_direita, estado_jogo, 'jogador_vermelho') ,'d')
        janela.onkeypress(functools.partial(jogador_cima, estado_jogo, 'jogador_azul') ,'Up')
        janela.onkeypress(functools.partial(jogador_baixo, estado_jogo, 'jogador_azul') ,'Down')
        janela.onkeypress(functools.partial(jogador_esquerda, estado_jogo, 'jogador_azul') ,'Left')
        janela.onkeypress(functools.partial(jogador_direita, estado_jogo, 'jogador_azul') ,'Right')
        janela.onkeypress(functools.partial(terminar_jogo, estado_jogo) ,'Escape')
        quadro = cria_quadro_resultados()
        estado_jogo['quadro'] = quadro
    desenha_linhas_campo()
    bola = criar_bola()
    jogador_vermelho = cria_jogador(-((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0, "red")
    jogador_azul = cria_jogador(((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0, "blue")
    estado_jogo['janela'] = janela
    estado_jogo['bola'] = bola
    estado_jogo['jogador_vermelho'] = jogador_vermelho
    estado_jogo['jogador_azul'] = jogador_azul


def update_board(estado_jogo):
    estado_jogo['quadro'].clear()
    estado_jogo['quadro'].write("Player A: {}\t\tPlayer B: {} ".format(estado_jogo['pontuacao_jogador_vermelho'], estado_jogo['pontuacao_jogador_azul']),align="center",font=('Monaco',24,"normal"))

def movimenta_bola(estado_jogo):
    bola =estado_jogo['bola']
    bola['objecto'].goto(bola['objecto'].xcor() + bola['direção_x']/2,bola['objecto'].ycor() + bola['direção_y']/2)
    bola['posicao_anterior'] = bola['objecto'].pos() 
    guarda_posicoes_para_var(estado_jogo)
    

def verifica_colisoes_ambiente(estado_jogo):
   
    bola = estado_jogo['bola']
    x_bola = bola['objecto'].xcor()
    y_bola = bola['objecto'].ycor()
    if y_bola + RAIO_BOLA >= ALTURA_JANELA/2 or y_bola - RAIO_BOLA <= -ALTURA_JANELA/2:
        bola['direção_y'] = -bola['direção_y']
    if x_bola + RAIO_BOLA >= LARGURA_JANELA/2 or x_bola -RAIO_BOLA <= -LARGURA_JANELA/2:
        bola['direção_x'] = -bola['direção_x']
        

def updatef(estado_jogo):
    v = str(estado_jogo['pontuacao_jogador_vermelho'])
    a = str(estado_jogo['pontuacao_jogador_azul'])
    guarda_posicoes_para_var(estado_jogo)
    cordb =''
    cordja =''
    cordjv = ''
    with open('replay_golo_jv_' + v + '_ja_' + a + ".txt", "w") as f:
        for cordenadas in estado_jogo['var']['bola']:
            cordb += "{:.3f},{:.3f};".format(cordenadas[0], cordenadas[1])
        f.write(cordb.rstrip(';'))
        f.write("\n")
        for cordenadas in estado_jogo['var']['jogador_vermelho']:
            cordjv += "{:.3f},{:.3f};".format(cordenadas[0], cordenadas[1])
        f.write(cordjv.rstrip(';'))
        f.write("\n")
        for cordenadas in estado_jogo['var']['jogador_azul']:
            cordja += "{:.3f},{:.3f};".format(cordenadas[0], cordenadas[1])
        f.write(cordja.rstrip(';'))
        f.write("\n")        

def verifica_golo_jogador_vermelho(estado_jogo):
    bola = estado_jogo['bola']
    x_bola = bola['objecto'].xcor()
    y_bola = bola['objecto'].ycor()    
    if x_bola + RAIO_BOLA >= LARGURA_JANELA/2: 
        if y_bola + RAIO_BOLA <= START_POS_BALIZAS/2 and y_bola -RAIO_BOLA >= -START_POS_BALIZAS/2:
            estado_jogo['pontuacao_jogador_vermelho'] +=1
            update_board(estado_jogo)
            bola['objecto'].goto(BOLA_START_POS)
            d = random.uniform(0,2*math.pi)
            bola['direção_x'] = math.cos(d)
            bola['direção_y'] = math.sin(d)
            updatef(estado_jogo)
            estado_jogo['var']['bola'] =[]
            estado_jogo['var']['jogador_vermelho'] = []
            estado_jogo['var']['jogador_azul']= []
            
            
                        
        
def verifica_golo_jogador_azul(estado_jogo):
    bola = estado_jogo['bola']
    x_bola = bola['objecto'].xcor()
    y_bola = bola['objecto'].ycor()    
    if x_bola -RAIO_BOLA <= -LARGURA_JANELA/2: 
        if y_bola + RAIO_BOLA <= START_POS_BALIZAS/2 and y_bola -RAIO_BOLA >= -START_POS_BALIZAS/2:
            estado_jogo['pontuacao_jogador_azul'] +=1
            update_board(estado_jogo)
            bola['objecto'].goto(BOLA_START_POS)
            d = random.uniform(0,2*math.pi)
            bola['direção_x'] = math.cos(d)
            bola['direção_y'] = math.sin(d)  
            updatef(estado_jogo)
            estado_jogo['var']['bola'] =[]
            estado_jogo['var']['jogador_vermelho'] = []
            estado_jogo['var']['jogador_azul']= []
                        

def verifica_golos(estado_jogo):
    verifica_golo_jogador_vermelho(estado_jogo)
    verifica_golo_jogador_azul(estado_jogo)
   


def verifica_toque_jogador_azul(estado_jogo):
    bola = estado_jogo['bola']
    jogador_azul = estado_jogo['jogador_azul']
   
    if jogador_azul.distance(bola['objecto']) <= RAIO_BOLA + RAIO_JOGADOR:
        angulo = math.atan2(bola['objecto'].ycor() - jogador_azul.ycor(), bola['objecto'].xcor() - jogador_azul.xcor())
        velocidade = math.sqrt(bola['direção_x']**2 + bola['direção_y']**2)
        bola['direção_x'] = math.cos(angulo) * velocidade
        bola['direção_y'] = math.sin(angulo) * velocidade    





def verifica_toque_jogador_vermelho(estado_jogo):
    bola = estado_jogo['bola']
    jogador_vermelho = estado_jogo['jogador_vermelho']
    if jogador_vermelho.distance(bola['objecto']) <= RAIO_BOLA + RAIO_JOGADOR:
        angulo = math.atan2(bola['objecto'].ycor() - jogador_vermelho.ycor(), bola['objecto'].xcor() - jogador_vermelho.xcor())
        velocidade = math.sqrt(bola['direção_x']**2 + bola['direção_y']**2)
        bola['direção_x'] = math.cos(angulo) * velocidade
        bola['direção_y'] = math.sin(angulo) * velocidade


def guarda_posicoes_para_var(estado_jogo):
    estado_jogo['var']['bola'].append(estado_jogo['bola']['objecto'].pos())
    estado_jogo['var']['jogador_vermelho'].append(estado_jogo['jogador_vermelho'].pos())
    estado_jogo['var']['jogador_azul'].append(estado_jogo['jogador_azul'].pos())


def main():
    estado_jogo = init_state()
    setup(estado_jogo, True)
    while True:
        estado_jogo['janela'].update()
        if estado_jogo['bola'] is not None:
            movimenta_bola(estado_jogo)
        verifica_colisoes_ambiente(estado_jogo)
        verifica_golos(estado_jogo)
        if estado_jogo['jogador_vermelho'] is not None:
            verifica_toque_jogador_azul(estado_jogo)
        if estado_jogo['jogador_azul'] is not None:
            verifica_toque_jogador_vermelho(estado_jogo)

if __name__ == '__main__':
    main()