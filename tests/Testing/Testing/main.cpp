#include "constants.hpp"

void copyArrayInfo(int* destination, int* source, int size, bool reverse);
void setWallMap(int ** originalWallMap, int ** original, int mapDimension);

int main() {

    int mapDimension = size.at(S) + BORDERS_SIZE_IN_MATRIX;

    int** original = new int* [mapDimension];
    for (int i = 0; i < mapDimension; i++) original[i] = new int[mapDimension];

    const int quarter_size = (size.at(S) - 1) / 2;
    int** top_left = new int* [quarter_size];
    for (int i = 0; i < quarter_size; i++) top_left[i] = new int[quarter_size];

    int** visited = new int* [quarter_size];
    for (int i = 0; i < quarter_size; i++) visited[i] = new int[quarter_size];

    SquareProperties topLeftGrid(1, quarter_size, 1);
    SquareProperties topRightGrid(1, quarter_size, quarter_size + 2);
    SquareProperties bottomRightGrid(quarter_size * 2 + 1, quarter_size + 2, quarter_size + 2);
    SquareProperties bottomLeftGrid(quarter_size * 2 + 1, quarter_size + 2, 1);

    int** originalWallMap = new int* [mapDimension];
    for (int i = 0; i < mapDimension; i++) {
        originalWallMap[i] = new int[mapDimension];
    }

    srand(static_cast<int>(time(0)));

    for(int i = 0; i < quarter_size; i++) {
        for(int j = 0; j < quarter_size; j++) {
            visited[i][j] = 0;
            if ((i + 1) % 2 == 0 || (j + 1) % 2 == 0) {
                top_left[i][j] = 8;
                visited[i][j] = 1;
            }
            else if (i == quarter_size - 1 && j == quarter_size - 1) {
                top_left[i][j] = 7;
            }
            else {
                top_left[i][j] = rand() % 7 + 1;
            }
            std::cout << visited[i][j] << " ";
        }
        std::cout << "\n";
    }

    //Uncomment this if you want to try black holes!
    top_left[1][1] = 0;

    std::stack<std::tuple<int, int>> algorithmCells;
    algorithmCells.push(std::tuple<int, int>{0, 0});
    visited[0][0] = 1;
    std::cout << "0,0" << std::endl;

    while (algorithmCells.size() > 0) {

        int i = std::get<0>(algorithmCells.top());
        int j = std::get<1>(algorithmCells.top());
        std::vector<std::tuple<int, int>> neighbors;

        if (i - 2 >= 0 && visited[i - 2][j] == 0) {
            neighbors.push_back(std::tuple<int, int>{i - 2, j});
        }
        if (i + 2 < quarter_size && visited[i + 2][j] == 0) {
            neighbors.push_back(std::tuple<int, int>{i + 2, j});
        }
        if (j - 2 >= 0 && visited[i][j - 2] == 0) {
            neighbors.push_back(std::tuple<int, int>{i, j - 2});
        }
        if (j + 2 < quarter_size && visited[i][j + 2] == 0) {
            neighbors.push_back(std::tuple<int, int>{i, j + 2});
        }

        if (neighbors.size() == 0) {
            std::cout << "CONTINUE!" << std::endl;
            algorithmCells.pop();
            continue;
        }

        int selectedCell = rand() % neighbors.size();
        int new_i = std::get<0>(neighbors.at(selectedCell));
        int new_j = std::get<1>(neighbors.at(selectedCell));
        visited[new_i][new_j] = 1;
        if (new_i - i == 0) {
            top_left[i][(new_j + j) / 2] = rand() % 7 + 1;
        }
        else if (new_j - j == 0) {
            top_left[(new_i + i) / 2][j] = rand() % 7 + 1;
        }
        algorithmCells.push(neighbors.at(selectedCell));

        std::cout << new_i << "," << new_j << std::endl;

        if (new_i == quarter_size - 1 && new_j == quarter_size - 1) {
            std::cout << "BREAK!" << std::endl;
            break;
        }
    }

    for (int i = 0; i < mapDimension; i++) {
        for (int j = 0; j < mapDimension; j++) {
            if (i >= topLeftGrid.getRowStartGrid() && i <= topLeftGrid.getRowEndGrid() && j == topLeftGrid.getColumnStartGrid()) {
                copyArrayInfo(&original[i][j], &top_left[i - 1][0], quarter_size, false);
            }
            else if (i >= topRightGrid.getRowStartGrid() && i <= topRightGrid.getRowEndGrid() && j == topRightGrid.getColumnStartGrid()) {
                copyArrayInfo(&original[i][j], &top_left[i - 1][0], quarter_size, true);
            }
            else if (i <= bottomRightGrid.getRowStartGrid() && i >= bottomRightGrid.getRowEndGrid() && j == bottomRightGrid.getColumnStartGrid()) {
                copyArrayInfo(&original[i][j], &top_left[(quarter_size - 1) - (i - 2 - quarter_size)][0], quarter_size, true);
            }
            else if (i <= bottomLeftGrid.getRowStartGrid() && i >= bottomLeftGrid.getRowEndGrid() && j == bottomLeftGrid.getColumnStartGrid()) {
                copyArrayInfo(&original[i][j], &top_left[(quarter_size - 1) - (i - 2 - quarter_size)][0], quarter_size, false);
            }
            else if ((i == mapDimension / 2 && j == mapDimension / 2) || (i == mapDimension / 2 && (j == (mapDimension / 2) - 1 || j == (mapDimension / 2) + 1)) || (j == mapDimension / 2 && (i == (mapDimension / 2) - 1 || i == (mapDimension / 2) + 1))) {
                original[i][j] = 7;
            }
            else if (i == 0 || j == 0 || i == mapDimension - 1 || j == mapDimension - 1 || i == mapDimension / 2 || j == mapDimension / 2){
                original[i][j] = 8;
            }
            if (original[i][j] != 8) {
                std::cout << original[i][j] << " ";
            }
            else {
                std::cout << "  ";
            }

        }
        std::cout << "\n";
    }

    setWallMap(originalWallMap, original, mapDimension);

    for (int i = 0; i < mapDimension; i++) {
        for (int j = 0; j < mapDimension; j++) {
            std::cout << std::setfill('0') << std::setw(2) << originalWallMap[i][j] << " ";
        }
        std::cout << "\n";
    }

    Engine engine = Engine(
        FRAMES_PER_SECOND,
        original,
        originalWallMap,
        mapDimension,
        X_ORIGIN,
        Y_ORIGIN
    );
    engine.run();

    return 1;
}

