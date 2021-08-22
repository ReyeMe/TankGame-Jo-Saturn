#include <jo/jo.h>
#include "modelLoader.h"

/* Model texture loader
 * @param texture Current texture to load
 * @param modelDir Current model folder
 * @param count Total number of textures to load
 * @return Index of first loaded texture
 */
static int ML_BasicTextureLoader(TmfTexture * texture, const char * modelDir, int count)
{
    int startIndex = -1;

	if (startIndex < 0)
	{
    	for (int index = 0; index < count; index++)
    	{
    	    if (texture[index].FileName[0] != '\0')
    	    {
    	        int loadedAt = jo_sprite_add_tga(modelDir, texture[index].FileName, JO_COLOR_Black);

    	        if (startIndex == -1)
    	            startIndex = loadedAt;
    	    }
    	}
	}

    return startIndex;
}

jo_3d_mesh * ML_LoadMesh(const char *file, const char *dir, int *loaded)
{
    return ML_LoadMeshWithCustomTextureLoader(file, dir, &ML_BasicTextureLoader, loaded);
}

jo_3d_mesh * ML_LoadMeshWithCustomTextureLoader(const char * file, const char * dir, int (*texture_loader)(TmfTexture*, const char *, int), int * loaded)
{
    int face;
    int model;
    int point;
    int coord;

    unsigned char *stream = (unsigned char *)jo_fs_read_file_in_dir(file, dir, JO_NULL);
    unsigned char *startAddress = stream;

    // Get header
    TmfHeader *header = (TmfHeader *)stream;
    stream += sizeof(TmfHeader);

    // Load textures
    TmfTexture *textures = (TmfTexture *)stream;
    int textureOffset = texture_loader(textures, dir, header->TextureCount);
    stream += sizeof(TmfTexture) * header->TextureCount;

    // Load meshes
    jo_3d_mesh *meshes = jo_malloc(sizeof(jo_3d_mesh) * header->ModelCount);
    *loaded = header->ModelCount;

    for (model = 0; model < header->ModelCount; model++)
    {
        // Get header and create mesh data
        TmfModelHeader *modelHeader = (TmfModelHeader *)stream;
        stream += sizeof(TmfModelHeader);

        meshes[model].data.pntbl = jo_malloc(sizeof(POINT) * modelHeader->VerticesCount);
        meshes[model].data.pltbl = jo_malloc(sizeof(POLYGON) * modelHeader->FaceCount);
        meshes[model].data.attbl = jo_malloc(sizeof(ATTR) * modelHeader->FaceCount);
        meshes[model].data.nbPoint = modelHeader->VerticesCount;
        meshes[model].data.nbPolygon = modelHeader->FaceCount;

        // Read vertices
        for (point = 0; point < modelHeader->VerticesCount; point++) {
            for (int xyz = 0; xyz < XYZ; xyz++) {
                meshes[model].data.pntbl[point][xyz] = (FIXED)(((*(stream)) << 24) | ((*(stream + 1)) << 16) | ((*(stream + 2)) << 8) | (*(stream + 3)));
                stream+=4;
            }
        }

        // Read quads
        for (face = 0; face < modelHeader->FaceCount; face++)
        {
            // Get quad data
            TmfFace *faceData = (TmfFace *)stream;
            stream += sizeof(TmfFace);

            TmfTexture texture = textures[faceData->Texture];

            // Load normal
            for (coord = 0; coord < 3; coord++)
                meshes[model].data.pltbl[face].norm[coord] = faceData->Normal[coord];

            // Load quad
            for (point = 0; point < 4; point++)
                meshes[model].data.pltbl[face].Vertices[point] = faceData->Indexes[point];

            // Load attributes
            unsigned short color = No_Palet;
            unsigned short textureId = No_Texture;

            // Use color if not texture file is referenced
            if (texture.FileName[0] == '\0')
            {
                color = JO_COLOR_RGB(texture.color[0], texture.color[1], texture.color[2]);
            }
            else
            {
                textureId = faceData->Texture + textureOffset;
            }

            ATTR attribute = ATTRIBUTE(
                ((faceData->Flags & Tmf_Face_Double) != 0) ? Dual_Plane : Single_Plane,
                SORT_CEN,
                textureId,
                color,
                CL32KRGB | No_Gouraud,
                CL32KRGB | (((faceData->Flags & Tmf_Face_Meshed) != 0) ? MESHon : MESHoff),
                (texture.FileName[0] == '\0' ? sprPolygon : sprNoflip),
                No_Option);

            meshes[model].data.attbl[face] = attribute;
        }
    }

    jo_free(startAddress);
    return meshes;
}
