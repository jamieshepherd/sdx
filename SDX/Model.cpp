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
        char streamChar;

        fileStream.open(filename);
        while (!fileStream.eof()) {
            // READ LINES
            fileStream.get(streamChar);
            // The line is a comment
            if (streamChar == '#') {
                while (streamChar != '\n') {
                    fileStream.get(streamChar);
                }
            }
            // Could be 'v' or 'vt' or 'vn'
            else if (streamChar == 'v') {
                fileStream.get(streamChar);
                // Vertex position
                if (streamChar == ' ') {
                    float vx, vy, vz;
                    fileStream >> std::skipws >> vx >> vy >> vz;
                    VERTEX newVertex = { XMFLOAT3(vx, vy, vz), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };
                    vertexPositions.push_back(newVertex);
                }
                // Vertex texture coordinates
                else if (streamChar == 't') {
                    float vtcu, vtcv;
                    fileStream >> vtcu >> vtcv;
                    vertexTextureCoord.push_back(XMFLOAT2(vtcu, vtcv));
                }
                // Vertex normal
                else if (streamChar == 'n') {
                    float vnx, vny, vnz;
                    fileStream >> vnx >> vny >> vnz;
                    vertexNormal.push_back(XMFLOAT3(vnx, vny, vnz));
                }
            }
            // Group
            else if (streamChar == 'g') {

            }
            // Index or face
            else if (streamChar == 'f') {
                UINT value = 0;
                UINT face = 0;
                std::wstring VertexDefinition;
                triangleCount = 0;

                fileStream.ignore(); // Get rid of the first space
                //fileStream.get(streamChar); // Take the first 

                // As long as we're on the same line
                while (streamChar != '\n') {

                    // TAKE THE FIRST CHAR, THIS IS AN INDEX
                    fileStream >> std::skipws >> value;
                    fileStream.get(streamChar);

                    while (streamChar != ' ') {
                        fileStream.get(streamChar);
                        if (streamChar == '\n') {
                            break;
                        }
                    }

                    indices.push_back((UINT)value);
                }

                triangleCount++;
            }
            // Material library filename
            else if (streamChar == 'm') {

            }
            // Use material
            else if (streamChar == 'u') {

            }
        }
        return true;
    }

    std::vector<VERTEX> Model::GetVertices()
    {
        return vertexPositions;
    }

    std::vector<UINT> Model::GetIndices()
    {
        return indices;
    }

    void Model::Release()
    {

    }

    float rand_color()
    {
        float randomColor = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        return randomColor;
    }
}
