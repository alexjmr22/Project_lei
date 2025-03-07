/**
 * Classe que representa o tipo de personagem "Mercenario".
 */
class Mercenario extends PoW {
    protected String armas;
    protected int munições;
    /**
     * Construtor da classe Mercenario.
     *  @param nome ->O nome do Mercenario.
     *  @param experiencia ->A experiência do Mercenario.
     *  @param armas -> As armas do Mercenario.
     */
    public Mercenario(String nome, int Experiência, String armas, int munições) {
        super(nome, Experiência);
        this.armas = armas;
        this.munições = munições;
        iniciais();  /** Chama o método para definir estatísticas iniciais*/
    }

    private void iniciais() { /** metodo para definir estatisticas iniciais do tipo de personagens*/
        this.Força = 3;
        this.Agilidade = 9;
        this.Inteligência = 4;
    }

    public boolean verArma() {
        return armas.equals("arco");
    }

    public void levelUp() {
        Experiência++;
        this.Força += this.Força * 0.08;
        this.Agilidade += this.Agilidade * 0.2;
        this.Inteligência += this.Inteligência * 0.08;
    }
}