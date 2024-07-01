# pamModule-emailSender
Modelo PAM utilizado para envio de e-mail diretamente do ArchLinux passando por um BlackJack

Este modelo envia um email após o usuário ganhar uma partida de 21 (blackjack) para permití-lo acessar o sistema do ArchLinux.

------------------------------------------------------------------------
Configuração:

 Ao clonar os arquivos no Linux (Archlinux utilizado), os seguintes comandos devem ser executados para que o programa seja compilado e adicionado à pasta do PAM:

	gcc -fPIC -fno-stack-protector -c pam.c -lcurl
	gcc -fPIC -fno-stack-protector -c 21pam.c -lcurl

	ld -x --shared -o /lib64/security/pam.so pam.o -lcurl
	ld -x --shared -o /lib64/security/21pam.so pam.o -lcurl

------------------------------------------------------------------------
 Para configurar de forma que o PAM reconheça os arquivos, é necessário editar (Utilizarei o editor de textos Nano) os arquivos que se encontram em "/etc/pam.d". O exemplo mais básico é colocá-los no "system-auth", que executará todas as vezes que o PAM for chamado pelo sistema. 
	
	nano /etc/pam.d/system-auth
	
	//insira os próximos comandos abaixo de "#auth required pam_skel.so"
	auth requisite 21pam.so
	auth required  pam.so

 Desta forma, o sistema deverá executar os scripts 21pam.so e pam.so para que o usuário consiga realizar login no sistema.

------------------------------------------------------------------------


