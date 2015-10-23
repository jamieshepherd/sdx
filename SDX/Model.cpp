#include "Model.h"

namespace SDX
{
    Model::Model()
    {

    }

    Model::~Model()
    {

    }

    bool Model::LoadModel(char* filename)
    {
        std::ifstream fileStream;
        wchar_t checkChar;

        fileStream.open(filename, std::ifstream::in);
        while (fileStream.good()) {
            // READ LINES
            checkChar = fileStream.get();
            // The line is a comment
            if (checkChar == '#') {
                while (checkChar != '\n') {
                    checkChar = fileStream.get();
                }
            }
            // Could be 'v' or 'vt' or 'vn'
            else if (checkChar == 'v') {
                checkChar = fileStream.get();
                // Vertex position
                if (checkChar == ' ') {
                    float vx, vy, vz;
                    fileStream >> vx >> vy >> vz;
                    vertexPosition.push_back(XMFLOAT3(vx, vy, vz));
                }
                // Vertex texture coordinates
                else if (checkChar == 't') {
                    float vtcu, vtcv;
                    fileStream >> vtcu >> vtcv;
                    vertexTextureCoord.push_back(XMFLOAT2(vtcu, vtcv));
                }
                // Vertex normal
                else if (checkChar == 'n') {
                    float vnx, vny, vnz;
                    fileStream >> vnx >> vny >> vnz;
                    vertexNormal.push_back(XMFLOAT3(vnx, vny, vnz));
                }
            }
            // Group
            else if (checkChar == 'g') {

            }
            // Index or face
            else if (checkChar == 'f') {

            }
            // Material library filename
            else if (checkChar == 'm') {

            }
            // Use material
            else if (checkChar == 'u') {

            }
        }
        return true;
    }

    std::vector<VERTEX> Model::GetVertices()
    {
        return{};
    }

    std::vector<WORD> Model::GetIndices()
    {
        return {};
    }

    void Model::Release()
    {

    }
}
