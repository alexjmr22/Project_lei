import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * A classe LeitorFicheiro proporciona métodos para ler perguntas a partir de um ficheiro.
 * Cada linha do ficheiro deve conter dados específicos sobre uma pergunta.
 */
public class LeitorFicheiro {

    /**
     * Lê perguntas a partir de um ficheiro especificado.
     *
     * @param nomeFicheiro O nome do ficheiro a ser lido.
     * @return Uma lista de perguntas lidas a partir do ficheiro.
     */
    public static List<Pergunta> lerFicheiro(String nomeFicheiro) {
        List<Pergunta> perguntas = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(nomeFicheiro))) {
            String linha;
            while ((linha = br.readLine()) != null) {
                String[] dados = linha.split(";");

                if (dados.length == 6) {  // Ajuste para 6, pois há 5 ponto e vírgula
                    String tipo = dados[0].trim().toLowerCase();
                    String dif = dados[1].trim().toUpperCase();
                    String pergunta = dados[2].trim();
                    String[] respostas = dados[3].split(", ");

                    try {
                        int solucao = Integer.parseInt(dados[4].trim());
                        int pontos = Integer.parseInt(dados[5].trim());

                        Pergunta novaPergunta = criarPergunta(tipo, dif, pergunta, respostas,  solucao, pontos);
                        perguntas.add(novaPergunta);
                    } catch (NumberFormatException e) {
                        System.err.println("Erro ao converter valor para número: " + dados[4].trim());
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException e) {
            System.err.println("Erro de leitura do arquivo: " + e.getMessage());
            e.printStackTrace();
        }

        return perguntas;
    }

    /**
     * Cria uma nova instância de Pergunta com base nos parâmetros fornecidos.
     *
     * @param tipo     O tipo de pergunta.
     * @param dif      A dificuldade da pergunta.
     * @param pergunta A descrição da pergunta.
     * @param respostas As opções de resposta.
     * @param solucao  A solução da pergunta.
     * @param pontos   Os pontos associados à pergunta.
     * @return Uma nova instância de Pergunta ou uma subclasse específica de Pergunta, dependendo do tipo.
     */
    private static Pergunta criarPergunta(String tipo, String dif, String pergunta, String[] respostas, int solucao, int pontos) {
        switch (tipo) {
            case "desporto":
                return new Desporto(dif, pergunta, respostas, solucao, pontos);
            case "ciencia":
                return new Ciencias(dif, pergunta, respostas, solucao, pontos);
            case "artes":
                return new Arte(dif, pergunta, respostas, solucao, pontos);
            case "futebol":
                return new Futebol(dif, pergunta, respostas, solucao, pontos);
            case "natacao":
                return new Natacao(dif, pergunta, respostas, solucao, pontos);
            case "ski":
                return new Ski(dif, pergunta, respostas, solucao, pontos);
            default:
                System.err.println("Tipo de pergunta desconhecido: " + tipo);
                return null;
        }
    }
}
