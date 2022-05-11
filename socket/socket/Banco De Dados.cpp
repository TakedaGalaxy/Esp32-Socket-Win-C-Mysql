#include "Banco De Dados.hpp"

void enviaDadosDB(std::vector<std::string> dados) {
    
    // --- Obitendo o id, tipo e valor de dentro dos dados recebidos ---

    std::string id;
    std::string tipo;
    std::string valor;

    //Usado para modificar as variaveis id, tipo e valor
    std::string* p = NULL;

    //Percorrendo o vector dados
    for (std::string dado : dados) {

        std::string identificador = "";

        //Retirando o identificador de dentro da string exemplo VALOR:900, identificador = VALOR
        for (int i = 0; i < dado.size(); i++) {

            if (dado[i] == ':') break;
            else identificador.push_back(dado[i]);
        
        }


        p = identificador == "ID"    ? &id    : 
            identificador == "TIPO"  ? &tipo  : 
            identificador == "VALOR" ? &valor : 
            NULL;

        if (p == NULL) {

            std::cout << "Erro ao obter id, tipo, valor" << std::endl;

            return;

        }

        //Recebendo o valor apos : na variavel apontada por p
        for (int i = dado.find(':') + 1; i < dado.size(); i++) p->push_back(dado[i]);

    }

    // --- Comunicação com o banco da dados ---
    
    //Criando ponteiro para conexão
    MYSQL* conn;

    //Inicializando mysql
    conn = mysql_init(0);

    //Conectando ao banco de dados com (ponteiro para conexão,ip, usuario, senha, nome do banco de dados, porta)
    conn = mysql_real_connect(conn, "localhost", "gravadorSK", "01234567890123456789", "teste_socket", 3306, NULL, 0);

    //Verificando se a conexão foi realizada
    if (conn) {

        std::cout << "Conectado com sucesso no banco de dados" << std::endl;

        //Armazena codigo SQL
        std::string query = "INSERT INTO dados_recebidos(ID, VALOR, TIPO) VALUES('" + id + "', '" + valor + "', '" + tipo + "')";

        //Convertendo string para uma cadeia de char
        const char* q = query.c_str();

        //Executando o codigo SQL
        int qstate = mysql_query(conn, q);

        //Verifica se ouve sucesso na execução do codigo SQL
        if (!qstate) {

            std::cout << "Sucesso na execucao do comando" << std::endl;

        }
        else {

            std::cout << "Falha na execucao do comando" << std::endl;

        }

    }
    else {

        std::cout << "Falha ao conectar ao banco de dados" << std::endl;

    }

    //Fechando a conexão com o banco de dados
    mysql_close(conn);
    
}