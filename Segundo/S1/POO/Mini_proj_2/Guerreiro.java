
/**
 * Classe que representa o tipo de personagem "Guerreiro".
 */
class Guerreiro extends PoW {
    protected boolean armaduras;
    protected String arma;
    /**
     * Construtor da classe Guerreiro.
     *  @param nome -> O nome do Guerreiro.
     *  @param experiencia ->A experiência do Guerreiro.
     *  @param armaduras ->Se o Guerreiro possui armaduras.
     *  @param arma-> A arma do Guerreiro.
     */
    public Guerreiro(String nome, int Experiência, boolean armaduras, String arma) {
        super(nome, Experiência);
        this.armaduras = armaduras;
        this.arma = arma;
        iniciais();  /** Chama o método para definir estatísticas iniciais*/
    }

    private void iniciais() {   /**metodo para definir estatisticas iniciais do tipo de personagens*/
        this.Força = 10;
        this.Agilidade = 5;
        this.Inteligência = 3;
    }

    public boolean verArma() {
        return armaduras;
    }

    public void levelUp() {
        Experiência++;
        this.Força += this.Força * 0.2;
        this.Agilidade += this.Agilidade * 0.1;
        this.Inteligência += this.Inteligência * 0.05;
    }
}