void copyArrayInfo(int destination [], int source [], int size, bool reverse) {
    if (reverse) {
        for (int i = 0; i < size; i++) {
            destination[size - i - 1] = source[i];
        }
    }
    else {
        for (int i = 0; i < size; i++) {
            destination[i] = source[i];
        }
    }
}

void setWallMap(int ** originalWallMap, int ** original, int mapDimension) {
    for (int i = 0; i < mapDimension; i++) {
        for (int j = 0; j < mapDimension; j++) {
                if (original[i][j] == 8) {
                    originalWallMap[i][j] =
                        (i != 0 ? (original[i - 1][j] == 8 ? 1 << (TOP_WALL_SHIFT - 1) : 0) : 0) +
                        (j != mapDimension - 1 ? (original[i][j + 1] == 8 ? 1 << (RIGHT_WALL_SHIFT - 1) : 0) : 0) +
                        (i != mapDimension - 1 && j != mapDimension - 1 ? (original[i + 1][j + 1] == 8 ? 1 << (BOTTOM_RIGHT_WALL_SHIFT - 1) : 0) : 0) +
                        (i != mapDimension - 1 ? (original[i + 1][j] == 8 ? 1 << (BOTTOM_WALL_SHIFT - 1) : 0) : 0) +
                        (i != mapDimension - 1 && j != 0 ? (original[i + 1][j - 1] == 8 ? 1 << (BOTTOM_LEFT_WALL_SHIFT - 1) : 0) : 0) +
                        (j != 0 ? (original[i][j - 1] == 8 ? 1 << (LEFT_WALL_SHIFT - 1) : 0) : 0);
                }
                else {
                    originalWallMap[i][j] = -1;
                }
        }
    }
}
