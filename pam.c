#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <curl/curl.h>

int codigoCerto;

#define sender  "otimoprimodopam2@outlook.com"
#define cc      "otimoprimodopam2@yahoo.com"
char reciver[50];

char str[50] = { 0 };

int codigo[8] = { 0 };
int codigoDigitado;
int i = 0;
int sucesso;

int achou;
int tentativas = 0;

char corpoEmail[500];

	struct upload_status {
		size_t bytes_read;
	};

	static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp){
		struct upload_status *upload_ctx = (struct upload_status *)userp;
		const char *data;
		size_t room = size * nmemb;
		
		if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)){
			return 0;
		}

		data = &corpoEmail[upload_ctx->bytes_read];

		if(data){
			size_t len = strlen(data);
			if(room < len){
				len = room;
			}
			memcpy(ptr, data, len);
			upload_ctx->bytes_read += len;

			return len;
		}

		return 0;
	}

int emailSender(){

	CURL *curl;
	CURLcode res = CURLE_OK;
	struct upload_status upload_ctx = { 0 };
	struct curl_slist *recipients = NULL;

	curl = curl_easy_init();

	snprintf(corpoEmail, sizeof(corpoEmail),
		"Date: Wed, 12 Jun 2024 18:00:00 +1100\r\n"
		"To: %s\r\n"
		"From: %s\r\n"
		"Cc: %s\r\n"
		"Subject: Mensagem enviada para a Verifica��o do Usu�rio\r\n"
		"\r\n"
		"PARAB�NS POR GANHAR O JOGO :D\r\n"
		"Aqui est� seu c�digo para logar: %i\r\n", reciver, sender, cc, codigoCerto);

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp-mail.outlook.com:587");
		curl_easy_setopt(curl, CURLOPT_USERNAME, "otimoprimodopam2@outlook.com");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "Otimoprimodopam2");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender);

		recipients = curl_slist_append(recipients, reciver);
		recipients = curl_slist_append(recipients, cc);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res =  curl_easy_perform(curl);

		if(res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		}
		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}
	
return (int)res;

}

int codigoRandomizador(){
	
	srand(time(NULL));
	i=0;
	
	memset(codigo, 0, sizeof(codigo));
	memset(str, 0, sizeof(str));
	
	while(i<8){
		codigo[i] = rand() % 10;
		if(codigo[0] == 0){
			codigo[0] = 9;
		}
		i++;
	}

	i=0;
	char buffer[10] = { 0 };
	while(i<8){
		sprintf(buffer, "%d", codigo[i]);
		strcat(str, buffer);
		i++;
	}
	
	codigoCerto = atoi(str);
	
	//printf("Codigo: %d", codigoCerto);
}

int codigoVerifier(){

	tentativas = 0;

	while(tentativas < 3){

		printf("\nDigite o Codigo que foi enviado para o email '%s': ", reciver);
		scanf("%i", &codigoDigitado);

		if(codigoCerto == codigoDigitado){
			printf("\nCodigo inserido CORRETAMENTE, entrando no sistema\n\n");
			achou = 1;
			break;
		}
		else{
			if(tentativas < 2){
				printf("\nCodigo digitado INCORRETAMENTE, digite novamente: ");
			}
			else{
				printf("\nFalha na autentica��o...");
				achou = 0;
			}
		}

	tentativas++;
	}

return achou;
}

int digiteEmail(){
	printf("\nDigite o email para enviar o codigo: ");
	scanf("%s", &reciver);
return 0;
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv){
	return PAM_SUCCESS;
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

	codigoRandomizador();
	digiteEmail();
	emailSender();
	if(codigoVerifier() == 1){ return PAM_SUCCESS; }


	return PAM_AUTH_ERR;
}
