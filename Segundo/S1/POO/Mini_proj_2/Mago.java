/**
 * Classe que representa o tipo de personagem "Mago".
 */
class Mago extends PoW {
    protected String sementes;
    /**
     * Construtor da classe Mago.
     *  @param nome ->O nome do Mago.
     *  @param  experiencia-> A experiência do Mago.
     *  @param sementes-> As sementes do Mago.
     */
    public Mago(String nome, int Experiência, String sementes) {
        super(nome, Experiência);
        this.sementes = sementes;
        iniciais();  /**Chama o método para definir estatísticas iniciais*/
    }/**metodo para definir estatisticas iniciais do tipo de personagens*/

    private void iniciais() { /**metodo para definir estatisticas iniciais do tipo de personagens*/
        this.Força = 2;
        this.Agilidade = 4;
        this.Inteligência = 10;
    }

    public boolean verArma() {
        return sementes.equals("abóbora");
    }

    public void levelUp() {
        Experiência++;
        this.Força += this.Força * 0.05;
        this.Agilidade += this.Agilidade * 0.1;
        this.Inteligência += this.Inteligência * 0.2;
    }
}