class Futebol extends Desporto{
    public Futebol(String dif,String pergunta, String[] resposta,  int solucao, int pontos) {
        super(dif,pergunta, resposta, solucao, pontos + 1);
    }
}
