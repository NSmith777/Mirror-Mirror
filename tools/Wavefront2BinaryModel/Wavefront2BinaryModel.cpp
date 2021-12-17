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
    vector<UINT> vertex_indices, uv_indices, normal_indices;
    vector<XMFLOAT3> temp_vertices;
    vector<XMFLOAT2> temp_uvs;
    vector<XMFLOAT3> temp_normals;

    // Open Wavefront OBJ file for reading
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Failed to open wavefront model file!\n");
        return -1;
    }

    // Process OBJ file line-by-line and read the vertex attributes
    while(1) {
        char line_header[128] = { '\0' };

        // read the first word of the line
        int res = fscanf(file, "%s", line_header);
        if (res == EOF)
            break;
        
        if (strcmp(line_header, "v") == 0) {
            XMFLOAT3 vertex;

            int read_vertex = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            assert(read_vertex == 3);

            temp_vertices.push_back(vertex);
        }
        else if (strcmp(line_header, "vt") == 0) {
            XMFLOAT2 uv;

            int read_uv = fscanf(file, "%f %f\n", &uv.x, &uv.y);
            assert(read_uv == 2);

            temp_uvs.push_back(uv);
        }
        else if (strcmp(line_header, "vn") == 0) {
            XMFLOAT3 normal;

            int read_normal = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            assert(read_normal == 3);

            temp_normals.push_back(normal);
        }
        else if (strcmp(line_header, "f") == 0) {
            unsigned int vertex_index[3], uv_index[3], normal_index[3];

            int read_face = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertex_index[0], &uv_index[0], &normal_index[0],
                &vertex_index[1], &uv_index[1], &normal_index[1],
                &vertex_index[2], &uv_index[2], &normal_index[2]
                );
            
            assert(read_face == 9);

            vertex_indices.push_back(vertex_index[0]);
            vertex_indices.push_back(vertex_index[1]);
            vertex_indices.push_back(vertex_index[2]);
            uv_indices.push_back(uv_index[0]);
            uv_indices.push_back(uv_index[1]);
            uv_indices.push_back(uv_index[2]);
            normal_indices.push_back(normal_index[0]);
            normal_indices.push_back(normal_index[1]);
            normal_indices.push_back(normal_index[2]);
        }
    }

    // We're done reading the input file
    fclose(file);

    // Open output binary model file for writing
    FILE* mdl_file = fopen(argv[2], "wb");

    // Write MDL header
    mdl_header mdl_hdr;
    mdl_hdr.magic        = 0x004C444D; // 'M' 'D' 'L' '00'
    mdl_hdr.verts_count  = vertex_indices.size();

    fwrite(&mdl_hdr, sizeof(mdl_hdr), 1, mdl_file);

    // Write MDL vertex data
    for (unsigned int i = 0; i < mdl_hdr.verts_count; i++) {
        fwrite(&temp_vertices[vertex_indices[i] - 1], sizeof(XMFLOAT3), 1, mdl_file);
        fwrite(&temp_uvs[uv_indices[i] - 1], sizeof(XMFLOAT2), 1, mdl_file);
        fwrite(&temp_normals[normal_indices[i] - 1], sizeof(XMFLOAT3), 1, mdl_file);
    }

    // We're done writing the output file
    fclose(mdl_file);

    return 0;
}
