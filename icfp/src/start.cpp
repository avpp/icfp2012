#include "MineMap.h"

#include <stdio.h>

/*
Для загрузки карты из файла необходимо в main открыть его с помощью MineMap.openFileForMap(char* FileName);
Закроется файл автоматически по завершению ввода.
иначе ввод производится через stdin.
*/
int main()
{

    MineMap mm;
    //mm.openFileForMap("input4.txt");
    mm.ReadMap();
    mm.PrintMap();


    return 0;
}
