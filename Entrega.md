# Programação Paralela Scheduling com OpenMP

Nome: Emílio B. Pedrollo

### Exemplo de saída:
```
Each thread should add its char to the array n times (n=20)

Case 1.a: static without chunk
BACACBABCBCABCABCACABCABCABACBACBACABACBACBAACBACBACBACBCBCB
A=20 B=20 C=20 

Case 1.b: static with chunk of 2
BCABACBACBACBCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA
A=20 B=20 C=20 

Case 1.c: static with chunk of 4
BCABCABCABCABCABCABCABCABCABCABCABCACBACBACBACBACBACBACBACBA
A=20 B=20 C=20 

Case 2.a: dynamic without chunk
BCABCABCACBACBABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA
A=20 B=20 C=20 

Case 2.b: dynamic with chunk of 2
CBACBABCABCABCACBACBACBACBABCABCABCABCABCABCABCACBACBABCABCA
A=20 B=20 C=20 

Case 2.c: dynamic with chunk of 4
CBABCABCABCABCABCABCABCABCABCABCACBACBACBACBACBACBACBABCABCA
A=20 B=20 C=20 

Case 2.d: dynamic with chunk of 8
CBABACCABACBACBABCABCBACBACBACBACBACBACBABCABCABCBAABBAABBBB
A=20 B=24 C=16 

Case 3.a: guided without chunk
BCABCABACBACBACBACBACBACBCBCBCBCBCBCBCBCBCCBCBCBCBCABABCABAC
A=12 B=24 C=24 

Case 3.b: guided with chunk of 2
CBACABCAACACABCABCABCABACBABCABCABCABCBACBACBACBACBACBCABCAB
A=21 B=19 C=20 

Case 3.c: guided with chunk of 4
BCACBACBACBACBACBACBACBACBACBACBACBACBACBACBABCABCABCABCABCC
A=19 B=20 C=21 

Case 4: auto
CBABACBCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCACBACBA
A=20 B=20 C=20 
```