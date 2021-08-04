#pragma once
#ifndef SQUAREPROPERTIES_H
#define SQUAREPROPERTIES_H
#endif

class SquareProperties {
    private:
        int rowStartGrid;
        int rowEndGrid;
        int columnStartGrid;

    public:
        SquareProperties(int rowStartGrid, int rowEndGrid, int columnStartGrid);

        void updateGrid(int rowStartGrid, int rowEndGrid, int columnStartGrid);

        int getRowStartGrid();
        int getRowEndGrid();
        int getColumnStartGrid();
};
