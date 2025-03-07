# Tiago Lopes Gonçalves uc2022230506 Alexandre Rodrigues uc2022249408 TP9
import time
import random
import functools
import turtle

MAX_X = 600
MAX_Y = 800
DEFAULT_SIZE = 20
SNAKE_SHAPE = 'square'
HIGH_SCORES_FILE_PATH = 'high_scores.txt'
# Controla a velocidade da cobra. Quanto menor o valor, mais rápido é o movimento da cobra.
SPEED = 0.075


def load_high_score(state):
    # se já existir um high score devem guardar o valor em state['high_score']
    state['high_score'] = 0
    score = open(HIGH_SCORES_FILE_PATH,"r")
    recordes=score.readlines()
    comprimento = len(recordes)
    for i in range(comprimento):       
        dado = recordes[i].split("\t")
        if state["high_score"] < int(dado[1]):
            state["high_score"] = int(dado[1])
    score.close()

def write_high_score_to_file(state):
    # devem escrever o valor que está em state['high_score'] no ficheiro de high scores
    name = input("Qual é o teu nome?")
    score = open(HIGH_SCORES_FILE_PATH,"a")
    score.write(str(name) + "\t" +str(state['new_high_score']) + "\n")
    score.close()

def create_score_board(state):
    score_board = turtle.Turtle()
    score_board.speed(0)
    score_board.shape("square")
    score_board.color("black")
    score_board.penup()
    score_board.hideturtle()
    score_board.goto(0, MAX_Y / 2.2)
    state['score_board'] = score_board
    load_high_score(state)
    update_score_board(state)

def update_score_board(state):
    state['score_board'].clear()
    state['score_board'].write("Score: {} High Score: {}".format(state['score'], state['high_score']), align="center", font=("Helvetica", 24, "normal"))

def go_up(state):
    if state['snake']['current_direction'] != 'down':
        state['snake']['current_direction'] = 'up'

def go_down(state):
    if state['snake']['current_direction'] != 'up':
        state['snake']['current_direction'] = 'down'

def go_left(state):
    if state['snake']['current_direction'] != 'right':
        state['snake']['current_direction'] = 'left'

def go_right(state):
    if state['snake']['current_direction'] != 'left':
        state['snake']['current_direction'] = 'right'

def init_state():
    state = {}
    # Informação necessária para a criação do score board
    state['score_board'] = None
    state['new_high_score'] = False
    state['high_score'] = 0
    state['score'] = 0
    # Para gerar a comida deverá criar um nova tartaruga e colocar a mesma numa posição aleatória do campo
    state['food'] = turtle.Turtle()
    state['window'] = None
    snake = {
        'head': None,                  # Variável que corresponde à cabeça da cobra
        'current_direction': None ,    # Indicação da direcção atual do movimento da cobra
        'pedacos': [None] 
    }
    state['snake'] = snake
    state['comando'] =[None,None]
    return state

def setup(state):
    window = turtle.Screen()
    window.setup(width=MAX_X, height=MAX_Y)
    window.listen()
    window.onkey(functools.partial(go_up, state), 'w')
    window.onkey(functools.partial(go_down, state), 's')
    window.onkey(functools.partial(go_left, state), 'a')
    window.onkey(functools.partial(go_right, state), 'd')
    window.tracer(0)
    state['window'] = window
    snake = state['snake']
    snake['current_direction'] = 'stop'
    snake['head'] = turtle.Turtle()
    snake['head'].shape(SNAKE_SHAPE)
    snake['head'].showturtle()
    snake['head'].pu()
    snake['head'].color('green')
    create_score_board(state)
    create_food(state)

def design(turtle):
    turtle.shape(SNAKE_SHAPE)
    turtle.color('black')
    turtle.pu()
    
