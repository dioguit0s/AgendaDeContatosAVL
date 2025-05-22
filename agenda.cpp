#include <string>
#include <ostream>
#include <iostream>

using namespace std;

//Criando uma classe contato
class Contato {
    public:
    string telefone;
    string nome;
    string email;
    bool favorito;

    Contato(string telefone, string nome, string email, bool favorito): telefone(telefone), nome(nome), email(email), favorito(favorito) {};

};

//Criando um nó padrão
class No {
    public:
        Contato *contato;
        No *esquerda;
        No *direita;
        int altura;

       
    No(string telefone, string nome, string email, bool favorito) : contato(new Contato(telefone, nome, email, favorito)), esquerda(nullptr), direita(nullptr), altura(1){}

};

//criando a arvore
class AVL {
public:
    No *raiz;

    //Construtor
    AVL(): raiz(nullptr){}

    //Função que busca a altura de um nó
    int getAltura(No *n) {
        if (n == nullptr){
            return 0;
        }
        return getAltura(n->esquerda) - getAltura(n->direita);
    }

    //função para calcular o fator de balenceamento de um nó
    int getBaleanceamento(No *n){
        if(n == nullptr)
            return 0;
        return getAltura(n->esquerda) - getAltura(n->direita);
    }

    //função que executa uma rotação à direita
    No *rotacaoDireita(No *y){
        No *x = y->esquerda;
        No *T2 = x->direita;

        //realiza a rotação
        x->direita = y;
        y->esquerda = T2;

        //Atualiza as alturas
        y->altura = max(getAltura(y->esquerda), getAltura(y->direita)) + 1;
        x->altura = max(getAltura(x->esquerda), getAltura(x->direita)) + 1;

        return x;
    }

    //função que executa uma rotação a esquerda
    No *rotacaoEsquerda(No *x){
        No *y = x->esquerda;
        No *T2 = y->direita;

        //realiza a rotação
        y->esquerda = x;
        x->direita = T2;

        //Atualiza as alturas
        x->altura = max(getAltura(x->esquerda), getAltura(x->direita)) + 1;
        y->altura = max(getAltura(y->esquerda), getAltura(y->direita)) + 1;

        return y;
    }

    //Inserindo um nó
    No *inserir(No *no, string chave, string nome, string email, bool favorito) {
        //1ºPasso: Inserção normal BST
        if (no == nullptr) {
            return new No(chave, nome, email, favorito);
        }

        if (nome < no->contato->nome){
            no->esquerda = inserir(no->esquerda, chave, nome, email, favorito );
        } else if (nome > no->contato->nome) {
            no->direita = inserir(no->direita, chave, nome, email, favorito);
        } else {// chaves iguais nao são permitidas na BST
            cout << "nome ja existente no sistema" << endl;
            return no;
            }
            
        //2ºPasso: atualizando a altura no nó raiz
        no->altura = 1 + max(getAltura(no->esquerda), getAltura(no->direita));

        //3ºPasso: obtendo o fator balanceamento desse no raiz
        //para verificar se ele está desbalanceado
        int balanceamento = getBaleanceamento(no);

        //se o nó se tornou desbalanceado, então temos 4 casos

        //Caso esquerda esquerda
        if (balanceamento > 1 && nome < no->contato->nome) {
            return rotacaoDireita(no);
        } 

        //caso direita direita
        if (balanceamento < -1 && nome > no->contato->nome) {
            return rotacaoEsquerda(no);
        }

        //caso esquerda direita
        if(balanceamento > 1 && nome > no->contato->nome) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        //caso direita esquerda
        if (balanceamento < -1 && nome < no->contato->nome) {
         no->direita = rotacaoDireita(no->direita);
         return rotacaoEsquerda(no);   
        }

        //retorna o ponteiro do nó (inalterado)
        return no;
    }

    //Encontrando o nó com menor valor na arvore (o mais a esquerda)
    No *noComMenorValor(No *no){
        No *atual = no;
        while(atual->esquerda != nullptr){
            atual = atual->esquerda;
        }
        return atual;
    }

