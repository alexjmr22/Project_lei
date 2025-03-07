import java.util.ArrayList;
import java.util.List;
/**
 * Classe base para personagens do jogo.
 */
class PoW {
    protected String nome;
    protected String TipoDePersonagem;
    protected int Experiência;
    protected float Força;
    protected float Agilidade;
    protected float Inteligência;
    List<PoW> Personagens = new ArrayList<>();
    /**
     * Construtor padrão da classe PoW.
     */
    public PoW(){
    }
    /**
     * Criação de personagens e os adiciona à lista de personagens.
     */
    public void criaPersona(){
        Guerreiro zeca = new Guerreiro("zeca", ((int) (Math.random() * 25) + 1), true, "machado");
        Guerreiro vila = new Guerreiro("titi", ((int) (Math.random() * 25) + 1), false, "faca");
        Guerreiro pirex = new Guerreiro("pirex", ((int) (Math.random() * 25) + 1), true, "espada");
        Guerreiro andre = new Guerreiro("andre", ((int) (Math.random() * 25) + 1), false, "espada");
        Guerreiro madeira = new Guerreiro("madeira", ((int) (Math.random() * 25) + 1), false, "faca");

        Mago bigodes = new Mago("barraca", ((int) (Math.random() * 25) + 1), "sesamo");
        Mago titi = new Mago("vila",  ((int) (Math.random() * 25) + 1), "girassol");
        Mago morenito = new Mago("morenito", ((int) (Math.random() * 25) + 1), "abóbora");
        Mago barata = new Mago("barata",  ((int) (Math.random() * 25) + 1), "sesamo");
        Mago barritz = new Mago("barritz",  ((int) (Math.random() * 25) + 1), "girassol");

        Mercenario rafa = new Mercenario("rafa",  ((int) (Math.random() * 25) + 1), "pedras", 21);
        Mercenario musa = new Mercenario("musa",  ((int) (Math.random() * 25) + 1), "arco", 21);
        Mercenario guedes = new Mercenario("guedes", ((int) (Math.random() * 25) + 1), "arco", 21);
        Mercenario neves = new Mercenario("neves", ((int) (Math.random() * 25) + 1), "pistolas", 21);
        Mercenario toninho = new Mercenario("toninho", ((int) (Math.random() * 25) + 1), "pistolas", 21);

        Personagens.add(zeca);
        Personagens.add(titi);
        Personagens.add(pirex);
        Personagens.add(andre);
        Personagens.add(madeira);
        Personagens.add(bigodes);
        Personagens.add(vila);
        Personagens.add(morenito);
        Personagens.add(barata);
        Personagens.add(barritz);
        Personagens.add(rafa);
        Personagens.add(musa);
        Personagens.add(guedes);
        Personagens.add(neves);
        Personagens.add(toninho);
    }
    /**
     * Construtor da classe PoW.
     * @param nome O nome do personagem.
     * @param experiencia A experiência do personagem.
     */
    public PoW(String nome, int Experiência) {

        this.nome = nome;
        this.TipoDePersonagem = TipoDePersonagem;
        this.Experiência = Experiência;
        float força = 0;
        float agilidade = 0;
        float inteligência = 0;

        this.Força = força;
        this.Agilidade = agilidade;
        this.Inteligência = inteligência;
    }
    /**
     * mostra as informações sobre personagens com experiência maiores de 10.
     */
    public  void levelTen() {
        for (PoW i : Personagens) {
            if (i.Experiência>10) {
                i.stat();
            }
        }
    }
    /**
     * Exibe informações sobre personagens com a arma / sementes / armadura solicitada.
     */
    public  void confere() {
        for (PoW i : Personagens) {
            if (i.verArma()) {
                i.stat();
            }
        }
    }
    /**
     * Exibe informações sobre o personagem.
     */
    public void stat(){ /** da print às várias variáveis*/
        System.out.println("Nome: " + nome + ", Tipo: " + TipoDePersonagem + ", Experiência: " + Experiência);
        System.out.println("Força: " + Força + ", Agilidade: " + Agilidade + ", Inteligência: " + Inteligência);
        System.out.println();
    }
    /**
     * Aumenta o nível de todos os personagens na lista.
     */
    public void levelUp() {
        for(PoW i : Personagens){
            i.levelUp();

        }
    }
    /**
     * Verifica se o personagem possui uma arma, sementes ou armadura específica.
     * @return true se o personagem possuir o item solicitado, caso contrário, false.
     */
    public boolean verArma(){ /** se o mercenario usa arco ou se o mago tem sementes de abobora ou  se o guerreiro tem armadura retorna true*/
        return true;
    }
}