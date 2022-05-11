#pragma once
#include <iostream>
#include <mysql/jdbc.h>
#include "mysql.h"

#include <vector>
#include <string>

// Realiza conexão com banco de dados e inseri os dados passado como parametro
void enviaDadosDB(std::vector<std::string> dados);