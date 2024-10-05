#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    Position inicio{.row=-1,.col=-1};
 
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo '" << file_name << "'." << std::endl;
        return inicio;  
    }

    file >> num_rows >> num_cols;
    maze.resize(num_rows, std::vector<char>(num_cols)); 
    std::cerr << "O Labirinto tem " << num_rows << " linhas e " << num_cols << " colunas" << std::endl;
    
    std::string line;
    std::getline(file, line);
    for(int linha = 0; linha < num_rows; linha++) {
        for(int coluna = 0; coluna < num_cols; coluna++) {
            char x;
            file.get(x);
            maze[linha][coluna] = x;
            if(x == 'e') {
                inicio.col = coluna;
                inicio.row = linha;
            }
        }
        file.ignore();
    }

    return inicio;
}

// Função para imprimir o labirinto
void print_maze() {
    for (const auto& row : maze) { 
        for (char cell : row) {     
            std::cerr << cell;      
        }
        std::cerr << '\n';           
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // A posição é válida se estiver dentro dos limites e for uma passagem ou a saída
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols) && 
           (maze[row][col] == 'x' || maze[row][col] == 's'); 
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // Verifique se a posição atual é a saída
    if (maze[pos.row][pos.col] == 's') {
        return true; // Saída encontrada
    }

    // Marque a posição atual como visitada
    maze[pos.row][pos.col] = '.';

    // Exiba o labirinto atual
    print_maze();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    std::vector<Position> directions = {
        {pos.row - 1, pos.col}, // Cima
        {pos.row + 1, pos.col}, // Baixo
        {pos.row, pos.col - 1}, // Esquerda
        {pos.row, pos.col + 1}  // Direita
    };

    // Tente explorar as posições adjacentes
    for (const auto& direction : directions) {
        if (is_valid_position(direction.row, direction.col)) {
            if (walk(direction)) {
                return true; // Se a saída for encontrada em uma das posições, propague o retorno
            }
        }
    }

    // Se todas as posições foram exploradas sem encontrar a saída, retorne false
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    print_maze();

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}
