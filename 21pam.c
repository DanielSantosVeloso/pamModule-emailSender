#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


	int numeroAleatorio(int min, int max){

		return min + rand() % (max - min +1);
	
	}
	
	int valorMao(int mao[], int numCartas){
	
		int soma = 0;
		int numeroAces = 0;
		
		for(int i = 0; i< numCartas; i++){
			
			soma += mao[i];
			
			if(mao[i]==1){
				
				numeroAces ++;
				
			}
			while(numeroAces > 0 && soma + 10 <= 21){
			
				soma += 10;
				numeroAces--;
			
			}
			
	
		}
	return soma;	
	}

	int jogar(){

		printf("\n\n================ 21 ===============\n\n");

		srand(time(NULL));
		int maoJogador[10];
		int maoBot[10];
		int numCartasjogador = 0;
		int numCartasbot = 0;
		

		maoJogador[numCartasjogador++] = numeroAleatorio(1,10);
		maoBot[numCartasbot++] = numeroAleatorio(1,10);

		char escolha;		

		while(1){
		
			printf("Sua mao: ");
		
			for(int i = 0; i < numCartasjogador; i++){

				if(i==0){ printf("%d", maoJogador[i]); }
				else{ printf(", %d",maoJogador[i]); }

			}	
			printf("\nValor da sua mao: %d\n",valorMao(maoJogador,numCartasjogador));

			if(valorMao(maoJogador, numCartasjogador) > 21) {

				printf("Voc� estourou 21. Voc� perdeu!\n");
				return PAM_AUTH_ERR;
		
			}
			printf("Deseja mais uma carta? (s/n): ");
			scanf("%c", &escolha);
			printf("\n");

			if(escolha == 's'){

				maoJogador[numCartasjogador++] = numeroAleatorio(1,10);		
			}

			else if(escolha == 'n'){
			
				while(valorMao(maoBot, numCartasbot) < 17){
				maoBot[numCartasbot++] = numeroAleatorio(1,10);

			}

			printf("Mao do bot: ");
		
			for(int i = 0; i < numCartasbot; i++){
				printf("%d ", maoBot[i]);
			}
			
			printf("\nValor da mao do bot: %d\n", valorMao(maoBot,numCartasbot));
		
			int pontosJogador = valorMao(maoJogador, numCartasjogador);
			int pontosBot = valorMao(maoBot, numCartasbot);
		
			if(pontosJogador > 21 || (pontosBot <= 21 && pontosBot > pontosJogador)){
				return PAM_AUTH_ERR;
			}

			else if (pontosJogador == pontosBot){
				printf("Empate!\n");
				return PAM_SUCCESS;	
			}

			else{
				printf("Voc� ganhou!\n");
				return PAM_SUCCESS;
			}
				
		}

		}


	}


	PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv){
	
	int rval;
	const char* username;
	struct pam_response *resp;
	struct pam_conv *conv;
	
	rval = pam_get_user(pamh, &username, NULL);
	
	if(rval != PAM_SUCCESS){
		return rval;
	}

	//Tire o comentário para que o Root não tenha que passar pelo PAM (NOS DOIS ARQUIVOS)
	/*if (strcmp("root",username) == 0){
		return PAM_SUCCESS;
	}*/

	return jogar();
		
	}
	PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv){
	return PAM_SUCCESS;
	}
