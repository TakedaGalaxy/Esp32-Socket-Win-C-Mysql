#include "Banco De Dados.hpp"

void exemploDB(void) {

    //Criando ponteiro para conexão
    MYSQL* conn;

    //Armazena as colunas do resultado
    MYSQL_ROW row;

    //Armazena a resposta de uma execução SQL
    MYSQL_RES* res;

    //Inicializando mysql
    conn = mysql_init(0);

    //Conectando ao banco de dados com (ponteiro para conexão,ip, usuario, senha, nome do banco de dados, porta)
    conn = mysql_real_connect(conn, "", "", "", "", 3306, NULL, 0);

    //Verificando se a conexão foi realizada
    if (conn) {

        std::cout << "Conectado com sucesso no banco de dados" << std::endl;

        //Armazena codigo SQL
        std::string query = "SELECT * FROM tabela_de_produtos";

        //Convertendo string para uma cadeia de char
        const char* q = query.c_str();



        //Executando o codigo SQL
        int qstate = mysql_query(conn, q);

        //Verifica se ouve sucesso na execução do codigo SQL
        if (!qstate) {

            //Armazena a resposta do codigo executado
            res = mysql_store_result(conn);

            //Percorre cada linha e devolve um array com as colunas
            while (row = mysql_fetch_row(res)) {

                std::cout << row[0] << row[1] << row[2] << row[3] << row[4] << row[5] << std::endl;

            }

        }
        else {

            std::cout << "Falha na busca" << std::endl;

        }

    }
    else {

        std::cout << "Falha ao conectar ao banco de dados" << std::endl;

    }
    
    //Fechando a conexão com o banco de dados
    mysql_close(conn);

}