    //removendo um nó
    No *remover(No *raiz, string nome){
        //1ºPasso: remoção padrão de BST
        if(raiz == nullptr){
            return raiz;
        }

        if(nome < raiz->contato->nome) {
            raiz->esquerda = remover(raiz->esquerda, nome);
        } else if (nome > raiz->contato->nome){
            raiz->direita = remover(raiz->direita, nome);
        } else{
            //nó com apenas um filho ou sem nenhum filho
            if((raiz->esquerda == nullptr) || (raiz->direita == nullptr)) {
                No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

                //caso sem filhos
                if(temp == nullptr) {
                    temp = raiz;
                    raiz = nullptr;
                } else // caso um filho 
                    *raiz = *temp; //copia os conteudos do filho não vazio

                delete temp;
            } else {
                //nó com dois filhos: obtém o sucessor na ordem
                No *temp = noComMenorValor(raiz->direita);

                //copia o valor sucessor in order para esse no
                raiz->contato->nome = temp->contato->nome;

                //remove o sucessor in order
                raiz->direita = remover(raiz->direita, temp->contato->nome);
            }
        }

        //se a arvore tinha apenas um nó então retorna
        if (raiz == nullptr)
            return raiz;

        //2ºPasso: atualiza a altura do nó atual
        raiz->altura = 1 + max(getAltura(raiz->esquerda), getAltura(raiz->direita));

        //3ºPasso: obtém o fator de balanceamento do nó
        int balanceamento = getBaleanceamento(raiz);

        //se o nó está desbalanceado temos 4 casos

        //caso esquerda esquerda
        if (balanceamento > 1 && getBaleanceamento(raiz->esquerda) >= 0){
            return rotacaoDireita(raiz);
        }

        //caso esquerda direita
        if (balanceamento > 1 && getBaleanceamento(raiz->esquerda) < 0){
            raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
            return rotacaoDireita(raiz);
        }

        //caso direita direita
        if (balanceamento < -1 && getBaleanceamento(raiz->direita) <= 0) {
            return rotacaoEsquerda(raiz);
        }

        //caso direita esquerda
        if (balanceamento < -1 && getBaleanceamento(raiz->direita) > 0) {
            raiz->direita = rotacaoDireita(raiz->direita);
            return rotacaoEsquerda(raiz);
        }

        return raiz;
    }

    //Busca um valor na arvore pelo nome e retorna os valores da agenda
    void buscar(No *raiz, string nome){

        string fav;
        if(raiz == nullptr) return;

        if(raiz->contato->nome == nome){
            
            cout << "Contato de: " << raiz->contato->nome << endl;
            cout << "-Telefone: " << raiz->contato->telefone << endl;
            cout << "-Email: " << raiz->contato->email<< endl;

            if(raiz->contato->favorito == true) fav = "sim";
            else fav = "nao";

            cout << "-Favorito: " << fav << endl;
        }

        if (nome < raiz->contato->nome)
            return buscar(raiz->esquerda, nome);
        else
            {return buscar(raiz->direita,  nome);}
    }

    //Busca um valor na arvore e retorna um valor booleano para caso exista ou náo
    bool buscarTrue(No *raiz, string nome){
        if(raiz == nullptr) return false;

        if(raiz->contato->nome == nome) return true;

        if (nome < raiz->contato->nome)
            return buscarTrue(raiz->esquerda, nome);
        else
            return buscarTrue(raiz->direita,  nome);
    }

    //exibe uma lista dos contatos marcados como favoritos
    void returnFavoritos(No *raiz) {
        if (raiz != nullptr) {
            emOrdem(raiz->esquerda);
            if (raiz->contato->favorito)
            {
                cout << raiz->contato->nome << " " << endl;
            }
            emOrdem(raiz->direita);
        } 
    }

    //Executando leitura Em Ordem (esquerda, raiz, direita)
    void emOrdem(No *raiz) {
        if (raiz != nullptr) {
            emOrdem(raiz->esquerda);
            cout << raiz->contato->nome << " " << endl;
            emOrdem(raiz->direita);
        } 
    }

};

