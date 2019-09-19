//Codice C per Fibonacci, chiede il numero di interazioni all utente, quindi stampa il risultato e lo ritorna.

#include <stdio.h>

int main () {
	int a=1;
	int b=1;
	int step;
	scanf("%d", &step);
	for (int i=0;i<step-2;++i) {
		b+=a;
		a=b-a;
	}
	printf("%d\n",b);
	return b;
}

/* Decompilazione con objdump del main, compilatore gcc v8.2.1, nessuna flag di ottimizzazione.

0000000000001159 <main>:
    1159:	55                   	push   %rbp				# Salva la vecchia base dello stack
    115a:	48 89 e5             	mov    %rsp,%rbp			# Punta la base dello stack del main alla fine di quello precedente
    115d:	48 83 ec 20          	sub    $0x20,%rsp			# Alloca 32 byte nello stack (8 int32)
    1161:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax			# Scrive la canary word in %rax
    1168:	00 00 
    116a:	48 89 45 f8          	mov    %rax,-0x8(%rbp)			# Quindi %rax alla base dallo stack meno 8
    116e:	31 c0                	xor    %eax,%eax			# Azzera %eax
    1170:	c7 45 ec 01 00 00 00 	movl   $0x1,-0x14(%rbp)			# Scrive 1 alla base dello stack meno 20, esegue l'assegnazione int a=1;
    1177:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)			# Scrive 1 alla base dello stack meno 16, esegue l'assegnazione int b=1;
    117e:	48 8d 45 e8          	lea    -0x18(%rbp),%rax			# Scrive l'indirizzo della base dello stack meno 34 in %rax
    1182:	48 89 c6             	mov    %rax,%rsi			# Quindi lo scrive in %rsi (source index)
    1185:	48 8d 3d 78 0e 00 00 	lea    0xe78(%rip),%rdi			# Scrive nel destination index l'indirizzo effettivo di 0xe78 (con offset per il 64 bit contenuto in %rip)
    118c:	b8 00 00 00 00       	mov    $0x0,%eax			# Azzera %eax
    1191:	e8 ba fe ff ff       	callq  1050 <__isoc99_scanf@plt>	# Chiama lo scanf
    1196:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)			# Muove long (32 bit) 0 alla base dello stack meno 12, int i=0
    119d:	eb 13                	jmp    11b2 <main+0x59>			# Salta a 11b2
    119f:	8b 45 ec             	mov    -0x14(%rbp),%eax			# Arrivo di interazione ciclo for, mette in %eax a
    11a2:	01 45 f0             	add    %eax,-0x10(%rbp)			# Somma a %eax il valore di b
    11a5:	8b 45 f0             	mov    -0x10(%rbp),%eax			# Lo assegna a b (b+=a)
    11a8:	2b 45 ec             	sub    -0x14(%rbp),%eax			# Sottrae a
    11ab:	89 45 ec             	mov    %eax,-0x14(%rbp)			# Lo assegna ad a
    11ae:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)			# Incrementa i di 1 (++i)
    11b2:	8b 45 e8             	mov    -0x18(%rbp),%eax			# Arriva con jump, recupera in valore di scanf (l'indirizzo era indicato nel source index)
    11b5:	83 e8 02             	sub    $0x2,%eax			# Sottrae 2
    11b8:	39 45 f4             	cmp    %eax,-0xc(%rbp)			# Lo confronta con i
    11bb:	7c e2                	jl     119f <main+0x46>			# Se inferiore salta a 119f
    11bd:	8b 45 f0             	mov    -0x10(%rbp),%eax			# Finito il for, mette b in %eax
    11c0:	89 c6                	mov    %eax,%esi			# E quindi lo sposta nel source index
    11c2:	48 8d 3d 3e 0e 00 00 	lea    0xe3e(%rip),%rdi         	# Scrive il destination index
    11c9:	b8 00 00 00 00       	mov    $0x0,%eax			# Azzera %eax
    11ce:	e8 6d fe ff ff       	callq  1040 <printf@plt>		# Chiama il printf, che stampera il source index
    11d3:	8b 45 f0             	mov    -0x10(%rbp),%eax			# Sposta b in %eax
    11d6:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx			# Sposta la canary word in %rdx
    11da:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx			# Controlla la canary word
    11e1:	00 00 
    11e3:	74 05                	je     11ea <main+0x91>			# Se corrispondono salta all indirizzo effettivo di main+0x91
    11e5:	e8 46 fe ff ff       	callq  1030 <__stack_chk_fail@plt> 	# In caso contrario chiama la funzione per possibile attacco di stack overflow
    11ea:	c9                   	leaveq					# Esce
    11eb:	c3                   	retq   					# Ritornando %eax, ovvero b
    11ec:	0f 1f 40 00          	nopl   0x0(%rax)			# Operazione nulla di 4 byte */