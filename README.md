
# Não olhe para trás

Seja bem-vindo ao repositório de Não Olhe para Trás, um jogo de terror e suspense desenvolvido pelos alunos do Instituto Federal, Campus Araraquara. Aqui, você encontrará o código-fonte do projeto, documentações e detalhes relacionados à construção do jogo.

![](https://i.imgur.com/8VhZT0o_d.webp?maxwidth=760&fidelity=grand)


## Introdução

Em Não Olhe para Trás, apresentamos a temática de um hotel abandonado em um jogo com um aspecto antigo, desenvolvido para provocar medo e ansiedade nos jogadores por meio de diversos elementos sonoros. O jogador explora o mapa em busca da chave que abrirá a porta final.

Esse projeto foi proposto pelo professor Ednilson Rossi, responsável pela disciplina Estrutura de Dados (ESDD). Como requisitos para este trabalho, foram estabelecidos dois pontos: o uso exclusivo da linguagem C na construção do jogo e a implementação de uma das estruturas de dados abordadas em aula na lógica principal do jogo.

## Construção
Em busca de cumprir os requisitos do trabalho, utilizamos a lógica de FIFO (First In, First Out) na representação de um monstro. Como isso funciona?

Os movimentos realizados pelo jogador no mapa preenchem a fila de posições, e, quando esta se encontra completa, iniciamos o monstro. Trata-se de um algoritmo que, ao ser iniciado, realiza uma diminuição contínua dessa fila.

## Como jogar
Clone ou baixe nosso repositõrio e siga as intruções abaixo de acordo com seu sistema operacional



> <span style="background-color: #eeca06; padding: 10px; display: block; border-left: 5px solid #b79a00; color: black">
> OBS: Todos os comandos abaixo devem ser executados em um terminal devidamente localizado na pasta junto com os arquivos do jogo. 
> </span>







#### Linux
Execute o comando a seguir para instalar as dependências do projeto:

 ```bash
  make linux-libs
  ```
Para compilar o projeto, use o comando a seguir para gerar um executável:

```bash
  make linux
  ```
#### Windows
Instale as bibliotecas obrigatórias a seguir:

- [SDL2](https://www.libsdl.org/)
- [SDL2_mixer](https://wiki.libsdl.org/SDL2_mixer/FrontPage)
- [SDL2_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage)

Com as devidas bibliotecas instaladas, execute o comando a seguir para gerar um executável:

```bash
  make windows
  ```

## Screenshots
![](https://i.imgur.com/z6Ui7bN.png)

![](https://i.imgur.com/mIWHUc3.png)

![](https://i.imgur.com/qD4BTca.png)

![](https://i.imgur.com/NziqAJr.png)

## Metodologia de desenvolvimento

Na construção deste projeto, utilizamos um método organizacional chamado Kanban para nos organizar. Com esse método, conseguimos estruturar e distribuir todas as tarefas relacionadas à construção do jogo.

Em nosso caso, optamos pelo aplicativo Trello, que nos oferece um Kanban para organizar cards utilizados para descrever as tarefas do jogo.

![](https://i.imgur.com/8CI7wJK_d.webp?maxwidth=760&fidelity=grand)

## Videos explicativos

<video width="1080" height="720" controls>
  <source src="https://i.imgur.com/6GHJ0Q3.mp4" type="video/mp4">
</video>

- [Lógica do projeto](https://www.youtube.com/watch?v=rhTQ_YKvdUI&ab_channel=AugustoSerrano)
- [GamePlay](https://www.youtube.com/watch?v=kMrowFIeegs&ab_channel=AugustoSerrano)

## Autores
- [Augusto Grandini Serrano](@gutsserrano) - [gutsserrano@gmail.com](mailto:gutsserrano@gmail.com)

- [Caio dos Santos Lopes](@caioslopes) - [caiolopes.social@gmail.com](mailto:caiolopes.social@gmail.com) 

- [Eduardo Pires Carvalho](@EduardoPC05) - [epc4129@gmail.com](mailto:epc4129@gmail.com)
  
