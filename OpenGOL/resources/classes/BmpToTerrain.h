#pragma once
#include <vector>
#include <istream>
#include <fstream>
using namespace std;
static class BmpToTerrain
{
public: 

	static void MakeTerrain(char const* filename, short* terrain, int terrainsize) {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();
        char tmp;
        char hi, lo;

        if (pos == 0) {
            return;
        }
        std::vector<char> readBytes(pos);
        ifs.seekg(0, std::ios::beg);
        ifs.read(&readBytes[0], pos);

        for (int i = 0; i < (terrainsize/2); i++)
        {

            tmp = (char)readBytes[0x76+i];
      
            hi = (char)tmp>>4;
            lo = (char)tmp<<4;
            lo = (char)lo>>4;
            
            terrain[i * 2] = hi<0 ? 1 : 0;
            terrain[(i * 2) + 1] = lo<0 ? 1 : 0;
        }

        ifs.close();
	}
};

