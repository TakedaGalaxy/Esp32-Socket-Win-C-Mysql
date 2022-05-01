#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main() {

    // --- Inicizaliznado winsock ---

    //Contem informações para a implementação do winsock
    WSADATA wsaData;    

    //Aramazena resultado do processo
    int iResult;

    //Inicializando Winsok
    //Primeiro parametro : Solicitação da versão do Winsock
    //Segundo prarametro : Iformações da implementação do winsock 
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    
    //Checando o resultado
    if (iResult != 0) {

        printf("WSAStartup falhou codigo : %d ", iResult);

        return 1;

    }

    //Criando estrutura para informações de endereço
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));

    //Especificando a familia de endereços IPv4
    hints.ai_family = AF_INET;

    //Especificando um soquete de fluxo
    hints.ai_socktype = SOCK_STREAM;

    //Especificando o protocolo TCP para comunicação
    hints.ai_protocol = IPPROTO_TCP;

    //---
    hints.ai_flags = AI_PASSIVE;

    //Provendo o ip e a porta para ser usada pelo servidor
    iResult = getaddrinfo("127.0.0.2", DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {

        printf("getaddrinfo falhou : %d\n", iResult);
        
        WSACleanup();
        
        return 1;
    
    }

    //Criando objeto para ouvir uma conexão de um cliente
    SOCKET ListenSocket = INVALID_SOCKET;

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET){

        printf("Erro ao inicializar socket : %d", WSAGetLastError());
        
        freeaddrinfo(result);
        
        WSACleanup();

        return 1;

    }

    //Configurando socket TCP para ouvir no ip e porta definidos
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    
    if (iResult == SOCKET_ERROR) {
        
        printf("Bind falhou : %d\n", WSAGetLastError());
        
        freeaddrinfo(result);
        
        closesocket(ListenSocket);
        
        WSACleanup();
        
        return 1;
    
    }

    //Desalocando a memoria não mais necessaria para a execução
    freeaddrinfo(result);

    //Escutando o socket

    //SOMAXCONN quantidade de conxões maximas
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        
        printf("Erro ao escutar socket : %ld\n", WSAGetLastError());
        
        closesocket(ListenSocket);
        WSACleanup();
        
        return 1;
    
    }
    
    //Para aceitar uma conexão, exemplo simples para lidar com uma conexão

    SOCKET ClientSocket = INVALID_SOCKET;

    //Aceitando conexão socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    
    if (ClientSocket == INVALID_SOCKET) {
        
        printf("Conexão nao aceita: %d\n", WSAGetLastError());
        
        closesocket(ListenSocket);
        WSACleanup();
        
        return 1;
    }
    
    //Recebendo e enviando dados

    iResult = 0;

    //Buffer contendo os dados recebidos
    char recvbuf[DEFAULT_BUFLEN];

    //Armazena o resultado do comando de enviar dados
    int iSendResult;

    //Armazena o tamanho do dados maximo para receber
    int recvbuflen = DEFAULT_BUFLEN;

    //Loop que recebe os dados até que o cliente feche a comunicação
    do {

        //Espera dados serem enviados pelo cliente
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        //Verificando se foi recebido com sucesso
        if (iResult > 0) {

            printf("Foram recebidos %d Bytes\n", iResult);
            
            printf("Dados :\n");

            for (int i = 0; i < iResult; i++)std::cout << recvbuf[i];
            
            std::cout << '\n';

            //Renviando os dados recebidos para o cliente
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            
            if (iSendResult == SOCKET_ERROR) {

                printf("Erro ao enviar dados para cliente : %d\n", WSAGetLastError());
                
                closesocket(ClientSocket);
                WSACleanup();
                
                return 1;
            
            }
            printf("Foram enviados %d Bytes\n", iSendResult);
        }
        else if (iResult == 0) //Verificando se a conexão foi fechada pelo cliente

            printf("Conexao fechada !\n");
        
        else {

            printf("Recebimentos de dados falhou : %d\n", WSAGetLastError());

            closesocket(ClientSocket);
            WSACleanup();

            return 1;
        
        }

    } while (iResult > 0);

    //return 0;
}