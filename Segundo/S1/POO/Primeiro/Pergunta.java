import java.io.Serializable;

/**
 * A classe Pergunta representa uma pergunta em um jogo.
 * Cada pergunta possui uma dificuldade, um enunciado, um conjunto de respostas,
 * uma solução e uma pontuação associada.
 */
public class Pergunta implements Serializable {

    /**
     * A dificuldade da pergunta.
     */
    protected String dif;

    /**
     * O enunciado da pergunta.
     */
    protected String pergunta;

    /**
     * O conjunto de respostas possíveis para a pergunta.
     */
    protected String[] resposta;

    /**
     * A posição da resposta correta no conjunto de respostas.
     */
    protected int solucao;

    /**
     * A pontuação associada à pergunta.
     */
    protected int pontos;

    /**
     * Construtor da classe Pergunta.
     *
     * @param dif      A dificuldade da pergunta.
     * @param pergunta O enunciado da pergunta.
     * @param resposta O conjunto de respostas possíveis.
     * @param solucao  A posição da resposta correta.
     * @param pontos   A pontuação associada à pergunta.
     */
    public Pergunta(String dif, String pergunta, String[] resposta, int solucao, int pontos) {
        this.dif = dif;
        this.pergunta = pergunta;
        this.resposta = resposta;
        this.solucao = solucao;
        this.pontos = pontos;
    }
}
