# Programação Paralela Scheduling com OpenMP

Nome: Emílio B. Pedrollo

### Como obter

Os binários do software (tanto para linux quanto para windows) estão disponíveis na aba [releases]

### Exemplo de saída
```
Each thread should add its char to the array n times (n=20)

Case 1.a: static without chunk
AAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
A=20 B=20 C=20

Case 1.b: static with chunk of 3
BBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAA
A=21 B=21 C=18

Case 1.c: static with chunk of 4
BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCA
A=20 B=20 C=20

Case 2.a: dynamic without chunk
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCAB
A=1 B=1 C=58

Case 2.b: dynamic with chunk of 2
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCAABB
A=2 B=2 C=56

Case 2.c: dynamic with chunk of 4
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCAAAABBBB
A=4 B=4 C=52

Case 2.d: dynamic with chunk of 8
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAACCCCCCCC
A=8 B=44 C=8

Case 3.a: guided without chunk
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAA
A=9 B=37 C=14

Case 3.b: guided with chunk of 2
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAA
A=9 B=14 C=37

Case 3.c: guided with chunk of 4
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAA
A=9 B=37 C=14

Case 4: auto
BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20
```
### Análise do resultado
#### Caso 1: static

```
Case 1.a: static without chunk
AAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
A=20 B=20 C=20

Case 1.b: static with chunk of 3
BBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAA
A=21 B=21 C=18

Case 1.c: static with chunk of 4
BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCA
A=20 B=20 C=20
```

O escalonamento estático automaticamente divide o numero de iterações no tamanho do chunk e distribui entre as threads 
em ordem circular.

Se o numero de chunks não for informado cada thread acaba ganhando um conjunto sequancial do loop, ou em outras 
palavras, o chunk passa a ser `(i_total/n_threads)`, cada thread recega um grupo definido por: 
`(i_total/n_threads)*thread_num` até `(i_total/n_threads)*(thread_num+1)-1`.

No resultado acima vemos que por 60 ser divisível por 3 no caso (a) aonde o chunk não é definido ele acaba sendo 
`60 / 3 = 20`, já no caso (b) aonde o chunk é 3 temos uma situação em que não é possivel dividir 60 usando grupos de 3
pois cada thread no final tem que acabar recebendo um numero múltiplo de 3 (com exceção do último grupo que fica com 
o 'resto') então a divisão acaba ficando `A=21 B=21 C=18`. Já no caso (c) com o chunk 4 já é possível novamente termos
uma divisão consistente pois 20 é um múltiplo de 4.

#### Caso 2: dynamic

```
Case 2.a: dynamic without chunk
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAC
A=1 B=58 C=1

Case 2.b: dynamic with chunk of 2
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAACC
A=2 B=56 C=2

Case 2.c: dynamic with chunk of 4
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAABBBBCCCC
A=8 B=48 C=4

Case 2.d: dynamic with chunk of 8
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAACCCCCCCC
A=8 B=44 C=8
```

Cada thread neste modo recebe um grupo do iterações no tamanho do chunk para executar e a medida que forem acabando a
execução mais grupos do tamanho do chunk é atribuido para a thread até que todas as iterações sejam atribuidas as 
threads disponíveis.

Por padrão o chunk deste modo é 1.

No resultado vemos que o SO acaba priorizando a execução de um thread, provavelmente por que não há no código do 
programa alguma chamada para I/O ou alguma operação que fosse gatilho para troca de contexto (devido ao método 
`spendSomeTime` sendo chamado dentro de uma parte critica do programa ele se comporta praticamente como um software 
sequencial). Porém ainda assim podemos ver que em todos os casos a segunda thread (B) for a priorizada, mas devido a 
atribuição inicial as demais thread receberam também um numero chunk de iterações para processar, mas provavelmente 
nunca tiveram chance de correr em paralelo e requisitar um novo grupo de iterações.

#### Caso 3: guided

```
Case 3.a: guided without chunk
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAA
A=9 B=37 C=14

Case 3.b: guided with chunk of 2
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAA
A=9 B=14 C=37

Case 3.c: guided with chunk of 4
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAA
A=9 B=37 C=14
```

O modo guiado é similar ao dinamico. A diferença está no tamanho dos grupo. O tamanho é proporcional ao numero de 
iterações não atribuídos dividido pelo numero total de threads, portanto o tamanho dos grupos decrementa com o passar 
do processamento. 
  
#### Caso 4: auto

```
Case 4: auto
BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20
```

O modo automático apenas delega a decisão do escalonamento para o compilador e/ou systema de runtime.

#### Nota: runtime

Por não fazer muita diferença prática se o escalonamento com numero conhecido de iterações é feito pelo compilador ou em
runtime, e pela simplicidade de compilar apenas um software, neste projeto foi usado apenas runtime para os testes para
poupar linhas de código, porém como dito acima, em um cenário aonde o número total de iterações é conhecido pelo 
compilador não há motivo para utilizar do modo runtime a não ser para testes de performance de algum método especifico 
de escalonamento.

### Referências
 * http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
 * https://www.openmp.org/wp-content/uploads/SC17-Kale-LoopSchedforOMP_BoothTalk.pdf
 * https://www.youtube.com/watch?v=nE-xN4Bf8XI&list=PLLX-Q6B8xqZ8n8bwjGdzBJ25X2utwnoEG
 
 [releases]: https://github.com/elc139/t3-emiliopedrollo/releases/latest