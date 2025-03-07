import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;

/**
 * A classe P_Frame representa a janela principal do jogo de perguntas.
 * Ela exibe as perguntas, opções de resposta e controla o fluxo do jogo.
 */

public class P_Frame extends JFrame {
    private List<Pergunta> perguntas;
    private int perguntaAtual = 0;
    private int perguntasExibidas = 0;
    private int pontosTotais = 0;
    private Jogador jogador;
    private JLabel perguntaLabel;
    private ButtonGroup opcoesGroup;
    private JRadioButton[] opcoesButtons;
    private JButton proximaPerguntaButton;

    /**
     * Construtor da classe P_Frame.
     *
     * @param perguntas Lista de perguntas para o jogo.
     * @param jogador   Jogador associado à instância do jogo.
     */

    public P_Frame(List<Pergunta> perguntas, Jogador jogador) {
        this.perguntas = perguntas;
        this.jogador = jogador;
        initComponents();
        exibirPerguntaAtual();
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                jogador.guardarEstadoJogo();
            }
        });
    }

    /**
     * Exibe a janela com o TOP 3 jogadores.
     */

     private void exibirTop3() {
        List<Jogador> todosJogadores = Jogador.carregarTodosEstadosJogo();
        
        // Verifica se a lista do TOP 3 é nula ou vazia antes de exibir a mensagem
        List<Jogador> top3 = Jogador.calcularTop3(todosJogadores);
        if (top3 != null && !top3.isEmpty()) {
            StringBuilder top3Message = new StringBuilder("TOP 3 Jogadores:\n");
            for (int i = 0; i < top3.size(); i++) {
                Jogador jogadorTop = top3.get(i);
                top3Message.append((i + 1)).append(". ").append(jogadorTop.obterNome())
                        .append(" - Pontuação: ").append(jogadorTop.obterPontos()).append("\n");
            }
    
            JOptionPane.showMessageDialog(this, top3Message.toString(), "TOP 3", JOptionPane.INFORMATION_MESSAGE);
        }
        // Se top3 for nulo ou vazio, não exibe nada.
    }

    /**
     * Inicializa os componentes da interface gráfica.
     */
    
    private void initComponents() {
        perguntaLabel = new JLabel();
        perguntaLabel.setAlignmentX(CENTER_ALIGNMENT);
        perguntaLabel.setVerticalAlignment(JLabel.TOP);
        perguntaLabel.setHorizontalAlignment(JLabel.CENTER);
        perguntaLabel.setOpaque(true);
    
        int quantidadeOpcoes = 5;
    
        opcoesButtons = new JRadioButton[quantidadeOpcoes];
        opcoesGroup = new ButtonGroup();
    
        for (int i = 0; i < quantidadeOpcoes; i++) {
            opcoesButtons[i] = new JRadioButton();
            opcoesGroup.add(opcoesButtons[i]);
        }
    
        proximaPerguntaButton = new JButton("Próxima Pergunta");
    
        setTitle("Jogo de Perguntas");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400, 300);
        setLocationRelativeTo(null);
    
        setLayout(new BorderLayout());
    
        JPanel panelPergunta = new JPanel();
        panelPergunta.setLayout(new BoxLayout(panelPergunta, BoxLayout.Y_AXIS));
        panelPergunta.add(Box.createVerticalStrut(20));
        panelPergunta.add(perguntaLabel);
        panelPergunta.add(Box.createVerticalStrut(40));
    
        JPanel panelOpcoes = new JPanel(new GridLayout(0, 1, 10, 10));
        for (int i = 0; i < quantidadeOpcoes; i++) {
            panelOpcoes.add(opcoesButtons[i]);
        }
    
        JPanel panelBotoes = new JPanel(new FlowLayout(FlowLayout.CENTER));
        panelBotoes.add(proximaPerguntaButton);
    
        add(panelPergunta, BorderLayout.NORTH);
        add(panelOpcoes, BorderLayout.CENTER);
        add(panelBotoes, BorderLayout.SOUTH);
    
        proximaPerguntaButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                processarResposta();
            }
        });
    }
    
    /**
     * Determina se uma pergunta fácil deve ser exibida com regras específicas.
     *
     * @param pergunta Pergunta a ser verificada.
     * @return true se a pergunta deve ser exibida de acordo com as regras específicas, false caso contrário.
     */
    
    private boolean deveExibirPerguntaFacil(Pergunta pergunta) {
        return pergunta.dif.equalsIgnoreCase("F") && perguntaAtual < 3;
    }

    private void processarResposta() {
        if (perguntaAtual < perguntas.size()) {
            Pergunta pergunta = perguntas.get(perguntaAtual);
            int respostaInt = -1;
    
            for (int i = 0; i < opcoesButtons.length; i++) {
                if (opcoesButtons[i].isSelected()) {
                    respostaInt = i;
                    break;
                }
            }
    
            if (deveExibirPerguntaFacil(pergunta)) {
                
                if (respostaInt == pergunta.solucao) {
                    JOptionPane.showMessageDialog(this, "Resposta Correta! Ganhou " + pergunta.pontos + " pontos!");
                    pontosTotais += pergunta.pontos;
                    jogador.acertouPergunta(pergunta);
                } else {
                    JOptionPane.showMessageDialog(this, "Resposta Incorreta.");
                    jogador.errouPergunta(pergunta);
                }
                perguntaAtual++;
                opcoesGroup.clearSelection();
                exibirPerguntaAtual();
            } else {
                if (respostaInt == pergunta.solucao) {
                    JOptionPane.showMessageDialog(this, "Resposta Correta! Ganhou " + pergunta.pontos + " pontos!");
                    pontosTotais += pergunta.pontos;
                    jogador.acertouPergunta(pergunta);
                } else {
                    JOptionPane.showMessageDialog(this, "Resposta Incorreta.");
                    jogador.errouPergunta(pergunta);
                }
    
                perguntaAtual++;
                opcoesGroup.clearSelection();
                if (perguntasExibidas == 5) {
                    JOptionPane.showMessageDialog(this, "Jogo concluído. Pontuação total: " + pontosTotais + " pontos.");
                    jogador.guardarEstadoJogo();
                    exibirTop3();
                    dispose();
                }
                exibirPerguntaAtual();
            }
        }
    }

    /**
     * Exibe a pergunta atual na interface gráfica.
     */

    private void exibirPerguntaAtual() {
        if (perguntaAtual < perguntas.size() && perguntasExibidas < 5) {
            Pergunta pergunta = perguntas.get(perguntaAtual);
            perguntaLabel.setText("<html><body style='width: 600px;'>" + pergunta.pergunta + "</body></html>");

            String[] opcoes = pergunta.resposta;

            for (int i = 0; i < opcoesButtons.length; i++) {
                if (i < opcoes.length) {
                    opcoesButtons[i].setText(opcoes[i]);
                    opcoesButtons[i].setVisible(true);
                } else {
                    opcoesButtons[i].setVisible(false);
                }
            }

            perguntasExibidas++;
        } else {
            if (perguntasExibidas == 5) {
                perguntaLabel.setText("<html><body style='width: 600px;'>Jogo concluído. Pontuação total: " + pontosTotais + " pontos.</body></html>");
                for (JRadioButton button : opcoesButtons) {
                    button.setVisible(false);
                }
                proximaPerguntaButton.setText("OK");
                proximaPerguntaButton.setEnabled(true);
            } else {
                perguntaLabel.setText("<html><body style='width: 600px;'>Jogo concluído. Parabéns!</body></html>");
                for (JRadioButton button : opcoesButtons) {
                    button.setVisible(false);
                }
                proximaPerguntaButton.setEnabled(false);
            }
        }
    }
}
