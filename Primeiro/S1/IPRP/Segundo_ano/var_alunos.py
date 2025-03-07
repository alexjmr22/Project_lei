#Alexandre José Martins Rodrigues; André Lourenço Albuquerque

import foosball_alunos

def le_replay(nome_ficheiro):
    replay_data = {'bola': [], 'jogador_vermelho': [], 'jogador_azul': []}
    with open(nome_ficheiro, "r") as f:

        b_line = f.readline()
        b_line = b_line.strip().split(';')
        for element in b_line:
            replay_data['bola'].append(tuple(map(float, element.split(','))))
        v_line = f.readline()
        v_line = v_line.strip().split(';')
        
        for element in v_line:
            replay_data['jogador_vermelho'].append(tuple(map(float, element.split(','))))
        a_line = f.readline()
        a_line = a_line.strip().split(';')
        
        for element in a_line:
            replay_data['jogador_azul'].append(tuple(map(float, element.split(','))))
        return replay_data
    
    
def main():
    estado_jogo = foosball_alunos.init_state()
    foosball_alunos.setup(estado_jogo, False)
    replay = le_replay('replay_golo_jv_1_ja_0.txt')
    for i in range(len(replay['bola'])):
        estado_jogo['janela'].update()
        estado_jogo['jogador_vermelho'].setpos(replay['jogador_vermelho'][i])
        estado_jogo['jogador_azul'].setpos(replay['jogador_azul'][i])
        estado_jogo['bola']['objecto'].setpos(replay['bola'][i])
    estado_jogo['janela'].exitonclick()


if __name__ == '__main__':
    main()