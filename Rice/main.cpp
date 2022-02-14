#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

#include <ctime>

#include "rice.h"


int fileSize(const char* file)
{
    std::ifstream f(file, std::ios::binary);
    std::vector<uint8_t>* buffer = new std::vector<uint8_t>(std::istreambuf_iterator<char>(f), {});

    uint8_t* _temp = (uint8_t*)buffer->data();

    return buffer->size();
}

bool equalFile(const char* f1, const char* f2) {
    std::ifstream fin1(f1, std::ios::binary);
    std::ifstream fin2(f2, std::ios::binary);
    if (fin2 && fin2)
    {
        char ch1, ch2;
        bool result = true;
        while (fin1.get(ch1) && fin2.get(ch2))
        {
            if (ch1 != ch2)
            {
                result = false;
                break;
            }
        }
        if (result)
            return true;
        else
            return false;
    }
    else
        return false;
}



unsigned int PRNG()
{
    // ���� ��������� ����� - 4 541
    static unsigned int seed = 4541;

    // ����� ��������� ����� �, � ��� �������, ���������� ����� ��������.
    // ��-�� ������������� ����� ������� ����� (� ������������) ������� ��������� ����� ������ �� ����������� - ����� ������
    seed = (8253729 * seed + 2396403);

    // ����� ��������� ����� � ���������� �������� � ��������� �� 0 �� 32767
    return seed % 32768;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    for (int j = 0; j < 100; j++)
    {
        std::ofstream in("test.bin", std::ios::binary);
        int test1 = 0, test2 = 0, test3 = 0;
        // ������� 100 ��������� �����
        int c = 1 + rand() % 10000;
        for (int count = 0; count < c; ++count)
            in << PRNG() << " ";
        in.close();
        rice* temp = new rice("test.bin");
        test1 = fileSize("test.bin");
        test2 = fileSize("compressed.rice");
        test3 = fileSize("decompressed.bin");
        if (!(equalFile("test.bin", "decompressed.bin") && test1 == test3)) {
            std::cout << "error equal file" << std::endl;
        }
        else
            std::cout << "success!" << std::endl;

        if (test1 < test2) {
            std::cout << "error filesize file: " << test1 << " > " << test2 << std::endl;
        }
        else
            std::cout << "success!" << std::endl;
    }

    
    //rice* temp = new rice("data.bin");

    return 0;


}