def move(state):
    ''' 
    Função responsável pelo movimento da cobra no ambiente.
    '''
    snake = state['snake']
    if snake['current_direction']!= 'stop':
        snake['pedacos'][0] = snake['head']
        snake['head'].forward(22)
        if snake['current_direction'] =='up':
            snake['head'].seth(90)
        if snake['current_direction'] =='right':
            snake['head'].seth(0)
        if snake['current_direction'] =='left':
            snake['head'].seth(180)
        if snake['current_direction'] =='down':
            snake['head'].seth(270) 
            
        for i in range(len(snake['pedacos'])-1,0,-1):
            snake['pedacos'][i].goto(state['comando'][(2*i)-2],state['comando'][(2*i)-1])
            state['comando'][2*i] = snake['pedacos'][i].xcor()
            state['comando'][(2*i)+1] = snake['pedacos'][i].ycor()
        state['comando'][0] = snake['head'].xcor()
        state['comando'][1] = snake['head'].ycor()        
            

def create_food(state):
    ''' 
        Função responsável pela criação da comida. Note que elas deverão ser colocadas em posições aleatórias, mas dentro dos limites do ambiente.
    '''
    food = state['food']
    snake = state['snake']
    food.shape('circle')
    food.color('red')
    food.up()
    place = True
    while place == True:
        place = False
        if snake['head'].distance(food)<=15:
            place = True
        food.goto(random.randint(-(MAX_X/2)+30,(MAX_X/2)-30),random.randint(-(MAX_Y/2)+30,(MAX_Y/2)-30))
        for i in range(1,len(snake['pedacos'])):
            if snake['pedacos'][i].distance(food)<=15:
                place = True
    # a informação sobre a comida deve ser guardada em state['food']

def check_if_food_to_eat(state):
    ''' 
        Função responsável por verificar se a cobra tem uma peça de comida para comer. Deverá considerar que se a comida estiver a uma distância inferior a 15 pixels a cobra pode comer a peça de comida. 
    '''
    food = state['food']
    snake = state['snake']
    if snake['head'].distance(food)<=15:
        state['comando'].append('')
        state['comando'].append('')
        snake['pedacos'].append(turtle.Turtle())
        design(snake['pedacos'][-1])
        snake['pedacos'][-1].goto(snake["pedacos"][-2].xcor()-15,snake["pedacos"][-2].ycor()-15)
        create_food(state)
        state['score'] = state['score'] + 10
        if state['high_score'] < state['score']:
            state['new_high_score'] = state['score'] 
            
        update_score_board(state)
        
    # para ler ou escrever os valores de high score, score e new high score, devem usar os respetivos campos do state: state['high_score'], state['score'] e state['new_high_score']

def boundaries_collision(state):
    ''' 
        Função responsável por verificar se a cobra colidiu com os limites do ambiente. Sempre que isto acontecer a função deverá returnar o valor booleano True, caso contrário retorna False.
    '''
    snake = state['snake']
    if -MAX_X/2 <= snake['head'].xcor() <= MAX_X/2 and -MAX_Y/2 <= snake['head'].ycor() <= MAX_Y/2:
        return False
    else:
        return True

def body_collision(state):
    snake = state['snake']
    detect = False
    for i in range(1,len(snake['pedacos'])):
        if snake['head'].distance(snake['pedacos'][i])<20:
            detect = True
    return detect

def check_collisions(state):
    '''
        Função responsável por avaliar se há colisões. Atualmente apenas chama a função que verifica se há colisões com os limites do ambiente. No entanto deverá escrever o código para verificar quando é que a tartaruga choca com uma parede ou com o seu corpo.
    '''
    snake = state['snake']
    return (boundaries_collision(state) or body_collision(state))

def main():
    state = init_state()
    setup(state)
    while not check_collisions(state):
        state['window'].update()
        check_if_food_to_eat(state)
        move(state)
        time.sleep(SPEED) 
    turtle.bye()
    print("YOU LOSE!")
    if state['new_high_score'] > state['high_score']:
        write_high_score_to_file(state)


main()
