#include "SearchTree.h"

SearchTree::SearchTree()
{
}

int SearchTree::search(float values[Columns])
{
  int x, y;
  for(int y = 0; y < Rows; y++)
  {
    for(x = 0; x < Columns; x++)
    {
      //If comparison failed, exit row
      if(values[x] > DATA[y][x] + Error || values[x] < DATA[y][x] - Error)
        break;
    }
    if(x == Columns) return 1; //If all comparisons succeed
  }
  return 0; //If nothing succeeded
}
