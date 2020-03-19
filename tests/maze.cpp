#include "constants.hpp"

using namespace std;
int main() {

    int * original[size.at(MEDIUM)][size.at(MEDIUM)];
    int quarter_size = (size.at(MEDIUM) - 1) / 2;
    int maze[quarter_size][quarter_size];
    int visited[quarter_size][quarter_size];
    /*
    int division_dimensions_x[quarter_size];
    int division_dimensions_y[quarter_size];

    for(int i = 0; i < quarter_size; i++) {
        division_dimensions_x[i] = i;
        division_dimensions_y[i] = i;
    }
    */

    srand(static_cast<int>(time(0)));

    for(int i = 0; i < quarter_size; i++) {
        for(int j = 0; j < quarter_size; j++) {
            if ((i + 1) % 2 == 0 || (j + 1) % 2 == 0) {
                maze[i][j] = 8;
            }
            else {
                maze[i][j] = 0;
            }
            cout << maze[i][j] << " ";
        }
        cout << "\n";
    }

/*
    while () {

    }
*/
    return 1;
}
