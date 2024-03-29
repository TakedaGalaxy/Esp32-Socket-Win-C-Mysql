#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#include "Banco De Dados.hpp"
#include "Utilidades.hpp"


int main() {

    // --- Inicizaliznado winsock ---

    //Contem informa��es para a implementa��o do winsock
    WSADATA wsaData;    

    //Aramazena resultado do processo
    int iResult;

    //Inicializando Winsok
    //Primeiro parametro : Solicita��o da vers�o do Winsock
    //Segundo prarametro : Iforma��es da implementa��o do winsock 
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    
    //Checando o resultado
    if (iResult != 0) {

        std::cout << "WSAStartup falhou codigo : " << iResult << std::endl;

        return 1;

    }

    //Criando estrutura para informa��es de endere�o
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));

    //Especificando a familia de endere�os IPv4
    hints.ai_family = AF_INET;

    //Especificando um soquete de fluxo
    hints.ai_socktype = SOCK_STREAM;

    //Especificando o protocolo TCP para comunica��o
    hints.ai_protocol = IPPROTO_TCP;

    //---
    hints.ai_flags = AI_PASSIVE;

    //Provendo o ip e a porta para ser usada pelo servidor
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {

        std::cout << "getaddrinfo falhou : " << iResult << std::endl;
        
        WSACleanup();
        
        return 1;
    
    }

    //Criando objeto para ouvir uma conex�o de um cliente
    SOCKET ListenSocket = INVALID_SOCKET;

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET){

        std::cout << "Erro ao inicializar socket : " << WSAGetLastError() << std::endl;
        
        freeaddrinfo(result);
        
        WSACleanup();

        return 1;

    }

    //Configurando socket TCP para ouvir no ip e porta definidos
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    
    if (iResult == SOCKET_ERROR) {
        
        std::cout << "Bind falhou : " << WSAGetLastError() << std::endl;
        
        freeaddrinfo(result);
        
        closesocket(ListenSocket);
        
        WSACleanup();
        
        return 1;
    
    }

    //Desalocando a memoria n�o mais necessaria para a execu��o
    freeaddrinfo(result);

    //Escutando o socket

    //SOMAXCONN quantidade de conx�es maximas
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        
        std::cout << "Erro ao escutar socket : " << WSAGetLastError() << std::endl;
        
        closesocket(ListenSocket);
        WSACleanup();
        
        return 1;
    
    }
    
    //Para aceitar uma conex�o, exemplo simples para lidar com uma conex�o

    SOCKET ClientSocket = INVALID_SOCKET;

    //Aceitando conex�o socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    
    if (ClientSocket == INVALID_SOCKET) {
        
        std::cout << "Conex�o nao aceita : " << WSAGetLastError() << std::endl;
        
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

    //Loop que recebe os dados at� que o cliente feche a comunica��o
    do {

        //Espera dados serem enviados pelo cliente
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        //Verificando se foi recebido com sucesso
        if (iResult > 0) {

            std::cout << "Foram recebidos " << iResult << " Bytes " << std::endl;

            std::cout << "Dados : Inicio" << std::endl;

            for (int i = 0; i < iResult; i++)std::cout << recvbuf[i];

            std::cout << "\nDados : Fim" << std::endl;

            // --- Enviando para banco de dados ---

            enviaDadosDB(decodificadorDados(recvbuf, iResult));

            //Renviando os dados recebidos para o cliente
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);

            if (iSendResult == SOCKET_ERROR) {

                std::cout << "Erro ao enviar dados para o cliente : " << WSAGetLastError() << std::endl;
                
                closesocket(ClientSocket);
                WSACleanup();

                return 1;

            }

            std::cout << "Foram enviados " << iSendResult << " Bytes para o cliente" << std::endl;

        }
        else if (iResult == 0) //Verificando se a conex�o foi fechada pelo cliente

            std::cout << "Conexao fechada !" << std::endl;
            
        else {

            std::cout << "Recebimentos de dados falahou : " << WSAGetLastError();

            closesocket(ClientSocket);
            WSACleanup();

            return 1;
        
        }

    } while (iResult > 0);

    //return 0;

    

}