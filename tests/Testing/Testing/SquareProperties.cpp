#include "SquareProperties.hpp"

// SquareProperties constructor
SquareProperties::SquareProperties(int rowStartGrid, int rowEndGrid, int columnStartGrid)
{
    this->rowStartGrid = rowStartGrid;
    this->rowEndGrid = rowEndGrid;
    this->columnStartGrid = columnStartGrid;
}

// Date member function
void SquareProperties::updateGrid(int rowStartGrid, int rowEndGrid, int columnStartGrid)
{
    this->rowStartGrid = rowStartGrid;
    this->rowEndGrid = rowEndGrid;
    this->columnStartGrid = columnStartGrid;
}

int SquareProperties::getRowStartGrid() {
    return this->rowStartGrid;
}

int SquareProperties::getRowEndGrid() {
    return this->rowEndGrid;
}

int SquareProperties::getColumnStartGrid() {
    return this->columnStartGrid;
}