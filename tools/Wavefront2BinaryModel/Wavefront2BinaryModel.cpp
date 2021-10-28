#include <windows.h>
#include <DirectXMath.h>

#include <string>
#include <vector>

using namespace std;
using namespace DirectX;

typedef struct {
    unsigned int magic;
    unsigned int verts_count;
} mdl_header;

int main(int argc, char **argv)
{
    vector<UINT> vertexIndices, uvIndices, normalIndices;
    vector<XMFLOAT3> temp_vertices;
    vector<XMFLOAT2> temp_uvs;
    vector<XMFLOAT3> temp_normals;

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Failed to open wavefront model file!\n");
        return -1;
    }

    while(1) {
        char lineHeader[128];

        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
    
        if (strcmp(lineHeader, "v") == 0) {
            XMFLOAT3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            XMFLOAT2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            XMFLOAT3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]
                );
            
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    fclose(file);

    // Write the binary model file

    FILE* mdl_file = fopen(argv[2], "wb");

    mdl_header mdlHdr;
    mdlHdr.magic        = 0x004C444D; // 'M' 'D' 'L' '00'
    mdlHdr.verts_count  = vertexIndices.size();

    fwrite(&mdlHdr, sizeof(mdlHdr), 1, mdl_file);

    for (unsigned int i = 0; i < mdlHdr.verts_count; i++) {
        fwrite(&temp_vertices[vertexIndices[i] - 1], sizeof(XMFLOAT3), 1, mdl_file);
        fwrite(&temp_uvs[uvIndices[i] - 1], sizeof(XMFLOAT2), 1, mdl_file);
        fwrite(&temp_normals[normalIndices[i] - 1], sizeof(XMFLOAT3), 1, mdl_file);
    }

    fclose(mdl_file);

    return 0;
}
