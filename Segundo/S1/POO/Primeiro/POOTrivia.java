import javax.swing.*;
import java.util.Collections;
import java.util.List;
import java.awt.event.*;

public class POOTrivia {
    
    /** 
     * @param args
     */
    public static void main(String[] args) {
        System.setProperty("file.encoding", "UTF-8");
        String nomeFicheiro = "pootrivia.txt";
        List<Pergunta> perguntas = LeitorFicheiro.lerFicheiro(nomeFicheiro);

        // Pedir o nome ao jogador
        String nomeJogador = JOptionPane.showInputDialog(null, "Digite o seu nome:");

        // Criar a instância do jogador com o nome fornecido
        Jogador jogador = new Jogador(nomeJogador);
        iniciarJogo(perguntas, jogador);
    }

    
    /** 
     * @param perguntas
     * @param jogador
     */
    private static void iniciarJogo(List<Pergunta> perguntas, Jogador jogador) {
        Collections.shuffle(perguntas);
    
        SwingUtilities.invokeLater(() -> {
            P_Frame frame = new P_Frame(perguntas, jogador);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
    
            frame.addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosed(WindowEvent e) {
                    // Ao fechar a janela, imprimir as informações do jogador
                    jogador.imprimirInformacoes();
                }
            });
        });
    }
}