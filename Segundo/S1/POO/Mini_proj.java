public class Exercicio1 {
    public static void main(String[] args) {
        String URLs[] = {
            "https://www.dei.uc.pt/poao/exames",
                    "http://www.scs.org/index.html",
                    "https://www.nato.int/events",
                    "https://www.btu.de/",
                    "https://www.dei.uc.pt/poao/exames",
                    "http://www.eth.ch/index.html",
                    "http://www.osu.edu/"
        };
        String paises[][] = {   
                {"pt", "Portugal"},
                {"org", "EUA"},
                {"fr", "França"},
                {"uk", "Reino Unido"},
                {"de", "Alemanha"},
                {"edu", "EUA"}};
        String links[] = {};
        String sigla[] = {};

        int outros = 0;
        int[] tab = new int[paises.length];


        String finais = null;
        for (int i = 0; i < URLs.length; i++) {
            links = URLs[i].split("/");
            sigla = links[2].split("\\.");
            finais = sigla[sigla.length - 1];
            boolean encontrado = false;
            for (int j = 0; j < paises.length; j++) {
                if (finais.equals(paises[j][0])) {
                    tab[j] += 1;
                    encontrado = true;
                    for(int k = 0; k < j; k++){
                        if(paises[k][1].equals(paises[j][1])){
                            tab[j] -=1;
                            tab[k] +=1;
                        }
                    }
                    break;


                }


            }
            if (!encontrado) {
                outros += 1;
            }

        }
        for (int i = 0; i < paises.length; i++) {
            if (tab[i] != 0) {
                System.out.println(paises[i][1] + ":" + tab[i]);
            }
        }
        if (outros != 0) {
            System.out.printf("Outros:%d", outros);
        }
    }
}

