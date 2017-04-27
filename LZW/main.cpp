#include "hash.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <strstream>

using namespace std;

void compress(ifstream& in, ofstream& out, int sizeOfHash)
{
    Hash hashTable(sizeOfHash);
    hashTable.init();                                           //инициализировали хэш

    string tempStr;
    int newCode=0;                                   //код новой строки

    while (!in.eof())                                           //пока не конец in файла
    {
        int c=in.get();                                         //читаем символ
        char symbol=(char)c;

        int code=hashTable.findCode(tempStr+symbol);            //пытаемся получить код новой строки из таблицы

        if (code==-1)                                           //если символа там нет
        {
            unsigned short outCode=(unsigned short)hashTable.findCode(tempStr);
            out.write((char*)&outCode,2);                       //ДОБАВИТЬ ХОЛОДНЫЙ СТАРТ!!!

            if (newCode<=sizeOfHash)
            {
                hashTable.insert(tempStr+symbol,newCode);
                newCode++;
            }
            tempStr=""+symbol;                                  //обновляем временную строку
        }
        else                                                    //если символ там есть
        {
            tempStr+=symbol;                                    //добавляем его к временной строке
        }
    }
    unsigned short outCode=(unsigned short)hashTable.findCode(tempStr);
    out.write((char*)&outCode,2);                               //ДОБАВИТЬ ХОЛОДНЫЙ СТАРТ!!!
}

int main(int argc, char *argv[])
{
    if (argc<4)
    {
        cerr<<"Incorrect count of parameters"<<endl;
        return 1;
    }

    strstream s;                                //получаем размер таблицы
    s<<argv[2];
    int size;
    s>>size;

    if (size<8 || size>16)                      //проверяем размер
    {
        cerr<<"Incorrect size of table"<<endl;
        return 1;
    }

    ifstream in;                                //поток на вход
    in.open(argv[1],ios::in | ios::binary);

    if (!in.is_open())
    {
        cerr<<"In file not open"<<endl;
        return 1;
    }

    ofstream out;                               //поток на выход
    out.open(argv[3],ios::out | ios::binary);

    if (!out.is_open())
    {
        cerr<<"Out file not open"<<endl;
        return 1;
    }

    compress(in,out,size);                      //осуществляем сжатие

    in.close();
    out.close();
    return 0;
}