//Desenhando o menu
void menu() {
        cout << "Software agenda de contatos" << endl;
        cout << "Selecione uma opção" << endl;
        cout << "1.Adicionar novo contato" << endl;
        cout << "2.Remover contato existente" << endl;
        cout << "3.Buscar contato" << endl;
        cout << "4.Listar todos os contatos por ordem alfabetica" << endl;
        cout << "5.Listar os contatos favoritos" << endl;
        cout << "0.Sair" << endl;

}

int main() {
    AVL arvore;

    //Insercoes teste
    arvore.raiz = arvore.inserir(arvore.raiz, "11987634567", "teste", "teste@gmail.com", false);
    arvore.raiz = arvore.inserir(arvore.raiz, "11943760839", "leo", "leo@gmail.com", true);
    arvore.raiz = arvore.inserir(arvore.raiz, "11943760838", "diogo", "diogo@gmail.com", false);
    arvore.raiz = arvore.inserir(arvore.raiz, "11943760837", "yohane", "yohane@gmail.com", true);
    arvore.raiz = arvore.inserir(arvore.raiz, "19437608361", "Freya", "freya@gmail.com", false);
    arvore.raiz = arvore.inserir(arvore.raiz, "11923456082", "Brokk", "freya@gmail.com", true);

    //Loop para se manter dentro do sistema no console
    while (true)
    {
        menu();
        int escolha;
        cin >> escolha;
    
        switch (escolha) {
        //Adiciona um contato
        case 1: {
            string nome;
            string telefone;
            string email;
            bool fav;

            cout << "Insira o nome do contato: " << endl;
            cin >> nome;  
            cout << "Insira o telefone do novo contato: " << endl;
            cin >> telefone;
            cout << "Insira o email do novo contato: " << endl;
            cin >> email;
            cout << "Deseja adicionar o novo contato como favorito?(1.Sim 2.Nao)" << endl;
            int choice;
            cin >> choice;
            if(choice == 1) fav = true;
            else if (choice == 2) fav = false;
            else {
                cout<< "Opcao invalida, tente novamente" << endl;
                cout << " " << endl;
                break;
            }

            arvore.raiz = arvore.inserir(arvore.raiz, telefone, nome, email, fav);
            
            cout << "contato adicionado com sucesso!" << endl;
            cout << " " << endl;
            break;
        }
        //Remove um contato pelo nome
        case 2: {
            cout << "insira o nome do contato que deseja excluir: " << endl;
            string nome;
            cin >>nome;

            bool achouNome = arvore.buscarTrue(arvore.raiz, nome);

            if (achouNome)
            {
                arvore.raiz = arvore.remover(arvore.raiz, nome);
                cout << "Contato deletado com sucesso!!" << endl;
                cout << " " << endl;
            } else {
                cout << "Contato nao encontrado" << endl;
                cout << " " << endl;
                break;
            }
            
            break;
        }
        //Retorna informacoes de um contato pelo nome    
        case 3:{
            cout << "insira o nome do contato que deseja buscar: " << endl;
            string nome;
            cin >>nome;

            bool achouNome = arvore.buscarTrue(arvore.raiz, nome);

            if (achouNome)
            {
                arvore.buscar(arvore.raiz, nome);
                cout << " " << endl;
            } else {
                cout << "Contato nao encontrado" << endl;
                cout << " " << endl;
                break;
            }
            break;
        }
        //Exibe uma lista de todos os contatos em ordem alfabetica
        case 4 :{
        cout << "Contatos em ordem alfabetica: " << endl;
            arvore.emOrdem(arvore.raiz);
            cout << " " << endl;
            break;
        }
        //Exibe uma lista de todos os contatos favoritos em ordem alfabetica
        case 5: {
            cout << "Lista de contatos favoritos: " << endl;
            arvore.returnFavoritos(arvore.raiz);
            cout << " " << endl;
            break;}
        
        case 0:
            return false;

        default:
            break;
        }    
    }
    

    return 0;
}